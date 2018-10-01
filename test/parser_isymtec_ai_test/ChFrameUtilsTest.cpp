#include "gtest/gtest.h"
#include "parser_isymtec_ai/ChFramesUtils.h"
#include "physics/ChBody.h"

using namespace chrono;

// The fixture for testing class Foo.
class ChFrameUtilsTest : public ::testing::Test {
protected:
	// You can do set-up work for each test here.
	ChFrameUtilsTest() {};

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~ChFrameUtilsTest() {};
};

TEST_F(ChFrameUtilsTest, createMarkerByRotationXToZTest) {
	auto body0 = std::make_shared<ChBody>();
	auto marker0 = std::make_shared<ChMarker>("M1", body0.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
	body0->AddMarker(marker0);
	marker0->Update(0);

	auto marker1 = frames_utils::createMarkerByRotationXToZ(*marker0, "Marker1");
	marker1->Update(0);

	auto& absCoor0 = marker0->GetAbsCoord();
	auto& quat0 = absCoor0.rot;
	ChVector<> xAxis0 = quat0.GetXaxis();
	ChVector<> yAxis0 = quat0.GetYaxis();
	ChVector<> zAxis0 = quat0.GetZaxis();

	auto& absCoor1 = marker1->GetAbsCoord();
	auto& quat1 = absCoor1.rot;
	ChVector<> xAxis1 = quat1.GetXaxis();
	ChVector<> yAxis1 = quat1.GetYaxis();
	ChVector<> zAxis1 = quat1.GetZaxis();

	EXPECT_TRUE(zAxis1.Equals(xAxis0, 10E-8));
}

TEST_F(ChFrameUtilsTest, createMarkerByRotationXToZTest2) {
	auto body0 = std::make_shared<ChBody>();
	auto marker0 = std::make_shared<ChMarker>("M1", body0.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
	body0->AddMarker(marker0);

	auto quat0From = frames_utils::createQuaternionFromEulerXYZ(chrono::ChVector<>(50, 115, 40));
	marker0->SetCoord(chrono::ChVector<>(0.4, 15, 7), quat0From);

	marker0->Update(0);

	auto marker1 = frames_utils::createMarkerByRotationXToZ(*marker0, "Marker1");
	marker1->Update(0);

	auto& absCoor0 = marker0->GetAbsCoord();
	auto& quat0 = absCoor0.rot;
	ChVector<> xAxis0 = quat0.GetXaxis();
	ChVector<> yAxis0 = quat0.GetYaxis();
	ChVector<> zAxis0 = quat0.GetZaxis();

	auto& absCoor1 = marker1->GetAbsCoord();
	auto& quat1 = absCoor1.rot;
	ChVector<> xAxis1 = quat1.GetXaxis();
	ChVector<> yAxis1 = quat1.GetYaxis();
	ChVector<> zAxis1 = quat1.GetZaxis();

	EXPECT_TRUE(zAxis1.Equals(xAxis0, 10E-8));
}

TEST_F(ChFrameUtilsTest, createMarkerByRotationYToZTest2) {
	auto body0 = std::make_shared<ChBody>();
	auto marker0 = std::make_shared<ChMarker>("M1", body0.get(), ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
	body0->AddMarker(marker0);

	auto quat0From = frames_utils::createQuaternionFromEulerXYZ(chrono::ChVector<>(50, 115, 40));
	marker0->SetCoord(chrono::ChVector<>(0.4, 15, 7), quat0From);

	marker0->Update(0);

	auto marker1 = frames_utils::createMarkerByRotationYToZ(*marker0, "Marker1");
	marker1->Update(0);

	auto& absCoor0 = marker0->GetAbsCoord();
	auto& quat0 = absCoor0.rot;
	ChVector<> xAxis0 = quat0.GetXaxis();
	ChVector<> yAxis0 = quat0.GetYaxis();
	ChVector<> zAxis0 = quat0.GetZaxis();

	auto& absCoor1 = marker1->GetAbsCoord();
	auto& quat1 = absCoor1.rot;
	ChVector<> xAxis1 = quat1.GetXaxis();
	ChVector<> yAxis1 = quat1.GetYaxis();
	ChVector<> zAxis1 = quat1.GetZaxis();

	EXPECT_TRUE(zAxis1.Equals(yAxis0, 10E-8));
}