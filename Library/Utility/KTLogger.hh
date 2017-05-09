/*
 * KTLogger.hh
 * Based on KLogger.h, from KATRIN's Kasper
 *
 *  Created on: Jan 23, 2014
 *      Author: nsoblath
 */

#ifndef KTLOGGER_HH_
#define KTLOGGER_HH_

#include "logger.hh"

/*
 * The Nymph logger is now a simple rebranding of the Scarab logger
 */

// PUBLIC MACROS

#define KTLOGGER(I,K)      static ::scarab::logger I(K);

#define LOG(...)          macro_dispatcher(__LOG_LOG_, __VA_ARGS__)(__VA_ARGS__)
#ifdef NDEBUG
#define LTRACE(...)
#define LDEBUG(...)
#else
#define LTRACE(...)       macro_dispatcher(__LOG_TRACE_, __VA_ARGS__)(__VA_ARGS__)
#define LDEBUG(...)       macro_dispatcher(__LOG_DEBUG_, __VA_ARGS__)(__VA_ARGS__)
#endif
#define LINFO(...)        macro_dispatcher(__LOG_INFO_, __VA_ARGS__)(__VA_ARGS__)
#define LPROG(...)        macro_dispatcher(__LOG_PROG_, __VA_ARGS__)(__VA_ARGS__)
#define LWARN(...)        macro_dispatcher(__LOG_WARN_, __VA_ARGS__)(__VA_ARGS__)
#define LERROR(...)       macro_dispatcher(__LOG_ERROR_, __VA_ARGS__)(__VA_ARGS__)
#define LFATAL(...)       macro_dispatcher(__LOG_FATAL_, __VA_ARGS__)(__VA_ARGS__)
#define LASSERT(...)      macro_dispatcher(__LOG_ASSERT_, __VA_ARGS__)(__VA_ARGS__)

#define LOG_ONCE(...)     macro_dispatcher(__LOG_LOG_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#ifdef NDEBUG
#define LTRACE_ONCE(...)
#define LDEBUG_ONCE(...)
#else
#define LTRACE_ONCE(...)  macro_dispatcher(__LOG_TRACE_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define LDEBUG_ONCE(...)  macro_dispatcher(__LOG_DEBUG_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#endif
#define LINFO_ONCE(...)   macro_dispatcher(__LOG_INFO_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define LPROG_ONCE(...)   macro_dispatcher(__LOG_PROG_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define LWARN_ONCE(...)   macro_dispatcher(__LOG_WARN_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define LERROR_ONCE(...)  macro_dispatcher(__LOG_ERROR_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define LFATAL_ONCE(...)  macro_dispatcher(__LOG_FATAL_ONCE_, __VA_ARGS__)(__VA_ARGS__)

#endif /* KTLOGGER_HH_ */
