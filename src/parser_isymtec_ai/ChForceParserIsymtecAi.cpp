#include "parser_isymtec_ai/ChForceParserIsymtecAi.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono/physics/ChMarker.h"
#include "chrono/physics/ChForce.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"
#include "chrono/physics/ChBody.h"
#include "parser_isymtec_ai/ChExceptionIsymtecAi.h"
#include "parser_isymtec_ai/ChMarkerDefined3DFunction.h"
#include "parser_isymtec_ai/Ch3DFunctionPart.h"
#include "chrono/physics/ChLinkSpring.h"
#include "chrono/physics/ChSystem.h"

using namespace force_isymtec_ai_params;
using namespace chrono;



ChForceParserIsymtecAi::ChForceParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations,
	std::shared_ptr<const ChFunctionStorage> functionStorage) :
	ChElementaryParserIsymtecAi(relations),
	m_FunctionStorage(functionStorage)
{
}

namespace {
	std::shared_ptr<ChForce> generateAppliedForce(const std::shared_ptr<chrono::ChMarker> marker1,
		ChForce::ForceType forceType) {
		auto force = std::make_shared<ChForce>();
		auto* body = marker1->GetBody();
		body->AddForce(force);
		force->SetFrame(ChForce::BODY); //<application point
		force->SetAlign(ChForce::WORLD_DIR);
		force->SetMode(forceType);
		auto& absCoord = marker1->GetAbsCoord();
		ChVector<> absPos = absCoord.pos;
		force->SetVpoint(absPos);
		return force;
	}

	std::shared_ptr<ChMarkerDefined3DFunction> add3DFunctions(ChForce& force,
		const std::shared_ptr<chrono::ChMarker> alignmentMarker) {
		auto output = std::make_shared<ChMarkerDefined3DFunction>(alignmentMarker);
		auto part0 = std::make_shared<Ch3DFunctionPart>(0, output);
		force.SetF_x(part0);
		auto part1 = std::make_shared<Ch3DFunctionPart>(1, output);
		force.SetF_y(part1);
		auto part2 = std::make_shared<Ch3DFunctionPart>(2, output);
		force.SetF_z(part2);
		return output;
	}

}


void ChForceParserIsymtecAi::doParseObject() {

	std::string forceType = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		force_isymtec_ai_params::FORCE_TYPE);
	if (forceType == force_isymtec_ai_params::APPLIED_FORCE) {
		createAppliedForce();
	}
	else if (forceType == force_isymtec_ai_params::SPRING_FORCE) {
		createSpringForce();
	}
	else {
		throwExeption(force_isymtec_ai_params::FORCE_TYPE,	"Unknown force type " + forceType);
	}
}

const std::shared_ptr<chrono::ChMarker> ChForceParserIsymtecAi::findMarker(std::string markerPropertyName) const
{
	std::string markerID = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		markerPropertyName);
	auto output = getRelations().getGenerateObjPtrFromUUID<chrono::ChMarker>(markerID);
	if (output == nullptr) {
		throwExeption(markerPropertyName, "Can't find marker");
	}
	return output;
}



void ChForceParserIsymtecAi::createAppliedForce() {
	auto marker1 = findMarker(force_isymtec_ai_params::CS1_ID);
	auto marker2 = findMarker(force_isymtec_ai_params::CS2_ID);


	auto force = generateAppliedForce(marker1, ChForce::FORCE);
	AddRelation(force);
	auto func3DForce = add3DFunctions(*force, marker2);
	setAppliedForceInputs(*func3DForce, applied_force_isymtec_ai_params::FORCE_VALUE);
	

	auto torque = generateAppliedForce(marker1, ChForce::TORQUE);
	AddRelation(torque);
	auto func3DTorque = add3DFunctions(*torque, marker2);
	setAppliedForceInputs(*func3DTorque, applied_force_isymtec_ai_params::TORQUE_VALUE);
}

std::shared_ptr<chrono::ChBody> ChForceParserIsymtecAi::GetBody(const chrono::ChMarker& marker) const
{
	auto bodyPtr = dynamic_cast<ChBodyAuxRef*>(marker.GetBody());
	auto output = getRelations().GetSystem()->SearchBody(bodyPtr->GetName());
	return output;
}


void ChForceParserIsymtecAi::createSpringForce() {
	auto marker1 = findMarker(force_isymtec_ai_params::CS1_ID);
	auto marker2 = findMarker(force_isymtec_ai_params::CS2_ID);

	double stiffness = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		spring_force_isymtec_ai_params::STIFFNESS);
	double damping = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		spring_force_isymtec_ai_params::DAMPING);
	double restLength = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		spring_force_isymtec_ai_params::REST_LENGTH);

	auto body1 = GetBody(*marker1);
	auto body2 = GetBody(*marker2);

	ChVector<> absPos1 = marker1->GetAbsCoord().pos;
	ChVector<> absPos2 = marker2->GetAbsCoord().pos;


	//TODO: set spring params
	auto spring = std::make_shared<ChLinkSpring>();
	spring->Initialize(body1, body2, false, absPos1, absPos2, false, restLength);
	spring->Set_SpringK(stiffness);
	spring->Set_SpringR(damping);
	getRelations().GetSystem()->AddLink(spring);
	AddRelation(spring);
}


void ChForceParserIsymtecAi::setAppliedForceInputs(ChMarkerDefined3DFunction& function3D, std::string propertyName) {
	auto forceValues = isymtec_ai_utils::GetStringFixSizeVectorProperty(getObjectFrom(),	propertyName, 3);
	for (unsigned index = 0; index < 3; index++)
	{
		auto functionInMarperPart = isymtec_ai_utils::FindOrCreateMuParserFunction(*m_FunctionStorage, forceValues[index]);
		function3D.setFunctionInMarker(index, functionInMarperPart);
	}
}
