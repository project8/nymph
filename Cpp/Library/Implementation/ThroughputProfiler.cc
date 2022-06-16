/*
 * ThroughputProfiler.cc
 *
 *  Created on: Nov 30, 2012
 *      Author: nsoblath
 */

#include "ThroughputProfiler.hh"

#include <sstream>


using std::string;
using std::stringstream;
using std::vector;

namespace Nymph
{
    LOGGER(proflog, "ThroughputProfiler");

    REGISTER_PROCESSOR(ThroughputProfiler, "throughput-profiler");

    ThroughputProfiler::ThroughputProfiler(const std::string& name) :
            Processor(name),
            fOutputFileFlag(false),
            fOutputFilename("throughput.json"),
            fTimeStart(),
            fTimeEnd(),
            fNDataProcessed(0)
    {
        RegisterSlot("start", this, &ThroughputProfiler::StartProfiling, {});
        RegisterSlot("data", this, &ThroughputProfiler::Data, {});
        RegisterSlot("stop", this, &ThroughputProfiler::Finish, {});
    };

    ThroughputProfiler::~ThroughputProfiler()
    {
    };

    void ThroughputProfiler::Configure(const scarab::param_node& node)
    {
        SetOutputFileFlag(node.get_value< bool >("output-file-flag", fOutputFileFlag));
        SetOutputFilename(node.get_value("output-filename-base", fOutputFilename));

        return ;
    }

    void ThroughputProfiler::Start()
    {
        fTimeStart = CurrentTime();
        DEBUG(proflog, "Start time: " << fTimeStart.tv_sec << " sec and " << fTimeStart.tv_nsec << " nsec");
        return;
    }

    void ThroughputProfiler::Stop()
    {
        fTimeEnd = CurrentTime();
        DEBUG(proflog, "End time: " << fTimeEnd.tv_sec << " sec and " << fTimeEnd.tv_nsec << " nsec");
        return;
    }

    timespec ThroughputProfiler::Elapsed()
    {
        return Diff(fTimeStart, fTimeEnd);
    }

    void ThroughputProfiler::StartProfiling(DataHandle header)
    {
        INFO(proflog, "Profiling started");
        fNDataProcessed = 0;
        Start();
        return;
    }

    void ThroughputProfiler::Data(DataHandle data)
    {
        (void)data;
        fNDataProcessed++;
        return;
    }

    void ThroughputProfiler::Finish()
    {
        Stop();
        INFO(proflog, "Profiling stopped");
        timespec diffTime = Elapsed();
        PROG(proflog, fNDataProcessed << " slices processed");
        double totalSeconds = TimeToSec(diffTime);
        PROG(proflog, "Throughput time: " << diffTime.tv_sec << " sec and " << diffTime.tv_nsec << " nsec (" << totalSeconds << " sec)");

        // Data production rate in bytes per second
        //double dataProductionRate = double(fEggHeader.GetNChannels()) * fEggHeader.GetAcquisitionRate() * double(fEggHeader.GetDataTypeSize());

        // Data throughput rate in bytes per second
        //double dataThroughputRate = 0.;
        //if (totalSeconds != 0)
        //    dataThroughputRate = double(fEggHeader.GetSliceSize() * fEggHeader.GetNChannels() * fNDataProcessed * fEggHeader.GetDataTypeSize()) / totalSeconds;

        //INFO(proflog, "Data production rate: " << dataProductionRate << " bytes per second");
        //INFO(proflog, "Data throughput rate: " << dataThroughputRate << " bytes per second");
        //PROG(proflog, "Analysis time factor: " << dataProductionRate / dataThroughputRate);

        return;
    }

    timespec ThroughputProfiler::CurrentTime()
    {
        timespec ts;
        GetTimeCurrent(&ts);
        return ts;
    }

    timespec ThroughputProfiler::Diff(timespec start, timespec end) const
    {
        timespec diff;
        TimeDiff(start, end, &diff);
        return diff;
    }

#ifdef __MACH__
    double ThroughputProfiler::sTimebase = 0.0;
    uint64_t ThroughputProfiler::sTimestart = 0;
#endif

    int ThroughputProfiler::GetTimeCurrent(struct timespec* time)
    {
    #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
        if (! sTimestart)
        {
            mach_timebase_info_data_t tb = { .numer = 0, .denom = 1 };
            mach_timebase_info(&tb);
            sTimebase = tb.numer;
            sTimebase /= tb.denom;
            sTimestart = mach_absolute_time();
        }
        double diff = (mach_absolute_time() - sTimestart) * sTimebase;
        time->tv_sec = diff * MACNANO;
        time->tv_nsec = diff - (time->tv_sec * MACGIGA);
        return 0;
    #else
        return clock_gettime(CLOCK_PROCESS_CPUTIME_ID, time);
    #endif

    }

    double ThroughputProfiler::TimeToSec(struct timespec time)
    {
        return (double)time.tv_sec + (double)time.tv_nsec / (double)NSEC_PER_SEC;
    }

    void ThroughputProfiler::TimeDiff(struct timespec start, struct timespec end, struct timespec* diff)
    {
        if ((end.tv_nsec - start.tv_nsec < 0))
        {
            diff->tv_sec = end.tv_sec - start.tv_sec - 1;
            diff->tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        }
        else
        {
            diff->tv_sec = end.tv_sec - start.tv_sec;
            diff->tv_nsec = end.tv_nsec - start.tv_nsec;
        }
        return;
    }

} /* namespace Nymph */
