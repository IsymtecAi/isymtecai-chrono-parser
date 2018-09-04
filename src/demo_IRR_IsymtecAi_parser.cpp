// =============================================================================
// =============================================================================
// Authors: Dmitry Vlasenko
// =============================================================================
// Animation demo for the Mbs.Isymtec.Ai  Chrono parser
// Parse the input file and animate the generated chrono model. 
// =============================================================================


#include "chrono_irrlicht/ChIrrApp.h"
#include "chrono/utils/ChUtilsValidation.h"
#include "chrono/utils/ChUtilsInputOutput.h"

#include <cassert>
#include "physics/ChGlobal.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChParserIsymtecAi.h"
#include "parser_isymtec_ai/ChSimulationParameters.h"
#include "parser_isymtec_ai/ChIsymtecAiConstants.h"

using namespace chrono;
using namespace chrono::irrlicht;
using namespace irr;

using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

void animate(std::shared_ptr <chrono::ChSystemSMC > &system, std::shared_ptr<ChSimulationParameters> simulationParmeters)
{
	//// Set up Irrlicht
	ChIrrApp application(system.get(), L"Model loaded from isymtec.ai file", irr::core::dimension2d<u32>(800, 600), false, true);
	application.AddTypicalLogo();
	application.AddTypicalSky();
	application.AddTypicalLights();

	application.AssetBindAll();
	application.AssetUpdateAll();

	// Simulation loop
	application.SetTimestep(simulationParmeters->GetIntegratorStepSize());

	irr::f32 sizeScene = 15;
	application.AddTypicalCamera(core::vector3df(0, 0, -sizeScene));
	system->SetTimestepperType(ChTimestepper::Type::EULER_IMPLICIT_LINEARIZED);

	IGUIStaticText* textTime =
		application.GetIGUIEnvironment()->addStaticText(L"Time:", rect<s32>(300, 85, 400, 100), false);


	while (application.GetDevice()->run()) {
		application.BeginScene();
		application.DrawAll();

		// .. draw a grid
		ChIrrTools::drawGrid(application.GetVideoDriver(), 0.5, 0.5);

		application.DoStep();
		application.EndScene();
		std::wstring time = L"Time: " + std::to_wstring(system->GetChTime());
		textTime->setText(time.c_str());
	}
}



int main(int argc, char* argv[]) {
    // Get isymtecAi input file (relative to the 'data' directory)
    std::string filename;
    if (argc > 1) {
        filename = std::string(argv[1]);
    } else {
		std::cout << "no input filename!!!";
		return -1;
    }

	auto parser = std::make_shared<ChParserIsymtecAi>();
	auto system = std::make_shared<ChSystemSMC>();
	bool isParsed = parser->Parse(system, filename);
	
	if (!isParsed) {
		std::cout << "Can't parse the file " + filename;
		return -1;
	}

	animate(system, parser->getSimulationParameters());

    return 0;
}
