#include "gtest/gtest.h"
#include "ParserTestUtils.h"

#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"

#include "parser_isymtec_ai/ChFramesUtils.h"
#include "parser_isymtec_ai/ChLinkParserIsymtecAi.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"

using namespace rapidjson;
using namespace chrono;
using namespace link_isymtec_ai_params;
using namespace parser_test_utils;

class ChLinkParserIsymtecAiTest : public ::testing::Test {
protected:
	// You can do set-up work for each test here.
	ChLinkParserIsymtecAiTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChLinkParserIsymtecAiTest() {};

	std::shared_ptr<Value> createDistanceLinkFrom() {
		auto& curAllocator = GetAllocator();
		auto output = createDummyObject("Link1", curAllocator);
		addStringMember(*output, LINK_TYPE, DISTANCE, curAllocator);
		std::string markerUUid1 = isymtec_ai_utils::getUUID(*m_Marker1From);
		std::string markerUUid2 = isymtec_ai_utils::getUUID(*m_Marker2From);
		addStringMember(*output, CS1_ID, markerUUid1, curAllocator);
		addStringMember(*output, CS2_ID, markerUUid2, curAllocator);
		return output;
	}

	void addMarker2(std::shared_ptr<chrono::ChBodyAuxRef> body) {
		m_Marker2 = std::make_shared<ChMarker>("Marker2", body.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
		m_Marker2From = createDummyObject("Marker2", GetAllocator());
		m_Relations->AddRelation(*m_Marker2From, m_Marker2);
		body->AddMarker(m_Marker2);
	}

	virtual void SetUp() {
		m_Document = new rapidjson::Document;
		m_Document->SetObject();
		m_Relations = std::make_shared<ChRelationsIsymtecAi>();
		m_System = std::make_shared<ChSystemSMC>();
		m_Relations->SetSystem(m_System);
		m_FunctionStorage = std::make_shared<ChFunctionStorage>();
		m_Parser = std::make_shared<ChLinkParserIsymtecAi>(m_Relations, m_FunctionStorage);
		m_Body = std::make_shared<ChBodyAuxRef>();
		m_Body->SetName("m_Body");
		m_System->AddBody(m_Body);

		m_Body2 = std::make_shared<ChBodyAuxRef>();
		m_Body2->SetName("m_Body2");
		m_System->AddBody(m_Body2);

		m_Marker1 = std::make_shared<ChMarker>("Marker1", m_Body.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
		m_Body->AddMarker(m_Marker1);
		m_Marker1From = createDummyObject("Marker1", GetAllocator());
		m_Relations->AddRelation(*m_Marker1From, m_Marker1);

		addMarker2(m_Body2);
	}

	virtual void TearDown() {
		delete m_Document;
	}

	Document::AllocatorType& GetAllocator() {
		return m_Document->GetAllocator();
	}

	rapidjson::Document* m_Document;

	std::shared_ptr<ChLinkParserIsymtecAi> m_Parser;
	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;
	std::shared_ptr<ChFunctionStorage> m_FunctionStorage;
	//Value* m_BodyFrom;
	std::shared_ptr<chrono::ChBodyAuxRef> m_Body2;
	std::shared_ptr<chrono::ChBodyAuxRef> m_Body;
	std::shared_ptr<chrono::ChMarker> m_Marker1;
	std::shared_ptr<Value> m_Marker1From;

	std::shared_ptr<chrono::ChMarker> m_Marker2;
	std::shared_ptr<Value> m_Marker2From;

	std::shared_ptr<ChSystemSMC> m_System;
};

///trivial test prove if force is added to the body
TEST_F(ChLinkParserIsymtecAiTest, ParseObjectTestIsAdded) {
	auto linkFrom1 = createDistanceLinkFrom();
	m_Parser->ParseObject(*linkFrom1);
	auto& linkList = m_System->Get_linklist();
	EXPECT_EQ(linkList.size(), 1);
}


///test when body1 and marker1 translated
TEST_F(ChLinkParserIsymtecAiTest, ParseTranslatedBody) {
	auto linkFrom1 = createDistanceLinkFrom();

	ChVector<> bodyTranslation{ 10, 0, 0 };
	ChFrame<> bodyRefToAbs{ bodyTranslation, chrono::ChQuaternion<>() };
	m_Body->SetFrame_REF_to_abs(bodyRefToAbs);

	ChVector<> markerTranslation{ 20, 0, 0 };
	Coordsys absCoor{ markerTranslation, chrono::ChQuaternion<>() };
	m_Marker1->Impose_Abs_Coord(absCoor);

	m_Parser->ParseObject(*linkFrom1);

	auto& link1 = dynamic_cast<ChLinkDistance&>(*m_System->Get_linklist()[0]);
	link1.Update(0.0);

	double distance = link1.GetCurrentDistance();
	EXPECT_NEAR(distance, markerTranslation[0], 1E-10);
}