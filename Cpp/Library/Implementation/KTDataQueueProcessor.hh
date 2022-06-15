/*
 * DataQueueProcessorTemplate.hh
 *
 *  Created on: Jan 10, 2013
 *      Author: nsoblath
 *  Modified for nymph2 by Yuhao Sun 2022 06 `5
 */

#ifndef DATAQUEUEPROCESSOR_HH_
#define DATAQUEUEPROCESSOR_HH_

#include "PrimaryProcessor.hh"

#include "ConcurrentQueue.hh"
#include "CoreData.hh"
#include "Logger.hh"
#include "Slot.hh"

namespace Nymph
{
    LOGGER(eqplog, "DataQueueProcessor");

    //***********************************
    // Data Queue Processor Template
    //***********************************

    /*!
     @class DataQueueProcessorTemplate
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
    class DataQueueProcessorTemplate : public PrimaryProcessor
    {
        public:
            typedef void (XProcessorType::*FuncPtrType)(DataHandle);

            struct DataAndFunc
            {
                DataHandle fData;
                FuncPtrType fFuncPtr;
            };

            typedef ConcurrentQueue< DataAndFunc > Queue;

            typedef bool (ConcurrentQueue< DataAndFunc >::*QueuePoppingFunc)(DataAndFunc&);

            enum Status
            {
                kStopped,
                kRunning,
                kContinue
            };

        public:
            DataQueueProcessorTemplate(const std::string& name = "default-data-queue-proc-template-name");
            virtual ~DataQueueProcessorTemplate();

            bool Configure(const scarab::param_node& node);
            virtual bool ConfigureSubClass(const scarab::param_node& node) = 0;

            Status GetStatus() const;
            void SetStatus(DataQueueProcessorTemplate< XProcessorType >::Status);

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
            void DoQueueData(DataHandle& data, FuncPtrType func);

            /// Queue an data object with fFuncPtr
            /// Assumes ownership of the data; original shared pointer will be nullified
            void DoQueueData(DataHandle& data);

            /// Queue a list of data objects
            /// Assumes ownership of all data objects and the list; original shared pointers will be nullified
            //void DoQueueDataList(std::list< DataHandle& >* dataList, void (XProcessorType::*fFuncPtr)(KTDataHandle));

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
            SignalDone fQueueDoneSignal;

    };


    //**************************
    // Data Queue Processor
    //**************************

    /*!
     @class DataQueueProcessor
     @author N. S. Oblath

     @brief Generic data queue for asynchronous processing

     @details
     Asynchronously emits a signal for each data object it receives.
     Allows the user to start an asynchronous chain of processors.

     Configuration name: "data-queue"

     Available configuration values:

     Slots:
     - "data": void (DataHandle) -- Queue a data object for asynchronous processing; use signal "data"

     Signals:
     - "data": void (DataHandle) -- Emitted for each data object in the queue
     - "queue-done": void () -- Emitted when queue is emptied (inherited from DataQueueProcessorTemplate)
    */
    class DataQueueProcessor : public DataQueueProcessorTemplate< DataQueueProcessor >
    {
        public:
            DataQueueProcessor(const std::string& name = "data-queue");
            virtual ~DataQueueProcessor();

            bool ConfigureSubClass(const scarab::param_node& node);

        public:
            void EmitDataSignal(DataHandle data);

            //***************
            // Signals
            //***************

        private:
            SignalData fDataSignal;

            //*********
            // Slots
            //*********
        public:
            /// Queue an data object; will emit data signal
            /// Assumes ownership of the data; original shared pointer will be nullified
            void QueueData(DataHandle& data);
            SlotWrapper* fQueueDataSW;

            /// Queue a list of data objects; will emit data signal
            /// Assumes ownership of all data objects and the list; original shared pointers will be nullified
            //void QueueDataList(std::list< DataHandle >* dataList);

    };


    //**************************************************
    // Data Queue Processor Template Implementation
    //**************************************************


    template< class XProcessorType >
    DataQueueProcessorTemplate< XProcessorType >::DataQueueProcessorTemplate(const std::string& name) :
            PrimaryProcessor({"queue-done"}, name),
            fStatus(kStopped),
            fFuncPtr(NULL),
            fQueue(),
            fPopFromQueue(&ConcurrentQueue< DataAndFunc >::wait_and_pop),
            fQueueDoneSignal("queue-done", this)
    {
        RegisterSlot("use-timed-pop", this, &DataQueueProcessorTemplate< XProcessorType >::SwitchToTimedPop, {});
        RegisterSlot("use-untimed-pop", this, &DataQueueProcessorTemplate< XProcessorType >::SwitchToUntimedPop, {});
        RegisterSlot("use-single-pop", this, &DataQueueProcessorTemplate< XProcessorType >::SwitchToSinglePop, {});
    }

    template< class XProcessorType >
    DataQueueProcessorTemplate< XProcessorType >::~DataQueueProcessorTemplate()
    {
        ClearQueue();
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::Configure(const scarab::param_node& node)
    {
        fQueue.set_timeout(node.get_value< unsigned >("timeout", fQueue.get_timeout()));

        if (! ConfigureSubClass(node)) 
        THROW_EXCEPT_HERE( ConfigException( node ) << "Invalid choice for data queue configuration" );
        return;
    }

    template< class XProcessorType >
    bool DataQueueProcessorTemplate< XProcessorType >::Run()
    {
        fStatus = kRunning;
        INFO(eqplog, "Queue started");
        return ProcessQueue();
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::Stop()
    {
        fStatus = kStopped;
        fQueue.interrupt();
        INFO(eqplog, "Queue stopped");
        return;
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::SetFuncPtr(FuncPtrType ptr)
    {
        fFuncPtr = ptr;
        return;
    }


    template< class XProcessorType >
    bool DataQueueProcessorTemplate< XProcessorType >::ProcessQueue()
    {
        INFO(eqplog, "Beginning to process queue");
        while (fStatus != kStopped)
        {
            DEBUG(eqplog, "processing . . .");
            DataAndFunc daf;
            if ((fQueue.*fPopFromQueue)(daf))
            {
                DEBUG(eqplog, "Data acquired for processing");
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
        INFO(eqplog, "Queue processing has ended");
        return true;
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::ClearQueue()
    {
        while (! fQueue.empty())
        {
            DataAndFunc daf;
            fQueue.try_pop(daf);
        }
        INFO(eqplog, "Queue cleared");
        return;
    }


    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::DoQueueData(DataHandle& data, FuncPtrType func)
    {
        DEBUG(eqplog, "Queueing data");
        DataAndFunc daf;
        daf.fData = data; // I'd like to use move semantics here (operator=(shared_ptr&&)), but they didn't work, so I bootstrapped with copy and reset.
        data.reset();
        daf.fFuncPtr = func;
        fQueue.push(daf);
        return;
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::DoQueueData(DataHandle& data)
    {
        DoQueueData(data, fFuncPtr);
        return;
    }
/*
    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::DoQueueDataList(std::list< DataHandle& >* dataList, void (XProcessorType::*func)(KTDataHandle))
    {
        typedef std::list< DataHandle > DataList;

        DEBUG(eqplog, "Queueing data objects");
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
    void DataQueueProcessorTemplate< XProcessorType >::SwitchToTimedPop()
    {
        if (fPopFromQueue == &ConcurrentQueue< DataAndFunc >::timed_wait_and_pop)
            return;

        DEBUG(eqplog, "Switching to timed pop function");
        fPopFromQueue = &ConcurrentQueue< DataAndFunc >::timed_wait_and_pop;
        if (fStatus == kRunning)
        {
            fStatus = kContinue;
            INFO(eqplog, "Pop function changed; interrupting queue");
            fQueue.interrupt();
        }
        return;
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::SwitchToUntimedPop()
    {
        if (fPopFromQueue == &ConcurrentQueue< DataAndFunc >::wait_and_pop)
            return;

        DEBUG(eqplog, "Switching to untimed pop function");
        this->fPopFromQueue = &ConcurrentQueue< DataAndFunc >::wait_and_pop;
        if (fStatus == kRunning)
        {
            fStatus = kContinue;
            INFO(eqplog, "Pop function changed; interrupting queue");
            fQueue.interrupt();
        }
        return;
    }

    template< class XProcessorType >
    void DataQueueProcessorTemplate< XProcessorType >::SwitchToSinglePop()
    {
        if (fPopFromQueue == &ConcurrentQueue< DataAndFunc >::try_pop)
            return;

        DEBUG(eqplog, "Switching to single-pop function");
        this->fPopFromQueue = &ConcurrentQueue< DataAndFunc >::try_pop;
        if (fStatus == kRunning)
        {
            fStatus = kContinue;
            INFO(eqplog, "Pop function changed; interrupting queue");
            fQueue.interrupt();
        }
        return;
    }

} /* namespace Nymph */
#endif /* DATAQUEUEPROCESSOR_HH_ */
