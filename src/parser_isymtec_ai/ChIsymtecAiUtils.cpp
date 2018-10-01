#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "core/ChException.h"
#include "physics/ChObject.h"
#include "core/ChVector.h"
#include <locale>
#include <algorithm>
#include <cctype>
#include <locale>
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"
#include "parser_isymtec_ai/ChExceptionIsymtecAi.h"

namespace {
	const char VEC_DELIMITER = ',';
	const char MATRIX_ROW_DELIMITER = ';';

	// trim from start (in place)
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	std::vector<std::string>  SplitStringToTockens(std::string inputString, char delimiter) {
		std::istringstream ss(inputString);
		std::string token;
		std::vector<std::string> output;
		while (std::getline(ss, token, delimiter)) {
			std::string trimedToken{ token };
			trim(trimedToken);
			output.push_back(trimedToken);
		}
		return output;
	}

	std::vector<double>  stringVecToDoubleVec(std::vector<std::string>  stringVec) {
		std::vector<double> output;
		output.reserve(stringVec.size());
		for (const auto& curString : stringVec) {
			double newDouble = std::stod(curString);
			output.push_back(newDouble);
		}
		return output;
	}
}

using namespace chrono;

namespace isymtec_ai_utils {
	rapidjson::Value& GetMember(rapidjson::Value &objFrom, const std::string &memberName)
	{
		if (!objFrom.HasMember(memberName.c_str())) {
			throw chrono::ChException("Can't read property  " + memberName);
		}
		auto& output = (objFrom)[memberName.c_str()];
		return output;
	}

	///need to define by ourself, because in release mode reading of non-existing variable in rapidjson can be undefined
	std::string GetStringProperty(rapidjson::Value& objFrom, const std::string& memberName)
	{
		auto& curMember = GetMember(objFrom, memberName);
		if (!curMember.IsString()) {
			throw chrono::ChException("Property " + memberName + " is not string");
		}
		std::string output = curMember.GetString();
		return output;
	}

	double GetDoubleProperty(rapidjson::Value& objFrom, const std::string& memberName)
	{
		auto& curMember = GetMember(objFrom, memberName);
		double output{ 0.0 };
		if (curMember.IsDouble()) {
			output = curMember.GetDouble();
		}
		else {
			std::string outputStr = GetStringProperty(objFrom, memberName);
			output = std::stod(outputStr);
		}
		return output;
	}

	int GetIntProperty(rapidjson::Value & objFrom, const std::string & memberName)
	{
		auto& curMember = GetMember(objFrom, memberName);
		if (!curMember.IsInt()) {
			throw ChExceptionIsymtecAi(&objFrom, memberName, "Is not integrer");
		}
		int output = curMember.GetInt();
		return output;
	}

	///need to define by ourself, because in release mode reading of non-existing variable in rapidjson can be undefined
	bool GetBoolProperty(rapidjson::Value& objFrom, const std::string& memberName)
	{
		auto& curMember = GetMember(objFrom, memberName);
		if (!curMember.IsBool()) {
			throw chrono::ChException("Property " + memberName + " is not string");
		}
		bool output = curMember.GetBool();
		return output;
	}

	rapidjson::Value::Array GetElementsInContainer(rapidjson::Value& objFrom, const std::string& containerName)
	{
		auto& container = GetMember(objFrom, containerName);
		auto& objsArr = GetMember(container, OBJECTS_MEMBER);
		if (!objsArr.IsArray()) {
			throw chrono::ChException("Member " + OBJECTS_MEMBER + " of container " + containerName +
				" is not  an array");
		}
		rapidjson::Value::Array output = objsArr.GetArray();
		return output;
	}

	ChVector<> getVectorProperty(rapidjson::Value& objFrom, const std::string& memberName)
	{
		auto stringTockens = GetStringVectorProperty(objFrom, memberName);
		auto vecDouble = stringVecToDoubleVec(stringTockens);
		if (vecDouble.size() != 3) {
			throw chrono::ChException("Vector property " + memberName + " should have size three!!!");
		}
		ChVector<> output(vecDouble[0], vecDouble[1], vecDouble[2]);
		return output;
	}

	ChApiIsymtecAI chrono::ChMatrix33<> GetMatrix33Property(rapidjson::Value& objFrom, const std::string& memberName)
	{
		std::string stingValue = GetStringProperty(objFrom, memberName);
		auto rowsTockens = SplitStringToTockens(stingValue, MATRIX_ROW_DELIMITER);
		if (rowsTockens.size() != 3) {
			throw chrono::ChException("Matrix property " + memberName + " should have three rows!!!");
		}
		chrono::ChMatrix33<> output;
		for (int curRow = 0; curRow < rowsTockens.size(); curRow++)
		{
			std::string curRowString = rowsTockens[curRow];
			auto rowElementsStr = SplitStringToTockens(curRowString, VEC_DELIMITER);
			if (rowElementsStr.size() != 3) {
				throw chrono::ChException("Matrix property " + memberName + " error. Row "
					+ std::to_string(curRow) + " should have three columns!!!");
			}
			auto rowElements = stringVecToDoubleVec(rowElementsStr);
			for (int curCol = 0; curCol < rowElements.size(); curCol++) {
				output[curRow][curCol] = rowElements[curCol];
			}
		}
		return output;
	}

	std::vector<std::string> GetStringVectorProperty(rapidjson::Value& objFrom, const std::string& memberName)
	{
		std::string stingValue = GetStringProperty(objFrom, memberName);
		auto stringTockens = SplitStringToTockens(stingValue, VEC_DELIMITER);
		return stringTockens;
	}

	std::vector<std::string> GetStringFixSizeVectorProperty(rapidjson::Value& objFrom, const std::string& memberName, size_t sizeVec) {
		auto output = GetStringVectorProperty(objFrom, memberName);
		if (output.size() != sizeVec) {
			std::string descriptions = "Has a size " + std::to_string(output.size()) +
				" (should be" + std::to_string(sizeVec) + ")";
			throw ChExceptionIsymtecAi(&objFrom, memberName, descriptions);
		}
		return output;
	}

	void SetObjectName(rapidjson::Value& parsedObject, chrono::ChObj& chronoObject, std::string postfix)
	{
		std::string name = GetName(parsedObject) + postfix;
		chronoObject.SetName(name.c_str());
	}

	std::string GetName(rapidjson::Value& parsedObject)
	{
		std::string name = GetStringProperty(parsedObject, PROPERTY_NAME);
		return name;
	}

	std::string getUUID(rapidjson::Value& parsedObject)
	{
		std::string uuid = GetStringProperty(parsedObject, PROPERTY_UUID);
		return uuid;
	}

	//TODO: change function to ChFunctionStorage call
	ChApiIsymtecAI std::shared_ptr<ChMuParserFunction> CreateMuParserFunction(const std::string & functionExpression)
	{
		auto function0 = std::make_shared<ChMuParserFunction>();
		function0->DefineInputVariable(TIME_VARIABLE);
		function0->SetExpresson(functionExpression);
		return function0;
	}

	std::shared_ptr<chrono::ChFunction> FindOrCreateMuParserFunction(const ChFunctionStorage& functionStorage, const std::string & functionExpression)
	{
		auto function = functionStorage.getFunction(functionExpression);
		if (function == nullptr) {
			function = CreateMuParserFunction(functionExpression);
		}
		return function;
	}
}