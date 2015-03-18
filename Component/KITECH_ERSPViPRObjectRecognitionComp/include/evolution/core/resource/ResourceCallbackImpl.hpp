//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE__RESOURCE_CALLBACK_IMPL_HPP
#define INCLUDE_EVOLUTION_RESOURCE__RESOURCE_CALLBACK_IMPL_HPP

/**
 * @file evolution/core/resource/ResourceCallbackImpl.hpp
 * @brief This class defines an implementation of IResourceCallback for
 * testing. Useful for invoking an asynchronous method on a resource and then
 * waiting for its completion.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ResourceCallbackImpl
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/IConditionVariable.hpp>
#include <evolution/core/base/Platform.hpp>
#include <evolution/core/base/AcquirableLock.hpp>
#include <evolution/core/resource/IResourceCallback.hpp>


namespace Evolution
{

    /**
     * @class ResourceCallbackImpl 
     *
     * @brief This class defines an implementation of IResourceCallback for testing. Useful for
     * invoking an asynchronous method on a resource and then waiting for
     * its completion.
     *
     * @ingroup core_resource
     **/
    class EVOLUTION_EXPORT_RESOURCE  ResourceCallbackImpl : public IResourceCallback
    {
    public: // Structors

        /**
         * @brief This constructor locks the condition.
         */
        ResourceCallbackImpl ()
            : _current_result(RESULT_TIMEOUT), _got_result(false)
        {
            _result_condition = Platform::create_condition_variable ();
            _result_condition->lock_mutex ();
        }

        /**
         * @brief This destructor unlocks and frees the condition.
         */
        ~ResourceCallbackImpl ()
        {
            _result_condition->unlock_mutex ();
            delete _result_condition;
        }

    public: // IResourceCallback interface

        /**
         * @brief This function handles a callback, signalling the condition.
         *
         * @param callback_id [in]; The callback's id
         * @param result      [in]; The result to handle
         * @param result_data [in]; Data specific to the result being handled
         */
        void handle_result (CallbackId callback_id,
                            Result result,
                            void* result_data)
        {
            AcquirableLock lock (*_result_condition);
            if (!lock.is_locked ())
            {
                return;
            }
            _current_result = result;
            _got_result = true;
            _result_condition->signal ();
        }

    public: // Waiting

        /**
         * @brief The wait_for_result function waits for the condition
         * to be signaled.
         *
         * Calling this method eats the current result, so the next wait will
         * return a new result.
         *
         * @param wait_time     [in, opt]; A timeout value
         * @param timed_out_ptr [out, opt]; Optional. Set to true if timed out
         *                                  before receiving a result
         */
        Result wait_for_result (unsigned long wait_time = ERSP_INFINITE,
                                bool *timed_out_ptr = NULL);

    protected: // Implementation

        /**
         * @brief Condition for wait
         */
        IConditionVariable* _result_condition;

        /**
         * @brief The most recent result received
         */
        Result _current_result;

        /**
         * @brief Whether we actually got a result on the last wait
         */
        bool _got_result;

    }; // end class ResourceCallbackImpl


} // end namespace Evoluton


#endif  // INCLUDE_EVOLUTION_RESOURCE__RESOURCE_CALLBACK_IMPL_HPP
