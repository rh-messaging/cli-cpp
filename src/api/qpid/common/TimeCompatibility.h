#ifndef __TIMECOMPATIBILITY_H__
#define __TIMECOMPATIBILITY_H__


#include <qpid/sys/Time.h>


// This trick prevents the unused warning when compiling through configlib
// For GCC >= 4.6 it saves the previously set unused warning state. For older 
// versions, it restores the default 'warning' behavior (default GCC level). 
#if defined __GNUC__
# if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || (__GNUC__ >= 5)
#   pragma GCC diagnostic push
# endif

# pragma GCC diagnostic ignored "-Wunused"
          

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

# if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || (__GNUC__ >= 5)
#   pragma GCC diagnostic pop
# else
# pragma GCC diagnostic warning "-Wunused"
# endif 
#endif // defined __GNUC__


#endif
