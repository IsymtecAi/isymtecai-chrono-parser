#ifndef CH_SIMULATION_PARAMETERS_H
#define CH_SIMULATION_PARAMETERS_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>
#include "physics/ChObject.h"



namespace chrono {
	class ChSystem;
}




/// @addtogroup parser_isymtec_ai
/// Class, storing simulation parameters (integrator time steps, etc.)

/// isymtec.ai MBS input file parser
class ChApiIsymtecAI ChSimulationParameters : public chrono::ChObj {
public:

	ChSimulationParameters();;
	~ChSimulationParameters() = default;

	double GetEndTime() const { return m_EndTime; }
	void SetEndTime(double val) { m_EndTime = val; }

	double GetIntegratorStepSize() const { return m_IntegratorStepSize; }
	void SetIntegratorStepSize(double val) { m_IntegratorStepSize = val; }

	double GetOutputStepSize() const { return m_OutputStepSize; }
	void SetOutputStepSize(double val) { m_OutputStepSize = val; }

	virtual ChSimulationParameters* Clone() const override { return new ChSimulationParameters(*this); };
private:
	double m_EndTime{ 0.0 };
	double m_IntegratorStepSize{ 0.0 };
	double m_OutputStepSize{ 0.0 };
};


#endif
