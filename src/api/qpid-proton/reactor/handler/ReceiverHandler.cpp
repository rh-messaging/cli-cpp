/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceiverHandler.cpp
 * Author: opiske
 * 
 * Created on October 19, 2015, 4:43 PM
 */

#include "ReceiverHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

ReceiverHandler::ReceiverHandler(const string &url)
    : super(url)
{
}

ReceiverHandler::~ReceiverHandler()
{
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */