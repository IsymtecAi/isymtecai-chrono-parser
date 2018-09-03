#include "gtest/gtest.h"
#include "ParserTestUtils.h"

#include "parser_isymtec_ai/ChMarkerParserIsymtecAi.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"

#include "chrono_thirdparty/rapidjson/stringbuffer.h"
#include <chrono_thirdparty/rapidjson/writer.h>
#include "chrono_thirdparty/rapidjson/filewritestream.h"
#include <chrono_thirdparty/rapidjson/writer.h>
#include "chrono_thirdparty/rapidjson/prettywriter.h"
#include "chrono_thirdparty/rapidjson/pointer.h"

#include <cstdio>
#include "parser_isymtec_ai/ChFramesUtils.h"

using namespace rapidjson;
using namespace chrono;
using namespace marker_isymtec_ai_params;
using namespace parser_test_utils;

namespace {
	std::shared_ptr<Value> createMarkerFrom(const std::string& name, const std::string& referenceCSId, Document::AllocatorType& curAllocator) {
		auto markerFrom = createDummyObject(name, curAllocator);
		addStringMember(*markerFrom, PROPERTY_TRANSLATION, "0, 0, 0", curAllocator);
		addStringMember(*markerFrom, PROPERTY_ORIENTATION, "0, 0, 0", curAllocator);
		addStringMember(*markerFrom, REFERENCE_CS_ID, referenceCSId, curAllocator);

		return markerFrom;
	}


	std::shared_ptr<ChMarkerParserIsymtecAi> createMarkerParser(
		std::shared_ptr<ChRelationsIsymtecAi> relations) {
		// Make a system
		auto curSystem = std::make_shared<ChSystemSMC>();


		relations->SetSystem(curSystem);
		auto markerParser = std::make_shared<ChMarkerParserIsymtecAi>(relations);
		return markerParser;
	}
}

// The fixture for testing class Foo.
class ChMarkerParserIsymtecAiTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ChMarkerParserIsymtecAiTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChMarkerParserIsymtecAiTest() {};

	virtual void SetUp() {
		m_Document = new rapidjson::Document;
		m_Document->SetObject();
		m_Relations = std::make_shared<ChRelationsIsymtecAi>();
		m_MarkerParser = createMarkerParser(m_Relations);
		m_Body = std::make_shared<ChBodyAuxRef>();

		auto bodyFrom = createDummyObject("Body1", GetAllocator());
		m_BodyFrom = &Pointer("/B1").GetWithDefault(*m_Document, *bodyFrom);
		Value markerContainer(rapidjson::kObjectType);
		Value objects(kArrayType);
		markerContainer.AddMember(StringRef(isymtec_ai_utils::OBJECTS_MEMBER.c_str()), objects, GetAllocator());
		m_BodyFrom->AddMember(StringRef(MARKER_CONTAINER.c_str()), markerContainer, GetAllocator());

		m_Relations->AddRelation(*m_BodyFrom, m_Body);
	}

	Value& getJsonMarkersArr() const {
		auto& markerContainer = (*m_BodyFrom)[MARKER_CONTAINER.c_str()];
		auto& markersFrom = markerContainer[isymtec_ai_utils::OBJECTS_MEMBER.c_str()];
		return markersFrom;
	}

	virtual void TearDown() {
		delete m_Document;
	}

	Document::AllocatorType& GetAllocator() {
		return m_Document->GetAllocator();
	}

	rapidjson::Document* m_Document;
	
	std::shared_ptr<ChMarkerParserIsymtecAi> m_MarkerParser;
	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;
	Value* m_BodyFrom;
	std::shared_ptr<chrono::ChBodyAuxRef> m_Body;
	
};


///prove if from one json marker will be generated exactly one chrono marker
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersOneMarker) {

	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1 = createMarkerFrom("Marker1", bodyUUid, GetAllocator());
	getJsonMarkersArr().PushBack(*marker1, GetAllocator());

	///writeInFile(*m_Document, "output.json");

	m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
	auto& markerlist = m_Body->GetMarkerList();
	size_t markersCount = markerlist.size();
	EXPECT_EQ(markersCount, 1);
}


///prove if from two json marker will be generated exactly two chrono markers
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersTwoMarker) {

	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1From = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	getJsonMarkersArr().PushBack(*marker1From, GetAllocator());

	Value& marker1 = *(getJsonMarkersArr().End() - 1); //get last element

	std::string marker1UUid = isymtec_ai_utils::getUUID(marker1);
	auto marker2 = createMarkerFrom("Marker2", marker1UUid, GetAllocator());
	getJsonMarkersArr().PushBack(*marker2, GetAllocator());

	//writeInFile(*m_Document, "output.json");

	m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
	auto& markerlist = m_Body->GetMarkerList();
	size_t markersCount = markerlist.size();
	EXPECT_EQ(markersCount, 2);
}



///wrong reference id in marker2
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersFalseUUID) {

	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1From = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	getJsonMarkersArr().PushBack(*marker1From, GetAllocator());

	Value& marker1 = *(getJsonMarkersArr().End() - 1); //get last element

	auto marker2 = createMarkerFrom("Marker2", "BadUUID", GetAllocator());
	getJsonMarkersArr().PushBack(*marker2, GetAllocator());

	//writeInFile(*m_Document, "output.json");

	bool canParse{ false };
	try
	{
		m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
		canParse = true;
	}
	catch (chrono::ChException exception)
	{
		canParse = false;
	}	
	EXPECT_FALSE(canParse);
}



///marker1 and marker2 reference each other, e.g. closed loop
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersClosedLoop) {

	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1From = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	getJsonMarkersArr().PushBack(*marker1From, GetAllocator());

	Value& marker1 = *(getJsonMarkersArr().End() - 1); //get last element
	std::string marker1UUid = isymtec_ai_utils::getUUID(marker1);
	auto marker2 = createMarkerFrom("Marker2", marker1UUid, GetAllocator());
	std::string marker2UUid = isymtec_ai_utils::getUUID(*marker2);
	getJsonMarkersArr().PushBack(*marker2, GetAllocator());

	setStringMember(marker1, REFERENCE_CS_ID, marker2UUid, GetAllocator());


	//writeInFile(*m_Document, "output.json");

	bool canParse{ false };
	try
	{
		m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
		canParse = true;
	}
	catch (chrono::ChException exception)
	{
		canParse = false;
	}
	EXPECT_FALSE(canParse);
}




///prove the absolute coordinates of marker by simple translation
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersTestTranslation) {
	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1 = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	ChVector<> absPosRef{ 10, 20, 30 };
	std::string absPosRefStr = ConvertVectorToString(absPosRef);
	setStringMember(*marker1, PROPERTY_TRANSLATION, absPosRefStr, GetAllocator());

	getJsonMarkersArr().PushBack(*marker1, GetAllocator());

	///writeInFile(*m_Document, "output.json");

	m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
	auto& markerlist = m_Body->GetMarkerList();
	size_t markersCount = markerlist.size();
	EXPECT_EQ(markersCount, 1);

	auto chronoMarker = markerlist[0];
	auto& absCoor = chronoMarker->GetAbsCoord();
	const auto& absPos = absCoor.pos;

	EXPECT_TRUE(absPosRef.Equals(absPos));
}





///prove the absolute coordinates of marker by translation of marker and body
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersTestTranslation2) {
	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1 = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	ChVector<> bodyAbsCoor{ 7, 4, 12};
	m_Body->SetFrame_REF_to_abs(ChFrame<>(bodyAbsCoor));

	ChVector<> relPosMarker{ 10, 20, 30 };
	std::string relPosMarkerStr = ConvertVectorToString(relPosMarker);
	setStringMember(*marker1, PROPERTY_TRANSLATION, relPosMarkerStr, GetAllocator());

	getJsonMarkersArr().PushBack(*marker1, GetAllocator());

	///writeInFile(*m_Document, "output.json");7

	m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
	auto& markerlist = m_Body->GetMarkerList();
	size_t markersCount = markerlist.size();
	EXPECT_EQ(markersCount, 1);

	ChVector<> markerAbsPosRef = bodyAbsCoor + relPosMarker;

	auto chronoMarker = markerlist[0];
	auto& absCoor = chronoMarker->GetAbsCoord();
	const auto& absPos = absCoor.pos;

	EXPECT_TRUE(markerAbsPosRef.Equals(absPos));
}



///prove the absolute coordinates of marker by translation and orientation of marker and body
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersTestCoor) {
	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1 = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	ChVector<> bodyAbsPos{ 7, 4, 12 };
	chrono::ChQuaternion<> bodyAbsRot = frames_utils::createQuaternionFromEulerXYZ(ChVector<>(10, 30, 70));

	m_Body->SetFrame_REF_to_abs(ChFrame<>(bodyAbsPos, bodyAbsRot));

	ChVector<> relPosMarker{ 10, 20, 30 };
	std::string relOrientMarker = "33, 90, 20";
	std::string relPosMarkerStr = ConvertVectorToString(relPosMarker);
	setStringMember(*marker1, PROPERTY_TRANSLATION, relPosMarkerStr, GetAllocator());
	setStringMember(*marker1, PROPERTY_ORIENTATION, relOrientMarker, GetAllocator());

	getJsonMarkersArr().PushBack(*marker1, GetAllocator());

	///writeInFile(*m_Document, "output.json");

	m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
	auto& markerlist = m_Body->GetMarkerList();
	size_t markersCount = markerlist.size();
	EXPECT_EQ(markersCount, 1);

	ChVector<> markerAbsPosRef{8.686027700272763, 14.144332326643553, 47.97568390233224};

	auto chronoMarker = markerlist[0];
	auto& absCoor = chronoMarker->GetAbsCoord();
	const auto& absPos = absCoor.pos;

	EXPECT_TRUE(markerAbsPosRef.Equals(absPos, 10E-8));

	auto& quat = absCoor.rot;
	ChVector<> xAxis = quat.GetXaxis();
	ChVector<> yAxis = quat.GetYaxis();
	ChVector<> zAxis = quat.GetZaxis();


	ChVector<> xAxisRef{ -0.9508352378957854, 0.2943438293264457, -0.09630192372486296 };
	ChVector<> yAxisRef{ -0.09043791541871038, 0.03350314998983439, 0.9953383959214458 };
	ChVector<> zAxisRef{ 0.29619813272602397, 0.9551121657052656, -0.005236133250197783 };

	EXPECT_TRUE(xAxis.Equals(xAxisRef, 10E-8));
	EXPECT_TRUE(yAxis.Equals(yAxisRef, 10E-8));
	EXPECT_TRUE(zAxis.Equals(zAxisRef, 10E-8));
}



///prove the absolute coordinates of marker by translation and orientation of two marker and body
TEST_F(ChMarkerParserIsymtecAiTest, ParseBodyMarkersTestCoor2) {
	std::string bodyUUid = isymtec_ai_utils::getUUID(*m_BodyFrom);
	auto marker1 = createMarkerFrom("Marker1", bodyUUid, GetAllocator());

	ChVector<> bodyAbsPos{ 7, 4, 12 };
	chrono::ChQuaternion<> bodyAbsRot = frames_utils::createQuaternionFromEulerXYZ(ChVector<>(10, 30, 70));

	m_Body->SetFrame_REF_to_abs(ChFrame<>(bodyAbsPos, bodyAbsRot));

	ChVector<> relPosMarker1{ 10, 20, 30 };
	std::string relOrientMarker1 = "33, 90, 20";
	std::string relPosMarkerStr1 = ConvertVectorToString(relPosMarker1);
	setStringMember(*marker1, PROPERTY_TRANSLATION, relPosMarkerStr1, GetAllocator());
	setStringMember(*marker1, PROPERTY_ORIENTATION, relOrientMarker1, GetAllocator());
	std::string marker1UUid = isymtec_ai_utils::getUUID(*marker1);

	getJsonMarkersArr().PushBack(*marker1, GetAllocator());

	auto marker2 = createMarkerFrom("Marker2", marker1UUid, GetAllocator());
	ChVector<> relPosMarker2{ -10, 7, 4};
	std::string relOrientMarker2 = "-60, -20, -40";
	std::string relPosMarkerStr2 = ConvertVectorToString(relPosMarker2);
	setStringMember(*marker2, PROPERTY_TRANSLATION, relPosMarkerStr2, GetAllocator());
	setStringMember(*marker2, PROPERTY_ORIENTATION, relOrientMarker2, GetAllocator());
	getJsonMarkersArr().PushBack(*marker2, GetAllocator());

	writeInFile(*m_Document, "output.json");

	m_MarkerParser->ParseBodyMarkers(*m_BodyFrom);
	auto& markerlist = m_Body->GetMarkerList();
	size_t markersCount = markerlist.size();
	EXPECT_EQ(markersCount, 2);

	ChVector<> markerAbsPosRef{ 18.74610720220374, 15.255864746129, 55.885127378030205 };

	auto chronoMarker = markerlist[1];
	auto& absCoor = chronoMarker->GetAbsCoord();
	const auto& absPos = absCoor.pos;

	EXPECT_TRUE(markerAbsPosRef.Equals(absPos, 10E-8));

	auto& quat = absCoor.rot;
	ChVector<> xAxis = quat.GetXaxis();
	ChVector<> yAxis = quat.GetYaxis();
	ChVector<> zAxis = quat.GetZaxis();

	ChVector<> xAxisRef{ -0.4722224217807234, 0.8655201310691735, -0.16697570805824036 };
	ChVector<> yAxisRef{ -0.7901275081064955, -0.33164421472661676, 0.5154712754090056 };
	ChVector<> zAxisRef{ 0.39077423827700053, 0.3753491941544135, 0.8404810986254614 };

	EXPECT_TRUE(xAxis.Equals(xAxisRef, 10E-8));
	EXPECT_TRUE(yAxis.Equals(yAxisRef, 10E-8));
	EXPECT_TRUE(zAxis.Equals(zAxisRef, 10E-8));
}



