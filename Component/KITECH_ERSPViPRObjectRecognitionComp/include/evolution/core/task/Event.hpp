//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK_EVENT_HPP
#define INCLUDE_EVOLUTION_TASK_EVENT_HPP


/**
 * @file   evolution/core/task/Event.hpp
 * @brief  Events are an asynchronous means for tasks to signal
 *         each other.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Event
 */


#include <map>
#include <queue>
#include <iostream>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/task/Types.hpp>
#include <evolution/core/task/TaskValue.hpp>


namespace Evolution
{

    // Forward declarations.
    class Task;
    class TaskManager;


    /**
     * @class Event
     * @brief An event is a broadcast message between asynchronous
     * tasks.
     *
     * Events have a type (a string) and a number of properties
     * (name/value pairs).
     *
     * @ingroup core_task
     */
    class EVOLUTION_EXPORT_TASK Event
    {
    public: // Types.

        // These are really only for internal TEL consumption, most
        // users won't need these.
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief A list of events.
         */
        typedef std::list<Event> EventList;

        /**
         * @brief A function that takes an Event as its only argument
         * and has no return value.
         */
        typedef void EventHandler(Event);
#endif

    public: // Structors.

        /**
         * @brief Default constructor.
         */
        Event ();

        /**
         * @brief The most useful constructor for constructing a new
         * event to be raised.
         *
         * @param type [in]; The type of event.
         */
        Event (const char* type);

        /**
         * @brief Copy constructor.
         *
         * @param other [in]; The event to copy
         */
        Event (const Event& other);

        /**
         * @brief Destructor.
         */
        virtual ~Event ();


    public: // Operators.

        /**
         * @brief Equality test for Events.
         *
         * @param other [in]; The event to assign
         *
         * @return A reference to the assigned object.
         */
        Event& operator= (const Event& other);


    public: // Accessors.

        /**
         * @brief Returns the event's type.
         *
         * @return The event type.
         */
        const char* get_type () const;

        /**
         * @brief Returns a property value.
         *
         * If the task does not have the specified property this
         * method returns TASK_NO_VALUE.
         *
         * @param name [in]; The name of the desired property
         *
         * @return The property value.
         */
        const TaskValue& get_property (const char* name) const;

        /**
         * @brief Returns the event's timestamp.
         *
         * @return The time when the event was fired.
         */
        Timestamp get_timestamp () const;

    public: // Setting.

        /**
         * @brief Sets a property value.
         *
         * @param name  [in]; The name of the property
         * @param value [in]; The value of the property
         */
        void set_property (const char* name, TaskValue value);

    public: // Status report.

        /**
         * @brief Checks whether the event is empty.
         *
         * An empty event may be returned by some functions if a
         * timeout occurrs before an event occurs.
         *
         * @return True if the event is empty, false otherwise. 
         */
        bool is_empty () const;

        /**
         * @brief Checks whether an event has a given property.
         *
         * @param name [in]; The name of the desired property
         *
         * @return True if the evaent has the given protected, false
         * otherwise.
         */
        bool has_property (const char* name) const;

    public: // Printing.

        /**
         * @brief Prints a description of the event to the specified
         * stream.
         *
         * @param stream [in]; The output stream
         */
        void print (std::ostream& stream) const;

    public: // Utilities

        /**
         * @brief Checks whether this event matches the specified pattern.
         *
         * @param pattern [in]; A null terminated pattern
         *
         * @return True if successful, false otherwise.
         */
        bool matches_pattern (const char* pattern) const;


        /**
         * @brief Checks whether an event type matches a pattern.
         *
         * @param type    [in]; The event type
         * @param pattern [in]; A null terminated pattern
         *
         * @return True if successful, false otherwise.
         */
        static bool matches_pattern (const char* type, const char* pattern);


        /**
         * @brief Checks whether an event type matches a pattern.
         *
         * @param type    [in]; The event type
         * @param pattern [in]; A pattern
         *
         * @return True if successful, false otherwise.
         */
        static bool matches_pattern (const String& type, const String& pattern);

    public: // Functors.

        /**
         * @brief This predicate checks whether an event matches the
         * specified pattern.
         */
        struct EventMatchesPattern
        {
        public:
            /**
             * @brief Constructor.
             *
             * @param pattern [in]; A null terminated pattern
             */
            EventMatchesPattern (const char* pattern);

            /**
             * @brief Predicate function.
             *
             * @param event [in]; An event
             *
             * @return True if the event's match, false otherwise.
             */
            bool operator () (const Event& event) const;

        protected:

            const char* _pattern;
        };

        /**
         * @brief This predicate checks whether a pattern matches the
         * type of the given event.
         */
        struct PatternMatchesEvent
        {
        public:
            /**
             * @brief Constructor.
             *
             * @param event [in]; The event to copy
             */
            PatternMatchesEvent (const Event& event);

            /**
             * @brief Predicate function.
             *
             * @param pattern [in]; The search pattern
             *
             * @return True if the pattern's match, false otherwise;
             */
            bool operator () (const String& pattern) const;

        protected:

            String _type;
        };

    public: // For internal use only.
#ifndef DOXYGEN_INTERNAL

        /**
         * @brief Copies an event.
         *
         * @param other [in]; The event to copy
         */
        void copy (const Event& other);

        /**
         * @brief Sets the event's timestamp to the current time.
         * Can only be called once.
         */
        void set_timestamp();
#endif // end DOXYGEN_INTERNAL

    protected: // Functors.

        struct ltstr
        {
#ifndef DOXYGEN_INTERNAL
            /**
             * @brief Predicate function.
             *
             * @param s1 [in]; An input string (LHS)
             * @param s2 [in]; An input string (RHS)
             *
             * @return True if s1 is lexicographically less than s2.
             */
            bool operator () (const String& s1, const String& s2) const
            {
                return s1 < s2;
#endif
            }
        };

    protected: // Types.

        typedef std::map<String, TaskValue, ltstr> PropertyTable;

    protected: // Subclass helpers.

        /**
         * @brief Obtain the event type.
         *
         * @param event [in]; The input event
         *
         * @return String representation of the event's type.
         */
        static const String&
        get_event_type (const Event& event) { return (event._type); }

        /**
         * @brief Obtain the event's property table.
         *
         * @param event [in]; The input event
         *
         * @return The event's property table.
         */
        static const PropertyTable&
        get_event_properties (const Event& event) { return (event._properties); }

    protected: // Implementation

        Timestamp     _timestamp;  ///< Event timestamp.
        String        _type;       ///< Event type (indicated with a string).
        PropertyTable _properties; ///< Event properties.

    }; // end class Event

    /**
     * @brief Event output operator.
     *
     * @param out_stream [in]; The output stream
     * @param event      [in]; The input event
     *
     * @return A reference to the ostream object.
     * @ingroup core_task
     */
    inline std::ostream& operator<< (std::ostream& out_stream, const Event& event)
    {
        event.print(out_stream);
        return out_stream;
    }

} // End namespace Evolution.

#endif // INCLUDE_EVOLUTION_TASK_EVENT_HPP
