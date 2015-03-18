//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_UTIL_RUN_QUEUE_HPP
#define INCLUDE_EVOLUTION_CORE_UTIL_RUN_QUEUE_HPP


/**
 * @file    evolution/core/util/RunQueue.hpp
 * @brief   The queue that runs IRunnable objects in a dispatch thread.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::RunRequest
 * @see Evolution::RunQueue
 **/


#include <vector>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Platform.hpp>
#include <evolution/core/base/IThread.hpp>
#include <evolution/core/base/IMutex.hpp>
#include <evolution/core/base/IConditionVariable.hpp>
#include <evolution/core/base/IRunnable.hpp>


namespace Evolution
{
    /**
     * @class RunRequest
     * 
     * @brief
     * This class encapsulates the request to run an IRunnable object.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL RunRequest
    {
    public:

        /**
         * @brief
         * Constructor
         *
         * @param runner [in]; Pointer to the runnable object.
         * @param notify [in]; Notify object for the runnable (may be NULL).
         * @param delete_after [in]; True if the runnable object should be 
         *      deleted after executing.
         * @param force [in];  Place holder.  Not currently used. 
         */
        RunRequest(IRunnable *runner, IConditionVariable* notify, bool delete_after, bool force);
        /**
         * @brief
         * Destructor
         */
        virtual ~RunRequest();

        /**
         * @brief
         * Run the runnable object.
         */ 
        virtual void run();

    public:

        IRunnable*          _runner;    ///< The object to run.
        IConditionVariable* _notify;    ///< Notify object for the runnable (each may be null).
        bool                _delete;    ///< Whether to delete runnable objects after execute.
        bool                _force_run; ///< Place holder.  Not currently used.
    };
    
    /**
     * @brief
     * A list of run requests.
     */
    typedef std::vector<RunRequest*> RunList;
    
    /**
     * @class RunQueue
     * 
     * @brief
     * Queue that runs IRunnable objects in a dispatch thread.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL RunQueue
    {
    public: // Structors
        /**
         * @brief
         * Default constructor
         */
        RunQueue();

        /**
         * @brief
         * Destructor
         */
        virtual ~RunQueue();

    public:

        /**
         * @brief This function is called to put a runnable event on the queue
         * for execution later, or by another thread.  
         *
         * If block is set, the call will not return until the runnable has
         * executed.  If delete_after is set, the queue will delete the object
         * after running it.  If force_run is set, the event will be run even
         * if the RunQueue is deleted before it gets a time slice. (Be careful
         * that the thread doesn't matter when setting this.) If prelocked is
         * set, the queue releases it's mutex, just before waiting on the
         * runner.  If post_queue_notifier is non-NULL, it will be run after
         * the runner is placed on the queue (but before we block and wait for
         * the runner, if we are blocking).
         *
         * @param runner [in]; Pointer to the IRunnable object.
         * @param block [in]; True if this method should block until after
         *      the runnable object has finished running.
         * @param delete_after [in]; True if the runnable object should be
         *      deleted after running.
         * @param force_run [in]; True if the event should be run even if
         *      the RunQueue is deleted before it get a time slice.
         * @param prelocked [in]; True if the run queue should release
         *      its mutex just before waiting on the runner.
         * @param post_queue_notifier [in]; If non-NULL, this object will
         *      be run after the runner is placed on the queue.
         */
        void queue(IRunnable* runner, bool block, bool delete_after=false,
                   bool force_run=false, bool prelocked=false, IRunnable* post_queue_notifier=NULL);

        /**
         * @brief Called by some external idle routine to process runnable
         * requests.
         */
        void dispatch();

        /**
         * @brief This function sets the immediate thread so that queued
         * runnables that would be serviced by the current thread (which would
         * block on a queue call) don't cause deadlock.
         *
         * @param id [in]; The id of the service thread.
         */
        void set_service_thread(ThreadId id);

        /**
         * @brief The lock function temporarily stalls dispatch from other
         * threads.
         */
        void lock();

        /**
         * @brief The release function re-allows dispatch from other threads.
         */
        void release();

    public: // Shared queue

        /**
         * @brief This is used to obtain a shared queue running in a single
         * thread.
         *
         * @return Pointer to the shared queue.
         */
        static RunQueue* get_shared_queue ();

    protected:

        /**
         * @brief
         * This pops and returns the last request (or NULL, if none) off the
         * list.
         *
         * @return Pointer to the last run request.
         */
        RunRequest* pop() ;
        
        /**
         * @brief
         * This toggles which queue is active.
         */
        void toggle_active_queue();

        ThreadId _serve_thread; ///< Thread that if we are alread in, we execute immediately (to avoid deadlock).
        bool     _has_thread;   ///< If there is an immediate execution thread.
        IMutex*  _mutex;        ///< Protection for the request list.
        RunList  _requests[2];  ///< Pending lists of objects to be run, 2 so that re-registers don't infinite loop.
        int      _active;       ///< Which of the _request lists to add new requests to.
    };


} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_UTIL_RUN_QUEUE_HPP
