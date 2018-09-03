#include "parser_isymtec_ai/ChFramesUtils.h"
#include "physics/ChObject.h"
#include "core/ChVector.h"
#include "chrono/physics/ChMarker.h"
#include "physics/ChBody.h"


using namespace chrono;

namespace
{
	///create marker, rotated with respect to previous one by quaternion
	std::shared_ptr<chrono::ChMarker> createRotatedMarker(const chrono::ChMarker& markerFrom, std::string markerName, ChQuaternion<> quat)
	{
		auto rot1 = markerFrom.GetRot();
		auto* body = markerFrom.GetBody();
		auto rotNew = rot1 * quat;
		auto marker = std::make_shared<ChMarker>(markerName, body, ChCoordsys<>(), ChCoordsys<>(), ChCoordsys<>());
		marker->SetCoord(markerFrom.GetPos(), rotNew);
		body->AddMarker(marker);
		return marker;
	}

}

namespace frames_utils {

	chrono::ChQuaternion<> createQuaternionFromEulerXYZ(const chrono::ChVector<>& eulerAnglesXYZ)
	{
		ChVector<> orientationRad = CH_C_DEG_TO_RAD * eulerAnglesXYZ;

		ChQuaternion<> quatX;
		quatX.Q_from_AngX(orientationRad[0]);

		ChQuaternion<> quatY;
		quatY.Q_from_AngY(orientationRad[1]);

		ChQuaternion<> quatZ;
		quatZ.Q_from_AngZ(orientationRad[2]);

		ChQuaternion<> quat = quatX * quatY * quatZ;
		return quat;
	}

	std::shared_ptr<chrono::ChMarker> createMarkerByRotationXToZ(const chrono::ChMarker& markerFrom, std::string markerName)
	{
		ChQuaternion<> quatZ;
		quatZ.Q_from_AngY(CH_C_PI / 2);
		return createRotatedMarker(markerFrom, markerName, quatZ);
	}

	std::shared_ptr<chrono::ChMarker> createMarkerByRotationYToZ(const chrono::ChMarker& markerFrom, std::string markerName)
	{
		ChQuaternion<> quatZ;
		quatZ.Q_from_AngX(-CH_C_PI / 2);
		return createRotatedMarker(markerFrom, markerName, quatZ);
	}

}


