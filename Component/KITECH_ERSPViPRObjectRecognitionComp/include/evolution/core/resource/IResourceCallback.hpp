//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IRESOURCE_CALLBACK_HPP
#define INCLUDE_EVOLUTION_CORE_IRESOURCE_CALLBACK_HPP


/**
 * @file   evolution/core/resource/IResourceCallback.hpp
 * @brief  This file defines a class to provide an interface to receive the results of a resource command asynchronously.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::IResourceCallback
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @interface IResourceCallback
     *
     * @brief This class defines a simple interface to receive the result of a
     * resource command asynchronously.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IResourceCallback
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        IResourceCallback (  )
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ IResourceCallback (  )
        {
        }

    public: // IResourceCallback interface

        /**
         * @brief The handle_result function is a method to receive an
         * asynchronous callback on a call.
         *
         * @param id          [in]; The ID of the callback
         * @param result      [in]; The result to handle
         * @param result_data [in]; Data specific to the result being handled
         */
        virtual void handle_result ( CallbackId id, Result result,
                                     void *result_data ) = 0;

    }; // end class IResourceCallback

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_IRESOURCE_CALLBACK_HPP
