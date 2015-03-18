//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__LOGGING_HIERARCHY_HPP
#define INCLUDE_EVOLUTION_BASE__LOGGING_HIERARCHY_HPP

#include <map>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/LoggingTypes.hpp>
#include <evolution/core/base/LoggingCategory.hpp>
#include <evolution/core/base/IMutex.hpp>

/**
 * @file   evolution/core/base/LoggingHierarchy.hpp
 * @brief  Class that manages logging hierarchies.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::LoggingHierarchy
 */


namespace Evolution
{
    /**
     * @brief Map that links the string defining the logging category
     * and the corresponding pointer to the LoggingCategory object
     */
    typedef std::map<String, LoggingCategory *> LoggingCategoryMap;

    /**
     * @class LoggingHierarchy
     * @brief Class that manages logging hierarchies.
     *
     * @ingroup core_base
     **/
    class EVOLUTION_EXPORT_BASE LoggingHierarchy
    {
    public:// Structors

        /**
         * @brief Default construtor.
         */
        LoggingHierarchy();

        /**
         * @brief Default destructor.
         */
        ~LoggingHierarchy();
        
    public: // functions

        /**
         * @brief Get an instance of the named category, if string is null or empty
         * the root of the hierarchy is returned.
         *
         * @param name [in]; The name of the category to retrieve
         *
         * @return An instance of the named category.
         */
        LoggingCategory * get_instance(const String &name) ;

        /**
         * @brief Gets the root of the logging hierarchy.
         *
         * @return The root of the logging hierarchy.
         */
        LoggingCategory & get_root_category() { return *_root;}

        /**
         * @brief Returns the value for the key name.
         *
         * @param name [in]; The value key
         *
         * @return An instance of the named category.
         */
        LoggingCategory & get_value(const String &name)
        {
            return (*find_category(name));
        }

        /**
         * @brief Deletes all categories from the list.
         */
        void clear_all();

#ifndef DOXYGEN_INTERNAL
    private: // internal functions

        // Tries to find the nearest parent, returns root if it is not found.
        LoggingCategory * nearest_parent(const String &name);

        // Returns the value for the key name, returns NULL if not found.
        LoggingCategory * find_category(const String &name);

    protected: // implementation

        LoggingCategoryMap  _category_map;
        LoggingCategory     *_root;
        IMutex*             _table_mutex;
#endif // DOXYGEN_INTERNAL
    }; // end class LoggingHierarchy

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE__LOGGING_HIERARCHY_HPP
