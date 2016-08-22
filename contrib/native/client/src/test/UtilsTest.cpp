#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "utils.hpp"

class UtilsTest: public CppUnit::TestFixture {
public:
    UtilsTest() {}

    CPPUNIT_TEST_SUITE( UtilsTest );
    CPPUNIT_TEST(testParseConnectStr);
    CPPUNIT_TEST_SUITE_END();


    void testParseConnectStr() {
        std::string protocol;
        std::string hostAndPort;
        std::string path;

        Drill::Utils::parseConnectStr("local=localhost:12345/path/to/drill",
                path,
                protocol,
                hostAndPort);

        CPPUNIT_ASSERT(protocol == "local");
        CPPUNIT_ASSERT(hostAndPort == "localhost:12345");
        CPPUNIT_ASSERT(path == "/path/to/drill");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( UtilsTest );
