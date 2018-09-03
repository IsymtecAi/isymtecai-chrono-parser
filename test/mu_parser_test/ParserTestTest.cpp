#include "gtest/gtest.h"
#include "muparser/muParser.h"
#include "muparser/muParserTest.h"

namespace {
	static const double PI_TEST = 3.141592653589793238462643383279;
	static const double E_TEST = 2.71828182845904523536;
}


///quick-and dirty testing of Parser test

class ParserTestTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ParserTestTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ParserTestTest() {};

};

//
//namespace {
//	// Function callback
//	double MySqr(double a_fVal)
//	{
//		return a_fVal * a_fVal;
//	}
//
//	// main program
//	int foo()
//	{
//		using namespace mu;
//
//		try
//		{
//			double fVal = 1;
//			Parser p;
//			p.DefineVar("a", &fVal);
//			p.DefineFun("MySqr", MySqr);
//			p.SetExpr("MySqr(a)*pi+min(10,a)");
//
//			for (std::size_t a = 0; a < 100; ++a)
//			{
//				fVal = a;  // Change value of variable a
//				std::cout << p.Eval() << std::endl;
//			}
//		}
//		catch (Parser::exception_type &e)
//		{
//			std::cout << e.GetMsg() << std::endl;
//		}
//		return 0;
//	}
//}
//
//
//TEST_F(ParserUnitTest, Test0) {
//	foo();
//}

TEST_F(ParserTestTest, Test1) {
	mu::Test::ParserTester parserTest;
	bool testsOk = parserTest.Run();
	ASSERT_TRUE(testsOk);
}
