#ifndef __TIMECOMPATIBILITY_H__
#define __TIMECOMPATIBILITY_H__


#include <qpid/sys/Time.h>


// This trick prevents the unused warning when compiling through configlib
// For GCC >= 4.6 it saves the previously set unused warning state. For older 
// versions, it restores the default 'warning' behavior (default GCC level). 
#if defined __GNUC__ && (__GNUC__ >= 4)
# define ATTRIBUTES __attribute__((unused))
#else
# define ATTRIBUTES
#endif // __GNUC__ 

// Duration duration_since_epoch(string in_var)
// gets duration since Epoch
// --------------------------------------------------------------------------
ATTRIBUTES static qpid::sys::Duration duration_since_epoch()
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
