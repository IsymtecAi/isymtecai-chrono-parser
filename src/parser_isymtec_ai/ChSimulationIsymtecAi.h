
#ifndef CH_SIMULATION_ISYMTEC_AI_H
#define CH_SIMULATION_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include <string>

class ChSimulationParameters;
class ChParserIsymtecAi;

namespace chrono {
	class ChSystemSMC;
}

/// @addtogroup parser_isymtec_ai
///Abstract Class, simulating models, generated by ChParserIsymtecAi

class ChApiIsymtecAI ChSimulationIsymtecAi {
  public:
	  ChSimulationIsymtecAi();;

	  void Simulate();
	  bool Parse(const std::string& filename);

	  std::shared_ptr<chrono::ChSystemSMC> GetSystem() const { return m_System; }
	  std::shared_ptr<ChSimulationParameters> GetSimulationParameters() const;
	  std::shared_ptr<ChParserIsymtecAi> GetParser() const { return m_Parser; }

	  ///function called after every output time step
	  virtual void ProcessOutputTimeStep() const = 0;
private:
	std::shared_ptr<chrono::ChSystemSMC> m_System;
	std::shared_ptr<ChParserIsymtecAi> m_Parser;
  
};


#endif