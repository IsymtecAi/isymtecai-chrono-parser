#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChSimulationIsymtecAi.h"
#include "chrono/physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiConstants.h"

using namespace chrono;

namespace {
	class ChSimulationSpringPendulum : public  ChSimulationIsymtecAi {
	public:
		mutable std::vector<double> m_CoordinateXBody;
		mutable std::vector<double> m_Time;
	private:
		virtual void ProcessOutputTimeStep() const override final {
			auto bodyList = GetSystem()->Get_bodylist();
			auto body = bodyList[1];
			auto bodyPos = body->GetPos();
			m_CoordinateXBody.push_back(bodyPos.x());
			m_Time.push_back(GetSystem()->GetChTime());
		}
	};
}

// The fixture for testing class Foo.
class ChSimulationSpringPendulumTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ChSimulationSpringPendulumTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChSimulationSpringPendulumTest() {};

};


TEST_F(ChSimulationSpringPendulumTest, Test0) {
	ChSimulationSpringPendulum simulation;
	std::string filename = "isymtecAi/test_spring_pendulum.isym";
	filename = isymtec_ai_constants::ISYMTEC_DATA_PATH + filename;
	bool parseOk = simulation.Parse(filename);
	EXPECT_TRUE(parseOk);

	simulation.Simulate();

	auto& timeVec = simulation.m_Time;
	double timeRef = 20;
	double outputTimeStepRef = 0.1;
	unsigned countStepsRef = unsigned(round(timeRef / outputTimeStepRef) + 1);
	EXPECT_NEAR((unsigned)timeVec.size(), countStepsRef, 20); //< = simulation time / sizeOutputstep + 1
	EXPECT_NEAR(timeVec.back(), timeRef, 10E-7);

	auto& coorX = simulation.m_CoordinateXBody;
	EXPECT_EQ(coorX.size(), timeVec.size());

	double minCoor = *std::min_element(coorX.begin(), coorX.end());
	EXPECT_NEAR(minCoor, 0.8, 0.2);
	double maxCoor = *std::max_element(coorX.begin(), coorX.end());

	EXPECT_NEAR(maxCoor, 10.0, 0.01);
	EXPECT_NEAR(coorX.back(), 5.0, 0.01);
}
