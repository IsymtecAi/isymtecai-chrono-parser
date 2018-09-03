#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "muparser/muParser.h"

using namespace chrono;



// The fixture for testing class Foo.
class ChMuParserFunctionTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ChMuParserFunctionTest() {};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChMuParserFunctionTest() {};
};



TEST_F(ChMuParserFunctionTest, getYTest) {
	ChMuParserFunction function0;
	double valueRef = 5.0;
	function0.SetExpresson(std::to_string(valueRef));
	double valueCalc = function0.Get_y(0);
	ASSERT_EQ(valueCalc, valueRef);
}



TEST_F(ChMuParserFunctionTest, getYTestPiSmall) {
	ChMuParserFunction function0;
	double valueRef = CH_C_PI;
	function0.SetExpresson("pi");
	double valueCalc = function0.Get_y(0);
	ASSERT_EQ(valueCalc, valueRef);
}

TEST_F(ChMuParserFunctionTest, getYTestPiBig) {
	ChMuParserFunction function0;
	double valueRef = CH_C_PI;
	function0.SetExpresson("PI");
	double valueCalc = function0.Get_y(0);
	ASSERT_EQ(valueCalc, valueRef);
}


TEST_F(ChMuParserFunctionTest, getYTestESmall) {
	ChMuParserFunction function0;
	double valueRef = CH_C_E;
	function0.SetExpresson("e");
	double valueCalc = function0.Get_y(0);
	ASSERT_EQ(valueCalc, valueRef);
}


TEST_F(ChMuParserFunctionTest, getYTestEBig) {
	ChMuParserFunction function0;
	double valueRef = CH_C_E;
	function0.SetExpresson("E");
	double valueCalc = function0.Get_y(0);
	ASSERT_EQ(valueCalc, valueRef);
}



///calculate square of input value
TEST_F(ChMuParserFunctionTest, getYTestSquare) {
	ChMuParserFunction function0;
	//auto parser = function0.getParser();

	double xValue = 3;
	double valueRef = xValue * xValue;
	function0.DefineInputVariable("inp1");
	function0.SetExpresson("inp1 * inp1");

	double valueCalc = function0.Get_y(xValue);
	ASSERT_NEAR(valueCalc, valueRef, 10E-14);
}




//
//
//TEST_F(ChMuParserFunctionTest, CloneTest) {
//	ChMuParserFunction function0;
//	auto parser0 = function0.getParser();
//
//	double valueRef0 = 5.0;
//	parser0->SetExpr(std::to_string(valueRef0));
//
//	///clone function1 from function 0 and prove it has the same value as function 0
//	ChMuParserFunction& function1 = *function0.Clone();
//	double valueCalc0_0 = function0.Get_y(0);
//	EXPECT_EQ(valueCalc0_0, valueRef0);
//
//	double valueCalc0_1 = function1.Get_y(0);
//	EXPECT_EQ(valueCalc0_1, valueRef0);
//
//
//	///change parser1 and prove, it has no influence on function 0
//	double valueRef1 = 7.0;
//	parser0->SetExpr(std::to_string(valueRef1));
//
//	double valueCalc1_1 = function1.Get_y(0);
//	EXPECT_EQ(valueCalc1_1, valueRef0);
//
//	double valueCalc1_0 = function0.Get_y(0);
//	EXPECT_EQ(valueCalc1_0, valueRef1);
//
//
//	delete &function1;
//}
