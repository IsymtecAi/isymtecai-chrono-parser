#ifndef CH_ISYMTEC_AI_UTILS_H
#define CH_ISYMTEC_AI_UTILS_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "core/ChVector.h"
#include "core/ChQuaternion.h"
#include "chrono/core/ChMatrix33.h"


namespace chrono {
	class ChObj;
	class ChFunction;
}

class ChMuParserFunction;
class ChFunctionStorage;


/// @addtogroup parser_isymtec_ai
namespace isymtec_ai_utils {

	const std::string PROPERTY_TYPE = "typeName";
	const std::string PROPERTY_NAME = "name";
	const std::string PROPERTY_UUID = "uuid";
	const std::string OBJECTS_MEMBER = "objects";
	const std::string TIME_VARIABLE = "t";

	///get member of object (if member does not exist, generate exception)
	ChApiIsymtecAI rapidjson::Value& GetMember(rapidjson::Value &objFrom, const std::string &memberName);

	///get string property of object (if no property exist, generate exception)
	ChApiIsymtecAI std::string GetStringProperty(rapidjson::Value& objFrom, const std::string& memberName);

	///get string property of object (if no property exist, generate exception)
	ChApiIsymtecAI double GetDoubleProperty(rapidjson::Value& objFrom, const std::string& memberName);

	///get string property of object (if no property exist, generate exception)
	ChApiIsymtecAI int GetIntProperty(rapidjson::Value& objFrom, const std::string& memberName);


	///get bool property of object (if no property exist, generate exception) 
	ChApiIsymtecAI bool GetBoolProperty(rapidjson::Value& objFrom, const std::string& memberName);

	ChApiIsymtecAI rapidjson::Value::Array GetElementsInContainer(rapidjson::Value& parsedObject, const std::string& containerName);


	ChApiIsymtecAI chrono::ChVector<> getVectorProperty(rapidjson::Value& objFrom, const std::string& memberName);

	ChApiIsymtecAI chrono::ChMatrix33<> GetMatrix33Property(rapidjson::Value& objFrom, const std::string& memberName);

	
	ChApiIsymtecAI std::vector<std::string> GetStringVectorProperty(rapidjson::Value& objFrom, const std::string& memberName);

	ChApiIsymtecAI std::vector<std::string> GetStringFixSizeVectorProperty(rapidjson::Value& objFrom, const std::string& memberName, size_t sizeVec);

	ChApiIsymtecAI void SetObjectName(rapidjson::Value& parsedObject, chrono::ChObj& chronoObject, std::string postfix = "");

	ChApiIsymtecAI std::string GetName(rapidjson::Value& parsedObject);

	ChApiIsymtecAI std::string getUUID(rapidjson::Value& parsedObject);



	ChApiIsymtecAI std::shared_ptr<ChMuParserFunction> CreateMuParserFunction(const std::string& functionExpression);


	///input - name of the function or expression
	ChApiIsymtecAI std::shared_ptr<chrono::ChFunction> FindOrCreateMuParserFunction(const ChFunctionStorage& functionStorage, const std::string& functionExpression);
	
}

#endif