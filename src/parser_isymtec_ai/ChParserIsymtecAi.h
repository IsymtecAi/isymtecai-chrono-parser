#ifndef CH_PARSER_ISYMTEC_AI_H
#define CH_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"


namespace chrono {
	class ChSystem;
	class ChArchiveOut;
}

class ChSimulationParameters;
class ChFunctionStorage;
class ChExceptionIsymtecAi;

namespace model_isymtec_ai_params {
	const std::string MODEL_PARAMS = "modelParams";
	const std::string GRAVITY = "gravity";
	const std::string START_TIME = "startTime";
	const std::string END_TIME = "endTime";
	const std::string INTEGRATOR_STEP_SIZE = "integratorStepSize";
	const std::string OUTPUT_STEP_SIZE = "outputStepSize";

}


/// @addtogroup parser_isymtec_ai
/// isymtec.ai MBS input file parser
class ChApiIsymtecAI ChParserIsymtecAi {
public:

	ChParserIsymtecAi();;
	~ChParserIsymtecAi() {}

	/// Parse the specified isymtec.ai MBS input file and create the model in the given system.
	///If can't parse, return false. 
	bool Parse(std::shared_ptr<chrono::ChSystem> system,    ///< [in] containing Chrono system
		const std::string& filename  ///< [in] isymtec.ai MBS input file name
	);

	std::shared_ptr<ChSimulationParameters> GetSimulationParameters() const { return m_SimulationParameters; }

	std::shared_ptr<ChFunctionStorage> GetFunctionStorage() const { return m_FunctionStorage; };

	const std::shared_ptr<ChRelationsIsymtecAi> GetRelations() const { return m_Relations; };

	/// Method to allow serialization of transient data to archives.
	virtual void ArchiveOUT(chrono::ChArchiveOut& marchive) ;
private:
	void CatchParserException(const ChExceptionIsymtecAi& parserException) const;

	std::shared_ptr<chrono::ChSystem> GetSystem() { return m_System; };
	void SetModelParams();
	void ProveIsModel();
	void CreateBodies();
	void CreateMarkers();
	void CreateGeometries();
	void CreateLinks();
	void CreateFunctions();
	void CreateForces();

	std::string m_FileName;
	std::shared_ptr<rapidjson::Document> m_Document;
	std::shared_ptr<chrono::ChSystem> m_System;
	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;

	std::shared_ptr<ChSimulationParameters> m_SimulationParameters;
	std::shared_ptr<ChFunctionStorage> m_FunctionStorage;

};


#endif
