/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectionMask.h
 * Author: opiske
 *
 * Created on April 20, 2016, 2:07 PM
 */

#ifndef OBJECTCONTROL_H
#define OBJECTCONTROL_H

namespace dtests {
namespace proton {
namespace reactor {

enum ObjectControl {
    CONNECTION = 0x01,
    SESSION = 0x02,
    SENDER = 0x04,
    RECEIVER = 0x08
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* OBJECTCONTROL_H */

