

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

namespace {
	struct ChRelationEntry {
		const std::string* m_GuiID;
		std::vector<std::shared_ptr<chrono::ChObj>>* m_GeneratedObjs;
		void ArchiveOUT(chrono::ChArchiveOut& marchive) {
			marchive << CHNVP(*m_GuiID, "GUI_ID");
			marchive << CHNVP(*m_GeneratedObjs, "Generated Objects");
		}
	};
}

void ChRelationsIsymtecAi::ArchiveOUT(chrono::ChArchiveOut & marchive) {
	ChRelationEntry relationEntry;
	for (auto& entry : m_GeneratedObjs)	{
		relationEntry.m_GuiID = &entry.first;
		relationEntry.m_GeneratedObjs = &entry.second;
		marchive << CHNVP(relationEntry, "Entry");
	}
}

std::string ChRelationsIsymtecAi::getUUid(rapidjson::Value& objFrom) const
{
	std::string output = isymtec_ai_utils::getUUID(objFrom);
	return output;
}

