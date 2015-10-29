/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FakeBean.cpp
 * Author: opiske
 * 
 * Created on October 29, 2015, 12:08 PM
 */

#include "FakeBean.h"

FakeBean::FakeBean()
{
}

FakeBean::~FakeBean()
{
}

void FakeBean::setListProperty(list<string> listProperty)
{
    this->listProperty = listProperty;
}

list<string> FakeBean::getListProperty() const
{
    return listProperty;
}

void FakeBean::setMapProperty(map<string, string> mapProperty)
{
    this->mapProperty = mapProperty;
}

map<string, string> FakeBean::getMapProperty() const
{
    return mapProperty;
}

void FakeBean::setIntProperty(int intProperty)
{
    this->intProperty = intProperty;
}

int FakeBean::getIntProperty() const
{
    return intProperty;
}

void FakeBean::setBooleanProperty(bool booleanProperty)
{
    this->booleanProperty = booleanProperty;
}

bool FakeBean::isBooleanProperty() const
{
    return booleanProperty;
}

void FakeBean::setStringProperty(string stringProperty)
{
    this->stringProperty = stringProperty;
}

string FakeBean::getStringProperty() const
{
    return stringProperty;
}

