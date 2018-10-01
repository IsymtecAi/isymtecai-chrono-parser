#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChSimulationIsymtecAi.h"
#include "chrono/physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiConstants.h"

using namespace chrono;

namespace {
	class ChSimulationFourBar : public  ChSimulationIsymtecAi {
	public:
		mutable std::vector<double> m_AngularVelocityLocBodyB;
		mutable std::vector<double> m_Time;
	private:
		virtual void ProcessOutputTimeStep() const override final {
			auto bodyList = GetSystem()->Get_bodylist();
			auto bodyB = bodyList[1];
			auto bodyBAngularVelLocal = bodyB->GetWvel_loc();
			m_AngularVelocityLocBodyB.push_back(bodyBAngularVelLocal.z());
			m_Time.push_back(GetSystem()->GetChTime());
		}
	};
}

// The fixture for testing class Foo.
class ChSimulationFourBarTest : public ::testing::Test {
protected:
	// You can do set-up work for each test here.
	ChSimulationFourBarTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChSimulationFourBarTest() {};
};

TEST_F(ChSimulationFourBarTest, Test0) {
	ChSimulationFourBar simulation;
	std::string filename = "isymtecAi/test_fourbar.isym";
	filename = isymtec_ai_constants::ISYMTEC_DATA_PATH + filename;
	bool parseOk = simulation.Parse(filename);
	EXPECT_TRUE(parseOk);

	simulation.Simulate();

	auto& timeVec = simulation.m_Time;
	EXPECT_NEAR(double(timeVec.size()), 51, 2); // = simulation time / sizeOutputstep + 1 (can be less, if integrator timestep is too big)
	EXPECT_NEAR(timeVec.back(), 5, 0.01);

	auto& angularVel = simulation.m_AngularVelocityLocBodyB;
	EXPECT_EQ(angularVel.size(), timeVec.size());

	double minVel = *std::min_element(angularVel.begin(), angularVel.end());
	double maxVel = *std::max_element(angularVel.begin(), angularVel.end());
	double velRef = -CH_C_PI;

	EXPECT_NEAR(minVel, velRef, 10E-2);
	EXPECT_NEAR(maxVel, velRef, 10E-2);
}