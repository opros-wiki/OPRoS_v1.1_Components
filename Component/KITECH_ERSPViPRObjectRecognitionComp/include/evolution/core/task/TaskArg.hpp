//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_TASKARG_HPP
#define INCLUDE_EVOLUTION_TASK_TASKARG_HPP


#include <evolution/core/behavior/BehaviorDataWriter.hpp>

#include <vector>


/**
 * @file evolution/core/task/TaskArg.hpp
 * @brief Task arguments.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 **/


namespace Evolution
{

    class TaskArg;

    /**
     * @brief A vector of TaskArg.
     */
    typedef std::vector<TaskArg> TaskArgVector;


    /**
     * @class TaskArg
     * @brief TaskArg is a variant datatype similar to BehaviorData,
     * used to hold task arguments.
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK TaskArg : public BehaviorDataWriter
    {
    public: // Structors.

        /**
         * @brief Empty constructor.
         */
        TaskArg ();

        /**
         * @brief Boolean constructor.
         *
         * @param b [in]; An initialization value
         */
        TaskArg (bool b);

        /**
         * @brief Double constructor.
         *
         * @param d [in]; An initialization value
         */
        TaskArg (double d);

        /**
         * @brief Int constructor.
         *
         * @param i [in]; An initialization value
         */
        TaskArg (int i);

        /**
         * @brief Long constructor.
         *
         * @param l [in]; An initialization value
         */
        TaskArg (long l);

        /**
         * @brief Unsigned long constructor.
         *
         * @param ul [in]; An initialization value
         */
        TaskArg (unsigned long ul);

        /**
         * @brief String constructor.
         *
         * @param s [in]; An initialization value
         */
        TaskArg (const char* s);

        /**
         * @brief Another string constructor.
         *
         * @param s [in]; An initialization value
         */
        TaskArg (const String& s);

        /**
         * @brief DoubleArray constructor.
         *
         * @param d [in]; An initialization value
         */
        TaskArg (const DoubleArray& d);

        /**
         * @brief Image constructor.
         *
         * @param image [in]; An initialization value
         */
        TaskArg (const Image& image);

        /**
         * @brief Pointer constructor.
         *
         * @param p [in]; An initialization value
         */
        TaskArg (void* p);

        /**
         * @brief IObject constructor.
         *
         * @param object [in]; An initialization value
         */
        TaskArg (IObject* object);

        /**
         * @brief TaskArg constructor.
         *
         * @param other [in]; An initialization value
         */
        TaskArg (const TaskArg& other);

        /**
         * @brief BehaviorData constructor.
         *
         * @param behavior_data [in]; An initialization value
         */
        TaskArg (const BehaviorData& behavior_data);

    public: // Equality.

        /**
         * @brief Checks for equality of TaskArgs.
         *
         * @param other [in]; The TaskArg to compare
         *
         * @return true if the TaskArgs are equivalent.
         */
        bool operator== (const TaskArg& other) const
        {
            return (BehaviorData::operator== (other));
        }

        /**
         * @brief Checks for inequality of TaskArgs.
         *
         * @param other [in]; The TaskArg to compare
         *
         * @return true if the TaskArgs are not equivalent.
         */
        bool operator!= (const TaskArg& other) const
        {
            return (BehaviorData::operator!= (other));
        }

    public: // Assignment.

        /**
         * @brief Assignment operator for TaskArgs.
         *
         * @param other [in]; The TaskArg to copy
         *
         * @return The copied TaskArg.
         */
        TaskArg& operator= (const TaskArg& other);

        /**
         * @brief Copy function.
         *
         * @param other [in]; The TaskArg to copy
         */
        void copy (const TaskArg& other);


    public: // Accessors.

        /**
         * @brief Sets a pointer value.
         *
         * @param p [in]; The pointer value
         */
        void set_pointer (void* p);

        /**
         * @brief Gets a pointer value.
         *
         * @return The pointer value.
         */
        void* get_pointer () const;

        /**
         * @brief Checks whether the TaskArg has a value.
         *
         * @return True if the TaskArg is equivalent to TASK_NO_VALUE, false
         * otherwise.
         */
        bool is_empty() const;

        /**
         * @brief Gets the data type of the TaskArg.
         *
         * @return The TaskArg data type.
         */
        DataType get_data_type () const { return ((DataType) get_type ()); }

        /**
         * @brief Makes the const method BehaviorData::get_string() available
         * again, after it was hidden somehow by the non-const
         * BehaviorDataWriter::get_string().
         *
         * @return the string value of the behavior data, or 0 if the
         * BehaviorData object does not hold a string value.
         *
         * @see BehaviorData::get_string()
         */
        const char* get_string () const
        {
            return (BehaviorData::get_string ());
        }

    public:

        /**
         * @brief Used to fill a TaskArgVector from an array of TaskArgs.
         *
         * @param v        [out]; The TaskArgVector to be filled
         * @param num_args [in];  The number of elements in the TaskArg list
         * @param args     [in];  The TaskArg list
         *
         * @return The modified TaskArgVector.
         */
        static TaskArgVector* set_vector (TaskArgVector* v, unsigned int num_args,
                                          const TaskArg args[]);

    }; // End class TaskArg.

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_TASK_TASKARG_HPP
