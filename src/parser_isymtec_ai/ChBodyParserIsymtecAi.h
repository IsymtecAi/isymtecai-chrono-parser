#ifndef CH_BODY_PARSER_ISYMTEC_AI_H
#define CH_BODY_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"
#include "parser_isymtec_ai/ChElementaryParserIsymtecAi.h"

namespace chrono {
	class ChSystem;
	class ChBodyAuxRef;
}

class ChRelationsIsymtecAi;

namespace body_isymtec_ai_params {
	const std::string PROPERTY_FIXED = "fixed";
	const std::string PROPERTY_TRANSLATION = "translation";
	const std::string PROPERTY_ORIENTATION = "orientation";
	const std::string  PROPERTY_MASS = "mass";
	const std::string  PROPERTY_CENTER_OF_MASS = "centerOfMass";
	const std::string  PROPERTY_INERTIA = "inertia"; // inertia in body reference frame, not in central frame
	const std::string  PROPERTY_LINEAR_VEL = "linearVel"; //<absolute linear velocity
	const std::string  PROPERTY_ANGULAR_VEL = "angularVel"; //<absolute angular velocity
}

/// @addtogroup parser_isymtec_ai
/// isymtec.ai Body parser
class ChApiIsymtecAI ChBodyParserIsymtecAi : public ChElementaryParserIsymtecAi {
public:
	ChBodyParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations);

private:

	virtual void doParseObject() override final;
	void SetBodyCoordinates();
	void SetBodyVelocities();

	///set mass, center of mass and inertia
	void setMassProperties();

	std::shared_ptr<chrono::ChBodyAuxRef> m_Body;
};

#endif
