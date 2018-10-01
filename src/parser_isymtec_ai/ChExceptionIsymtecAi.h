#ifndef CH_EXEPTION_ISYMTEC_AI_H
#define CH_EXEPTION_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"

#include <string>

/// @addtogroup parser_isymtec_ai
/// Exception class for isymtec.ai input files.

class ChApiIsymtecAI ChExceptionIsymtecAi : public std::exception {
public:

	ChExceptionIsymtecAi(std::string errorDescription) : m_ErrorDescription(errorDescription) {};
	ChExceptionIsymtecAi(rapidjson::Value* rapidJsonObject, std::string propertyName,
		std::string errorDescription) :
		m_RapidJsonObject(rapidJsonObject),
		m_PropertyName(propertyName),
		m_ErrorDescription(errorDescription) {};

	virtual const char* what() const throw() { return m_ErrorDescription.c_str(); }

	rapidjson::Value* getRapidJsonObject() const { return m_RapidJsonObject; }
	std::string getPropertyName() const { return  m_PropertyName; }
	std::string getErrorDescription() const { return  m_ErrorDescription; }

private:
	rapidjson::Value* m_RapidJsonObject;
	std::string m_PropertyName;
	std::string m_ErrorDescription;
};

#endif
