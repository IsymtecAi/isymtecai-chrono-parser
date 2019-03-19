#include "parser_isymtec_ai/ChBodyParserIsymtecAi.h"
#include "chrono/physics/ChBodyAuxRef.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono/physics/ChSystem.h"
#include "core/ChQuaternion.h"
#include "ChFramesUtils.h"

using namespace body_isymtec_ai_params;
using namespace chrono;

namespace {
	///compute tensor of inertia in body central frame from the inertia in body frame,
	///assuming that body frame is parallel to the body central frame
	ChMatrix33<> computeCentralInertia(double mass,
		const ChVector<>& center,
		ChMatrix33<> inertiaFrom)
	{
		ChMatrix33<> inertia = inertiaFrom;
		inertia[0][0] -= mass * (center.y() * center.y() + center.z() * center.z());
		inertia[0][1] += mass * center.x() * center.y();
		inertia[0][2] += mass * center.z() * center.x();
		inertia[1][0] = inertia[0][1];
		inertia[1][1] -= mass * (center.z() * center.z() + center.x() * center.x());
		inertia[1][2] += mass * center.y() * center.z();
		inertia[2][0] = inertia[0][2];
		inertia[2][1] = inertia[1][2];
		inertia[2][2] -= mass * (center.x() * center.x() + center.y() * center.y());
		return inertia;
	}
}

ChBodyParserIsymtecAi::ChBodyParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations) :
	ChElementaryParserIsymtecAi(relations)
{
}

void ChBodyParserIsymtecAi::doParseObject()
{
	//GetBoolProperty
	m_Body = std::make_shared<ChBodyAuxRef>();
	getRelations().AddRelation(getObjectFrom(), m_Body);

	isymtec_ai_utils::SetObjectName(getObjectFrom(), *m_Body);
	getRelations().GetSystem()->AddBody(m_Body);

	//create zero marker, it will make parsing of geometries and links easier
	auto marker = std::make_shared<ChMarker>(m_Body->GetName(), m_Body.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
	getRelations().AddRelation(getObjectFrom(), marker);
	m_Body->AddMarker(marker);

	bool fixed = isymtec_ai_utils::GetBoolProperty(getObjectFrom(), PROPERTY_FIXED);
	m_Body->SetBodyFixed(fixed);

	SetBodyCoordinates();
	SetBodyVelocities();
	setMassProperties();

	//return *m_Body;
}

void ChBodyParserIsymtecAi::SetBodyCoordinates()
{
	ChVector<> translation = isymtec_ai_utils::getVectorProperty(getObjectFrom(), PROPERTY_TRANSLATION);
	ChVector<> orientation = isymtec_ai_utils::getVectorProperty(getObjectFrom(), PROPERTY_ORIENTATION);
	chrono::ChQuaternion<> quat = frames_utils::createQuaternionFromEulerXYZ(orientation);
	ChFrame<> refToAbs{ translation, quat };
	m_Body->SetFrame_REF_to_abs(refToAbs);
}

void ChBodyParserIsymtecAi::SetBodyVelocities() {
	ChVector<> linearVelAbs = isymtec_ai_utils::getVectorProperty(getObjectFrom(), PROPERTY_LINEAR_VEL);
	m_Body->SetPos_dt(linearVelAbs);
	ChVector<> angularVelAbs = isymtec_ai_utils::getVectorProperty(getObjectFrom(), PROPERTY_ANGULAR_VEL);
	m_Body->SetWvel_par(angularVelAbs);
}

void ChBodyParserIsymtecAi::setMassProperties()
{
	double mass = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(), PROPERTY_MASS);
	m_Body->SetMass(mass);

	ChVector<> centerOfMass = isymtec_ai_utils::getVectorProperty(getObjectFrom(), PROPERTY_CENTER_OF_MASS);
	m_Body->SetFrame_COG_to_REF(ChFrame<>(centerOfMass));

	ChMatrix33<> userDefinedInertia = isymtec_ai_utils::GetMatrix33Property(getObjectFrom(), PROPERTY_INERTIA);

	///TODO: delete provement. We will always have this member
	bool isInertiaInReferenceFrame = true;
	if (getObjectFrom().HasMember(PROPERTY_IS_CENTRAL_INERTIA.c_str())) {
		isInertiaInReferenceFrame = !(isymtec_ai_utils::GetBoolProperty(getObjectFrom(), PROPERTY_IS_CENTRAL_INERTIA));
	}
	ChMatrix33<> inertiaInCentralFrame = isInertiaInReferenceFrame ?
		computeCentralInertia(mass, centerOfMass, userDefinedInertia) :
		userDefinedInertia;
	m_Body->SetInertia(inertiaInCentralFrame);
}