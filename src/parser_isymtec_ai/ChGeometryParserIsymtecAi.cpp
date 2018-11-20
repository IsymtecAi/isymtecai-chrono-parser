#include "parser_isymtec_ai/ChGeometryParserIsymtecAi.h"
#include "ChIsymtecAiUtils.h"
#include "assets/ChVisualization.h"
#include "assets/ChCylinderShape.h"
#include "ChRelationsIsymtecAi.h"
#include "physics/ChBodyAuxRef.h"
#include "assets/ChBoxShape.h"

using namespace chrono;

ChGeometryParserIsymtecAi::ChGeometryParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations) :
	ChElementaryParserIsymtecAi(relations)
{
}

void ChGeometryParserIsymtecAi::doParseObject()
{
	std::string markerID = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		geometry_isymtec_ai_params::REFERENCE_CS_ID);

	m_Marker = getRelations().getGenerateObjPtrFromUUID<chrono::ChMarker>(markerID);
	if (m_Marker == nullptr) {
		throwExeption(geometry_isymtec_ai_params::REFERENCE_CS_ID, "Can't find marker");
	}
	m_Body = dynamic_cast<ChBodyAuxRef*>(m_Marker->GetBody());
	auto generatedGeometry = CreateGeometry();
	if (generatedGeometry != nullptr) {
		m_Body->AddAsset(generatedGeometry);
	}
}

std::shared_ptr<chrono::ChVisualization> ChGeometryParserIsymtecAi::CreateGeometry()
{
	std::string geometryType = isymtec_ai_utils::GetStringProperty(getObjectFrom(),
		geometry_isymtec_ai_params::GEOMETRY_TYPE);
	if (geometryType == geometry_isymtec_ai_params::CYLINDER) {
		return CreateCylinder();
	}
	else if (geometryType == geometry_isymtec_ai_params::BOX) {
		return CreateBox();
	}
	else if (geometryType == geometry_isymtec_ai_params::CAD_FILE) {
		return nullptr;
	}
	else {
		throwExeption(geometry_isymtec_ai_params::GEOMETRY_TYPE, "Unknown geometry type " + geometryType);
		return nullptr;
	}
}

std::shared_ptr<chrono::ChCylinderShape> ChGeometryParserIsymtecAi::CreateCylinder() {
	auto cylinder = std::make_shared<ChCylinderShape>();
	double radius = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		cylinder_isymtec_ai_params::RADIUS);
	cylinder->GetCylinderGeometry().rad = radius;

	double height = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		cylinder_isymtec_ai_params::HEIGHT);

	cylinder->GetCylinderGeometry().p1 = m_Marker->TransformPointLocalToParent(ChVector<>(0, height / 2, 0));
	cylinder->GetCylinderGeometry().p2 = m_Marker->TransformPointLocalToParent(ChVector<>(0, -height / 2, 0));

	return cylinder;
}

std::shared_ptr<chrono::ChBoxShape> ChGeometryParserIsymtecAi::CreateBox()
{
	auto box = std::make_shared<ChBoxShape>();

	box->GetBoxGeometry().Pos = m_Marker->TransformPointLocalToParent(ChVector<>(0, 0, 0));
	box->GetBoxGeometry().Rot = m_Marker->Amatrix;

	double width = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		box_isymtec_ai_params::WIDTH);
	double height = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		box_isymtec_ai_params::HEIGHT);
	double depth = isymtec_ai_utils::GetDoubleProperty(getObjectFrom(),
		box_isymtec_ai_params::DEPTH);

	box->GetBoxGeometry().SetLengths(ChVector<>(width, height, depth));

	return box;
}