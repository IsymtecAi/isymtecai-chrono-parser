

#include "parser_isymtec_ai/ChParserIsymtecAi.h"
#include "core/ChException.h"
#include "core/ChLog.h"
#include "chrono_thirdparty/rapidjson/filereadstream.h"
#include "chrono_thirdparty/rapidjson/document.h"
#include "chrono/physics/ChSystem.h"
#include "physics/ChObject.h"

#include "parser_isymtec_ai/ChIsymtecAiUtils.h"
#include "parser_isymtec_ai/ChRelationsIsymtecAi.h"
#include "parser_isymtec_ai/ChBodyParserIsymtecAi.h"
#include "ChMarkerParserIsymtecAi.h"
#include "ChGeometryParserIsymtecAi.h"
#include "ChLinkParserIsymtecAi.h"
#include "ChSimulationParameters.h"
#include "parser_isymtec_ai/ChFunctionParserIsymtecAi.h"
#include "parser_isymtec_ai/ChFunctionStorage.h"
#include "parser_isymtec_ai/ChMuParserFunction.h"
#include "parser_isymtec_ai/ChForceParserIsymtecAi.h"
#include "parser_isymtec_ai/ChExceptionIsymtecAi.h"

using namespace chrono;


namespace {

	const std::string CAE_MODEL = "CAEModel";
	const std::string BODY_CONTAINER = "bodyContainer";
	const std::string LINK_CONTAINER = "linkContainer";
	const std::string GEOMETRY_CONTAINER = "geometryContainer";
	const std::string FUNCTION_CONTAINER = "functionContainer";
	const std::string FORCE_CONTAINER = "forceContainer";



	///create rapidjson::Document from the file
	std::shared_ptr<rapidjson::Document> CreateDocument(const std::string &filename)
	{
		FILE* fp = fopen(filename.c_str(), "r");
		if (fp == NULL) {
			std::string errorString = "Can not find file " + filename;
			throw chrono::ChException(errorString);
		}
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		auto document = std::make_shared<rapidjson::Document>();
		document->ParseStream(is);
		fclose(fp);
		return document;
	}
}


ChParserIsymtecAi::ChParserIsymtecAi() :
	m_System(nullptr)
{
	m_SimulationParameters = std::make_shared<ChSimulationParameters>();
	m_FunctionStorage = std::make_shared<ChFunctionStorage>();
}

bool ChParserIsymtecAi::Parse(std::shared_ptr<ChSystem> system,
	const std::string& filename)
{
	try {
		m_System = system;
		m_Relations = std::make_shared<ChRelationsIsymtecAi>();
		m_Relations->Clear();
		m_Relations->SetSystem(m_System);

		m_Document = CreateDocument(filename);
		CreateFunctions();
		SetModelParams();
		CreateBodies();
		CreateMarkers();
		CreateGeometries();
		CreateLinks();
		CreateForces();
		GetLog() << "ChParserIsymtecAi: Parse is ok";
		return true;
	}
	catch (ChExceptionIsymtecAi& parserException) {
		CatchParserException(parserException);
		return false;
	}
	catch (std::exception& myex) {
		// Ops.. file could not be opened or read.. echo what happened!
		GetLog() << "ERROR: " << std::string(myex.what());
		return false;
	}
	catch (...) {
		GetLog() << "Unknown error in ChParserIsymtecAi";
		return false;
	}

}


void ChParserIsymtecAi::CatchParserException(const ChExceptionIsymtecAi& parserException) const {
	
	rapidjson::Value* rapidJsonObj = parserException.getRapidJsonObject();
	std::string objName;
	if (rapidJsonObj != nullptr) {
		try {
			objName = isymtec_ai_utils::GetName(*rapidJsonObj);
		}
		catch (...) {
		}
	}
	std::string propertyName = parserException.getPropertyName();
	GetLog() << "ERROR in ChParserIsymtecAi \n";
	if (objName != "") {
		GetLog() << "Object: " << objName << " \n";
	}
	if (propertyName != "") {
		GetLog() << "Property: " << propertyName << " \n";
	}
	GetLog() << "Error Description: " << parserException.getErrorDescription() << " \n";
}

void ChParserIsymtecAi::SetModelParams()
{
	ProveIsModel();
	isymtec_ai_utils::SetObjectName(*m_Document, *m_System);

	auto& modelParams = isymtec_ai_utils::GetMember(*m_Document, model_isymtec_ai_params::MODEL_PARAMS);
	
	ChVector<> gravityAccel = isymtec_ai_utils::getVectorProperty(modelParams, model_isymtec_ai_params::GRAVITY);
	m_System->Set_G_acc(gravityAccel);

	double startTime = isymtec_ai_utils::GetDoubleProperty(modelParams, model_isymtec_ai_params::START_TIME);
	m_System->SetChTime(startTime);

	double endTime = isymtec_ai_utils::GetDoubleProperty(modelParams, model_isymtec_ai_params::END_TIME);
	m_SimulationParameters->SetEndTime(endTime);

	double integratorTimeStep = isymtec_ai_utils::GetDoubleProperty(modelParams, model_isymtec_ai_params::INTEGRATOR_STEP_SIZE);
	m_SimulationParameters->SetIntegratorStepSize(integratorTimeStep);

	double outputStepSize = isymtec_ai_utils::GetDoubleProperty(modelParams, model_isymtec_ai_params::OUTPUT_STEP_SIZE);
	m_SimulationParameters->SetOutputStepSize(outputStepSize);

}

void ChParserIsymtecAi::ProveIsModel()
{
	std::string type = isymtec_ai_utils::GetStringProperty(*m_Document, isymtec_ai_utils::PROPERTY_TYPE);
	if (type != CAE_MODEL) {
		throw chrono::ChException("Model is of type " + type + " instead of  " + CAE_MODEL);
	}
}

void ChParserIsymtecAi::CreateBodies()
{
	ChBodyParserIsymtecAi bodiesParser(m_Relations);
	auto bodies = isymtec_ai_utils::GetElementsInContainer(*m_Document, BODY_CONTAINER);
	for (auto& bodyFrom : bodies) {
		bodiesParser.ParseObject(bodyFrom);
	}
}

void ChParserIsymtecAi::CreateMarkers()
{
	ChMarkerParserIsymtecAi markerParser(m_Relations);
	auto bodies = isymtec_ai_utils::GetElementsInContainer(*m_Document, BODY_CONTAINER);
	for (auto& bodyFrom : bodies) {
		markerParser.ParseBodyMarkers(bodyFrom);
	}
}

void ChParserIsymtecAi::CreateGeometries()
{
	ChGeometryParserIsymtecAi geometriesParser(m_Relations);
	auto bodies = isymtec_ai_utils::GetElementsInContainer(*m_Document, BODY_CONTAINER);
	for (auto& bodyFrom : bodies) {
		auto geometries = isymtec_ai_utils::GetElementsInContainer(bodyFrom, GEOMETRY_CONTAINER);
		for (auto& geometryFrom : geometries) {
			geometriesParser.ParseObject(geometryFrom);
		}
	}
}

void ChParserIsymtecAi::CreateLinks()
{
	ChLinkParserIsymtecAi linkParser(m_Relations, m_FunctionStorage);
	auto links = isymtec_ai_utils::GetElementsInContainer(*m_Document, LINK_CONTAINER);
	for (auto& linkFrom : links) {
		linkParser.ParseObject(linkFrom);
	}

}

void ChParserIsymtecAi::CreateFunctions() {
	ChFunctionParserIsymtecAi functionParser(m_Relations, m_FunctionStorage);
	auto functions = isymtec_ai_utils::GetElementsInContainer(*m_Document, FUNCTION_CONTAINER);
	for (auto& funcFrom : functions) {
		functionParser.ParseObject(funcFrom);
	}
}

void ChParserIsymtecAi::CreateForces() {
	ChForceParserIsymtecAi forcesParser(m_Relations, m_FunctionStorage);
	auto forces = isymtec_ai_utils::GetElementsInContainer(*m_Document, FORCE_CONTAINER);
	for (auto& forceFrom : forces) {
		forcesParser.ParseObject(forceFrom);
	}
}
