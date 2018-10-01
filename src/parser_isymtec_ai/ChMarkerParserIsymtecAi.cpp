#include "parser_isymtec_ai/ChMarkerParserIsymtecAi.h"
#include "chrono/physics/ChBodyAuxRef.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono/physics/ChSystem.h"
#include "core/ChQuaternion.h"
#include "physics/ChBody.h"
#include "ChFramesUtils.h"

using namespace chrono;
using namespace marker_isymtec_ai_params;

ChMarkerParserIsymtecAi::ChMarkerParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations) :
	m_BodyFrom(nullptr),
	m_Relations(relations)
{
}

void ChMarkerParserIsymtecAi::ParseBodyMarkers(rapidjson::Value& bodyFrom)
{
	m_Graph.clear();
	m_BodyFrom = &bodyFrom;
	m_Body = m_Relations->getGenerateObjPtr<chrono::ChBodyAuxRef>(*m_BodyFrom);
	if (m_Body == nullptr) {
		std::string bodyName = isymtec_ai_utils::GetName(*m_BodyFrom);
		throw chrono::ChException("Error in Marker parser. Can't find generated body " + bodyName);
	}
	CreateGraph();
	ProveNonExistingMarkersInGraph();
	SetTreeFlagInGraphRecursively(getBodyUUID());
	ProveAllNodesInTree();

	const Coordsys& bodyAbsCoor = m_Body->GetFrame_REF_to_abs().coord;
	createDependentMarkes(getBodyUUID(), bodyAbsCoor);
}

void ChMarkerParserIsymtecAi::CreateGraph()
{
	m_Graph[getBodyUUID()].m_MarkerFrom = m_BodyFrom;

	auto markers = isymtec_ai_utils::GetElementsInContainer(*m_BodyFrom, MARKER_CONTAINER);
	for (auto& markerFrom : markers) {
		std::string uuid = isymtec_ai_utils::getUUID(markerFrom);
		m_Graph[uuid].m_MarkerFrom = &markerFrom;
		std::string referenceCS = isymtec_ai_utils::GetStringProperty(markerFrom, REFERENCE_CS_ID);
		auto& dependentMarkers = m_Graph[referenceCS].m_DependentMarkers;
		dependentMarkers.push_back(uuid);
	}
}

std::string ChMarkerParserIsymtecAi::getBodyUUID() const
{
	std::string uuid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	return uuid;
}

void ChMarkerParserIsymtecAi::ProveNonExistingMarkersInGraph()
{
	for (auto& node : m_Graph) {
		auto* markerfrom = node.second.m_MarkerFrom;
		if (markerfrom == nullptr) {
			std::string bodyName = isymtec_ai_utils::GetName(*m_BodyFrom);
			throw chrono::ChException("Error in Marker parser. Can't find marker with UUID " + node.first + " Body " + bodyName);
		}
	}
}

void ChMarkerParserIsymtecAi::SetTreeFlagInGraphRecursively(const std::string& uuid)
{
	m_Graph[uuid].m_IsInTree = true;
	auto& dependentMarkers = m_Graph[uuid].m_DependentMarkers;
	for (const auto& markerUUid : dependentMarkers) {
		SetTreeFlagInGraphRecursively(markerUUid);
	}
}

void ChMarkerParserIsymtecAi::ProveAllNodesInTree()
{
	for (const auto& nodePair : m_Graph) {
		const auto&  node = nodePair.second;
		if (!node.m_IsInTree)
		{
			auto* markerFrom = node.m_MarkerFrom;
			std::string markerName = isymtec_ai_utils::GetName(*markerFrom);
			std::string bodyName = isymtec_ai_utils::GetName(*m_BodyFrom);
			throw chrono::ChException("Error in Marker parser. Marker " + markerName +
				" in Body " + bodyName + " has a wrong reference Coordinate System");
		}
	}
}

//void ChMarkerParserIsymtecAi::CreateMarkersRecursively(const std::string& uuid, bool createOnlyChildren)
//{
//	if (!createOnlyChildren)	{
//		auto* markerFrom = m_Graph[uuid].m_MarkerFrom;
//	//	createMarker(*markerFrom);
//	}
//
//
//
//	auto& dependentMarkers = m_Graph[uuid].m_DependentMarkers;
//	for (const std::string& markerUUid : dependentMarkers) {
//		CreateMarkersRecursively(markerUUid, false);
//	}
//}

void ChMarkerParserIsymtecAi::createDependentMarkes(const std::string& markerUUID, const Coordsys& parentMarkerAbsCoor)
{
	auto& dependentMarkers = m_Graph[markerUUID].m_DependentMarkers;
	for (const std::string& dependentMarkerUUID : dependentMarkers) {
		auto* dependentMarkerFrom = m_Graph[dependentMarkerUUID].m_MarkerFrom;
		createMarker(*dependentMarkerFrom, parentMarkerAbsCoor);
	}
}

void ChMarkerParserIsymtecAi::createMarker(rapidjson::Value& markerFrom,
	const Coordsys& parentMarkerAbsCoor)
{
	ChVector<> translation = isymtec_ai_utils::getVectorProperty(markerFrom, PROPERTY_TRANSLATION);
	ChVector<> orientation = isymtec_ai_utils::getVectorProperty(markerFrom, PROPERTY_ORIENTATION);
	chrono::ChQuaternion<> quat = frames_utils::createQuaternionFromEulerXYZ(orientation);
	Coordsys relCoor{ translation, quat };

	Coordsys absCoor = parentMarkerAbsCoor * relCoor;
	std::string markerName = isymtec_ai_utils::GetName(markerFrom);
	auto marker = std::make_shared<ChMarker>(markerName, m_Body.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
	m_Relations->AddRelation(markerFrom, marker);
	m_Body->AddMarker(marker);
	marker->Impose_Abs_Coord(absCoor);

	std::string uuid = isymtec_ai_utils::getUUID(markerFrom);
	createDependentMarkes(uuid, absCoor);
}