/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FakeBean.h
 * Author: opiske
 *
 * Created on October 29, 2015, 12:08 PM
 */

#ifndef FAKEBEAN_H
#define FAKEBEAN_H

#include <string>
#include <map>
#include <list>

using std::string;
using std::map;
using std::list;

class FakeBean {
  public:
    FakeBean();
    virtual ~FakeBean();
    
    void setListProperty(list<string> listProperty);
    list<string> getListProperty() const;
    
    void setMapProperty(map<string, string> mapProperty);
    map<string, string> getMapProperty() const;
    
    void setIntProperty(int intProperty);
    int getIntProperty() const;
    
    void setBooleanProperty(bool booleanProperty);
    bool isBooleanProperty() const;
    
    void setStringProperty(string stringProperty);
    string getStringProperty() const;
  
  private:
    
    string stringProperty;
    bool booleanProperty;
    int intProperty;
    map<string, string> mapProperty;
    list<string> listProperty;

};

#endif /* FAKEBEAN_H */

