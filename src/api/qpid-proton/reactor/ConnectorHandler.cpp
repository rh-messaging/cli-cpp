/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectorHandler.cpp
 * Author: opiske
 * 
 * Created on October 14, 2015, 3:33 PM
 */

#include "ConnectorHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

ConnectorHandler::ConnectorHandler(const string &url)
	: super(url)
{
}

ConnectorHandler::~ConnectorHandler()
{
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
