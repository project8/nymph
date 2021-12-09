/*
 * KTDataQueueProcessorTemplate.hh
 *
 *  Created on: Jan 10, 2013
 *      Author: nsoblath
 */

#ifndef KTDATAQUEUEPROCESSOR_HH_
#define KTDATAQUEUEPROCESSOR_HH_

#include "KTPrimaryProcessor.hh"

#include "KTConcurrentQueue.hh"
#include "KTCoreData.hh"
#include "KTLogger.hh"
#include "KTSlot.hh"

namespace Nymph
{
    KTLOGGER(eqplog, "KTDataQueueProcessor");

    //***********************************
    // Data Queue Processor Template
    //***********************************

    /*!
     @class KTDataQueueProcessorTemplate
     @author N. S. Oblath

     @brief Template class for creating data queueing processors

     @details

     Available configuration values:
     - "timeout": unsigned -- maximum time to wait for new data (integer number of milliseconds)

     Slots:
     - "use-timed-pop": void () -- Switch the queue-popping function to the TIMED version
     - "use-untimed-pop": void () -- Switch the queue-popping function to the UNTIMED version
     - "use-single-pop": void () -- Switch to the single-pop queue-popping function

     Signals:
     - "queue-done": void () -- Emitted when queue is emptied
    */
    template< class XProcessorType >
    class KTDataQueueProcessorTemplate : public KTPrimaryProcessor
    {
        public:
            typedef void (XProcessorType::*FuncPtrType)(KTDataHandle);

            struct DataAndFunc
            {
                KTDataHandle fData;
                FuncPtrType fFuncPtr;
            };

            typedef KTConcurrentQueue< DataAndFunc > Queue;

            typedef bool (KTConcurrentQueue< DataAndFunc >::*QueuePoppingFunc)(DataAndFunc&);

            enum Status
            {
                kStopped,
                kRunning,
                kContinue
            };

        public:
            KTDataQueueProcessorTemplate(const std::string& name = "default-data-queue-proc-template-name");
            virtual ~KTDataQueueProcessorTemplate();

            bool Configure(const scarab::param_node& node);
            virtual bool ConfigureSubClass(const scarab::param_node& node) = 0;

            Status GetStatus() const;
            void SetStatus(KTDataQueueProcessorTemplate< XProcessorType >::Status);

        protected:
            Status fStatus;

            //**********************************************
            // Derived Processor function pointer (optional)
            //**********************************************
        public:
            void SetFuncPtr(FuncPtrType ptr);

        protected:
            FuncPtrType fFuncPtr;


            //*********
            // Queue
            //*********
        public:
            /// Begins processing of queue (switches status from kStopped to kRunning)
            bool Run();

            /// Stops processing of queue (switches status to kStopped)
            void Stop();

            /// Begins processing of queue if status is already kRunning; otherwise does nothing.
            bool ProcessQueue();

            void ClearQueue();

        protected:
            Queue fQueue;
            QueuePoppingFunc fPopFromQueue;

            //*********
            // Queueing functions for slots
            //*********
        protected:
            /// Queue an data object with a provided function
            /// Assumes ownership of the data; original shared pointer will be nullified
            void DoQueueData(KTDataHandle& data, FuncPtrType func);

            /// Queue an data object with fFuncPtr
            /// Assumes ownership of the data; original shared pointer will be nullified
            void DoQueueData(KTDataHandle& data);

            /// Queue a list of data objects
            /// Assumes ownership of all data objects and the list; original shared pointers will be nullified
            //void DoQueueDataList(std::list< KTDataHandle& >* dataList, void (XProcessorType::*fFuncPtr)(KTDataHandle));

            //*********
            // Slots
            //*********
        public:
            void SwitchToTimedPop();
            void SwitchToUntimedPop();
            void SwitchToSinglePop();

            //*********
            // Signals
            //*********
        protected:
            KTSignalDone fQueueDoneSignal;

    };


    //**************************
    // Data Queue Processor
    //**************************

    /*!
     @class KTDataQueueProcessor
     @author N. S. Oblath

     @brief Generic data queue for asynchronous processing

     @details
     Asynchronously emits a signal for each data object it receives.
     Allows the user to start an asynchronous chain of processors.

     Configuration name: "data-queue"

     Available configuration values:

     Slots:
     - "data": void (KTDataHandle) -- Queue a data object for asynchronous processing; use signal "data"

     Signals:
     - "data": void (KTDataHandle) -- Emitted for each data object in the queue
     - "queue-done": void () -- Emitted when queue is emptied (inherited from KTDataQueueProcessorTemplate)
    */
    class KTDataQueueProcessor : public KTDataQueueProcessorTemplate< KTDataQueueProcessor >
    {
        public:
            KTDataQueueProcessor(const std::string& name = "data-queue");
            virtual ~KTDataQueueProcessor();

            bool ConfigureSubClass(const scarab::param_node& node);

        public:
            void EmitDataSignal(KTDataHandle data);

            //***************
            // Signals
            //***************

        private:
            KTSignalData fDataSignal;

            //*********
            // Slots
            //*********
        public:
            /// Queue an data object; will emit data signal
            /// Assumes ownership of the data; original shared pointer will be nullified
            void QueueData(KTDataHandle& data);
            KTSlotWrapper* fQueueDataSW;

            /// Queue a list of data objects; will emit data signal
            /// Assumes ownership of all data objects and the list; original shared pointers will be nullified
            //void QueueDataList(std::list< KTDataHandle >* dataList);

    };


    //**************************************************
    // Data Queue Processor Template Implementation
    //**************************************************


    template< class XProcessorType >
    KTDataQueueProcessorTemplate< XProcessorType >::KTDataQueueProcessorTemplate(const std::string& name) :
            KTPrimaryProcessor({"queue-done"}, name),
            fStatus(kStopped),
            fFuncPtr(NULL),
            fQueue(),
            fPopFromQueue(&KTConcurrentQueue< DataAndFunc >::wait_and_pop),
            fQueueDoneSignal("queue-done", this)
    {
        RegisterSlot("use-timed-pop", this, &KTDataQueueProcessorTemplate< XProcessorType >::SwitchToTimedPop, {});
        RegisterSlot("use-untimed-pop", this, &KTDataQueueProcessorTemplate< XProcessorType >::SwitchToUntimedPop, {});
        RegisterSlot("use-single-pop", this, &KTDataQueueProcessorTemplate< XProcessorType >::SwitchToSinglePop, {});
    }

    template< class XProcessorType >
    KTDataQueueProcessorTemplate< XProcessorType >::~KTDataQueueProcessorTemplate()
    {
        ClearQueue();
    }

    template< class XProcessorType >
    bool KTDataQueueProcessorTemplate< XProcessorType >::Configure(const scarab::param_node& node)
    {
        fQueue.set_timeout(node.get_value< unsigned >("timeout", fQueue.get_timeout()));

        if (! ConfigureSubClass(node)) return false;
        return true;
    }

    template< class XProcessorType >
    bool KTDataQueueProcessorTemplate< XProcessorType >::Run()
    {
        fStatus = kRunning;
        KTINFO(eqplog, "Queue started");
        return ProcessQueue();
    }

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::Stop()
    {
        fStatus = kStopped;
        fQueue.interrupt();
        KTINFO(eqplog, "Queue stopped");
        return;
    }

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::SetFuncPtr(FuncPtrType ptr)
    {
        fFuncPtr = ptr;
        return;
    }


    template< class XProcessorType >
    bool KTDataQueueProcessorTemplate< XProcessorType >::ProcessQueue()
    {
        KTINFO(eqplog, "Beginning to process queue");
        while (fStatus != kStopped)
        {
            KTDEBUG(eqplog, "processing . . .");
            DataAndFunc daf;
            if ((fQueue.*fPopFromQueue)(daf))
            {
                KTDEBUG(eqplog, "Data acquired for processing");
                (static_cast<XProcessorType*>(this)->*(daf.fFuncPtr))(daf.fData);
                if (daf.fData->GetLastData()) fStatus = kStopped;
            }
            else
            {
                if (fStatus == kContinue)
                {
                    fStatus = kRunning;
                }
                else
                {
                    fStatus = kStopped;
                }
            }
        }
        fQueueDoneSignal();
        KTINFO(eqplog, "Queue processing has ended");
        return true;
    }

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::ClearQueue()
    {
        while (! fQueue.empty())
        {
            DataAndFunc daf;
            fQueue.try_pop(daf);
        }
        KTINFO(eqplog, "Queue cleared");
        return;
    }


    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::DoQueueData(KTDataHandle& data, FuncPtrType func)
    {
        KTDEBUG(eqplog, "Queueing data");
        DataAndFunc daf;
        daf.fData = data; // I'd like to use move semantics here (operator=(shared_ptr&&)), but they didn't work, so I bootstrapped with copy and reset.
        data.reset();
        daf.fFuncPtr = func;
        fQueue.push(daf);
        return;
    }

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::DoQueueData(KTDataHandle& data)
    {
        DoQueueData(data, fFuncPtr);
        return;
    }
/*
    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::DoQueueDataList(std::list< KTDataHandle& >* dataList, void (XProcessorType::*func)(KTDataHandle))
    {
        typedef std::list< KTDataHandle > DataList;

        KTDEBUG(eqplog, "Queueing data objects");
        DataAndFunc daf;
        while (! dataList->empty())
        {
            daf.fData = &(dataList->front()); // using move semantics
            daf.fFuncPtr = func;
            dataList->pop_front();
            fQueue.push(daf);
        }
        delete dataList;
        return;
    }
*/

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::SwitchToTimedPop()
    {
        if (fPopFromQueue == &KTConcurrentQueue< DataAndFunc >::timed_wait_and_pop)
            return;

        KTDEBUG(eqplog, "Switching to timed pop function");
        fPopFromQueue = &KTConcurrentQueue< DataAndFunc >::timed_wait_and_pop;
        if (fStatus == kRunning)
        {
            fStatus = kContinue;
            KTINFO(eqplog, "Pop function changed; interrupting queue");
            fQueue.interrupt();
        }
        return;
    }

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::SwitchToUntimedPop()
    {
        if (fPopFromQueue == &KTConcurrentQueue< DataAndFunc >::wait_and_pop)
            return;

        KTDEBUG(eqplog, "Switching to untimed pop function");
        this->fPopFromQueue = &KTConcurrentQueue< DataAndFunc >::wait_and_pop;
        if (fStatus == kRunning)
        {
            fStatus = kContinue;
            KTINFO(eqplog, "Pop function changed; interrupting queue");
            fQueue.interrupt();
        }
        return;
    }

    template< class XProcessorType >
    void KTDataQueueProcessorTemplate< XProcessorType >::SwitchToSinglePop()
    {
        if (fPopFromQueue == &KTConcurrentQueue< DataAndFunc >::try_pop)
            return;

        KTDEBUG(eqplog, "Switching to single-pop function");
        this->fPopFromQueue = &KTConcurrentQueue< DataAndFunc >::try_pop;
        if (fStatus == kRunning)
        {
            fStatus = kContinue;
            KTINFO(eqplog, "Pop function changed; interrupting queue");
            fQueue.interrupt();
        }
        return;
    }

} /* namespace Nymph */
#endif /* KTDATAQUEUEPROCESSOR_HH_ */