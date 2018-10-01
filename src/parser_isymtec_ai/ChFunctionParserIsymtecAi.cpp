#include "parser_isymtec_ai/ChFunctionParserIsymtecAi.h"
#include "chrono/physics/ChBodyAuxRef.h"
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "chrono/physics/ChSystem.h"
#include "core/ChQuaternion.h"
#include "ChFramesUtils.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"

using namespace function_isymtec_ai_params;
using namespace chrono;

ChFunctionParserIsymtecAi::ChFunctionParserIsymtecAi(
	std::shared_ptr<ChRelationsIsymtecAi> relations,
	std::shared_ptr<ChFunctionStorage> functionStorage) :
	ChElementaryParserIsymtecAi(relations),
	m_FunctionStorage(functionStorage)
{
}

void ChFunctionParserIsymtecAi::doParseObject() {
	std::string funcValue = isymtec_ai_utils::GetStringProperty(getObjectFrom(), FUNC_VALUE);
	auto generatedFunction = isymtec_ai_utils::CreateMuParserFunction(funcValue);
	isymtec_ai_utils::SetObjectName(getObjectFrom(), *generatedFunction);
	int countDerivatives = isymtec_ai_utils::GetIntProperty(getObjectFrom(), DERIVATIVES_COUNT);
	generatedFunction->SetCountDerivatives(countDerivatives);
	std::string funcValueDif = isymtec_ai_utils::GetStringProperty(getObjectFrom(), DVALUE);
	generatedFunction->SetExpressionDif(funcValueDif);
	std::string funcValueDifDif = isymtec_ai_utils::GetStringProperty(getObjectFrom(), DDVALUE);
	generatedFunction->SetExpressionDifDif(funcValueDifDif);

	getRelations().AddRelation(getObjectFrom(), generatedFunction);

	std::string name = isymtec_ai_utils::GetName(getObjectFrom());
	m_FunctionStorage->addFunction(name, generatedFunction);

	//return generatedFunction;
}