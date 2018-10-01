#ifndef CH_3DFUNCTION_PART
#define CH_3DFUNCTION_PART

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include <map>
#include <vector>
#include "motion_functions/ChFunction_Base.h"
#include "chrono/physics/ChObject.h"

namespace chrono {
	class ChSystem;
}

class ChMarkerDefined3DFunction;

/// @addtogroup parser_isymtec_ai
/// part of ChMarkerDefined3DFunction.
/// this.get_y = ChMarkerDefined3DFunction(index, Get_y)

class ChApiIsymtecAI Ch3DFunctionPart : public chrono::ChFunction, public chrono::ChObj {
public:
	Ch3DFunctionPart(unsigned index, std::shared_ptr<ChMarkerDefined3DFunction> function3D);

	virtual Ch3DFunctionPart* Clone() const override;

	virtual double Get_y(double x) const override;

private:
	std::shared_ptr<ChMarkerDefined3DFunction> m_Function3D;
	unsigned m_Index;
};

#endif
