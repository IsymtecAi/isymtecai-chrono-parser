#ifndef CH_MARKER_DEFINED_3DFUNCTION
#define CH_MARKER_DEFINED_3DFUNCTION

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include <map>
#include <vector>
#include "motion_functions/ChFunction_Base.h"
#include "chrono/physics/ChObject.h"

namespace chrono {
	class ChSystem;
	class ChMarker;
}

/// @addtogroup parser_isymtec_ai
///3D-Function, converting function, defined in Marker, to the absolute Frame

/// Class converting function 3D function, defined in Marker, to the absolute Frame

class ChApiIsymtecAI ChMarkerDefined3DFunction : public chrono::ChObj {
public:
	ChMarkerDefined3DFunction(std::shared_ptr<chrono::ChMarker> marker);

	virtual ChMarkerDefined3DFunction* Clone() const override;

	void setFunctionInMarker(unsigned index, std::shared_ptr<const chrono::ChFunction> functionInMarker);

	double Get_y(unsigned index, double time) const;

private:
	std::shared_ptr<const chrono::ChFunction> m_Function0;
	std::shared_ptr<const chrono::ChFunction> m_Function1;
	std::shared_ptr<const chrono::ChFunction> m_Function2;
	std::shared_ptr<chrono::ChMarker> m_Marker;
};

#endif
