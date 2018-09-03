#--------------------------------------------------------------
# === 3 ===
# Add the executable from your project and specify all C++ 
# files in your project. 
#--------------------------------------------------------------


set(elementary_parsers_SOURCES 
	ChBodyParserIsymtecAi.cpp	
	ChGeometryParserIsymtecAi.cpp
	ChLinkParserIsymtecAi.cpp
	ChFunctionParserIsymtecAi.cpp
	ChForceParserIsymtecAi.cpp
	ChElementaryParserIsymtecAi.cpp
)

set(elementary_parsers_HEADERS
	ChBodyParserIsymtecAi.h
	ChGeometryParserIsymtecAi.h
	ChLinkParserIsymtecAi.h
	ChFunctionParserIsymtecAi.h
	ChForceParserIsymtecAi.h
	ChElementaryParserIsymtecAi.h
)

source_group(headers\\elementary_parsers FILES
    ${elementary_parsers_HEADERS})	

source_group(sources\\elementary_parsers FILES
    ${elementary_parsers_SOURCES})	


set(parser_isymtec_ai_SOURCES
	ChIsymtecAiUtils.cpp
	ChParserIsymtecAi.cpp
	ChRelationsIsymtecAi.cpp
	ChMarkerParserIsymtecAi.cpp
	ChMuParserFunction.cpp
	ChFramesUtils.cpp
	ChSimulationParameters.cpp
	ChFunctionStorage.cpp
	ChSimulationIsymtecAi.cpp
	ChMarkerDefined3DFunction.cpp
	Ch3DFunctionPart.cpp
    ) 

set(parser_isymtec_ai_HEADERS
	ChApiIsymtecAi.h
	ChIsymtecAiUtils.h
	ChParserIsymtecAi.h	
	ChRelationsIsymtecAi.h
	ChMarkerParserIsymtecAi.h
	ChMuParserFunction.h
	ChFramesUtils.h
	ChSimulationParameters.h
	ChFunctionStorage.h
	ChSimulationIsymtecAi.h
	ChForceParserIsymtecAi.h
	ChExceptionIsymtecAi.h
	ChMarkerDefined3DFunction.h
	Ch3DFunctionPart.h
	ChIsymtecAiConstants.h
	)

source_group(headers\\parser_isymtec_ai FILES
    ${parser_isymtec_ai_HEADERS})	

source_group(sources\\parser_isymtec_ai FILES
    ${parser_isymtec_ai_SOURCES})		

set(parser_isymtec_ai_FILES
    ${elementary_parsers_SOURCES}
    ${elementary_parsers_HEADERS}
    ${parser_isymtec_ai_SOURCES}
    ${parser_isymtec_ai_HEADERS}	
    )	


add_library(IsymtecAi_parser SHARED ${parser_isymtec_ai_FILES})

#--------------------------------------------------------------
# Set properties for your executable target
# 
# Note that here we define a macro CHRONO_DATA_DIR which will
# contain the path to the Chrono data directory, either in its
# source tree (if using a build version of Chrono), or in its
# install tree (if using an installed version of Chrono).
#--------------------------------------------------------------

set_target_properties(IsymtecAi_parser PROPERTIES 
	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

target_compile_definitions(IsymtecAi_parser PRIVATE "CH_API_COMPILE_ISYMTEC_AI")

#--------------------------------------------------------------
# Link to Chrono libraries and dependency libraries
#--------------------------------------------------------------

target_link_libraries(IsymtecAi_parser 
	muparser
	${CHRONO_LIBRARIES}
)


# Install the main IsymtecAi_parser library
install(TARGETS IsymtecAi_parser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib64
    ARCHIVE DESTINATION lib64)

# Install all header files for the main IsymtecAi_parser library 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/parser_isymtec_ai 
    FILES_MATCHING REGEX ".*\\.(h|inl)$")
#    FILES_MATCHING PATTERN "*.h")
