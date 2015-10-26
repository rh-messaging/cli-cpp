/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectorOptionsParser.h
 * Author: opiske
 *
 * Created on October 14, 2015, 1:35 PM
 */

#ifndef CONNECTOROPTIONSPARSER_H
#define CONNECTOROPTIONSPARSER_H

#include <optparse/OptionParser.h>

#include "options/modern/ModernOptionsParser.h"

class ConnectorOptionsParser : public ModernOptionsParser {
  public:
    ConnectorOptionsParser();
    virtual ~ConnectorOptionsParser();

    virtual void validate(const Values &options) const;
  private:

    typedef ModernOptionsParser super;

};

#endif /* CONNECTOROPTIONSPARSER_H */

