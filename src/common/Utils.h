#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>


// get_time ()
// get current time downto miliseconds (as double) since epoch
// inspired by python's time.time()
// --------------------------------------------------------------------------
double get_time();

// platform independent sleep functions (milisecond precision)
// --------------------------------------------------------------------------
void msleep(long in_dur_ms);
void sleep(float in_dur_sec);

// sleep for next loop action
// sleeps for next loop iteration, defined by
//   * in_ts       initial timestamp
//   * in_count    number of iterations
//   * in_duration total time of all iterations
//   * in_indx     next iteration index
// function may be used to define equidistant events
// --------------------------------------------------------------------------
void sleep4next(double in_ts, int in_count, int in_duration, int in_indx);


// string get_env(string in_var)
// gets value of requested environment variable (in_var)
// --------------------------------------------------------------------------
std::string get_env(std::string in_var);

// set_stdx_unbuffered()
// sets stdout/err as non-buffered operations
// --------------------------------------------------------------------------
void set_stdx_unbuffered();


/** Simple string formatter.
  * string str_fmt( const string& in_str, int in_indx )
  *
  *   const string& in_str   - input string
  *   int in_indx            - integer index
  */
std::string str_fmt( const std::string& in_str, int in_indx );

// ---------------------------------------------------------------------------
// timestamping methods
// ---------------------------------------------------------------------------

// double * ts_init(double *in_tsdata, std::string in_optmode)
// timestamping initializer returning timestamp array pointer (to in_tsdata) or
// NULL (based on timesnapping mode in_optmode)
// in_tsdata is pointr to statically allocated array
double * ts_init(double *in_tsdata, std::string in_optmode);

// void ts_snap_store(double *in_tsdata, char in_step, std::string in_optmode,
//                    double in_msg_ts)
// add timestamp of a step (in_step) to timestamp store/array
// (in_tsdata) if timesnapping mode (in_optmode) is allowing that
// or process and store message timestam into latency fields
void ts_snap_store(double *in_tsdata, char in_step, std::string in_optmode,
                   double in_msg_ts = -1.0);

// ts_report(in_tsdata, in_msgcnt, in_msgsize)
// return curent timestamp python dictionary as string
std::string ts_report(double *in_tsdata, long in_msgcnt, long in_msgsize,
                      int in_ecode);


#endif
