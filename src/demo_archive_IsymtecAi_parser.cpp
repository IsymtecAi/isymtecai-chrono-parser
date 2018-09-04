// =============================================================================
// =============================================================================
// Authors: Dmitry Vlasenko
// =============================================================================
// Serialization Demo for the Mbs.Isymtec.Ai  Chrono parser
// Parse the input file, simulates it and  and write the serialized model in 
///demo_archive_IsymtecAi_parser.json file
// =============================================================================

#include "chrono/utils/ChUtilsValidation.h"
#include "chrono/utils/ChUtilsInputOutput.h"

#include <cassert>
#include "physics/ChGlobal.h"
#include "physics/ChSystemSMC.h"
#include "parser_isymtec_ai/ChParserIsymtecAi.h"
#include "parser_isymtec_ai/ChSimulationParameters.h"
#include "parser_isymtec_ai/ChIsymtecAiConstants.h"

#include "chrono/core/ChFileutils.h"
#include "chrono/serialization/ChArchive.h"
#include "chrono/serialization/ChArchiveJSON.h"

using namespace chrono;

#include "parser_isymtec_ai/ChSimulationIsymtecAi.h"

class ChSimulationDemo : public  ChSimulationIsymtecAi {
public:
	mutable std::vector<double> m_CoordinateXBody;
	mutable std::vector<double> m_Time;
private:
	virtual void ProcessOutputTimeStep() const override final {}
};

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc > 1) {
        filename = std::string(argv[1]);
    } else {
		std::cout << "no input filename!!!";
		return -1;
    }
	ChSimulationDemo simulation;
	bool parseOk = simulation.Parse(filename);

	if (!parseOk) {
		std::cout << "Can't parse the file " + filename;
		return -1;
	}

	//simulation.Simulate();

	
	const std::string out_dir = GetChronoOutputPath() + "DEMO_ARCHIVE";
	if (ChFileutils::MakeDirectory(out_dir.c_str()) < 0) {
		std::cout << "Error creating directory " << out_dir << std::endl;
		return -1;
	}

	std::string jsonfile = out_dir + "/demo_archive_IsymtecAi_parser.json";
	ChStreamOutAsciiFile mfileo(jsonfile.c_str());

	// Use a JSON archive object to serialize C++ objects into the file
	ChArchiveOutJSON marchiveout(mfileo);


	auto system = simulation.GetSystem();

	marchiveout << CHNVP(simulation, "simulation");
	//marchiveout << CHNVP(system, "system");  
}
