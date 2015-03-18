//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_IOBJECT_HPP
#define INCLUDE_EVOLUTION_BASE_IOBJECT_HPP


/**
 * @file   evolution/core/base/IObject.hpp
 * @brief  Base interface class, with reference counting and the
 *         ability to obtain other interfaces.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IObject
 */

#include <evolution/core/base/Types.hpp>


namespace Evolution
{


    /**
     * @interface IObject
     * @brief Basic interface class interface. 
     *
     * Base interface class, with reference counting and the ability to obtain
     * of other interfaces.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE IObject
    {
    public: // Structors

        /// Constructor.
        IObject () {}

    protected:
        /// Destructor.
        virtual ~IObject () {}

    public: // Reference counting

        /**
         * @brief
         * Adds a reference to the object.
         *
         * @return  The updated reference count.
         */
        virtual unsigned add_ref () = 0;

        /**
         * @brief
         * Removes a reference from the object, freeing its memory if
         * the count is zero.
         *
         * @return The updated reference count.
         */
        virtual unsigned remove_ref () = 0;
        
        /**
         * @brief
         * Returns the current reference count.
         *
         * @return The current reference count.
         */
        virtual unsigned ref_count () = 0;

    public: // Interfaces

        /**
         * @brief
         * Obtains the specified interface from the object if the
         * object supports that interface, and the caller's security ticket
         * has access to it.
         *
         * It is important that all implementations of this function
         * call add_ref() on the resulting pointer before returning a
         * successful result.
         *
         * @param   ticket        [in];  Security ticket (a placeholder)
         * @param   interface_id  [in];  The interface ID of the requested interface.
         *            This ID should be declared with the INTERFACE_ID constant
         *            member on the interface class.
         * @param   interface_ptr [out]; This is the pointer to the interface. 
         *            It is non-NULL if the call succeeds.
         * @return  RESULT_SUCCESS,
         *          RESULT_NOT_FOUND,
         *          RESULT_INSUFFICIENT_ACCESS,
         *          or another context-dependent error.
         */
        virtual Result obtain_interface (TicketId ticket, 
                                         const char* interface_id,
                                         void** interface_ptr) = 0;

        /**
         * @brief Prints a simple description of the object contents
         *
         * @param out [in]; ostream to write to.
         */
        virtual inline void print (std::ostream& out) const
        {
            out << "{IObject " << std::hex << this << "}";
        }

    }; // end class IObject

    /**
     * @brief Ostream output operator for IObject.
     *
     * Function to allow IObjects to be written to ostreams easily.
     *
     * @param out [in]; ostream to write to.
     * @param obj [in]; object to write.
     */
    inline std::ostream& operator<< (std::ostream& out, const IObject& obj)
    {
        obj.print(out);
        return out;
    }

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_IOBJECT_HPP

