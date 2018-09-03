
#ifndef CH_ELEMENTARY_PARSER_ISYMTEC_AI_H
#define CH_ELEMENTARY_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"

namespace chrono {
	class ChSystem;
	class ChObj;
}

class ChFunctionStorage;
class ChMarkerDefined3DFunction;


/// @addtogroup parser_isymtec_ai
///Abstract class, parsing parising an elementary object (Link, force, etc.) for isym.ai input file 

/// isymtec.ai Body parser
class ChApiIsymtecAI ChElementaryParserIsymtecAi {
public:
	ChElementaryParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations);

	void ParseObject(rapidjson::Value& objectFrom);

	template<class T>
	std::shared_ptr<T> getGenerateObjPtr() {
		auto output = m_Relations->getGenerateObjPtr<T>(getObjectFrom());
		return output;
	}
protected:
	void AddRelation(std::shared_ptr<chrono::ChObj> generatedObject);

	ChRelationsIsymtecAi & getRelations() const { return *m_Relations; };

	rapidjson::Value& getObjectFrom() const { return *m_ObjectFrom; };

	void throwExeption(std::string propertyName,
		std::string errorDescription) const;

private:


	virtual void doParseObject() = 0;


	rapidjson::Value* m_ObjectFrom{ nullptr };
	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;
};

#endif
