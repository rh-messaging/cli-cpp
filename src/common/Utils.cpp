
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "sys/timeb.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <cstdio>

#include "Utils.h"

//MAX_INT is 2^64 so the number can take a maximum of 20 characters
//INT_CONSTANT is 32 just to be sure
#define INT_CONSTANT 32

// get_time ()
// get current time downto miliseconds (as double) since epoch
// inspired by python's time.time()
// --------------------------------------------------------------------------

double get_time()
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return ((unsigned long int) (ts.tv_sec) + (ts.tv_nsec / (1000.0 * 1000.0)));
}


// platform independent sleep functions
// --------------------------------------------------------------------------

void msleep(long in_dur_ms)
{
    if (in_dur_ms > 0) {
#ifdef _WIN32
        Sleep(in_dur_ms);
#else
        ::usleep(1000 * in_dur_ms);
#endif
    }
}

void sleep(float in_dur_sec)
{
    if (in_dur_sec > 0.0) {
        msleep(long(in_dur_sec * 1000));
    }
}

// sleep for next loop action
// sleeps for next loop iteration, defined by
//   * in_ts       initial timestamp
//   * in_count    number of iterations
//   * in_duration total time of all iterations
//   * in_indx     next iteration index
// function may be used to define equidistant events
// --------------------------------------------------------------------------

void sleep4next(double in_ts, int in_count, int in_duration, int in_indx)
{
    if ((in_duration > 0) && (in_count > 0)) {
        double cummulative_dur = (1.0 * in_indx * in_duration) / in_count;
        while (true) {
            if (get_time() - in_ts - cummulative_dur > -0.02)
                break;

            msleep(100);
        }
    }
}

// string get_env(string in_var)
// gets value of requested environment variable (in_var)
// --------------------------------------------------------------------------

std::string get_env(std::string in_var)
{
    char * p_env;
    p_env = ::getenv(in_var.c_str());

    if (p_env != NULL)
        return (std::string(p_env));
    else
        return (std::string(""));
}

// set_stdx_unbuffered()
// sets stdout/err as non-buffered operations
// --------------------------------------------------------------------------

void set_stdx_unbuffered()
{
    std::cout.setf(std::ios::unitbuf);
    std::cerr.setf(std::ios::unitbuf);
}

/** Simple string formatter.
 * string str_fmt( const string& in_str, int in_indx )
 *
 *   const string& in_str   - input string
 *   int in_indx            - integer index
 */
std::string str_fmt(const std::string& in_str, int in_indx)
{
    std::string outputString;
    char *str_int = new char[sizeof(*str_int) *
                             (in_str.size() + INT_CONSTANT)];
    if (sprintf(str_int, in_str.c_str(), in_indx) > 0) {
        outputString = str_int;
    } else {
        outputString = in_str;
    }
    delete [] str_int;

    // return the string value
    return (outputString);
}


// --------------------------------------------------------------------------
// timestamping methods
// --------------------------------------------------------------------------

// double * ts_init(double *in_tsdata, std::string in_optmode)
// timestamping initializer returning timestamp array pointer (to in_tsdata) or
// NULL (based on timesnapping mode in_optmode)
// in_tsdata is pointr to statically allocated array

double * ts_init(double *in_tsdata, std::string in_optmode)
{
    double *int_ptr = NULL;
    if (in_optmode.find("perf") != std::string::npos)
        int_ptr = in_tsdata;

    return (int_ptr);
}

// void ts_snap_store(double *in_tsdata, char in_step, std::string in_optmode, 
//                    double in_msg_ts)
// add timestamp of a step (in_step) to timestamp store/array
// (in_tsdata) if timesnapping mode (in_optmode) is allowing that
// or process and store message timestam into latency fields

void ts_snap_store(double *in_tsdata, char in_step, std::string in_optmode,
        double in_msg_ts)
{
    if (in_tsdata != NULL) {
        if ((in_step == 'L') && (in_msg_ts != -1.0)) {
            // latency manipulation
            double int_lat = get_time() - in_msg_ts;
            if (int_lat < *(in_tsdata + 7))
                *(in_tsdata + 7) = int_lat;
            if (int_lat > *(in_tsdata + 8))
                *(in_tsdata + 8) = int_lat;
            *(in_tsdata + 9) = *(in_tsdata + 9) + int_lat;
        } else {
            if ((in_optmode.find(in_step) != std::string::npos)) {
                // regular timestamp snap
                long int_index = (int) (in_step) - (int) ('A');
                *(in_tsdata + int_index) = get_time();
            }
        }
    }
}

// ts_report(in_tsdata, in_msgcnt, in_msgsize)
// return curent timestamp python dictionary as string

std::string ts_report(double *in_tsdata, long in_msgcnt, long in_msgsize,
        int in_ecode)
{
    std::stringstream int_result;
    int int_indx;

    if (in_tsdata != NULL) {
        int_result << "{ 'msg-cnt' : " << in_msgcnt << ", 'msg-size' : "
                << in_msgsize << ", 'ecode' : " << in_ecode;
        for (int_indx = 0; int_indx < 7; int_indx++) {
            double int_value = *(in_tsdata + int_indx);
            int_result << ", 'ts-" << (char) ((int) (int_indx) + (int) ('A')) << "' : ";
            if (int_value <= 0)
                int_result << "";
            else
                int_result << std::fixed << int_value;
        }
        int_result << ", 'lat-min' : " << *(in_tsdata + 7) << ", 'lat-max' : "
                << *(in_tsdata + 8) << ", 'lat-acc' : " << *(in_tsdata + 9);
        if (in_msgcnt != 0)
            int_result << ", 'lat-avg' : " << (*(in_tsdata + 9)) / in_msgcnt;

        int_result << " }";
    } else {
        int_result << "{ }";
    }

    return (int_result.str());
}


// eof
