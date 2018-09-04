#ifndef CH_RELATIONS_ISYMTEC_AI_H
#define CH_RELATIONS_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"
#include <map>
#include <vector>


namespace chrono {
	class ChObj;
	class ChSystem;
	class ChArchiveOut;
}

/// @addtogroup parser_isymtec_ai
/// Class, storing relation between objects in isym.ai input file and generated chrono objects

/// isymtec.ai Body parser
class ChApiIsymtecAI ChRelationsIsymtecAi {
  public:
	  ChRelationsIsymtecAi() {};
	  
	  void Clear();
	  void SetSystem(std::shared_ptr<chrono::ChSystem> system) { m_System = system; };
	  std::shared_ptr<chrono::ChSystem> GetSystem() { return m_System; };

	  void AddRelation(rapidjson::Value& objFrom, std::shared_ptr<chrono::ChObj> generatedObject);

	  template<class T>
	  std::shared_ptr<T> getGenerateObjPtrFromUUID(const std::string& uuid) {
		  auto it = m_GeneratedObjs.find(uuid);
		  if (it == m_GeneratedObjs.end()) {
			  return nullptr;
		  }
		  auto& generatedObjects = it->second;
		  for (std::shared_ptr<chrono::ChObj> curObj : generatedObjects) {
			  auto derivedObj = std::dynamic_pointer_cast<T> (curObj);
			  if (derivedObj != nullptr) {
				  return derivedObj;
			  }
		  }
		  return nullptr;
	  }

	  template<class T>
	  std::shared_ptr<T> getGenerateObjPtr(rapidjson::Value& objFrom) {
		  const std::string uuid = getUUid(objFrom);
		  auto output = getGenerateObjPtrFromUUID<T>(uuid);
		  return output;
	  }

	  void ArchiveOUT(chrono::ChArchiveOut& marchive);
private:
	std::string getUUid(rapidjson::Value& objFrom) const;

	std::map<std::string, std::vector<std::shared_ptr<chrono::ChObj>> > m_GeneratedObjs;



	std::shared_ptr<chrono::ChSystem> m_System;

	  
};


#endif
