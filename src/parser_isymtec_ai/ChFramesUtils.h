#ifndef CH_FRAMES_UTILS_H
#define CH_FRAMES_UTILS_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "core/ChVector.h"
#include "core/ChQuaternion.h"

namespace chrono {
	class ChObj;
	class ChMarker;
}

/// @addtogroup parser_isymtec_ai
namespace frames_utils {
	///creaete quaternion from xyz euler angles (see https://threejs.org/docs/index.html#api/math/Euler)
	///angles expressed in degrees
	ChApiIsymtecAI chrono::ChQuaternion<> createQuaternionFromEulerXYZ(const chrono::ChVector<>& eulerAnglesXYZ);

	///create marker1 whose z-axis is equal to x axis of original marker
	ChApiIsymtecAI std::shared_ptr<chrono::ChMarker> createMarkerByRotationXToZ(const chrono::ChMarker& markerFrom, std::string markerName);

	///create marker1 whose z-axis is equal to y axis of original marker
	ChApiIsymtecAI std::shared_ptr<chrono::ChMarker> createMarkerByRotationYToZ(const chrono::ChMarker& markerFrom, std::string markerName);
}

#endif