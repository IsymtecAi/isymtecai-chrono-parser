#ifndef CH_FUNCTION_PARSER_ISYMTEC_AI_H
#define CH_FUNCTION_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "parser_isymtec_ai/ChElementaryParserIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"

namespace chrono {
	class ChSystem;
}

class ChMuParserFunction;
class ChRelationsIsymtecAi;
class ChFunctionStorage;

namespace function_isymtec_ai_params {
	const std::string FUNC_VALUE = "funcValue";
	const std::string DVALUE = "dValue";
	const std::string DDVALUE = "ddValue";
	const std::string DERIVATIVES_COUNT = "derivativesCount";
}

/// @addtogroup parser_isymtec_ai
/// @{
/// isymtec.ai Function parser
/// asssumed that functions depend only on time
class ChApiIsymtecAI ChFunctionParserIsymtecAi : public ChElementaryParserIsymtecAi {
public:
	ChFunctionParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations,
		std::shared_ptr<ChFunctionStorage> functionStorage);

private:
	virtual void doParseObject() override final;

	std::shared_ptr<ChFunctionStorage> m_FunctionStorage;
};

#endif
