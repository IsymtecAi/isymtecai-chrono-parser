#ifndef CH_FORCE_PARSER_ISYMTEC_AI_H
#define CH_FORCE_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>
#include "parser_isymtec_ai/ChElementaryParserIsymtecAi.h"

#include "chrono_thirdparty/rapidjson/document.h"

namespace chrono {
	class ChSystem;
	class ChForce;
	class ChMarker;
	class ChBody;
}

class ChRelationsIsymtecAi;
class ChFunctionStorage;
class ChMarkerDefined3DFunction;

namespace force_isymtec_ai_params {
	const std::string FORCE_TYPE = "forceType";
	const std::string APPLIED_FORCE = "Applied force";
	const std::string SPRING_FORCE = "Spring force";

	const std::string CS1_ID = "cs1ID"; //<id of marker 1
	const std::string CS2_ID = "cs2ID"; //<id of marker 2
}

namespace applied_force_isymtec_ai_params {
	const std::string FORCE_VALUE = "forceValue";//<in global CS
	const std::string TORQUE_VALUE = "torqueValue";//in global CS
}

namespace spring_force_isymtec_ai_params {
	const std::string STIFFNESS = "stiffness";
	const std::string DAMPING = "damping";
	const std::string REST_LENGTH = "restLength";
}

/// @addtogroup parser_isymtec_ai
/// isymtec.ai Force parser
class ChApiIsymtecAI ChForceParserIsymtecAi : public ChElementaryParserIsymtecAi {
public:
	ChForceParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations,
		std::shared_ptr<const ChFunctionStorage> functionStorage);

private:
	virtual void doParseObject() override final;

	std::shared_ptr<chrono::ChBody> GetBody(const chrono::ChMarker& marker) const;

	const std::shared_ptr<chrono::ChMarker> findMarker(std::string markerPropertyName) const;

	void setAppliedForceInputs(ChMarkerDefined3DFunction& function3D, std::string propertyName);
	void createAppliedForce();
	void createSpringForce();

	std::shared_ptr<const ChFunctionStorage> m_FunctionStorage;
};

#endif
