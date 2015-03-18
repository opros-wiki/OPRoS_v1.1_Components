//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IRESOURCE_DRIVER_HPP
#define INCLUDE_EVOLUTION_CORE_IRESOURCE_DRIVER_HPP


/**
 * @file    evolution/core/resource/IResourceDriver.hpp
 * @brief   This file defines a class to encapsulate a resource driver and 
 *          expose the interfaces that a given driver supports.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IResourceDriver
 */


#include <string>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/resource/ResourceConfigTypes.hpp>
#include <evolution/core/resource/IResourceContainer.hpp>
#include <evolution/core/resource/ResourceRegistry.hpp>


namespace Evolution
{


    /**
     * @interface IResourceDriver
     *
     * @brief This class encapsulates a resource driver, exposing the
     * interfaces that the given driver supports.
     *
     * @see Resource
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IResourceDriver
    {
    public:                  // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IResourceDriver"
    public: // Structors

        /**
         * @brief Constructor
         */
        IResourceDriver () {}

        /**
         * Destructor.
         */
        virtual ~IResourceDriver () {}

    public: // Reference count

        /**
         * @brief This adds a reference to the driver.
         * @return Number of references after this addition
         */
        virtual unsigned add_ref () = 0;

        /**
         * @brief This removes a reference from the driver.
         * @return Number of references after this removal
         */
        virtual unsigned remove_ref () = 0;

    public: // Activation

        /**
         * @brief This activates the driver. The method should not fail if the
         * driver is already activated. That is, successful completion
         * of the method simply ensures that the driver is activated.
         **/
        virtual Result activate () = 0;

        /**
         * @brief This deactivates the driver. Similar to activate(), the method
         * should not fail if the driver is already deactivated. That
         * is, successful completion of the method simply ensures that
         * the driver is deactivated.
         **/
        virtual Result deactivate () = 0;

    public: // Status report.

        /**
         * @brief This returns 1 if the driver is active, 0 if deactivated.
         *
         * @return true if the driver is active.
         */
        virtual bool is_active () = 0;

        /**
         * @brief This returns the resource's id.
         *
         * @param id [out]; The returned resource id.
         *  
         * @return RESULT_SUCCESS if successful, or an error code if not.
         * @ingroup drivers_rcm
         */
        virtual Result get_id (String* id) = 0;
        
    public: // Interface

        /**
         * @brief This function specifies whether the resource should be polled.
         *
         * @param ticket [in]; The security ticket.
         * @param poll [in]; true if the resource should be polled.
         * 
         * @return RESULT_SUCCESS if successful, or an error code if not.
         * @ingroup drivers_rcm
         */
        virtual Result set_poll (TicketId ticket, bool poll) = 0;

        /**
         * @brief Use this function to query the resource for all resource data.
         *
         * @param ticket [in]; The security ticket.
         * 
         * @return RESULT_SUCCESS if successful, or an error code if not.
         * @ingroup drivers_rcm
         */
        virtual Result query (TicketId ticket) = 0;
        
        /**
         * @brief This function obtains the specified interface for the owning token, if
         * the driver supports that interface.
         *
         * @param  ticket [in]: The ticket of the entity obtaining the interface
         * @param  interface_name [in]: The name of the interface to obtain
         * @param  resource_interface [out]: Pointer to internal interface, caller does NOT free and must pass in valid pointer
         * @param  reservation_count [out]: The number of entities that may 
         *       simultaneously hold this interface (zero = unlimited)
         */
        virtual Result obtain_interface (TicketId ticket,
                                         const char* interface_name,
                                         void** resource_interface,
                                         unsigned* reservation_count) = 0;
        
    }; // end class IResourceDriver


} // end namespace Evolution

/**
 * @brief This macro should be put in each instantiable resource's klass.hpp file in a
 * public declaration section.  It declares a factory creation function for
 * the resource and a string name to reference it, used in registry lookup.
 *
 * @param klass  Name of the class (eg Foo)
 * @param driver Name of the driver (eg "Evolution.Foo")
 **/
#define DECLARE_RESOURCE(klass, driver)  \
static Evolution::Result create_resource(Evolution::TicketId ticket,    \
                              const Evolution::ResourceConfig& config,   \
                              Evolution::IResourceContainer& container,   \
                              Evolution::IResourceDriver** resource_ptr);  \
static const char* __get_driver_id () { return (driver); }  \
static const char * const _DRIVER_ID;

/**
 * @brief The macro forces the behavior to be registered in the resource registry
 * when the library is loaded.  It is implicitly called from the
 * IMPLEMENT_RESOURCE macro, so it only needs to be included when the behavior
 * designer is not using IMPLEMENT_RESOURCE.  This macro should be included
 * in the klass.cpp file.
 *
 * @param klass  Name of the class (eg Foo)
 **/
#define REGISTER_RESOURCE(klass) \
static Evolution::Result _registered = \
Evolution::ResourceRegistry::register_resource (klass::__get_driver_id (), klass::create_resource); \
const char * const klass::_DRIVER_ID = klass::__get_driver_id();


#endif  // INCLUDE_EVOLUTION_CORE_IRESOURCE_DRIVER_HPP
