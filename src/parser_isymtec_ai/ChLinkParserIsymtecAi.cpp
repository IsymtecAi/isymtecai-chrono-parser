
#include "parser_isymtec_ai/ChLinkParserIsymtecAi.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "parser_isymtec_ai/ChFramesUtils.h"


#include "chrono/physics/ChSystem.h"

#include "physics/ChLinkLock.h"
#include "physics/ChAssembly.h"
#include "physics/ChLinkMarkers.h"
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "chrono/physics/ChLinkMotorLinearSpeed.h"
#include "chrono/physics/ChLinkMotorRotationSpeed.h"


using namespace link_isymtec_ai_params;

using namespace chrono;



ChLinkParserIsymtecAi::ChLinkParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations,
	std::shared_ptr<const ChFunctionStorage> functionStorage):
	ChElementaryParserIsymtecAi(relations),
	m_FunctionStorage(functionStorage)
{
}


void ChLinkParserIsymtecAi::doParseObject() {
	std::string marker1ID = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		link_isymtec_ai_params::CS1_ID);

	m_Marker1 = getRelations().getGenerateObjPtrFromUUID<chrono::ChMarker>(marker1ID);
	if (m_Marker1 == nullptr) {
		throwExeption(link_isymtec_ai_params::CS1_ID, "Can't find marker");
	}

	std::string marker2ID = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		link_isymtec_ai_params::CS2_ID);

	m_Marker2 = getRelations().getGenerateObjPtrFromUUID<chrono::ChMarker>(marker2ID);
	if (m_Marker2 == nullptr) {
		throwExeption(link_isymtec_ai_params::CS2_ID, "Can't find marker");
	}

	auto links = CreateLinks();
	for (auto link : links) {
		getRelations().GetSystem()->AddLink(link);
	}
}

std::shared_ptr<chrono::ChBody> ChLinkParserIsymtecAi::GetBody1() const
{
	auto bodyPtr = dynamic_cast<ChBodyAuxRef*>(m_Marker1->GetBody());
	auto output = getRelations().GetSystem()->SearchBody(bodyPtr->GetName());
	return output;
}


std::shared_ptr<chrono::ChBody> ChLinkParserIsymtecAi::GetBody2() const
{
	auto bodyPtr = dynamic_cast<ChBodyAuxRef*>(m_Marker2->GetBody());
	auto output = getRelations().GetSystem()->SearchBody(bodyPtr->GetName());
	return output;
}



std::shared_ptr<chrono::ChLinkMotorLinearSpeed> ChLinkParserIsymtecAi::CreateTranslationalVelocityLink(const std::string& dofExpression, size_t dofIndex)
{
	if (dofExpression == link_isymtec_ai_params::FREE_DOF) {
		return nullptr;
	}
	auto output = std::make_shared<ChLinkMotorLinearSpeed>();
	output->SetConstrainedCoords(false, false, true, false, false, false);

	auto function = isymtec_ai_utils::FindOrCreateMuParserFunction(*m_FunctionStorage, dofExpression);
	output->SetSpeedFunction(function);

	std::string commonPostfix = "VelTrans";
	initializeVelocityLink(dofIndex, output, commonPostfix);

	return output;
}


std::shared_ptr<chrono::ChLinkMotorRotationSpeed> ChLinkParserIsymtecAi::CreateRotationalVelocityLink(const std::string& dofExpression, size_t dofIndex)
{
	if (dofExpression == link_isymtec_ai_params::FREE_DOF) {
		return nullptr;
	}
	auto output = std::make_shared<ChLinkMotorRotationSpeed>();
	output->SetConstrainedCoords(false, false, false, false, false, true);

	auto linkFunction = isymtec_ai_utils::FindOrCreateMuParserFunction(*m_FunctionStorage, dofExpression);
	output->SetSpeedFunction(linkFunction);

	std::string commonPostfix = "VelRot";
	initializeVelocityLink(dofIndex, output, commonPostfix);

	return output;
}


void ChLinkParserIsymtecAi::initializeVelocityLink(size_t dofIndex, 
	std::shared_ptr<chrono::ChLinkMateGeneric> link, std::string markerNamePostfix)
{
	std::shared_ptr<chrono::ChMarker> marker1 = m_Marker1;
	std::shared_ptr<chrono::ChMarker> marker2 = m_Marker1;
	if (dofIndex == 0) {
		std::string postfix = "X" + markerNamePostfix;
		marker1 = frames_utils::createMarkerByRotationXToZ(*m_Marker1, std::string(m_Marker1->GetName()) + postfix);
		marker2 = frames_utils::createMarkerByRotationXToZ(*m_Marker2, std::string(m_Marker2->GetName()) + postfix);
	}
	if (dofIndex == 1) {
		std::string postfix = "Y" + markerNamePostfix;
		marker1 = frames_utils::createMarkerByRotationYToZ(*m_Marker1, std::string(m_Marker1->GetName()) + postfix);
		marker2 = frames_utils::createMarkerByRotationYToZ(*m_Marker2, std::string(m_Marker2->GetName()) + postfix);
	}
	link->Initialize(GetBody1(), GetBody2(), false, *marker1, *marker2);
}

std::vector<std::shared_ptr<chrono::ChLink>> ChLinkParserIsymtecAi::CreateVelocityLinks()
{
	std::vector<std::shared_ptr<chrono::ChLink>> output;
	auto translationalDOFs = isymtec_ai_utils::GetStringVectorProperty(getObjectFrom(),
		link_isymtec_ai_params::TRANSLATIONAL_DOF_S);
	if (translationalDOFs.size() != 3) {
		throwExeption(link_isymtec_ai_params::TRANSLATIONAL_DOF_S, "Should be a vector of size three");
	}
	for (size_t index = 0; index < 3; index++)
	{
		auto link = CreateTranslationalVelocityLink(translationalDOFs[index], index);
		if (link != nullptr) {
			output.push_back(link);
		}		
	}

	auto rotationalDOFs = isymtec_ai_utils::GetStringVectorProperty(getObjectFrom(),
		link_isymtec_ai_params::ROTATIONAL_DOF_S);
	if (rotationalDOFs.size() != 3) {
		throwExeption(link_isymtec_ai_params::ROTATIONAL_DOF_S, "Should be a vector of size three");
	}
	for (size_t index = 0; index < 3; index++)
	{
		auto link = CreateRotationalVelocityLink(rotationalDOFs[index], index);
		if (link != nullptr) {
			output.push_back(link);
		}
	}	

	return output;
}

std::vector<std::shared_ptr<chrono::ChLink>> ChLinkParserIsymtecAi::CreateLinks()
{
	std::string linkType = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		link_isymtec_ai_params::LINK_TYPE);
	std::shared_ptr<chrono::ChLinkMarkers> output;
	if (linkType == link_isymtec_ai_params::VELOCITY) {
		return CreateVelocityLinks();
	}
	else if (linkType == link_isymtec_ai_params::LOCK) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockLock());
	}
	else if (linkType == link_isymtec_ai_params::SPHERICAL) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockSpherical());
	}
	else if (linkType == link_isymtec_ai_params::POINTPLANE) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockPointPlane());
	}

	else if (linkType == link_isymtec_ai_params::POINTLINE) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockPointLine());
	}
	else if (linkType == link_isymtec_ai_params::CYLINDRICAL) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockCylindrical());
	}
	else if (linkType == link_isymtec_ai_params::PRISMATIC) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockPrismatic());
	}
	else if (linkType == link_isymtec_ai_params::PLANEPLANE) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockPlanePlane());
	}
	else if (linkType == link_isymtec_ai_params::FREE) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockFree());
	}
	else if (linkType == link_isymtec_ai_params::ALIGN) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockAlign());
	}
	else if (linkType == link_isymtec_ai_params::PARALLEL) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockParallel());
	}
	else if (linkType == link_isymtec_ai_params::PERPEND) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockPerpend());
	}
	//else if (linkType == link_isymtec_ai_params::TRAJECTORY) {
	//	output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkTrajectory());
	//}
	//else if (linkType == link_isymtec_ai_params::CLEARANCE) {
	//	output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkClearance());
	//}
	else if (linkType == link_isymtec_ai_params::REVOLUTEPRISMATIC) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockRevolutePrismatic());
	}
	else if (linkType == link_isymtec_ai_params::REVOLUTE) {
		output = std::shared_ptr<chrono::ChLinkMarkers>(new ChLinkLockRevolute());
	}

	else {
		throwExeption(link_isymtec_ai_params::LINK_TYPE, "Unknown link type " + linkType);
	}
	if (output == nullptr) {
		return {};
	}
	output->Initialize(m_Marker1, m_Marker2);
	isymtec_ai_utils::SetObjectName(getObjectFrom(), *output);
	return { output };
}

