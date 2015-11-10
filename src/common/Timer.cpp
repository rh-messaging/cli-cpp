/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimeoutHandler.cpp
 * Author: opiske
 * 
 * Created on November 5, 2015, 5:05 PM
 */

#include "Timer.h"

using namespace dtests::common;

Timer::Timer(int t, const string &name)
    : timeout(t), remaining(t), name(name)
{
}

Timer::~Timer()
{
}

void Timer::reset() {
    remaining = timeout;
}

bool Timer::isExpired() const {
    return (remaining == 0);
}

Timer &Timer::operator--()
{
    remaining--;
    if (remaining < 0) {
        throw TimerExpiredException("The timer " + name + " has expired");
    }
    return *this;
}

Timer Timer::operator--(int) {
    Timer tmp(*this);
    operator--();
    return tmp;
}

