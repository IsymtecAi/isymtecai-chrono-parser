// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Dmitry Vlasenko
// =============================================================================
//
// Demo for the IsymtecAi -> Chrono parser
//
// =============================================================================


#include "chrono_irrlicht/ChIrrApp.h"
#include "chrono/utils/ChUtilsValidation.h"
#include "chrono/utils/ChUtilsInputOutput.h"

//
#include <cassert>
#include "physics/ChGlobal.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChParserIsymtecAi.h"
#include "parser_isymtec_ai/ChSimulationParameters.h"
#include "parser_isymtec_ai/ChIsymtecAiConstants.h"

//#include <cmath>
//#include <functional>
//
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
        filename = "isymtecAi/test_fourbar.isym";
		filename = isymtec_ai_constants::ISYMTEC_DATA_PATH + filename;
    }

	//ChSimulationIsymtecAI simulation;
	//simulation.Parse(filename);
	//simulation.Simulate();
	//simulation.WriteData(filename + ".out");

	auto parser = std::make_shared<ChParserIsymtecAi>();
	auto system = std::make_shared<ChSystemSMC>();
	bool isParsed = parser->Parse(system, filename);
	assert(isParsed);

	animate(system, parser->getSimulationParameters());

    return 0;
}