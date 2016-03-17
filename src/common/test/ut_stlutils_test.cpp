/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ut_stlutils_test.cpp
 * Author: opiske
 *
 * Created on March 16, 2016, 3:00 PM
 */

#include <cstdlib>
#include <vector>
#include <string>

#include <TestUtils.h>
#include <STLUtils.h>

using namespace std;

using dtests::common::split;
using dtests::common::convertToMap;
using dtests::common::convertPropertiesToMap;

int testSplitEmpty() {
    vector<string> vec = split("", ";");
    
    if (assertEquals("", vec[0].c_str())) { 
        return 0;
    }
    
    return 1;
}

int testSplitSingle() {
    vector<string> vec = split("abc", ";");
    
    if (assertEquals("abc", vec[0].c_str())) { 
        return 0;
    }
    
    return 1;
}
        
        
int testSplitMultiple() {
    vector<string> vec = split("abc;123;456;764", ";");
    
    if (assertEquals("abc", vec[0].c_str())) { 
        if (assertEquals("123", vec[1].c_str())) { 
            if (assertEquals("456", vec[2].c_str())) { 
                if (assertEquals("764", vec[3].c_str())) {
                    return 0;
                }
            }
        }
    }
    
    return 1;
}


int testConvertToMap() {
    vector<string> vec = split("key1=123;key2=764;key3=999;key4=longervalue", ";");
    
    map<string, string> properties = map<string, string>();
    convertPropertiesToMap(vec, properties, "=");
    
    if (assertEquals("123", properties["key1"].c_str())) { 
        if (assertEquals("764", properties["key2"].c_str())) { 
            if (assertEquals("999", properties["key3"].c_str())) { 
                if (assertEquals("longervalue", properties["key4"].c_str())) {
                    return 0;
                }
            }
        }
    }
    
    return 1;
}
        
/*
 * 
 */
int main(int argc, char** argv) {
    int ret = testSplitMultiple();
    
    if (ret != 0) {
        return ret;
    }
    
    ret = testSplitSingle(); 
    if (ret != 0) {
        return ret;
    }
    
    ret = testSplitEmpty(); 
    if (ret != 0) {
        return ret;
    }
    
    ret = testConvertToMap(); 
    if (ret != 0) {
        return ret;
    }
    
    return 0;
}

