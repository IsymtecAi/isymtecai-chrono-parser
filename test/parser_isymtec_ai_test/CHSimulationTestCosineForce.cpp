#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChSimulationIsymtecAi.h"
#include "chrono/physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChIsymtecAiConstants.h"

using namespace chrono;

namespace {
	class ChSimulationCosineForce : public  ChSimulationIsymtecAi {
	public:
		mutable std::vector<double> m_CoordinateZBody;
		mutable std::vector<double> m_Time;
	private:
		virtual void ProcessOutputTimeStep() const override final {
			auto bodyList = GetSystem()->Get_bodylist();
			auto body = bodyList[0];
			auto bodyPos = body->GetPos();
			m_CoordinateZBody.push_back(bodyPos.z());
			m_Time.push_back(GetSystem()->GetChTime());
		}
	};
}

class ChSimulationCosineForceTest : public ::testing::Test {

protected:
	// You can do set-up work for each test here.
	ChSimulationCosineForceTest() {
	};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChSimulationCosineForceTest() {};

};


TEST_F(ChSimulationCosineForceTest, Test0) {
	ChSimulationCosineForce simulation;
	std::string filename = "isymtecAi/test_cosine_force.isym";
	filename = isymtec_ai_constants::ISYMTEC_DATA_PATH + filename;
	bool parseOk = simulation.Parse(filename);
	EXPECT_TRUE(parseOk);

	simulation.Simulate();

	auto& timeVec = simulation.m_Time;
	double timeRef = 2.;
	double outputTimeStepRef = 0.1;
	size_t countStepsRef = size_t(round(timeRef / outputTimeStepRef) + 1);
	EXPECT_EQ(timeVec.size(), countStepsRef); //< = simulation time / sizeOutputstep + 1
	EXPECT_NEAR(timeVec.back(), timeRef, 10E-7);

	auto& coorZ = simulation.m_CoordinateZBody;
	EXPECT_EQ(coorZ.size(), timeVec.size());

	double minCoor = *std::min_element(coorZ.begin(), coorZ.end());
	EXPECT_GE(minCoor, 0.0);
	double maxCoor = *std::max_element(coorZ.begin(), coorZ.end());


	EXPECT_NEAR(maxCoor, 0.2026, 0.01);
	EXPECT_NEAR(coorZ.back(), 0.0, 0.015);
}
