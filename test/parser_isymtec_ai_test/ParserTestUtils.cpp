#include "ParserTestUtils.h"
#include <cstdio>
#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "chrono_thirdparty/rapidjson/stringbuffer.h"
#include <chrono_thirdparty/rapidjson/writer.h>
#include "chrono_thirdparty/rapidjson/filewritestream.h"
#include <chrono_thirdparty/rapidjson/writer.h>
#include "chrono_thirdparty/rapidjson/prettywriter.h"
#include "chrono_thirdparty/rapidjson/pointer.h"

using namespace rapidjson;
std::string parser_test_utils::ConvertVectorToString(const chrono::ChVector<>& position)
{
	std::string output = std::to_string(position.x()) + ", " +
		std::to_string(position.y()) + ", " + std::to_string(position.z());
	return output;
}

void parser_test_utils::writeInFile(rapidjson::Document& document, const std::string& fileName)
{
	FILE* fp = fopen(fileName.c_str(), "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);
}

void parser_test_utils::addStringMember(rapidjson::Value& objFrom, const std::string propertyName, const std::string value, rapidjson::Document::AllocatorType& curAllocator)
{
	//objFrom.AddMember(StringRef(propertyName.c_str()), StringRef(value.c_str()), curAllocator);
	rapidjson::Value jsonValue(value.c_str(), curAllocator);
	rapidjson::Value jsonPropName(propertyName.c_str(), curAllocator);
	objFrom.AddMember(jsonPropName, jsonValue, curAllocator);
}


void parser_test_utils::addIntMember(rapidjson::Value& objFrom, const std::string propertyName, int value, rapidjson::Document::AllocatorType& curAllocator)
{
	//rapidjson::Value jsonValue(value, curAllocator);
	rapidjson::Value jsonPropName(propertyName.c_str(), curAllocator);
	objFrom.AddMember(jsonPropName, value, curAllocator);
}


void parser_test_utils::setStringMember(Value& objFrom, const std::string propertyName, const std::string value, Document::AllocatorType& curAllocator) {
	//objFrom.AddMember(StringRef(propertyName.c_str()), StringRef(value.c_str()), curAllocator);
	Value jsonValue(value.c_str(), curAllocator);
	//Value jsonPropName(propertyName.c_str(), curAllocator);
	objFrom[propertyName.c_str()] = jsonValue;
}

void parser_test_utils::setIntMember(rapidjson::Value & objFrom, const std::string propertyName, int value, rapidjson::Document::AllocatorType & curAllocator) {
	objFrom[propertyName.c_str()] = value;
}

std::shared_ptr<RAPIDJSON_NAMESPACE::Value> parser_test_utils::createDummyObject(std::string objectName, Document::AllocatorType& curAllocator)
{
	auto objFrom = std::make_shared<Value >(rapidjson::kObjectType);
	addStringMember(*objFrom, isymtec_ai_utils::PROPERTY_UUID, objectName + "_UUD", curAllocator);
	addStringMember(*objFrom, isymtec_ai_utils::PROPERTY_NAME, objectName, curAllocator);
	addStringMember(*objFrom, isymtec_ai_utils::PROPERTY_TYPE, objectName + "_type", curAllocator);
	return objFrom;
}
