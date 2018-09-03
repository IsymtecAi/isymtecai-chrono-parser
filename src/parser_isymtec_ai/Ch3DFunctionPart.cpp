
#include "parser_isymtec_ai/Ch3DFunctionPart.h"
#include "physics/ChObject.h"
#include "parser_isymtec_ai/ChMarkerDefined3DFunction.h"

using namespace chrono;



Ch3DFunctionPart::Ch3DFunctionPart(unsigned index, std::shared_ptr<ChMarkerDefined3DFunction> function3D):
	m_Function3D{ function3D },
	m_Index {index} {
}

Ch3DFunctionPart * Ch3DFunctionPart::Clone() const
{
	throw chrono::ChException("Clone method of Ch3DFunctionPart is not implemented yet");
}

double Ch3DFunctionPart::Get_y(double time) const
{
	return m_Function3D->Get_y(m_Index, time);
}
