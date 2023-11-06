/**
 * @file main.cpp
 * @author Denys Stovbun (denis.stovbun@lanars.com)
 * @brief Gateway service main entry point
 * @version 1.0.0
 * @date 2023-05-18
 *
 *
 *
 */
// #define _GNU_SOURCE
#include <stdio.h>
#include <sys/time.h>
#include <getopt.h>
#include <stdbool.h>
#include <string>

#include "app.h"
#include "biostar2sdk/BS_API.h"
#include "server.h"
#include "grpc.h"

static int  gLogLevel = LOG_LEVEL_WARNING;    // Logging level
static int  gLogType  = LOG_TYPE_NORMAL;


const char *logLevelHeaders[] = {
    "\033[1;31mERR\033[0m",  // LOG_LEVEL_ERROR     // q = quiet
    "\033[1;91mWRN\033[0m",  // LOG_LEVEL_WARNING   //   = default
    "\033[1;37mINF\033[0m",  // LOG_LEVEL_INFO      // v = verbose
    "\033[1;36mDBG\033[0m",  // LOG_LEVEL_DEBUG     // vvv = verbose++
    "\033[1;33mTRC\033[0m"   // LOG_LEVEL_TRACE     // vv = verbose+
};

const char *logLevelColor[] = {
    "\033[0;31m",  // LOG_LEVEL_ERROR   #BC1B27
    "\033[0;91m",  // LOG_LEVEL_WARNING #F15E42
    "\033[0;37m",  // LOG_LEVEL_INFO    #D0CFCC
    "\033[0;36m",  // LOG_LEVEL_DEBUG   #2AA1B3
    "\033[0;33m"   // LOG_LEVEL_TRACE   #A2734C
};

// Long command line options
const struct option longOptions[] = {
    {"verbose",      no_argument,       0,  'v'},
    {"quiet",        no_argument,       0,  'q'},
    {"extended-log", no_argument,       0,  'x'},
    {"clean",        no_argument,       0,  'c'},
    {"log",          required_argument, 0,  'l'},
    {"max-log",      required_argument, 0,  'm'}
};

/**
 * @brief Logging main body
 *
 * @param file current file
 * @param line current line
 * @param func current function
 * @param lvl debug (verbosity) level
 * @param fmt log format
 * @param argp log other arguments
 */
void selfLogFunction(const char *file, int line, const char *func, int lvl, const char* fmt, ...) {
    // Variables
    int r;
    size_t sz;
    char buf[30] = {0};
    struct timeval tv;
    struct tm t = {0};

    // Create timestamp with ms
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &t);
    std::string msec = std::to_string(tv.tv_usec / 1000);
    while(msec.length() < 3) msec.insert(0, "0");

    sz = strftime(buf, sizeof(buf), "%F %T", &t); // %F => %Y-%m-%d,  %T => %H:%M:%S
    sprintf(buf + sz, ".%s", msec.c_str());
    char *msg = NULL;

    // Format log message
    va_list arglist;
    va_start(arglist, fmt);
    r = vasprintf(&msg, fmt, arglist);
    va_end(arglist);
    (void)r;

    // Output log to stdout
    if(lvl <= gLogLevel) {
        if (lvl < 0) {
            printf ("%s: [---] %s %s\n", buf, func, msg);
        } else {
            switch (gLogType) {
                case LOG_TYPE_EXTENDED:
                    printf ("%s: [%s] %s %s%s\033[0m [%s:%d]\n", buf, logLevelHeaders[lvl], func, logLevelColor[lvl], msg, file, line);
                    break;

                default: // LOG_TYPE_NORMAL
                    printf ("%s: [%s] %s %s%s\033[0m\n", buf, logLevelHeaders[lvl], func, logLevelColor[lvl], msg);
                    break;
            }
        }
        fflush (stdout);
    }

    // Free formatted log buffer
    free(msg);
}


int main(int argc, char *argv[]) {
    // Variable
    int r;
    bool clean = false;
    uint32_t logIndex = UINT32_MAX;
    uint32_t logMax = 100;

    // Parse cmdline options
    while ((r = getopt_long(argc, argv, "vqxcl:m:", longOptions, NULL)) != -1) {
        switch(r) {
            case 'v': // verbose
                gLogLevel++;
                if (gLogLevel > LOG_LEVEL_MAX) {
                    gLogLevel = LOG_LEVEL_MAX;
                }
                break;

            case 'q': // quiet
                gLogLevel = LOG_LEVEL_ERROR;
                break;

            case 'x': // extended-log
                gLogType = LOG_TYPE_EXTENDED;
                break;

            case 'c': // clean
                clean = true;
                break;

            case 'l': // log
                logIndex = atoi (optarg);
                break;

            case 'm': // max-log
                logMax = atoi (optarg);
                break;

            default:
                break;
        }
    }

    selfLog ("Started v.%d.%d.%d LogLevel=%s", VERSION_MAJOR, VERSION_MINOR, VERSION_REV, logLevelHeaders[gLogLevel]);

    r = server_init (clean, logIndex, logMax);
    if (r) {
        r = server_run ();
        // r = grpc_run ();
    }

    selfLogInf("Stopped. Exit code %d", r);
    return r;
}