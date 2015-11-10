/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimerExpiredException.cpp
 * Author: opiske
 * 
 * Created on November 10, 2015, 1:23 PM
 */

#include "TimerExpiredException.h"

using namespace dtests::common;

TimerExpiredException::TimerExpiredException(const string &msg)
    : super(msg)
{

}

TimerExpiredException::~TimerExpiredException()
{
}

