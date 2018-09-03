file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(mu_parser_test ${SRCS})

TARGET_LINK_LIBRARIES(mu_parser_test
	muparser
    gtest
    gmock_main
)


add_test(NAME mu_parser_test
         COMMAND mu_parser_test)
