#include "ChSimulationIsymtecAi.h"

#include "parser_isymtec_ai/ChSimulationIsymtecAi.h"
#include "parser_isymtec_ai/ChParserIsymtecAi.h"
#include "chrono/physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChSimulationParameters.h"

using namespace chrono;

ChSimulationIsymtecAi::ChSimulationIsymtecAi() {
}

bool ChSimulationIsymtecAi::Parse(const std::string& filename)
{
	m_Parser = std::make_shared<ChParserIsymtecAi>();
	m_System = std::make_shared<ChSystemSMC>();
	bool isParsed = m_Parser->Parse(m_System, filename);
	return isParsed;
}


void ChSimulationIsymtecAi::Simulate() {
	m_System->DoFullAssembly();
	double endTime = GetSimulationParameters()->GetEndTime();
	double integratorStepSize = GetSimulationParameters()->GetIntegratorStepSize();
	double outputStepSize = GetSimulationParameters()->GetOutputStepSize();
	double lastWrittenStep = m_System->GetChTime() - outputStepSize * 2;
	while (m_System->GetChTime() < endTime) {
		if (m_System->GetChTime() - lastWrittenStep > outputStepSize) {
			ProcessOutputTimeStep();
			lastWrittenStep = m_System->GetChTime();
		}
		m_System->DoStepDynamics(integratorStepSize);
	}
	ProcessOutputTimeStep();
}

std::shared_ptr<ChSimulationParameters> ChSimulationIsymtecAi::GetSimulationParameters() const { 
	auto output = m_Parser->GetSimulationParameters();
	return output;
}

void ChSimulationIsymtecAi::ArchiveOUT(chrono::ChArchiveOut & marchive) {
	marchive << CHNVP(m_System);
	marchive << CHNVP(GetSimulationParameters(), "SimulationParameters");

	marchive << CHNVP(m_Parser, "Parser");
}
