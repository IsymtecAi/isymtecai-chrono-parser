#--------------------------------------------------------------
# === 3 ===
# Add the executable from your project and specify all C++ 
# files in your project. 
#--------------------------------------------------------------

set(muparser_SOURCES
	muParserBase.cpp
	muParserBytecode.cpp
	muParserCallback.cpp
	muParser.cpp
	muParserDLL.cpp
	muParserError.cpp
	muParserInt.cpp
	muParserTest.cpp
	muParserTokenReader.cpp
    )

set(muparser_HEADERS
    muParserBase.h
    muParserBytecode.h
    muParserCallback.h
    muParserDef.h
    muParserDLL.h
    muParserError.h
    muParserFixes.h
    muParser.h
    muParserInt.h
    muParserStack.h
    muParserTemplateMagic.h
    muParserTest.h
    muParserToken.h
    muParserTokenReader.h
	)
	


set(muparser_FILES
    ${muparser_SOURCES}
    ${muparser_HEADERS}	
    )	


add_library(muparser SHARED ${muparser_FILES})


# this compiles the "DLL" interface (C API)
target_compile_definitions(muparser PRIVATE MUPARSER_DLL)
target_compile_definitions(muparser PRIVATE MUPARSERLIB_EXPORTS)


# Install the main muparser library
install(TARGETS muparser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib64
    ARCHIVE DESTINATION lib64)

# Install all header files for the main muparser library 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/muparser 
    FILES_MATCHING REGEX ".*\\.(h|inl)$")
#    FILES_MATCHING PATTERN "*.h")
