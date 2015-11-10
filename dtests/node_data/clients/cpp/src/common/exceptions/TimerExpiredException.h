/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimerExpiredException.h
 * Author: opiske
 *
 * Created on November 10, 2015, 1:23 PM
 */

#ifndef DTESTS_COMMON_TIMEREXPIREDEXCEPTION_H
#define DTESTS_COMMON_

#include "BaseException.h"

#include <string>

namespace dtests {
namespace common {

using namespace std;


class TimerExpiredException: public BaseException {
  public:
    TimerExpiredException(const string &msg);
    virtual ~TimerExpiredException();
  
  private:
    typedef BaseException super;

};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_TIMEREXPIREDEXCEPTION_H */

