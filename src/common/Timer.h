/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimeoutHandler.h
 * Author: opiske
 *
 * Created on November 5, 2015, 5:05 PM
 */

#ifndef TIMEOUTHANDLER_H
#define TIMEOUTHANDLER_H

#include "exceptions/TimerExpiredException.h"

namespace dtests {
namespace common {

using std::string;

class Timer {
  public:
    Timer(int, const string &name = "");
    virtual ~Timer();
      
    void reset();
    bool isExpired() const;
    
    Timer &operator--();
    Timer operator--(int);
    
  private:
    const int timeout;
    int remaining;
    string name;

};

} /* namespace common */
} /* namespace dtests */


#endif /* TIMEOUTHANDLER_H */

