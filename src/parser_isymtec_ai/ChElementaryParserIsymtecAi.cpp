#include "parser_isymtec_ai/ChElementaryParserIsymtecAi.h"
#include "parser_isymtec_ai/ChExceptionIsymtecAi.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"

using namespace chrono;

ChElementaryParserIsymtecAi::ChElementaryParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations) :
	m_Relations(relations) {
}

void ChElementaryParserIsymtecAi::ParseObject(rapidjson::Value & jsonElement) {
	m_ObjectFrom = &jsonElement;
	doParseObject();
}

void ChElementaryParserIsymtecAi::AddRelation(std::shared_ptr<chrono::ChObj> generatedObject) {
	getRelations().AddRelation(getObjectFrom(), generatedObject);
}

void ChElementaryParserIsymtecAi::throwExeption(std::string propertyName, std::string errorDescription) const {
	throw ChExceptionIsymtecAi(m_ObjectFrom, propertyName, errorDescription);
}