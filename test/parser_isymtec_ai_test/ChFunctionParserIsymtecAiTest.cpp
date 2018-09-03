#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "ParserTestUtils.h"
#include "parser_isymtec_ai/ChFunctionParserIsymtecAi.h"
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"

using namespace rapidjson;
using namespace chrono;
using namespace parser_test_utils;
using namespace function_isymtec_ai_params;

namespace {

	std::shared_ptr<Value> createFuncFrom(const std::string& name, Document::AllocatorType& curAllocator) {
		auto output = createDummyObject(name, curAllocator);
		addStringMember(*output, FUNC_VALUE, "0", curAllocator);
		addStringMember(*output, DVALUE, "0", curAllocator);
		addStringMember(*output, DDVALUE, "0", curAllocator);
		addIntMember(*output, DERIVATIVES_COUNT, 0, curAllocator);
		return output;
	}
}

// The fixture for testing class Foo.
class ChFunctionParserIsymtecAiTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ChFunctionParserIsymtecAiTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChFunctionParserIsymtecAiTest() {};

	virtual void SetUp() {
		m_Document = new rapidjson::Document;
		m_Document->SetObject();
		m_Relations = std::make_shared<ChRelationsIsymtecAi>();
		auto curSystem = std::make_shared<ChSystemSMC>();
		m_Relations->SetSystem(curSystem);
		m_FunctionStorage = std::make_shared<ChFunctionStorage>();
		m_Parser = std::make_shared<ChFunctionParserIsymtecAi>(m_Relations, m_FunctionStorage);
	}

	Document::AllocatorType& GetAllocator() {
		return m_Document->GetAllocator();
	}

	rapidjson::Document* m_Document;
	std::shared_ptr<ChFunctionParserIsymtecAi> m_Parser;
	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;
	std::shared_ptr<ChFunctionStorage> m_FunctionStorage;
};

///trivial test prove if zero force is generated
TEST_F(ChFunctionParserIsymtecAiTest, ParseFunctionTestZero) {
	auto funcFrom1 = createFuncFrom("Func1",  GetAllocator());
	double funcValueRef = 5.3;
	setStringMember(*funcFrom1, FUNC_VALUE, std::to_string(funcValueRef), GetAllocator());

	m_Parser->ParseObject(*funcFrom1);
	auto function1 = m_Parser->getGenerateObjPtr<ChMuParserFunction>();
	ASSERT_NE(function1, nullptr);

	double funcValue{ function1->Get_y(0.0) };
	EXPECT_EQ(funcValue, funcValueRef);
}

///test sqrt(time) function
TEST_F(ChFunctionParserIsymtecAiTest, ParseFunctionTestSqrt) {
	auto funcFrom1 = createFuncFrom("Func1", GetAllocator());
	std::string funcValueStr = "sqrt(" + isymtec_ai_utils::TIME_VARIABLE + ")";
	setStringMember(*funcFrom1, FUNC_VALUE, funcValueStr, GetAllocator());

	m_Parser->ParseObject(*funcFrom1);
	auto function1 = m_Parser->getGenerateObjPtr<ChMuParserFunction>();
	
	double time{ 4.0 };
	double funcValueRef = sqrt(time);

	double funcValue{ function1->Get_y(time) };
	EXPECT_NEAR(funcValue, funcValueRef, 1E-15);
}


///test sqrt(time) function and derivative
TEST_F(ChFunctionParserIsymtecAiTest, ParseFunctionTestDerivativeSqrt) {
	auto funcFrom1 = createFuncFrom("Func1", GetAllocator());
	std::string funcValueStr = "sqrt(" + isymtec_ai_utils::TIME_VARIABLE + ")";
	std::string funcValueDifStr = "1/2/sqrt(" + isymtec_ai_utils::TIME_VARIABLE + ")";

	setStringMember(*funcFrom1, FUNC_VALUE, funcValueStr, GetAllocator());
	setStringMember(*funcFrom1, DVALUE, funcValueDifStr, GetAllocator());
	setIntMember(*funcFrom1, DERIVATIVES_COUNT, 1, GetAllocator());

	m_Parser->ParseObject(*funcFrom1);
	auto function1 = m_Parser->getGenerateObjPtr<ChMuParserFunction>();

	double time{ 4.0 };
	double funcValueRef{ sqrt(time) };

	double funcValue{ function1->Get_y(time) };
	EXPECT_NEAR(funcValue, funcValueRef, 1E-15);

	double funcDValueRef{ 1 / sqrt(time) / 2};
	double funcDValue { function1->Get_y_dx(time) };

	EXPECT_NEAR(funcDValue, funcDValueRef, 1E-15);
}




///test sqrt(time) function and two derivatives
TEST_F(ChFunctionParserIsymtecAiTest, ParseFunctionTestTwoDerivativesSqrt) {
	auto funcFrom1 = createFuncFrom("Func1", GetAllocator());
	std::string funcValueStr = "sqrt(" + isymtec_ai_utils::TIME_VARIABLE + ")";
	std::string funcValueDifStr = "1/2/sqrt(" + isymtec_ai_utils::TIME_VARIABLE + ")";
	std::string funcValueDifDifStr = "-1/4/" + isymtec_ai_utils::TIME_VARIABLE + "/sqrt(" + isymtec_ai_utils::TIME_VARIABLE + ")";

	setStringMember(*funcFrom1, FUNC_VALUE, funcValueStr, GetAllocator());
	setStringMember(*funcFrom1, DVALUE, funcValueDifStr, GetAllocator());
	setStringMember(*funcFrom1, DDVALUE, funcValueDifDifStr, GetAllocator());
	setIntMember(*funcFrom1, DERIVATIVES_COUNT, 2, GetAllocator());

	m_Parser->ParseObject(*funcFrom1);
	auto function1 = m_Parser->getGenerateObjPtr<ChMuParserFunction>();

	double time{ 4.0 };
	double funcValueRef{ sqrt(time) };

	double funcValue{ function1->Get_y(time) };
	EXPECT_NEAR(funcValue, funcValueRef, 1E-15);

	double funcDValueRef{ 1. / sqrt(time) / 2. };
	double funcDValue{ function1->Get_y_dx(time) };
	EXPECT_NEAR(funcDValue, funcDValueRef, 1E-15);

	double funcDDValueRef{ -1. / 4. / time / sqrt(time) };
	double funcDDValue{ function1->Get_y_dxdx(time) };
	EXPECT_NEAR(funcDDValue, funcDDValueRef, 1E-15);


}



