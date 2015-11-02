#ifndef DTESTS_COMMON_UTILS_H_
#define DTESTS_COMMON_UTILS_H_

#include <string>


/**
 * Get current time downto miliseconds (as double) since epoch inspired by 
 * python's time.time()
 */
double get_time();


/**
 * Platform independent sleep function with miliseconds precision
 */
void msleep(long in_dur_ms);

/**
 * Platform independent sleep function with seconds precision
 */
void sleep(float in_dur_sec);

/**
 * Sleep for next loop action (sleeps for next loop iteration, defined by:)
 * @param in_ts initial timestamp
 * @param in_count number of iterations
 * @param in_duration total time of all iterations
 * @param in_indx next iteration index
 * @note this function may be used to define equidistant events
 */
void sleep4next(double in_ts, int in_count, int in_duration, int in_indx);



/**
 * Gets the value of requested environment variable (in_var)
 */
std::string get_env(std::string in_var);


/**
 * Sets stdout/err as non-buffered operations
 */
void set_stdx_unbuffered();


/** Simple string formatter.
 *
 * @param in_str input string
 * @param in_indx integer index
 */
std::string str_fmt(const std::string& in_str, int in_indx);

// ---------------------------------------------------------------------------
// timestamping methods
// ---------------------------------------------------------------------------

/** 
 * Timestamping initializer returning timestamp array pointer (to in_tsdata) or
 * NULL (based on timesnapping mode in_optmode)
 * @param in_tsdata is pointr to statically allocated array
 * @param in_optmode timesnapping mode
 */
double * ts_init(double *in_tsdata, std::string in_optmode);


/**
 * Add timestamp of a step (in_step) to timestamp store/array (in_tsdata) if 
 * timesnapping mode (in_optmode) is allowing that or process and store message 
 * timestam into latency fields
 * 
 * @param in_tsdata timestamp store/array
 * @param in_optmode timesnapping mode
 */
void ts_snap_store(double *in_tsdata, char in_step, std::string in_optmode,
        double in_msg_ts = -1.0);

/**
 * Returns current timestamp python dictionary as string
 * 
 * @param in_tsdata
 * @param in_msgcnt
 * @param in_msgsize
 * @param in_ecode
 * @return the current timestamp python dictionary as string
 */
std::string ts_report(double *in_tsdata, long in_msgcnt, long in_msgsize,
        int in_ecode);


#endif /* DTESTS_COMMON_UTILS_H_ */
