//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_BEHAVIOR_IBEHAVIOR_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_IBEHAVIOR_HPP

/**
 * @file    evolution/core/behavior/IBehavior.hpp
 * @brief   Interface for a behavior.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::IBehavior
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IObject.hpp>
#include <evolution/core/behavior/BehaviorConfigTypes.hpp>
#include <evolution/core/behavior/BehaviorData.hpp>
#include <evolution/core/behavior/IBehaviorContainer.hpp>
#include <evolution/core/behavior/BehaviorRegistry.hpp>

namespace Evolution
{

    /**
     * @brief
     * Interface for a behavior.
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR IBehavior : public IObject
    {
    public: // Types

        typedef unsigned short PortId;      ///< The port id type.
        typedef PortId         PortCount;   ///< The port count type.

    public: // Structors

        /**
         * @brief
         * Empty constructor.
         */
        IBehavior () {}

        /**
         * @brief
         * Empty destructor.
         */
        virtual ~IBehavior () {}

    public: // Information

        /**
         * @brief
         * Obtains the ID of the behavior instance.
         * 
         * @return the id of the behavior.
         **/
        virtual const char* get_id () const = 0;

        /**
         * @brief
         * Obtains the type of the behavior.
         * 
         * @return the type of the behavior.
         **/
        virtual const char* get_type () const = 0;

        /**
         * @brief Returns the sum of input and output connections supported by
         * the behavior.
         *
         * @return The number of connections.
         **/
        virtual unsigned int get_num_connections() const = 0;

        /**
         * @brief Returns the number of input connections supported by the
         * behavior.
         *
         * @return The number of input connections.
         **/
        virtual unsigned int get_num_input_connections() const = 0;

        /**
         * @brief Returns the number of output connections supported by the
         * behavior.
         *
         * @return The number of output connections.
         **/
        virtual unsigned int get_num_output_connections() const = 0;

    public: // Configuration

        /**
         * @brief
         * Obtains the string value of the specified parameter.
         *
         * Redefine in subclasses using the DECLARE_BEHAVIOR_PARAMS macro and
         * friends.
         * 
         * @param ticket [in]; The security ticket.
         * @param param [in]; The name of the parameter.
         * @param value [out]; The returned string value of the parameter.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        virtual Result get_parameter (TicketId ticket, const char* param,
                                      String* value) const = 0;

        /**
         * @brief
         * Sets the value of the specified parameter as a string.
         *
         * Redefine in subclasses using the DECLARE_BEHAVIOR_PARAMS macro and
         * friends.
         * 
         * @param ticket [in]; The security ticket.
         * @param param [in]; The name of the parameter to set.
         * @param value [in]; The new string value of the parameter.
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        virtual Result set_parameter (TicketId ticket, const char* param,
                                      const char* value) = 0;

        /**
         * @brief
         * Provides a zero-indexed enumeration of behavior parameters.
         * 
         * @param ticket [in]; The security ticket.
         * @param index [in]; The parameter index.
         * @param name [out]; The name of the parameter at the specified index.
         * @param value [out]; The value of the parameter as a string.
         * 
         * @return RESULT_SUCCESS on success or RESULT_NOT_FOUND when
         *      index exceeds the number of parameters.
         **/
        virtual Result enum_parameter (TicketId ticket, unsigned index,
                                       String* name, String* value) = 0;

    public: // Activation

        /**
         * @brief  Indicates if the behavior is active.
         *
         * @return true if the behavior is active.
         **/
        virtual bool is_active () const = 0;

        /**
         * @brief  Activates the behavior.
         *
         * Subclasses should call this base method if they redefine
         * it to set _behavior_is_active to true.
         * 
         * @param ticket [in]; The security ticket.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result activate (TicketId ticket) = 0;

        /**
         * @brief
         * Deactivates the behavior.
         *
         * @param ticket [in]; The security ticket.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result deactivate (TicketId ticket) = 0;

    public: // Data propagation

        /**
         * @brief
         * Pushes input data to from the output of another behavior to the
         * input of this behavior.
         * 
         * Pushes input data to the behavior, so that it will be called by
         * another, upstream behavior. The behavior does not need to push its
         * output further downstream from this method. It can assume
         * that invoke_push() will be called when appropriate. This
         * method is the first stage of data propagation in a behavior
         * network with a push model run by a central executive.
         *
         * If the called behavior wishes to retain the data after the
         * completion of the call, it must add a reference and remove
         * it when no longer needed.
         *
         * @param   ticket [in]; Security ticket.
         * @param   input_port [in]; The input port to which the data is heading.
         * @param   source [in]; The ID of the upstream behavior, and the source
         *                       of the data.
         * @param   source_port [in]; The output port from which the data
         *                            is coming.
         * @param   data [in, opt]; The data to be received.
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre input_port is valid
         * @pre source is valid
         */
        virtual Result push_input (TicketId ticket,
                                   PortId input_port,
                                   IBehavior* source, 
                                   PortId source_port,
                                   BehaviorData* data) = 0;

        /**
         * Instructs the behavior that it should push its data
         * downstream to subsequent behaviors. This method is the
         * second stage of data propagation in a behavior network with
         * a push model run by an executive. In general, this method
         * will be called only by the executive.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result invoke (TicketId ticket) = 0;

    public: // Connections

        /**
         * Connects an output from the specified output port to the
         * target and target port. This method should be implemented
         * by all behaviors providing a push interface.
         *
         * @note
         * Behaviors should not call this method directly. Rather they
         * should request the connection through the behavior container,
         * which will make the appropriate calls.
         *
         * @note
         * To prevent the possibility of the target behavior being
         * deleted unexpectedly, the implementation should add a
         * reference to it.
         *
         * @param ticket [in]; The security ticket.
         * @param output_port [in]; The id of the output port from this 
         *                          behavior.
         * @param target [in]; Pointer to the target behavior of the connection.
         * @param target_port [in]; The id of the target port on the target
         *                          behavior.
         *
         * @return  RESULT_SUCCESS or RESULT_ALREADY_EXISTS
         *
         * @pre     output_port is valid on this, the "source" behavior
         * @pre     target != NULL
         * @pre     target_port is valid on the target behavior
         **/
        virtual Result connect_output (TicketId ticket,
                                       PortId output_port, 
                                       IBehavior* target,
                                       PortId target_port) = 0;

        /**
         * @brief
         * Disconnects an output from the specified output port to the
         * target and target port.
         *
         * @note
         * Behaviors should not call this method directly. Rather they
         * should request the connection through the behavior container,
         * which will make the appropriate calls.
         *
         * @note
         * Assuming that connect_output() added reference to the
         * target behavior, the implementation should now remove that
         * reference.
         * 
         * @param ticket [in]; The security ticket.
         * @param output_port [in]; The id of the output port in this behavior.
         * @param target [in]; Pointer to the target behavior of the connection.
         * @param target_port [in]; The id of target port on the target
         *      behavior.
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         *
         * @pre     output_port is valid on this, the "source" behavior
         * @pre     target != NULL
         * @pre     target_port is valid on the target behavior
         **/
        virtual Result disconnect_output (TicketId ticket,
                                          PortId output_port, 
                                          IBehavior* target,
                                          PortId target_port) = 0;

        /**
         * @brief
         * Detaches all connections to the specified behavior.
         *
         * The behavior container will call this method when the other
         * behavior is removed.
         * 
         * @param ticket [in]; The security ticket.
         * @param other [in]; The behavior from which to detach all connections.
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result detach_behavior (TicketId ticket, IBehavior* other) = 0;


        /**
         * @brief
         * 
         * @param ticket [in]; The security ticket.
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        virtual Result remove_all_connections (TicketId ticket) = 0;

    }; // end class IBehavior

} // end namespace Evolution

/**
 * @brief This macro should be put in each instantiable behavior's klass.hpp
 * file in a public declaration section.  It declares a factory creation
 * function for the behavior and a string name to reference it, used in
 * registry lookup.
 *
 * @param klass [in]; Name of the class (eg Foo)
 * @param type  [in]; Name of the behavior (eg "Evolution.Foo")
 *
 * @see Evolution::BehaviorRegistry
 */
#define DECLARE_BEHAVIOR(klass, type) \
static Evolution::Result create_behavior(Evolution::TicketId ticket,               \
                              Evolution::BehaviorConfig& config,                   \
                              Evolution::IBehaviorContainer& container,            \
                              Evolution::IBehavior** behavior_ptr);                \
static const char* __get_type_id () { return (type); } \
static const char * const _TYPE_ID;


/**
 * @brief This macro accesses the behavior's registered type.
 *
 * @param klass [in]; Name of the class (eg Foo)
 */
#define BEHAVIOR_TYPE(klass)  (klass::_TYPE_ID)


/**
 * @brief This macro forces the behavior to be registered in the behavior
 * registry when the library is loaded.  It is implicitly called from the
 * IMPLEMENT_BEHAVIOR macro, so it only needs to be included when the behavior
 * designer is not using IMPLEMENT_BEHAVIOR.  This macro should be included
 * in the klass.cpp file.
 *
 * @param klass [in]; Name of the class (eg Foo)
 */
#define REGISTER_BEHAVIOR(klass) \
static Evolution::Result _registered_##klass##_IBehavior_result = \
Evolution::BehaviorRegistry::register_behavior (klass::__get_type_id (), klass::create_behavior); \
const char * const klass::_TYPE_ID = klass::__get_type_id ();

#endif // INCLUDE_EVOLUTION_BEHAVIOR_IBEHAVIOR_HPP
