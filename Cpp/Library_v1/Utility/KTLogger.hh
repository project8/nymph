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

#define KTLOG(...)          macro_dispatcher(__LOG_LOG_, __VA_ARGS__)(__VA_ARGS__)
#ifdef NDEBUG
#define KTTRACE(...)
#define KTDEBUG(...)
#else
#define KTTRACE(...)       macro_dispatcher(__LOG_TRACE_, __VA_ARGS__)(__VA_ARGS__)
#define KTDEBUG(...)       macro_dispatcher(__LOG_DEBUG_, __VA_ARGS__)(__VA_ARGS__)
#endif
#define KTINFO(...)        macro_dispatcher(__LOG_INFO_, __VA_ARGS__)(__VA_ARGS__)
#define KTPROG(...)        macro_dispatcher(__LOG_PROG_, __VA_ARGS__)(__VA_ARGS__)
#define KTWARN(...)        macro_dispatcher(__LOG_WARN_, __VA_ARGS__)(__VA_ARGS__)
#define KTERROR(...)       macro_dispatcher(__LOG_ERROR_, __VA_ARGS__)(__VA_ARGS__)
#define KTFATAL(...)       macro_dispatcher(__LOG_FATAL_, __VA_ARGS__)(__VA_ARGS__)
#define KTASSERT(...)      macro_dispatcher(__LOG_ASSERT_, __VA_ARGS__)(__VA_ARGS__)

#define KTLOG_ONCE(...)     macro_dispatcher(__LOG_LOG_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#ifdef NDEBUG
#define KTTRACE_ONCE(...)
#define KTDEBUG_ONCE(...)
#else
#define KTTRACE_ONCE(...)  macro_dispatcher(__LOG_TRACE_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define KTDEBUG_ONCE(...)  macro_dispatcher(__LOG_DEBUG_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#endif
#define KTINFO_ONCE(...)   macro_dispatcher(__LOG_INFO_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define KTPROG_ONCE(...)   macro_dispatcher(__LOG_PROG_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define KTWARN_ONCE(...)   macro_dispatcher(__LOG_WARN_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define KTERROR_ONCE(...)  macro_dispatcher(__LOG_ERROR_ONCE_, __VA_ARGS__)(__VA_ARGS__)
#define KTFATAL_ONCE(...)  macro_dispatcher(__LOG_FATAL_ONCE_, __VA_ARGS__)(__VA_ARGS__)

#endif /* KTLOGGER_HH_ */
