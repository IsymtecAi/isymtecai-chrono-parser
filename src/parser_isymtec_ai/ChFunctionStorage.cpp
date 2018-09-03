
#include "parser_isymtec_ai/ChFunctionStorage.h"
#include "physics/ChObject.h"
#include <memory>
#include "ChIsymtecAiUtils.h"

using namespace chrono;

void ChFunctionStorage::Clear()
{
	m_FunctionNamesMap.clear();
}

void ChFunctionStorage::addFunction(const std::string& functionName, std::shared_ptr<chrono::ChFunction> functionObj)
{
	auto it = m_FunctionNamesMap.find(functionName);
	if (it != m_FunctionNamesMap.end()) {
		throw chrono::ChException("Function " + functionName + " is already defined!!!");
	}
	m_FunctionNamesMap[functionName] = functionObj;
}

std::shared_ptr<chrono::ChFunction> ChFunctionStorage::getFunction(const std::string & functionName) const
{
	auto it = m_FunctionNamesMap.find(functionName);
	if (it == m_FunctionNamesMap.end()) {
		return nullptr;
	}
	auto output = it->second;
	return output;
}
