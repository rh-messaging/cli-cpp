/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ut_options_test.cpp
 * Author: opiske
 *
 * Created on November 3, 2015, 6:51 PM
 */

#include <cstdlib>
#include <map>
#include <stdlib.h>
#include <optparse/OptionParser.h>
#include <OptionsHelper.h>


using namespace std;
using namespace dtests::common;
using namespace optparse;

class FakeBean {
public:
    void setTestDataMap(map<string, string> testDataMap)
    {
        this->testDataMap = testDataMap;
    }

    map<string, string> getTestDataMap() const
    {
        return testDataMap;
    }

private:
    map<string, string> testDataMap;

};

class TestOptionsParser : public OptionParser {
    
public:
    TestOptionsParser() { 
        OptionParser::add_option("-m", "--map")
            .dest("map")
            .help("sample-map");
    }
};

/*
 * 
 */
int main(int argc, char** argv)
{
    TestOptionsParser parser = TestOptionsParser();

    optparse::Values options = parser.parse_args(argc, argv);

    OptionsSetter setter = OptionsSetter(options);
    
    FakeBean fb;
    
    setter.setMap("map", &fb, (&FakeBean::setTestDataMap));

    map<string, string> m = fb.getTestDataMap();
    
    size_t expected_size = 2;
    if (m.size() != expected_size) {
       cerr << "The map size does not match the expected size:" << 
               m.size() << " != " << expected_size << endl;
       
       return EXIT_FAILURE;
    }
    
    string firstValue = m["country1"];
    if (firstValue != "Bahamas") {
        cerr << "The fist value " << firstValue << 
                " does match the expected value" << endl;
       
       return EXIT_FAILURE;
    }
    
    string secondValue = m["country2"];
    if (secondValue != "Czech Republic") {
        cerr << "The fist value " << secondValue << 
                " does match the expected value" << endl;
       
       return EXIT_FAILURE;
    }
    
    return 0;
}

