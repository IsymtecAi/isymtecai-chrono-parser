#include "gtest/gtest.h"
#include "ParserTestUtils.h"

#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"

#include "parser_isymtec_ai/ChFramesUtils.h"
#include "parser_isymtec_ai/ChForceParserIsymtecAi.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"

using namespace rapidjson;
using namespace chrono;
using namespace force_isymtec_ai_params;
using namespace parser_test_utils;

namespace {

	void setForceValue(const ChVector<> forceValue, Value& forceFrom, Document::AllocatorType& allocator1) {
		std::string valueStr = ConvertVectorToString(forceValue);
		setStringMember(forceFrom, applied_force_isymtec_ai_params::FORCE_VALUE, valueStr, allocator1);
	}

	void setTorqueValue(const ChVector<> torqueValue, Value& forceFrom, Document::AllocatorType& allocator1) {
		std::string valueStr = ConvertVectorToString(torqueValue);
		setStringMember(forceFrom, applied_force_isymtec_ai_params::TORQUE_VALUE, valueStr, allocator1);
	}

}

class ChAppliedForceParserIsymtecAiTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ChAppliedForceParserIsymtecAiTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChAppliedForceParserIsymtecAiTest() {};

	std::shared_ptr<Value> createAppliedForceFrom() {
		auto& curAllocator = GetAllocator();
		auto output = createDummyObject("Force1", curAllocator);
		addStringMember(*output, FORCE_TYPE, APPLIED_FORCE, curAllocator);
		addStringMember(*output, applied_force_isymtec_ai_params::FORCE_VALUE, "0, 0, 0", curAllocator);
		addStringMember(*output, applied_force_isymtec_ai_params::TORQUE_VALUE, "0, 0, 0", curAllocator);
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
		auto curSystem = std::make_shared<ChSystemSMC>();
		m_Relations->SetSystem(curSystem);
		m_FunctionStorage = std::make_shared<ChFunctionStorage>();
		m_Parser = std::make_shared<ChForceParserIsymtecAi>(m_Relations, m_FunctionStorage);
		m_Body = std::make_shared<ChBodyAuxRef>();
		
		m_Marker1 = std::make_shared<ChMarker>("Marker1", m_Body.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
		m_Body->AddMarker(m_Marker1);
		m_Marker1From = createDummyObject("Marker1", GetAllocator());
		m_Relations->AddRelation(*m_Marker1From, m_Marker1);

	}

	virtual void TearDown() {
		delete m_Document;
	}

	Document::AllocatorType& GetAllocator() {
		return m_Document->GetAllocator();
	}

	rapidjson::Document* m_Document;
	
	std::shared_ptr<ChForceParserIsymtecAi> m_Parser;
	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;
	std::shared_ptr<ChFunctionStorage> m_FunctionStorage;
	//Value* m_BodyFrom;
	std::shared_ptr<chrono::ChBodyAuxRef> m_Body;	
	std::shared_ptr<chrono::ChMarker> m_Marker1;
	std::shared_ptr<Value> m_Marker1From;

	std::shared_ptr<chrono::ChMarker> m_Marker2;
	std::shared_ptr<Value> m_Marker2From;

};


///trivial test prove if force is added to the body
TEST_F(ChAppliedForceParserIsymtecAiTest, ParseObjectTestIsAdded) {
	addMarker2(m_Body);
	auto forceFrom1 = createAppliedForceFrom();

	m_Parser->ParseObject(*forceFrom1);
	auto& forcelist = m_Body->GetForceList();
	size_t forceCount = forcelist.size();
	EXPECT_EQ(forceCount, 2);
}


///test  force applied on center of mass 
TEST_F(ChAppliedForceParserIsymtecAiTest, ParseObjectTestCentralForce) {
	addMarker2(m_Body);
	auto forceFrom1 = createAppliedForceFrom();

	ChVector<> absForceRef{ 10, 20, 30 };
	setForceValue(absForceRef, *forceFrom1, GetAllocator());

	m_Parser->ParseObject(*forceFrom1);

	m_Body->UpdateForces(0.0);
	ChVector<> absForce = m_Body->Get_Xforce();
	EXPECT_TRUE(absForce.Equals(absForceRef));
}


///test force when body is rotated
TEST_F(ChAppliedForceParserIsymtecAiTest, ParseObjectTestRotatedBody) {
	auto ground = std::make_shared<ChBodyAuxRef>();
	addMarker2(ground);
	auto forceFrom1 = createAppliedForceFrom();

	ChVector<> absForceRef{ 10, 20, 30 };
	setForceValue(absForceRef, *forceFrom1, GetAllocator());

	ChVector<> orientationBody{ 15, 45, 90 }; 
	chrono::ChQuaternion<> quatBody = frames_utils::createQuaternionFromEulerXYZ(orientationBody);
	ChFrame<> refToAbs { ChVector<>(), quatBody };
	m_Body->SetFrame_REF_to_abs(refToAbs);

	m_Parser->ParseObject(*forceFrom1);
	m_Body->UpdateMarkers(0.0);
	m_Body->UpdateForces(0.0);
	ChVector<> absForce = m_Body->Get_Xforce();
	EXPECT_TRUE(absForce.Equals(absForceRef));
}



///test force applied not on center of mass 
TEST_F(ChAppliedForceParserIsymtecAiTest, ParseObjectTestAppliedNonCenter) {
	auto ground = std::make_shared<ChBodyAuxRef>();
	addMarker2(ground);
	auto forceFrom1 = createAppliedForceFrom();

	ChVector<> absForceRef{ 11, 23, 3 };
	setForceValue(absForceRef, *forceFrom1, GetAllocator());

	ChVector<> markerTranslation{ 7, 11, 13 };
	chrono::ChQuaternion<> markerQuat = frames_utils::createQuaternionFromEulerXYZ(ChVector<>{20, 30, 40});
	Coordsys absCoor{ markerTranslation, markerQuat };

	m_Marker1->Impose_Abs_Coord(absCoor);

	m_Parser->ParseObject(*forceFrom1);


	m_Body->UpdateForces(0.0);
	ChVector<> absForce = m_Body->Get_Xforce();
	EXPECT_TRUE(absForce.Equals(absForceRef));


	ChVector<> localTorque = m_Body->Get_Xtorque();
	auto absTorqueRef = Vcross(markerTranslation, absForceRef);
	EXPECT_TRUE(localTorque.Equals(absTorqueRef)); //<body does not rotate, that is why local and abs are equall
}




///test force applied not on center of mass 
TEST_F(ChAppliedForceParserIsymtecAiTest, ParseObjectTestAppliedNonCenterRotatedBody) {
	auto ground = std::make_shared<ChBodyAuxRef>();
	addMarker2(ground);
	auto forceFrom1 = createAppliedForceFrom();

	ChVector<> absForceRef{ 3, 20, 45};
	setForceValue(absForceRef, *forceFrom1, GetAllocator());


	ChVector<> orientationBody{ 20, 15, 33};
	chrono::ChQuaternion<> quatBody = frames_utils::createQuaternionFromEulerXYZ(orientationBody);
	ChFrame<> refToAbs{ ChVector<>(), quatBody };
	m_Body->SetFrame_REF_to_abs(refToAbs);


	ChVector<> markerTranslation{ 15, 44, 7 };
	chrono::ChQuaternion<> markerQuat = frames_utils::createQuaternionFromEulerXYZ(ChVector<>{20, 30, 40});
	Coordsys absCoor{ markerTranslation, markerQuat };
	m_Marker1->Impose_Abs_Coord(absCoor);

	m_Parser->ParseObject(*forceFrom1);


	m_Body->UpdateForces(0.0);
	ChVector<> absForce = m_Body->Get_Xforce();
	EXPECT_TRUE(absForce.Equals(absForceRef));


	ChVector<> localTorque = m_Body->Get_Xtorque();
	ChVector<> absTorque = m_Body->Dir_Body2World(localTorque);
	auto absTorqueRef = Vcross(markerTranslation, absForceRef);
	EXPECT_TRUE(absTorque.Equals(absTorqueRef, 10E-8));
}




///test torque when body is rotated
TEST_F(ChAppliedForceParserIsymtecAiTest, ParseObjectTestTorqueRotatedBody) {
	auto ground = std::make_shared<ChBodyAuxRef>();
	addMarker2(ground);
	auto forceFrom1 = createAppliedForceFrom();

	ChVector<> absTorqueRef{ 10, 20, 30 };
	setTorqueValue(absTorqueRef, *forceFrom1, GetAllocator());

	ChVector<> orientationBody{ 15, 45, 90 };
	chrono::ChQuaternion<> quatBody = frames_utils::createQuaternionFromEulerXYZ(orientationBody);
	ChFrame<> refToAbs{ ChVector<>(), quatBody };
	m_Body->SetFrame_REF_to_abs(refToAbs);

	m_Parser->ParseObject(*forceFrom1);

	m_Body->UpdateForces(0.0);

	ChVector<> localTorque = m_Body->Get_Xtorque();
	ChVector<> absTorque = m_Body->Dir_Body2World(localTorque);
	EXPECT_TRUE(absTorque.Equals(absTorqueRef, 10E-8));
}


