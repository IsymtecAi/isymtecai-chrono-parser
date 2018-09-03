
#ifndef CH_FUNCTION_STORAGE_H
#define CH_FUNCTION_STORAGE_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include <map>
#include <vector>


namespace chrono {
	class ChFunction;
}

/// @addtogroup parser_isymtec_ai
/// Class, storing function and their names

/// isymtec.ai Body parser
class ChApiIsymtecAI ChFunctionStorage {
  public:
	  ChFunctionStorage() {};
	  
	  void Clear();

	  void addFunction(const std::string& functionName, std::shared_ptr<chrono::ChFunction> functionObj);

	  std::shared_ptr<chrono::ChFunction> getFunction(const std::string& functionName) const;

private:
	///mapping function name to the function
	std::map<std::string, std::shared_ptr<chrono::ChFunction> > m_FunctionNamesMap;
  
};


#endif
