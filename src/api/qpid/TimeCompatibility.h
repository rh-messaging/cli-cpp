#ifndef __TIMECOMPATIBILITY_H__
#define __TIMECOMPATIBILITY_H__


#include <qpid/sys/Time.h>

// Duration duration_since_epoch(string in_var)
// gets duration since Epoch
// --------------------------------------------------------------------------

static qpid::sys::Duration duration_since_epoch()
{
#ifdef QPIDSYSEPOCH
  return(qpid::sys::Duration(qpid::sys::EPOCH, qpid::sys::now()));
#elif QPIDSYSDURATIONFROMEPOCH
  return(qpid::sys::Duration::FromEpoch());
#else
  #error Either QPIDSYSEPOCH or QPIDSYSDURATIONFROMEPOCH macro has to be defined!
#endif
}


#endif
