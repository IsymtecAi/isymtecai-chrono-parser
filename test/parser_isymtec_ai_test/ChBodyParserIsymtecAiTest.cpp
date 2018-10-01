#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChBodyParserIsymtecAi.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "ParserTestUtils.h"

using namespace rapidjson;
using namespace chrono;
using namespace parser_test_utils;

namespace {
	///Create .json body object with all needed properties
	std::shared_ptr<Value> createDummyBodyFrom(Document& document) {
		using namespace body_isymtec_ai_params;
		Document::AllocatorType& curAllocator = document.GetAllocator();
		//auto bodyFrom = std::make_shared<Value >(rapidjson::kObjectType);
		auto bodyFrom = createDummyObject("Body", curAllocator);

		addStringMember(*bodyFrom, PROPERTY_MASS, "1", curAllocator);
		addStringMember(*bodyFrom, PROPERTY_CENTER_OF_MASS, "0, 0, 0", curAllocator);
		addStringMember(*bodyFrom, PROPERTY_INERTIA, "1, 0, 0; 0, 1, 0; 0, 0, 1", curAllocator);
		addStringMember(*bodyFrom, PROPERTY_TRANSLATION, "0, 0, 0", curAllocator);
		addStringMember(*bodyFrom, PROPERTY_ORIENTATION, "0, 0, 0", curAllocator);
		addStringMember(*bodyFrom, PROPERTY_ANGULAR_VEL, "0, 0, 0", curAllocator);
		addStringMember(*bodyFrom, PROPERTY_LINEAR_VEL, "0, 0, 0", curAllocator);
		addStringMember(*bodyFrom, isymtec_ai_utils::PROPERTY_NAME, "my_body_A", curAllocator);
		bodyFrom->AddMember(StringRef(PROPERTY_FIXED.c_str()), false, curAllocator);
		return bodyFrom;
	}

	std::shared_ptr<ChBodyParserIsymtecAi> createBodyParser() {
		// Make a system
		auto curSystem = std::make_shared<ChSystemSMC>();

		auto relations = std::make_shared<ChRelationsIsymtecAi>();
		relations->SetSystem(curSystem);
		auto bodyParser = std::make_shared<ChBodyParserIsymtecAi>(relations);
		return bodyParser;
	}
}

// The fixture for testing class Foo.
class ChBodyParserIsymtecAiTest : public ::testing::Test {
protected:
	// You can do set-up work for each test here.
	ChBodyParserIsymtecAiTest() {
		m_BodyParser = createBodyParser();
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChBodyParserIsymtecAiTest() {};

	std::shared_ptr<Value> createBodyFrom() {
		return createDummyBodyFrom(m_Document);
	}

	Document::AllocatorType& GetAllocator() {
		return m_Document.GetAllocator();
	}

	rapidjson::Document m_Document;
	std::shared_ptr<ChBodyParserIsymtecAi> m_BodyParser;
};

TEST_F(ChBodyParserIsymtecAiTest, ParseTranslationTest) {
	using namespace body_isymtec_ai_params;

	auto bodyFrom = createBodyFrom();

	ChVector<> absPosRef{ 10, 20, 30 };
	std::string absPosRefStr = ConvertVectorToString(absPosRef);
	setStringMember(*bodyFrom, PROPERTY_TRANSLATION, absPosRefStr, GetAllocator());

	m_BodyParser->ParseObject(*bodyFrom);
	auto& body = *m_BodyParser->getGenerateObjPtr<chrono::ChBodyAuxRef>();

	auto& absCoor = body.GetFrame_REF_to_abs();
	auto& absPos = absCoor.GetPos();

	EXPECT_TRUE(absPosRef.Equals(absPos));
}

///test rotation by rotation about x on 90 degrees
TEST_F(ChBodyParserIsymtecAiTest, ParseRotationsTest_90DegX) {
	using namespace body_isymtec_ai_params;
	auto bodyFrom = createBodyFrom();

	std::string absOrientRefStr = "90, 0, 0";
	setStringMember(*bodyFrom, PROPERTY_ORIENTATION, absOrientRefStr, GetAllocator());

	m_BodyParser->ParseObject(*bodyFrom);
	auto& body = *m_BodyParser->getGenerateObjPtr<chrono::ChBodyAuxRef>();

	//need to use static cast because GetRotAxis is non-cost
	//auto& absCoor = static_cast<ChFrameMoving<>>(body.GetFrame_REF_to_abs());
	auto absCoor = body.GetFrame_REF_to_abs();

	double rotAngle = absCoor.GetRotAngle();
	EXPECT_NEAR(rotAngle, CH_C_PI / 2, 1E-12);

	ChVector<> absAxis = absCoor.GetRotAxis();
	ChVector<> absAxisRef(1, 0, 0);
	EXPECT_TRUE(absAxis.Equals(absAxisRef));
}

///test complex rotation
TEST_F(ChBodyParserIsymtecAiTest, ParseRotationsTest_Complex) {
	using namespace body_isymtec_ai_params;
	auto bodyFrom = createBodyFrom();

	std::string absOrientRefStr = "30, 70, 15";
	setStringMember(*bodyFrom, PROPERTY_ORIENTATION, absOrientRefStr, GetAllocator());

	m_BodyParser->ParseObject(*bodyFrom);
	auto& body = *m_BodyParser->getGenerateObjPtr<chrono::ChBodyAuxRef>();

	//need to use static cast because GetRotAxis is non-cost
	//auto& absCoor = static_cast<ChFrameMoving<>>(body.GetFrame_REF_to_abs());
	auto absCoor = body.GetFrame_REF_to_abs();

	auto& quat = absCoor.GetRot();
	ChVector<> xAxis = quat.GetXaxis();
	ChVector<> yAxis = quat.GetYaxis();
	ChVector<> zAxis = quat.GetZaxis();
	//ChMatrix33<> mC(quat);

	ChVector<> xAxisRef{ 0.3303660895493522, 0.6779805536371977, -0.6566586752382613 };
	EXPECT_TRUE(xAxis.Equals(xAxisRef, 10E-8));

	ChVector<> yAxisRef{ -0.08852132690137689, 0.714911130336961, 0.6935892519380245 };
	EXPECT_TRUE(yAxis.Equals(yAxisRef, 10E-8));

	ChVector<> zAxisRef{ 0.9396926207859083, -0.17101007166283438, 0.29619813272602397 };
	EXPECT_TRUE(zAxis.Equals(zAxisRef, 10E-8));
}

///test complex rotation
TEST_F(ChBodyParserIsymtecAiTest, ParseInertiaTest) {
	using namespace body_isymtec_ai_params;
	auto bodyFrom = createBodyFrom();

	double massRef = 5303.972;
	ChVector<> centerOfMassRef(-5.18566, 0.42927, 12.1929);
	std::string inertia = "1.170E+07,  5.458E+04,  1.977E+06;  5.458E+04,  1.220E+07,  -1.283E+05;  1.977E+06,  -1.283E+05,  1.370E+07";
	setStringMember(*bodyFrom, PROPERTY_MASS, std::to_string(massRef), GetAllocator());
	std::string centerOfMassStr = ConvertVectorToString(centerOfMassRef);
	setStringMember(*bodyFrom, PROPERTY_CENTER_OF_MASS, centerOfMassStr, GetAllocator());
	setStringMember(*bodyFrom, PROPERTY_INERTIA, inertia, GetAllocator());

	m_BodyParser->ParseObject(*bodyFrom);
	auto& body = *m_BodyParser->getGenerateObjPtr<chrono::ChBodyAuxRef>();
	double massProve = body.GetMass();

	EXPECT_NEAR(massRef, massProve, 10E-12);

	ChVector<> centerOfMassProve = body.GetFrame_COG_to_REF().GetPos();
	bool sameCenterOfMass = centerOfMassRef.Equals(centerOfMassProve, 10E-12);
	EXPECT_TRUE(sameCenterOfMass);

	auto& inertiaCentralProve = body.GetInertia();

	ChMatrix33<> inertiaCentralRef;
	inertiaCentralRef[0][0] = 1.091E+07;
	inertiaCentralRef[0][1] = 4.277E+04;
	inertiaCentralRef[0][2] = 1.642E+06;
	inertiaCentralRef[1][0] = 4.277E+04;
	inertiaCentralRef[1][1] = 1.127E+07;
	inertiaCentralRef[1][2] = -1.006E+05;
	inertiaCentralRef[2][0] = 1.642E+06;
	inertiaCentralRef[2][1] = -1.006E+05;
	inertiaCentralRef[2][2] = 1.356E+07;

	bool sameInertia = inertiaCentralRef.Equals(inertiaCentralProve, 10E+3);
	EXPECT_TRUE(sameInertia);
}

///test angular and linear velocities
TEST_F(ChBodyParserIsymtecAiTest, ParseVelocitiesTest) {
	using namespace body_isymtec_ai_params;
	auto bodyFrom = createBodyFrom();

	double massRef = 5303.972;
	ChVector<> centerOfMassRef(-5.18566, 0.42927, 12.1929);
	std::string inertia = "1.170E+07,  5.458E+04,  1.977E+06;  5.458E+04,  1.220E+07,  -1.283E+05;  1.977E+06,  -1.283E+05,  1.370E+07";
	setStringMember(*bodyFrom, PROPERTY_MASS, std::to_string(massRef), GetAllocator());
	std::string centerOfMassStr = ConvertVectorToString(centerOfMassRef);
	setStringMember(*bodyFrom, PROPERTY_CENTER_OF_MASS, centerOfMassStr, GetAllocator());
	setStringMember(*bodyFrom, PROPERTY_INERTIA, inertia, GetAllocator());

	ChVector<> angularVelAbsRef(3.0, 5.0, 7.0);
	std::string angularVelAbsRefStr = ConvertVectorToString(angularVelAbsRef);
	setStringMember(*bodyFrom, PROPERTY_ANGULAR_VEL, angularVelAbsRefStr, GetAllocator());

	ChVector<> linearVelAbsRef(1.0, 11.0, 4.0);
	std::string linearVelAbsRefStr = ConvertVectorToString(linearVelAbsRef);
	setStringMember(*bodyFrom, PROPERTY_LINEAR_VEL, linearVelAbsRefStr, GetAllocator());

	std::string absOrientRefStr = "90, 0, 0";
	setStringMember(*bodyFrom, PROPERTY_ORIENTATION, absOrientRefStr, GetAllocator());

	///parse
	m_BodyParser->ParseObject(*bodyFrom);
	auto& body = *m_BodyParser->getGenerateObjPtr<chrono::ChBodyAuxRef>();

	ChVector<>linVelAbs = body.GetFrame_REF_to_abs().GetPos_dt();
	EXPECT_TRUE(linVelAbs.Equals(linearVelAbsRef, 10E-8));

	ChVector<>angVelLoc = body.GetFrame_REF_to_abs().GetWvel_loc();
	ChVector<> angVelAbs = body.Dir_Body2World(angVelLoc);
	EXPECT_TRUE(angVelAbs.Equals(angularVelAbsRef, 10E-8));
}