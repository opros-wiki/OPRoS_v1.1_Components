//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_TASK__EVENT_FILTER_HPP
#define INCLUDE_EVOLUTION_TASK__EVENT_FILTER_HPP


/**
 * @file   evolution/core/task/EventFilter.hpp
 * @brief  Class to filter events.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::EventFilter
 **/


#include <evolution/core/task/Event.hpp>


namespace Evolution
{


    /**
     * @class EventFilter
     * @brief
     * A filter on events, used to match raised events against those
     * in which a task has registered interest.
     *
     * @ingroup core_task
     **/
    class EVOLUTION_EXPORT_TASK EventFilter : public Event
    {
    public: // Structors

        /**
         * @brief Constructor.
         *
         * If a valid pattern is specified, the type and properties of
         * the filter will be set.
         *
         * @param pattern [in, opt]; An optional filter pattern
         **/
        EventFilter (const char* pattern = NULL);

    public: // Status report

        /**
         * @brief True if the filter pattern contains no property matching.
         *
         * @return True/false.
         */
        bool is_simple () const
        {
            return (_is_simple);
        }

    public: // Access

        /**
         * @brief Obtains the original pattern string.
         *
         * @return The pattern string.
         */
        const String& get_pattern () const { return (_pattern); }

    public: // Setting

        /**
         * @brief Sets the filter as specified by the pattern.
         *
         * @param pattern [in]; A null terminated pattern
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        Result set_pattern (const char* pattern);

    public: // Query

        /**
         * @brief Equality, using original pattern for efficiency.
         *
         * @param filter [in]; The event filter
         *
         * @return True if the event filters are equal, or false otherwise.
         */
        bool operator== (const EventFilter& filter) const
        {
            return (_pattern == filter._pattern);
        }

        /**
         * @brief Indicates if the filter matches the given event.
         *
         * @param event [in]; An event to compare against
         *
         * @return True if the events match, false otherwise.
         */
        bool matches (const Event& event) const;

    public: // Predicates

        /**
         * @brief
         * Can be used as an STL predicate to match events to a particular
         * filter.  For example, can be used with the STL functor 
         * std::find_if to find an event that matches a filter.
         *
         * @ingroup core_task
         */
        struct MatchesEvent
        {
        public: // Structors

            /**
             * @brief Constructor.
             *
             * @param filter [in]; An event filter
             */
            MatchesEvent (const EventFilter& filter) : _filter (filter) {}

        public: // Query
            
            /**
             * @brief Predicate function.
             *
             * @param event [in]; An event.
             *
             * @return True if the event's match, false otherwise.
             */
            bool operator() (const Event& event) const { return (_filter.matches (event)); }

        protected: // Implementation

            const EventFilter& _filter;
        };

    protected: // Implementation

        bool    _is_simple;   ///< True if the pattern contains no property matching.
        String  _pattern;     ///< Original pattern string.
    }; // end class EventFilter


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_TASK__EVENT_FILTER_HPP
