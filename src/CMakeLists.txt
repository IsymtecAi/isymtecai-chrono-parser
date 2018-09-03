#-----------------------------------------------------------------------------
# Set the base include directories
#-----------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(parser_isymtec_ai)
add_subdirectory(muparser)

if (ENABLE_MODULE_IRRLICHT)

#--------------------------------------------------------------
# === 3 ===
# Add the executable from your project and specify all C++ 
# files in your project. 
#--------------------------------------------------------------


add_executable(demo_IRR_IsymtecAi_parser demo_IRR_IsymtecAi_parser.cpp)


#add_executable(demo_IRR_IsymtecAi_parser my_example.cpp)

#--------------------------------------------------------------
# Set properties for your executable target
# 
# Note that here we define a macro CHRONO_DATA_DIR which will
# contain the path to the Chrono data directory, either in its
# source tree (if using a build version of Chrono), or in its
# install tree (if using an installed version of Chrono).
#--------------------------------------------------------------

set_target_properties(demo_IRR_IsymtecAi_parser PROPERTIES 
	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

#--------------------------------------------------------------
# Link to Chrono libraries and dependency libraries
#--------------------------------------------------------------

target_link_libraries(demo_IRR_IsymtecAi_parser IsymtecAi_parser muparser ${CHRONO_LIBRARIES})
endif(ENABLE_MODULE_IRRLICHT)