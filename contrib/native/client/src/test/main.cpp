#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char **argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* testSuite = registry.makeTest();
    CppUnit::Test* test;
    if (argc > 1) {
        test = testSuite->findTest(argv[1]);
    }
    else {
        test = testSuite;
    }

    runner.addTest( testSuite );
    bool wasSuccessful = runner.run("", false );
    return !wasSuccessful;
}
