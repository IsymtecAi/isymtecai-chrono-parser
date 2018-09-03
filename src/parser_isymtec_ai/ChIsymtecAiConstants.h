#ifndef CH_ISYMTEC_AI_CONSTANTS_H
#define CH_ISYMTEC_AI_CONSTANTS_H

#include <string>

/// @addtogroup parser_isymtec_ai
namespace isymtec_ai_constants {
#if defined(_WIN32) || defined(_WIN64)
	static std::string ISYMTEC_DATA_PATH{ "../../data/" };
#else
	static std::string ISYMTEC_DATA_PATH{ "../data/" };
#endif
}

#endif