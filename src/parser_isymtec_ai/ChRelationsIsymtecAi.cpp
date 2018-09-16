

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
		std::string type = isymtec_ai_utils::GetStringProperty(objFrom, isymtec_ai_utils::PROPERTY_TYPE);
		m_GuiTypes[uuid] = type;
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

std::string ChRelationsIsymtecAi::getGuiType(const std::string & uuid) const
{
	auto it = m_GuiTypes.find(uuid);
	if (it == m_GuiTypes.end()) {
		throw chrono::ChException("ChRelationsIsymtecAi::getGuiType error. Element with UUid " + uuid + " is not in not added in ChRelationsIsymtecAi!!!");
	}
	std::string output = it->second;
	return output;
}

std::vector<std::string> ChRelationsIsymtecAi::getUUids() const
{
	std::vector< std::string> output;
	for (auto& curPair : m_GeneratedObjs) {
		output.push_back(curPair.first);
	}
	return output;
}

const std::vector<std::shared_ptr<chrono::ChObj>>& ChRelationsIsymtecAi::getGeneratedOjbects(const std::string & uuid) const
{
	auto it = m_GeneratedObjs.find(uuid);
	if (it == m_GeneratedObjs.end()) {
		throw chrono::ChException("ChRelationsIsymtecAi::getGeneratedOjbects error. Element with UUid " + uuid + " is not in not added in ChRelationsIsymtecAi!!!");
	}
	return it->second;
}


std::string ChRelationsIsymtecAi::getUUid(rapidjson::Value& objFrom) const
{
	std::string output = isymtec_ai_utils::getUUID(objFrom);
	return output;
}

