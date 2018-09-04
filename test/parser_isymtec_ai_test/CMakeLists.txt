file(GLOB SRCS *.cpp *.h)

ADD_EXECUTABLE(IsymtecAi_parser_test ${SRCS})

TARGET_LINK_LIBRARIES(IsymtecAi_parser_test
    IsymtecAi_parser
	muparser
    gtest
    gmock_main
)

add_test(NAME IsymtecAi_parser_test
         COMMAND IsymtecAi_parser_test
		 WORKING_DIRECTORY ${TEST_DATA_DIRECTORY})

message("Data dir " ${TEST_DATA_DIRECTORY})