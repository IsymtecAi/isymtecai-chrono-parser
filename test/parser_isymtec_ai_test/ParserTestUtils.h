#include "chrono_thirdparty/rapidjson/document.h"
#include "core/ChVector.h"
#include <memory>

namespace parser_test_utils {
	std::string ConvertVectorToString(const chrono::ChVector<>& position);

	void writeInFile(rapidjson::Document& document, const std::string& fileName);

	void addStringMember(rapidjson::Value& objFrom, const std::string propertyName, const std::string value, rapidjson::Document::AllocatorType& curAllocator);

	void addIntMember(rapidjson::Value& objFrom, const std::string propertyName, int value, rapidjson::Document::AllocatorType& curAllocator);

	void setStringMember(rapidjson::Value& objFrom, const std::string propertyName,
		const std::string value, rapidjson::Document::AllocatorType& curAllocator);

	void setIntMember(rapidjson::Value& objFrom, const std::string propertyName,
		int value, rapidjson::Document::AllocatorType& curAllocator);

	///Create .json body object with all needed properties
	std::shared_ptr<rapidjson::Value> createDummyObject(std::string objectName, rapidjson::Document::AllocatorType& curAllocator);
}
