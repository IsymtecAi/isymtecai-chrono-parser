#ifndef CH_GEOMETRY_PARSER_ISYMTEC_AI_H
#define CH_GEOMETRY_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>
#include "parser_isymtec_ai/ChElementaryParserIsymtecAi.h"

#include "chrono_thirdparty/rapidjson/document.h"
#include <vector>
#include <map>


namespace chrono {
	class ChSystem;
	class ChBodyAuxRef;
	class ChVisualization;
	class ChCylinderShape;
	class ChMarker;
	class ChBoxShape;
}
	class ChRelationsIsymtecAi;

	namespace geometry_isymtec_ai_params {
		const std::string GEOMETRY_TYPE = "geometryType";
		const std::string REFERENCE_CS_ID = "referenceCSID";
		const std::string CYLINDER = "Cylinder";
		const std::string BOX = "Box";

	}

	namespace cylinder_isymtec_ai_params {
		const std::string RADIUS = "radius";
		const std::string HEIGHT = "height";
	}

	namespace box_isymtec_ai_params {
		const std::string WIDTH = "width";
		const std::string HEIGHT = "height";
		const std::string DEPTH = "depth";
	}

/// @addtogroup parser_isymtec_ai
/// isymtec.ai Geometry parser
class ChApiIsymtecAI ChGeometryParserIsymtecAi: public ChElementaryParserIsymtecAi {

  public:
	  ChGeometryParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations);
	  
private:

	virtual void doParseObject() override final;

	std::shared_ptr<chrono::ChVisualization> CreateGeometry();

	std::shared_ptr<chrono::ChCylinderShape> CreateCylinder();
	std::shared_ptr<chrono::ChBoxShape> CreateBox();

	chrono::ChBodyAuxRef* m_Body{ nullptr };
	std::shared_ptr<chrono::ChMarker> m_Marker;

};

#endif
