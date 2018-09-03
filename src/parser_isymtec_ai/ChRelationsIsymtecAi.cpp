

#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "physics/ChObject.h"
#include <memory>
#include "ChIsymtecAiUtils.h"

using namespace chrono;

void ChRelationsIsymtecAi::Clear()
{
	m_GeneratedObjs.clear();
}

void ChRelationsIsymtecAi::AddRelation(rapidjson::Value& objFrom, std::shared_ptr<ChObj> generatedObject) {
	std::string uuid = getUUid(objFrom);
	auto it = m_GeneratedObjs.find(uuid);
	if (it == m_GeneratedObjs.end()) {
		//std::vector< std::shared_ptr<ChObj> > newVec;
		m_GeneratedObjs[uuid] = {};
	}
	m_GeneratedObjs[uuid].push_back(generatedObject);
}

std::string ChRelationsIsymtecAi::getUUid(rapidjson::Value& objFrom) const
{
	std::string output = isymtec_ai_utils::getUUID(objFrom);
	return output;
}

