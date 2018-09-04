#include "parser_isymtec_ai/ChSimulationParameters.h"
#include "chrono/serialization/ChArchive.h"

using namespace chrono;
ChSimulationParameters::ChSimulationParameters()
{

}

void ChSimulationParameters::ArchiveOUT(chrono::ChArchiveOut & marchive)
{
	marchive << CHNVP(m_EndTime, "EndTime");
	marchive << CHNVP(m_IntegratorStepSize, "IntegratorStepSize");
	marchive << CHNVP(m_OutputStepSize, "OutputStepSize");
	
}