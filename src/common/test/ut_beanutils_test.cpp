/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ut_beanutils_test.c
 * Author: opiske
 *
 * Created on October 29, 2015, 2:22 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include <TestUtils.h>
#include <BeanUtils.h>

#include "FakeBean.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    FakeBean fakeBean;
    bool ret;
    
    BeanUtils::set("testString", &fakeBean, &FakeBean::setStringProperty);
   
    string stringProperty = fakeBean.getStringProperty();
    ret = assertEquals("testString", stringProperty.c_str()); 
    if (!ret) {
        return 1;
    }
    
    /*
    
    
    void setBooleanProperty(bool booleanProperty);
    bool isBooleanProperty() const;
     */
    const int expectedInt = 6589;
    BeanUtils::set(expectedInt, &fakeBean, &FakeBean::setIntProperty);
   
    int intRet = fakeBean.getIntProperty();
    if (intRet != expectedInt) {
        cerr << "The returned value " << intRet << " does not match the "
                << " ex value: " << expectedInt << endl;
        return 1;
    }
    
    const bool expectedBool = false; 
    BeanUtils::set(expectedBool, &fakeBean, &FakeBean::setBooleanProperty);
    
    bool boolRed = fakeBean.isBooleanProperty(); 
    if (expectedBool != boolRed) {
        cerr << "The returned value " << expectedBool << " does not match the "
                << " ex value: " << expectedBool << endl;
        return 1;
    }
    
    
    return 0;
}

