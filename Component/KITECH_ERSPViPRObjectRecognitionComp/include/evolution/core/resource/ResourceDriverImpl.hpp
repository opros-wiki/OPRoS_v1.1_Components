//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_DRIVER_IMPL_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_DRIVER_IMPL_HPP


/**
 * @file    evolution/core/resource/ResourceDriverImpl.hpp
 * @brief   A basic implementation of IResourceDriver.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ResourceDriverImpl
 */


#include <string>
#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/resource/IResourceDriver.hpp>


namespace Evolution
{


    /**
     * @class ResourceDriverImpl 
     *
     * @brief This class provides a default implementation of the bookkeeping
     * aspects of ResourceDriver.
     *
     * @see ResourceDriver
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ResourceDriverImpl : public IResourceDriver,
                                                         public ObjectImpl
    {
    public: // Structors

        /**
         * Constructor
         *
         * @deprecated  
         * The argument order of this constructor is inconsistent with
         * the rest of the platform (the ticket is not the first
         * parameter). Use the other constructor instead.
         */
        ResourceDriverImpl (const ResourceConfig& resource_config,
                            IResourceContainer& resource_container,
                            TicketId ticket)
            : IResourceDriver (),
              _resource_config (resource_config),
              _resource_container (resource_container),
              _resource_ticket (ticket),
              _poll (false)
        {
        } // end ResourceDriverImpl()

        /**
         * @brief Constructor
         *
         * @param ticket             [in]; The (TicketId) for security purposes.
         * @param resource_config    [in]; The resource configuration object
         * @param resource_container [in]; The resource container object
         */
        ResourceDriverImpl (TicketId ticket,
                            const ResourceConfig& resource_config,
                            IResourceContainer& resource_container)
            : IResourceDriver (),
              _resource_config (resource_config),
              _resource_container (resource_container),
              _resource_ticket (ticket),
              _poll (false)
                  
        {
        } // end ResourceDriverImpl()

        /**
         * Destructor
         */
        virtual ~ResourceDriverImpl () {}

    private: // Assignment

        /**
         * @brief Copy constructor.
         *
         * @param other [in]; The ResourceDriverImpl to copy from
         */
        ResourceDriverImpl (const ResourceDriverImpl& other)
            : IResourceDriver (),
              _resource_config (other._resource_config),
              _resource_container (other._resource_container) {}

        ResourceDriverImpl& operator= (const ResourceDriverImpl& other) { return (*this); }

    public: // Reference count

        /**
         * @brief The add_ref function adds a reference to the driver.
         *
         * @return  The updated reference count.
         */
        virtual unsigned add_ref ()
        {
            return (ObjectImpl::add_ref ());
        } // end add_ref()

        /**
         * @brief The remove_ref removes a reference from the driver.
         *
         * @return  The updated reference count.
         */
        virtual unsigned remove_ref ()
        {
            return (ObjectImpl::remove_ref ());
        } // end remove_ref()

    public: // IResourceDriver

        /**
         * @brief The get_id function returns the resource's id.
         *
         * @param id [out]; The returned resource id
         *  
         * @return RESULT_SUCCESS if successful, or an error code if not.
         * @ingroup drivers_rcm
         */
        virtual Result get_id (String* id) 
        {
            return _resource_config.get_id(id);
        }

        /**
         * @brief The set_poll function specifies whether the resource should be polled.
         *
         * @param ticket [in]; The security ticket
         * @param poll   [in]; true if the resource should be polled
         * 
         * @return RESULT_SUCCESS if successful, or an error code if not
         * @ingroup drivers_rcm
         */
        virtual Result set_poll (TicketId ticket, bool poll)
        {
            _poll = poll;
            return RESULT_SUCCESS;
        }

        /**
         * @brief The query function queries the resource for all resource data.
         *
         * @param ticket [in]; The security ticket
         * 
         * @return RESULT_SUCCESS if successful, or an error code if not
         * @ingroup drivers_rcm
         */
        virtual Result query (TicketId ticket)
        {
            return RESULT_NOT_IMPLEMENTED;
        }
        
    protected: // Implementation

        const ResourceConfig& _resource_config;   ///< Information about the resource from the config file.
        IResourceContainer& _resource_container;  ///< Reference to the container of the resource, through which
                                                  ///<   it interacts with the outside world. 
        TicketId _resource_ticket;                ///< Ticket for the resource.
        bool _poll;                               ///< True if the resource should be polled.
    }; // end class Resource 

} // end namespace Evolution


/**
 * @brief This macro defines the klass::create_resource function which was declared
 * by DECLARE_RESOURCE.  This macro should be included in the klass.cpp file.
 * The macro also automatically registers the resource with the resource
 * registry.  Users who need a non-standard factory creation function can
 * manually rewrite this function instead of including this default version
 * as written in the macro.  In doing so, remember to include the
 * REGISTER_RESOURCE macro to register the class with the registry.
 *
 * @param klass [in];  Name of the class (eg Foo)
 *
 * @deprecated This macro assumes that the constructor of the resource driver
 * class takes and argument order that is inconsistent with the rest
 * of the platform (the ticket is not the first parameter). Use
 * IMPLEMENT_RESOURCE2 instead.
 */
#define IMPLEMENT_RESOURCE(klass)                                           \
REGISTER_RESOURCE(klass)                                                    \
Result klass::create_resource(TicketId ticket,                              \
                              const ResourceConfig& config,                 \
                              IResourceContainer& container,                \
                              IResourceDriver** resource_ptr)               \
{                                                                           \
    ResourceDriverImpl *resource = new klass(config, container, ticket);    \
    if (resource == NULL)                                                   \
        return RESULT_OUT_OF_MEMORY;                                        \
                                                                            \
    *resource_ptr = resource;                                               \
    return RESULT_SUCCESS;                                                  \
}

#ifndef DOXYGEN_INTERNAL
#define IMPLEMENT_RESOURCE2(klass)                                                     \
REGISTER_RESOURCE(klass)                                                               \
Evolution::Result klass::create_resource(Evolution::TicketId ticket,                   \
                              const Evolution::ResourceConfig& config,                 \
                              Evolution::IResourceContainer& container,                \
                              Evolution::IResourceDriver** resource_ptr)               \
{                                                                                      \
    Evolution::ResourceDriverImpl *resource = new klass(ticket, config, container);    \
    if (resource == NULL)                                                              \
        return Evolution::RESULT_OUT_OF_MEMORY;                                        \
                                                                                       \
    *resource_ptr = resource;                                                          \
    return Evolution::RESULT_SUCCESS;                                                  \
}
#endif // end DOXYGEN_INTERNAL

#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE_DRIVER_IMPL_HPP
