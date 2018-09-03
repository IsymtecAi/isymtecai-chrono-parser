
#ifndef CH_LINK_PARSER_ISYMTEC_AI_H
#define CH_LINK_PARSER_ISYMTEC_AI_H

#include "parser_isymtec_ai/ChApiIsymtecAi.h"
#include "chrono/core/ChApiCE.h"
#include <string>

#include "chrono_thirdparty/rapidjson/document.h"
#include <vector>
#include "parser_isymtec_ai/ChElementaryParserIsymtecAi.h"


namespace chrono {
	class ChSystem;
	class ChMarker;
	class ChLinkMarkers;
	class ChLinkLockRevolute;
	class ChBody;
	class ChLinkMotorLinearSpeed;
	class ChLinkMotor;
	class ChLink;
	class ChLinkMateGeneric;
	class ChLinkMotorRotationSpeed;

}
	class ChRelationsIsymtecAi;
	class ChFunctionStorage;

	namespace link_isymtec_ai_params {
		const std::string LINK_TYPE = "linkType";
		const std::string TRANSLATIONAL_DOFS = "translationalDOFs";
		const std::string ROTATIONAL_DOFS = "rotationalDOFs";
		const std::string CS1_ID = "cs1ID";
		const std::string CS2_ID = "cs2ID";


		///types of joints
		const std::string   LOCK = "Lock";
		const std::string 	SPHERICAL = "Spherical";
		const std::string 	POINTPLANE = "PointPlane";
		const std::string 	POINTLINE = "PointLine";
		const std::string 	CYLINDRICAL = "Cylindrical";
		const std::string 	PRISMATIC = "Prismatic";
		const std::string 	PLANEPLANE = "PlanePlane";
		const std::string 	OLDHAM = "Oldham";
		const std::string 	REVOLUTE = "Revolute";
		const std::string 	FREE = "Free";
		const std::string 	ALIGN = "Align";
		const std::string 	PARALLEL = "Parallel";
		const std::string 	PERPEND = "Perpend";
		/*const std::string 	TRAJECTORY = "Trajectory";*/ //TODO: develop trajectory link
		const std::string 	REVOLUTEPRISMATIC = "RevolutePrismatic";
		/*const std::string 	DISTANCE = "Distance";*/ //TODO: develop distance link



		const std::string VELOCITY = "Velocity";		
		const std::string TRANSLATIONAL_DOF_S = "translationalDOFs";
		const std::string ROTATIONAL_DOF_S = "rotationalDOFs";
		const std::string FREE_DOF = "free";
	}

/// @addtogroup parser_isymtec_ai
/// @{
/// isymtec.ai Link parser
class ChApiIsymtecAI ChLinkParserIsymtecAi: public ChElementaryParserIsymtecAi {
  public:

	  ChLinkParserIsymtecAi(std::shared_ptr<ChRelationsIsymtecAi> relations,
		  std::shared_ptr<const ChFunctionStorage> functionStorage);
  
	 
private:
	virtual void doParseObject() override final;

	std::shared_ptr<chrono::ChBody> GetBody1() const;
	std::shared_ptr<chrono::ChBody> GetBody2() const;

	std::shared_ptr<chrono::ChLinkMotorLinearSpeed> CreateTranslationalVelocityLink(const std::string& dofExpression, size_t dofIndex);
	std::shared_ptr<chrono::ChLinkMotorRotationSpeed> CreateRotationalVelocityLink(const std::string& dofExpression, size_t dofIndex);
	void initializeVelocityLink(size_t dofIndex, std::shared_ptr<chrono::ChLinkMateGeneric> link, std::string markerNamePostfix);

	std::vector<std::shared_ptr<chrono::ChLink>> CreateVelocityLinks();
	std::vector<std::shared_ptr<chrono::ChLink>> CreateLinks();


	std::shared_ptr<chrono::ChMarker> m_Marker1;
	std::shared_ptr<chrono::ChMarker> m_Marker2;
	
	std::shared_ptr<const ChFunctionStorage> m_FunctionStorage;
};

#endif
