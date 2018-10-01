#ifndef CH_MARKER_PARSER_ISYMTEC_AI_H
#define CH_MARKER_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"
#include <vector>
#include <map>
#include "core/ChCoordsys.h"

namespace chrono {
	class ChSystem;
	class ChBodyAuxRef;
}

class ChRelationsIsymtecAi;

namespace marker_isymtec_ai_params {
	const std::string PROPERTY_TRANSLATION = "translation";
	const std::string PROPERTY_ORIENTATION = "orientation";
	const std::string MARKER_CONTAINER = "markerContainer";
	const std::string REFERENCE_CS_ID = "referenceCSID";
}

/// @addtogroup parser_isymtec_ai
/// isymtec.ai Marker parser
class ChApiIsymtecAI ChMarkerParserIsymtecAi {
	struct ChGraphNode {
		std::vector<std::string>  m_DependentMarkers;
		bool m_IsInTree{ false }; //<showing if there is a way from this marker to body (e.g. it is in tree with the body as a root)
		rapidjson::Value* m_MarkerFrom{ nullptr };
	};

public:
	ChMarkerParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations);

	void ParseBodyMarkers(rapidjson::Value& bodyFrom);

private:
	void CreateGraph();

	std::string getBodyUUID() const;

	void ProveNonExistingMarkersInGraph();
	void SetTreeFlagInGraphRecursively(const std::string& uuid);
	void ProveAllNodesInTree();

	/////first call is with bodyUUid, that is why createOnlyChildren firstly should be true
	//void CreateMarkersRecursively(const std::string& uuid, bool createOnlyChildren);

	void createMarker(rapidjson::Value& markerFrom, const chrono::Coordsys& absCoorRelMarker);

	void createDependentMarkes(const std::string& markerUUID, const chrono::Coordsys& parentMarkerAbsCoor);

	rapidjson::Value* m_BodyFrom;

	std::shared_ptr<ChRelationsIsymtecAi> m_Relations;
	std::shared_ptr<chrono::ChBodyAuxRef> m_Body;

	std::map<std::string, ChGraphNode> m_Graph;
};

#endif
