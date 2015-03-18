//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_PRIMITIVE_HPP
#define INCLUDE_EVOLUTION_TASK_PRIMITIVE_HPP



#include <evolution/core/base/Types.hpp>
#include <evolution/core/behavior/BehaviorImpl.hpp>

#include "TaskManager.hpp"


/**
 * @file   evolution/core/task/Primitive.hpp
 * @brief  Primitive tasks.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Primitive
 */


namespace Evolution
{

    class IMutex; ///< Pre-declaration
    class BehaviorManager; ///< Pre-declaration


    /**
     * @class Primitive
     * @brief New task primitives (interfaces between the Task
     * Execution Layer and the Behavior Execution Layer) are defined
     * by defining subclasses of Primitive.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK Primitive
    {
    public: // New primitives may find these useful.

        /**
         * @brief Constructor.
         *
         * @pre network_name is a valid null terminated string
         *
         * @param context      [in]; The task context
         * @param network_name [in]; The name of the task network 
         */
        Primitive (TaskContext* context, const char* network_name);

        /**
         * @brief Returns the primitive's TaskContext, which contains its
         * arguments.
         *
         * @return The current TaskContext.
         */
        TaskContext* get_context () const;

        /**
         * @brief Sets the primitive's "output value".
         *
         * @param val [in]; The task value to set
         */
        void set_result (const TaskValue& val);

        /**
         * @brief Sets the primitive's status.
         *
         * @param status [in]; The task status to set
         */
        void set_status (TaskStatus status);


    protected: // New primitives should define these.

        /**
         * @brief This method is called after the TaskManager has loaded and
         * instantiated the primitive's behavior network, but before
         * the network has started running.  Usually a primitive will
         * want to provide a definition of this method that sets up
         * the inputs for the network and registers callbacks for the
         * outputs.
         *
         * (Actually called by Primitive::initialize, which is called
         * by the TaskManager.)
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result start () = 0;

        /**
         * @brief This method is called after each time the behavior network
         * is stepped (run once).  Often a primitive will use this
         * opportunity to decide whether it has succeeded or failed
         * (signalled by calling set_status with either TASK_FAILURE
         * or TASK_SUCCESS).
         *
         * (Actually called by Primitive::do_compute, which is called
         * by the TaskManager.)
         */
        virtual void compute () = 0;

        /**
         * @brief This method is called when the Primitive has completed
         * (successfully or not) and it's behavior network has been
         * unloaded.  A primitive could perform some cleanup at this
         * time.
         *
         * A default definition is provided that does nothing.
         *
         * (Actually called by Primitive::shutdown, which is called by
         * the TaskManager.)
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result finish ();

    public: // These are for the exclusive use of the TaskManager.

        /**
         * @brief Returns the primitive's id.
         *
         * @return The task id.
         */
        virtual const char* get_id () const;

        /**
         * @brief Returns the current task status.
         *
         * @return The task status.
         */
        TaskStatus get_status () const;

        /**
         * @brief Returns the result of the task's execution.
         *
         * @return The task result.
         */
        TaskValue get_result () const;

        /**
         * @brief This method is called by the TaskManager after each time the
         * behavior network is stepped (run once).
         */
        virtual void do_compute ();

        /**
         * @brief Stops the execution of the primitive and removes it from
         * the task manager. 
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result shutdown ();

        /**
         * @brief Initializes the primitive and invokes Primitive::start().
         *
         * @param manager [in]; The task manager associated with this
         *                      primitive
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result initialize (TaskManager* manager);

        /**
         * @brief Sets the flag that indicates that the primitive is complete
         * and its status is final.
         */
        void mark_as_complete ();

        /**
         * @brief Checks if the the current task has completed its processing.
         *
         * @param status_ptr [out]; The final status of the task
         *
         * @return True if the task has completed its processing, false otherwise.
         */
        bool is_complete (TaskStatus* status_ptr);

        /**
         * @brief Returns useful information (primitives memory address and
         * network association) for debugging a primitive.
         *
         * @return A sting containing some characteristics of the primitive.
         */
        String debug_str () const;

    public: // Structors

        ///Destructor
        virtual ~Primitive ();

    protected: // Helpers

        /**
         * @brief Locates the path to the task network _network_name,
         * storing the result in _network_path.
         *
         * @return True if the task has completed its processing, false otherwise.
         */
        Result find_task_network ();

        /**
         * @brief Returns the behavior manager that's running the
         * network associated with this primitive.
         *
         * @return the string value of the behavior data, or 0 if the
         * BehaviorData object does not hold a string value.
         */
        BehaviorManager* get_behavior_manager() const;

    protected: // Implementation

        char          _id[128];
        String        _network_name;
        String        _network_path;
        TaskManager*  _manager;
        TaskManager::NetworkId _network_id;
        TaskStatus   _status;
        IMutex*      _status_mutex;
        TaskContext* _context;
        TaskValue    _result;
        bool         _complete;

    }; // end class Primitive

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_TASK_PRIMITIVE_HPP
