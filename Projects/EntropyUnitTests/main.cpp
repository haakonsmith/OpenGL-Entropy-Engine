#define CATCH_CONFIG_MAIN
#include <catch2.hpp>

#include <Entropy.hpp>

#include <string>
#include <fstream>

using namespace std;

void testTimeFunc()
{
    PROFILE_FUNCTION();
}

void compareFiles(ifstream &test, ifstream &ref)
{
    std::string string1, string2;
    int j = 0;

    cout << "File comparison" << endl;
    while (getline(test, string1))
    {
        
        getline(ref, string2);

        cout << "Test: " << string1 << endl;
        cout << "Ref:  " << string2 << endl;


        INFO("The Lines are not equal:");
        INFO("Line " << (j + 1) << "    -> " << string1);
        INFO("Should be ->" << string2);
        CHECK(string2 == string1);
        
        j++;
    }
}

TEST_CASE("Profile tests", "[Entropy]")
{
    using namespace Entropy;
    App::profiler.startFrame();
    testTimeFunc();
    App::profiler.endFrame();

    ifstream profile, test;

    test.open("Reference/Tracing/main.json");
    profile.open("Tracing/main.json");

    compareFiles(profile, test);
}