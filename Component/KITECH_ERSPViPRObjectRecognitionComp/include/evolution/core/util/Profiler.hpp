//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/Profiler.hpp
 * @brief   Implements a simple timing-based profiler.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Profiler
 */

#ifndef INCLUDE_EVOLUTION_BASE_PROFILER_HPP
#define INCLUDE_EVOLUTION_BASE_PROFILER_HPP

#include <stdio.h>
#include <evolution/core/base/Platform.hpp>

namespace Evolution
{


    /**
     * @class Profile
     * 
     * @brief A class for profiling execution times
     *
     * Each profile encapsulate a particular timing point in the code.
     * 
     * @ingroup core_util
     */
    struct EVOLUTION_EXPORT_UTIL Profile
    {
    public: // Types
        typedef double usec_type;         ///< Micro_seconds type

    public: // Structors

        /**
         * @brief Constructor
         */
        Profile() : iterations(0), parent(0), child(0), next(0), added(false), free(false)
        {
            start_time = 0;
            us_elapsed = 0;
            max_cost = 0;
        }

        /**
         * @brief Constructor
         *
         * @param name [in]; The name of the profile
         */
        Profile(const char* name) : iterations(0), parent(0), child(0), next(0), added(false), free(false)
        {
            this->name = name;
            start_time = 0;
            us_elapsed = 0;
            max_cost = 0;
        }

        /**
         * @brief Destructor
         */
        ~Profile() {}

        /**
         * @brief Set profile name
         *
         * @param name [in]; The profile name
         */
        void set_name(const char* name)
        {
            this->name = name;
        }

    public: // Data
        unsigned    iterations;   ///< Number of iterations
        usec_type   us_elapsed;   ///< Elapsed time in micro seconds
        usec_type   start_time;   ///< Start time
        usec_type   start_cost;   ///< Start cost
        usec_type   max_cost;     ///< Max cost
        Profile*    parent;       ///< Profiler parent
        Profile*    child;        ///< Profiler child
        Profile*    next;         ///< Next node in Profiler
        bool        added;        ///< Added
        bool        free;         ///< Free
        String      name;         ///< Profiler name
    };

    /**
     * @class Profiler
     * 
     * @brief This class encapsulates the profiling mechanism.
     *
     * Each profiler can contain multiple profiles.  Each profile
     * is a particular timing points in the code.
     *      
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL Profiler
    {
    public: // Types

        typedef Profile::usec_type usec_type;  ///< Micro second type
        static usec_type    _timing_cost;      ///< Timing cost

    public: // Constants

        enum
        {
            MAX_PROFILE_DEPTH = 200        ///< Define max profile depth as 200
        };

    public: // Structors

        /**
         * @brief Constructor
         */
        Profiler() : _root(0), _level(0), _profile_cost(0)
        {
            // Calculate the cost of a get_usec_time.
            usec_type t0 = Platform::get_microsecond_time();
            _timing_cost = Platform::get_microsecond_time() - t0;

        }

        /**
         * @brief Destructor
         */
        ~Profiler()
        {
            //delete_profile(_root);
        }

    public: // Methods

        /**
         * @brief Start profiling using specified profile.
         *
         * @param profile [in]; The profile to start
         */
        inline void start_profile(Profile& profile);

        /**
         * @brief Start profiling using specified profile.
         *
         * @param name [in]; The name of the file to start.
         * @param profile [in]; The profile to start
         */
        inline Profile* start_profile(const char* name, Profile* profile);

        /**
         * @brief End profiling of specified profile.
         *
         * @param profile [in]; The profile to stop profiling.
         */
        inline void end_profile(Profile& profile);

        /**
         * @brief Add a profile to the profiler.
         *
         * @param profile [in]; The profile to add to the profiler
         */
        void add_profile(Profile* profile)
        {
            if (_root == 0) {
                _root = profile;
                profile->added = true;
                return;
            }

            Profile* last = _previous[_level - 1]->child;
            if (last == 0) {
                _previous[_level - 1]->child = profile;
            }
            else {
                while (last->next != 0) {
                    last = last->next;
                }
                last->next = profile;
            }
            profile->parent = _previous[_level - 1];
            profile->added = true;
            return;
        }

        /**
         * @brief Add a profile to the profiler by name.
         *
         * @param name [in]; The name of the profile to added.
         *
         * @return Pointer to the added profile.
         */
        Profile* add_profile(const char* name)
        {
            Profile* profile = new Profile(name);
            profile->free = true;
            add_profile(profile);
            return profile;
        }

        /**
         * @brief Delete the specified profile.
         *
         * @param profile [in]; The profile to delete.
         */
        void delete_profile(Profile* profile)
        {
            if (profile->free) {
                Profile* child = profile->child;
                while (child != 0) {
                    Profile* next = child->next;
                    delete_profile(child);
                    child = next;
                }
                delete profile;
            }
        }

        /**
         * @brief Generate the profiling report to a string.
         * 
         * @param report_string [out]; The string to contain the profiling
         * report
         */
        void report(String& report_string)
        {
            usec_type elapsed = 0;
            report_string += "[[[  Profiler Report  ]]]\n";
            report(*_root, 0, elapsed, report_string);
            char buffer[200];
            ersp_snprintf(buffer, 200, "Profile cost: %.0f usec\n",
                          _profile_cost);
            report_string += buffer;
        }

        /**
         * @brief Generate the profiling report to a file.
         *
         * @param fp [in]; The file pointer to which the report should be
         *      written.
         */
        void report(std::FILE* fp)
        {
            if (fp == 0)
            {
                return;
            }
            String report_string;
            report(report_string);
            fputs(report_string.c_str(), fp);
        }

        /**
         * @brief Generate the profiling report to a file specified by name.
         *
         * @param file [in]; The name of the file to contain the profiling
         *      report.
         */
        void report(const char* file)
        {
            std::FILE* fp = fopen(file, "wt");
            report(fp);
            fclose(fp);
        }

        /**
         * @brief Get number of profiled iterations.
         *
         * @return The number of profiled iterations.
         */
        unsigned long get_iterations()
        {
            return (_root) ?  _root->iterations : 0;
        }

    public: // Profiler

        /**
         * @brief Profiler internal object
         */
        static Profiler profiler;

    protected:

        /**
         * @brief Recursively generate the profiling report to a string.
         *
         * @param profile [in]; The profile to report on
         * @param level [in]; The nesting level of the report
         * @param parent_elapsed [in]; The time elapsed in the parent profile
         * @param report_string [out]; The string containing the generated
         *      profiling report.
         */
        void report(Profile& profile, int level, usec_type parent_elapsed, String& report_string)
        {
            for (int i = 0; i < level; ++i)
            {
                report_string += "  ";
            }
            double percent = (parent_elapsed == 0) ? 100.0 :
                100.0 * profile.us_elapsed / parent_elapsed;
            double mean_time = profile.us_elapsed / profile.iterations;
            char buffer[400];
            ersp_snprintf(buffer, 400,
                          "%.2f%%:  %s (%.0f usec mean, %.0f usec max,  %d iter, %.0f usec total)\n",
                          percent, profile.name.c_str(),
                          mean_time, profile.max_cost, profile.iterations, profile.us_elapsed);
            report_string += buffer;
            Profile* child = profile.child;
            while (child != 0)
            {
                Profile* next = child->next;
                report(*child, level + 1, profile.us_elapsed, report_string);
                child = next;
            }
        }

    protected:
        Profile*        _root;                          ///< Root profile
        Profile*        _current;                       ///< Current profiler
        Profile*        _previous[MAX_PROFILE_DEPTH];   ///< Previous profile
        int             _level;                         ///< Current level
        usec_type       _profile_cost;                  ///< Profile cost
    };

    inline
    void Profiler::start_profile(Profile& profile)
    {
        usec_type start_time = Platform:: get_microsecond_time();
        if (!profile.added) {
            add_profile(&profile);
            _previous[_level] = &profile;
        }
        _level++;
        profile.start_time = Platform:: get_microsecond_time();
        profile.start_cost = profile.start_time - start_time;
    }

    inline
    Profile* Profiler::start_profile(const char* name, Profile* profile)
    {
        usec_type start_time = Platform:: get_microsecond_time();
        if (profile == 0) {
            profile = add_profile(name);
            _previous[_level] = profile;
        }
        _level++;
        profile->start_time = Platform:: get_microsecond_time();
        profile->start_cost = profile->start_time - start_time;
        return profile;
    }

    inline
    void Profiler::end_profile(Profile& profile)
    {
        usec_type end_time = Platform:: get_microsecond_time();
        _level--;
        if (profile.start_time == 0) {
            return;
        }
        profile.iterations++;
        usec_type profile_cost = end_time - profile.start_time - _timing_cost;
        if (profile.max_cost < profile_cost) {
            profile.max_cost = profile_cost;
        }
        profile.us_elapsed += profile_cost;
        usec_type accounting_time = Platform::get_microsecond_time();
        usec_type end_cost = accounting_time - end_time;

        // Removing profiling cost from parent profile.
        if (profile.parent) {
            usec_type profile_cost = (profile.start_cost + end_cost +  _timing_cost * 2);
            _profile_cost += profile_cost;
            // Remove profile cost and accounting cost from parent profile.
            // profile.parent->us_elapsed -= profile_cost + _timing_cost + (- accounting_time + Platform::get_microsecond_time());
            profile.parent->us_elapsed -= profile_cost;
        }
    }
}

//#define INIT_PROFILE(id)              static Profile* __profile__##id##__ = 0

/**
 * @brief Initialize profile macro
 */
#define INIT_PROFILE(id, name)          static Profile __profile__##id##__(name)

/**
 * @brief Start profile macro
 */
#define START_PROFILE(p, id)            (p).start_profile(__profile__##id##__)

/**
 * @brief End profile macro
 */
#define END_PROFILE(p, id)              p.end_profile(__profile__##id##__)

#endif // INCLUDE_EVOLUTION_BASE_PROFILER_HPP
