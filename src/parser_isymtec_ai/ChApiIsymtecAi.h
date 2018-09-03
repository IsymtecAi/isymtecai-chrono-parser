#ifndef CHAPI_ISYMTEC_AI_H
#define CHAPI_ISYMTEC_AI_H

#include <memory>
#include "chrono/ChVersion.h"
#include "chrono/core/ChPlatform.h"

// When compiling this library, remember to define CH_API_COMPILE_ISYMTEC_AI
// (so that the symbols with 'ChApiIsymtecAI' in front of them will be
// marked as exported). Otherwise, just do not define it if you
// link the library to your code, and the symbols will be imported.

#if defined(CH_API_COMPILE_ISYMTEC_AI)
#define ChApiIsymtecAI ChApiEXPORT
#else
#define ChApiIsymtecAI ChApiIMPORT
#endif

/**
    @defgroup IsymtecAi_parser_module IsymtecAi parser
    @brief Parser of Isymtec.Ai MBS output files

*/

#endif