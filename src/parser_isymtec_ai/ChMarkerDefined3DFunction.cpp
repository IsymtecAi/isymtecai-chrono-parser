
#include "parser_isymtec_ai/ChMarkerDefined3DFunction.h"
#include "physics/ChObject.h"
#include "chrono/physics/ChMarker.h"

using namespace chrono;



ChMarkerDefined3DFunction::ChMarkerDefined3DFunction(std::shared_ptr<chrono::ChMarker> marker):
	m_Marker {marker}
{	
}

ChMarkerDefined3DFunction * ChMarkerDefined3DFunction::Clone() const
{
	throw chrono::ChException("Clone method of ChMarkerDefined3DFunction is not implemented yet");
}

void ChMarkerDefined3DFunction::setFunctionInMarker(unsigned index, std::shared_ptr<const chrono::ChFunction> functionInMarker)
{
	if (index > 2) {
		throw chrono::ChException("False index");
	}
	if (index == 0) {
		m_Function0 = functionInMarker;
	}
	else if (index == 1) {
		m_Function1 = functionInMarker;
	}
	else if (index == 2) {
		m_Function2 = functionInMarker;
	}
}

double ChMarkerDefined3DFunction::Get_y(unsigned index, double time) const
{
	ChVector<> function3DInMarker{ 0.0, 0.0, 0.0 };
	if (m_Function0 != nullptr) {
		function3DInMarker[0] = m_Function0->Get_y(time);
	}
	if (m_Function1 != nullptr) {
		function3DInMarker[1] = m_Function1->Get_y(time);
	}
	if (m_Function2 != nullptr) {
		function3DInMarker[2] = m_Function2->Get_y(time);
	}
	ChVector<> functionInAbs = m_Marker->Dir_Ref2World(&function3DInMarker);
	return functionInAbs[index];
}
