
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
	double endTime = GetSimulationParameters()->GetEndTime();
	m_System->DoStepDynamics(1E-10);
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

//
//void ChSimulationIsymtecAI::SaveData() {
//	m_Time.push_back(m_system->GetChTime());
//	auto body2 = m_system->Get_bodylist()[2];
//	m_Coor1.push_back(body2->GetPos().x());
//}
//
//void ChSimulationIsymtecAI::WriteData(const std::string& filename)
//{
//	utils::CSV_writer csv(" ");
//
//	for (size_t it = 0; it < m_Time.size(); ++it) {
//		csv << m_Time[it] << m_Coor1[it] << std::endl;
//	}
//	csv.write_to_file(filename);
//}

std::shared_ptr<ChSimulationParameters> ChSimulationIsymtecAi::GetSimulationParameters() const { 
	auto output = m_Parser->getSimulationParameters();
	return output;
}
