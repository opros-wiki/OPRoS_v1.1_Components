//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR__BEHAVIOR_IMPL_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR__BEHAVIOR_IMPL_HPP


/**
 * @file    evolution/core/behavior/BehaviorImpl.hpp
 * @brief   A standard implementation of the IBehavior interface.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::BehaviorImpl
 */

#include <stdio.h>

#include <set>
#include <map>
#include <vector>

#include <evolution/core/behavior/IBehavior.hpp>
#include <evolution/core/behavior/BehaviorDataWriter.hpp>
#include <evolution/core/math/Pose2DT.hpp>

namespace Evolution
{

    class IAudioPlay;

    /**
     * @brief
     * A standard implementation of the IBehavior interface.
     *
     * BehaviorImpl provides basic implementations for
     * synchronous behaviors following the push and/or pull data
     * propagation models.
     *
     * @ingroup core_behavior
     */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorImpl : public IBehavior
    {
    public: // Types

        typedef PortId PortCount;   ///< Different semantic usage.

    public: // Structors

        /**
         * @brief 
         * Constructor.
         *
         * @param ticket [in]; The security ticket.
         * @param container [in]; Reference to the IBehaviorContainer
         *      object that will contain this behavior.
         * @param input_count [in]; The number of input ports that the
         *      behavior will have.
         * @param output_count [in]; The number of output ports that 
         *      the behavior will have.
         */
        BehaviorImpl (TicketId ticket, IBehaviorContainer& container,
                      PortCount input_count, PortCount output_count);

        /**
         * @brief
         * Destructor. Deactivates behaviors.
         */
        virtual ~BehaviorImpl ();

        /**
         * @brief
         * Loads the configuration information, setting the ID and
         * parameters, and checking that inputs and outputs are as
         * expected.
         *
         * Conceptually, the loading of configuration information
         * should happen in the constructor. But setting
         * the parameters correctly requires subclass method calls, which
         * are not possible from a constructor. Accordingly, this
         * method should only be called once.
         *
         * @pre   _behavior_config == NULL (this has not been called successfully yet)
         * @param ticket [in]; The security ticket.
         * @param config [in]; The BehaviorConfig object containing the 
         *      behavior's configuration.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        virtual Result load_configuration (TicketId ticket, BehaviorConfig& config);

        /**
         * @brief
         * A method to be overloaded with initialization code specific
         * to the subclass. This method should be called after
         * load_configuration().
         * 
         * @param ticket [in]; The security ticket.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        virtual Result initialize (TicketId ticket);

    public: // IObject interface

        /**
         * @brief
         * Adds a reference to the driver.
         *
         * @return the current reference count.
         * 
         * @see  IObject::add_ref()
         **/
        virtual unsigned add_ref ();

        /**
         * @brief
         * Removes a reference from the driver.
         *
         * @return the current reference count.
         *
         * @see  IObject::remove_ref()
         **/
        virtual unsigned remove_ref ();

        /**
         * @brief
         * Returns the current reference count for the object. 
         * @return The number of current references.
         *
         * @see  IObject::ref_count()
         **/
        unsigned ref_count ();

        /**
         * @brief
         * Obtains the specified interface from the object if the
         * object supports that interface and the caller's security ticket
         * has access to it.
         *
         * @param ticket [in]; The security ticket.
         * @param interface_id [in]; The id of the interface to be obtained.
         * @param interface_ptr [out]; The pointer to the obtained interface.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         * 
         * @see  IObject::obtain_interface()
         **/
        virtual Result obtain_interface (TicketId ticket,
                                         const char* interface_id,
                                         void** interface_ptr);

    public: // IBehavior interface

        /**
         * @brief
         * Obtains the ID of the behavior instance.
         * 
         * @return the id of the behavior.
         * 
         * @see  IBehavior::get_id()
         **/
        virtual const char* get_id () const;

        /**
         * @brief
         * Obtains the type of the behavior.
         * 
         * @return the type of the behavior.
         *
         * @see  IBehavior::get_type()
         **/
        virtual const char* get_type () const;

        /**
         * @brief Returns the sum of input and output connections supported by
         * the behavior.
         *
         * @return The number of connections.
         **/
        virtual unsigned int get_num_connections() const;

        /**
         * @brief Returns the number of input connections supported by the
         * behavior.
         *
         * @return The number of input connections.
         **/
        virtual unsigned int get_num_input_connections() const;

        /**
         * @brief Returns the number of output connections supported by the
         * behavior.
         *
         * @return The number of output connections.
         **/
        virtual unsigned int get_num_output_connections() const;

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
         * 
         * @see  IBehavior::get_parameter(), DECLARE_BEHAVIOR_PARAMS
         **/
        virtual Result get_parameter (TicketId ticket,
                                      const char* param,
                                      String* value) const;

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
         *
         * @see  IBehavior::set_parameter(), DECLARE_BEHAVIOR_PARAMS
         **/
        virtual Result set_parameter (TicketId ticket,
                                      const char* param,
                                      const char* value);

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
         *
         * @see  IBehavior::enum_parameter()
         **/
        virtual Result enum_parameter (TicketId ticket, unsigned index,
                                       String* name, String* value);
        /**
         * @brief  Indicates if the behavior is active.
         *
         * @return true if the behavior is active.
         *
         * @see  IBehavior::is_active()
         **/
        virtual bool is_active () const;

        /**
         * @brief  Activates the behavior.
         *
         * Subclasses should call this base method if they redefine
         * it to set _behavior_is_active to true.
         * 
         * @param ticket [in]; The security ticket.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         *
         * @see  IBehavior::activate()
         **/
        virtual Result activate (TicketId ticket);

        /**
         * @brief
         * Deactivates the behavior.
         *
         * Subclasses should call this base method if they redefine
         * it to set _behavior_is_active to false.
         * 
         * @param ticket [in]; The security ticket.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         *
         * @see IBehavior::deactivate()
         **/
        virtual Result deactivate (TicketId ticket);

        /**
         * @brief
         * Pushes input data to the behavior to be called by another,
         * upstream behavior.
         *
         * @param   ticket [in]; security ticket
         * @param   input_port [in]; the input port to which the data is headed
         * @param   source [in]; the ID of the upstream behavior, source of the data
         * @param   source_port [in]; the output port from which the data is coming.
         * @param   data [in, opt]; the data to be received
         *
         * @return  RESULT_SUCCESS or a failure code.
         * @see  IBehavior::push_input()
         */
        virtual Result push_input (TicketId ticket,
                                   PortId input_port,
                                   IBehavior* source,
                                   PortId source_port,
                                   BehaviorData* data);

        /**
         * @brief
         * Instructs the behavior that it should push its data
         * downstream to subsequent behaviors.
         *
         * @param ticket [in]; The security ticket.
         *
         * @return  RESULT_SUCCESS or a failure code.
         * 
         * @see  IBehavior::invoke()
         */
        virtual Result invoke (TicketId ticket);

        /**
         * @brief
         * Connects an output from the specified output port to the
         * target and the target port.
         *
         * @param ticket [in]; The security ticket.
         * @param output_port [in]; The id of the output port from this 
         *      behavior.
         * @param target [in]; Pointer to the target behavior of the connection.
         * @param target_port [in]; The id of the target port on the target
         *      behavior.
         *
         * @return  RESULT_SUCCESS or RESULT_ALREADY_EXISTS
         *
         * @see  IBehavior::connect_output()
         */
        virtual Result connect_output (TicketId ticket, PortId output_port,
                                       IBehavior* target, PortId target_port);

        /**
         * @brief
         * Disconnects an output from the specified output port to the
         * target and the target port.
         * 
         * @param ticket [in]; The security ticket.
         * @param output_port [in]; The id of the output port in this behavior.
         * @param target [in]; Pointer to the target behavior of the connection.
         * @param target_port [in]; The id of target port on the target
         *      behavior.
         *
         * @return  RESULT_SUCCESS or an error code on failure.
         *
         * @see  IBehavior::disconnect_output()
         */
        virtual Result disconnect_output (TicketId ticket, PortId output_port,
                                          IBehavior* target, PortId target_port);

        /**
         * @brief
         * Detaches all connections to the specified behavior.
         * 
         * @param ticket [in]; The security ticket.
         * @param other [in]; The behavior from which to detach all connections.
         * 
         * @return RESULT_SUCCESS or an error code on failure.
         *
         * @see  IBehavior::detach_behavior()
         **/
        virtual Result detach_behavior (TicketId ticket, IBehavior* other);

        /**
         * @brief
         * Removes all connections from the behavior.
         * 
         * @param ticket [in]; The security ticket.
         *
         * @return RESULT_SUCCESS or an error code on failure.
         *
         * @see  IBehavior::remove_all_connections()
         */
        virtual Result remove_all_connections (TicketId ticket);

    public: // Overrides

        /**
         * @brief
         * Method to be implemented by subclasses, to compute output
         * for each output port from input on the input ports.
         *
         * Most implementations will need to use only the following
         * methods from BehaviorImpl:
         *
         *   get_port_input()    - To iterate over data on a given input port
         *
         *   get_port_output()   - To obtain the data storage for a given output port.
         *                         This method also calls mark_output_valid() to
         *                         schedule the data to be propagated.
         *
         * In some cases, the following methods also will be of use:
         *
         *   mark_output_valid()    - To mark a port as having or not having valid data.
         *                            This is usually used to reverse the effects of
         *                            get_port_output() and instead have NULL pushed.
         *
         *   release_cached_input() - Input data usually is cached until new data from
         *                            the same connection is received. This method clears
         *                            the cache for all connections.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        virtual Result compute_output () = 0;

    public: // Setting

        /**
         * @brief
         * Sets the ID of the behavior, if it has not been set.
         * @param ticket [in]; The security ticket.
         * @param behavior_id [in]; The behavior id to set.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        Result set_id (TicketId ticket, const char* behavior_id);

    public: // Information

        /**
         * @brief
         * Obtains the number of input ports.
         *
         * @return the number of input ports.
         */
        PortCount get_input_count () const { return (_input_count); }

        /**
         * @brief
         * Obtains the number of output ports.
         *
         * @return the number of output ports.
         */
        PortCount get_output_count () const { return (_output_count); }

    public: // Internal types
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief
         * A behavior pointer and port ID pair that uniquely
         * identifies specific port on a specific behavior instance.
         *
         * Used as a key in connections and input data.
         */
        struct EVOLUTION_EXPORT_BEHAVIOR BehaviorPortKey
        {
        public: /// BehaviorPortKey constructor

            explicit BehaviorPortKey (IBehavior* a_behavior = NULL,
                                      PortId a_port = 0)
                : behavior (a_behavior), port (a_port) {}

        public: /// Equality operators

            bool operator== (const BehaviorPortKey& other) const
            {
                return ((behavior == other.behavior) && (port == other.port));
            }

        public: // Data

            IBehavior*  behavior;    ///< Behavior interface pointer
            PortId      port;        ///< Behavior port id
        }; // end struct BehaviorPortKey

        /// Comparison functor for BehaviorPortKey.
        struct EVOLUTION_EXPORT_BEHAVIOR BehaviorPortKeyLess
        {
        public: // Comparison

            /// The comparison operator.
            bool operator() (const BehaviorPortKey& a,
                             const BehaviorPortKey& b) const
            {
                if (a.behavior == b.behavior)
                {
                    return (a.port < b.port);
                }
                else
                {
                    return (a.behavior < b.behavior);
                }
            }
        }; // end struct BehaviorPortKeyLess

        /**
         * @brief
         * A table indicating connections from a single local port to
         * other behaviors.
         *
         * The table may be used for both input and
         * output connections. The port IDs stored in the table
         * indicate the ports on the other behaviors.
         */
        typedef std::set<BehaviorPortKey, BehaviorPortKeyLess> PortConnectionTable;

        /// A table of connections for all input or all output ports.
        typedef std::vector<PortConnectionTable> ConnectionTable;

        /**
         * @brief
         * A table of input data from upstream behaviors connected to a
         * single input port.
         */
        struct EVOLUTION_EXPORT_BEHAVIOR PortInputTable :
            std::map<BehaviorPortKey, BehaviorData*, BehaviorPortKeyLess>
        {
            bool get_boolean(bool default_value = false) const;
            double get_double(double default_value = 0.0) const;
            const Image *get_image(const Image *image = 0) const;
            const DoubleArray *get_double_array() const;
            const char *get_string() const;

            // returns a reference to the object
            IObject *get_object() const;
        };

        /**
         * @brief
         * Iterator subclass providing more user-friendly and
         * semantically relevant accessors.
         **/
        class EVOLUTION_EXPORT_BEHAVIOR PortInputIterator :
            public PortInputTable::const_iterator
        {
        public: // Structors

            /// Default constructor.
            PortInputIterator () : PortInputTable::const_iterator () {}

            /// Copy constructor.
            PortInputIterator (const PortInputTable::const_iterator& other)
                : PortInputTable::const_iterator (other) {}

        public: // Assignment

            /// Assignment operator.
            PortInputIterator& operator= (const PortInputTable::const_iterator& other)
            {
                PortInputTable::const_iterator::operator = (other);
                return (*this);
            }

        public: // Access

            /// Obtains the source behavior for the current data.
            IBehavior* get_source_behavior ()
            {
                return ((*this)->first.behavior);
            } // end get_source_behavior ()

            /// Obtains the source port for the current data on an input iterator.
            PortId get_source_port ()
            {
                return ((*this)->first.port);
            } // end get_source_port()

            /// Obtains the current data on an input iterator.
            BehaviorData* get_data ()
            {
                return ((*this)->second);
            } // end get_data()
        }; // end class PortInputIterator

        /// A table of connections for all input ports.
        typedef std::vector<PortInputTable> InputTable;

        /// A table of connections for all output ports.
        typedef std::vector<BehaviorDataWriter*> OutputTable;

        /**
         * @brief
         * Inner class used to flag when an output port has valid data.
         *
         * Basically a vector of boolean values, but optimized to use
         * only a single bit for each value.
         **/
        class EVOLUTION_EXPORT_BEHAVIOR BitSet :
            protected std::vector<unsigned char>
        {
        public: // Types

            typedef std::vector<unsigned char> VectorType; ///< Convenience typedef.

        public: // Structors

            /// Constructor.
            BitSet (PortCount bit_size = 0)
                : VectorType (bit_to_index (bit_size)
                              + (bit_to_subindex (bit_size) ? 1 : 0)),
                  _bit_size (bit_size)
            {}

        public: // Access

            /// Checks if the specified bit is set.
            bool is_set (PortCount bit) const
            {
                return ((at (bit_to_index (bit)) &
                        (1 << bit_to_subindex (bit))) != 0);
            } // end is_set()

            /// Obtains the number of bits.
            PortCount get_size () const { return (_bit_size); }

        public: // Setting

            /// Sets the specified bit to the value.
            void set_bit (PortCount bit, bool value)
            {
                if (value)
                {
                    at (bit_to_index (bit)) |= (1 << bit_to_subindex (bit));
                }
                else
                {
                    at (bit_to_index (bit)) &= ~(1 << bit_to_subindex (bit));
                }
            } // end set_bit()

            /// Sets all bits to the specified value.
            void set_all (bool value)
            {
                unsigned char mask = (value ? 0xFF : 0x00);
                assign (size (), mask);
            } // end set_all()

        public: // Sizing

            /* Resizes the bit set.
             * @param bit_size [in]; The new size of the bit set.
             */
            void resize (PortCount bit_size)
            {
                VectorType::resize (bit_to_index (bit_size)
                                    + (bit_to_subindex (bit_size) ? 1 : 0));
            } // end resize()

        protected: // Helpers

            /// Bit to index conversion
            PortCount bit_to_index (PortCount bit) const { return (bit / 8); }

            /// Bit to sub index conversion
            PortCount bit_to_subindex (PortCount bit) const { return (bit % 8); }

        protected: // Implementation

            PortCount  _bit_size;   ///< Number of bits.
        }; // end class BitSet

    protected: // Subclass helpers

        /**
         * @brief
         * Obtains the table of input data and its priorities for the
         * specified source on the input port.
         */
        const PortInputTable& get_port_input (PortId input_port);

        /**
         * @brief
         * Obtains the output data for the specified output port.
         */
        virtual BehaviorDataWriter* get_port_output (PortId output_port);

        /**
         * @brief
         * Replaces the data memory for the specified port's output data.
         **/
        virtual void replace_port_output (PortId output_port,
                                          BehaviorDataWriter* data);

        /**
         * @brief
         * Marks output from the specified port as valid or invalid;
         * if invalid, null will be passed from the port.
         **/
        virtual Result mark_output_valid (PortCount output_port, bool is_valid);

        /**
         * @brief
         * Plays an audio file on the previously specified audio resource
         *
         * @param ticket [in]; The security ticket.
         * @param filename [in]; The name of the file to play.
         * @see  setup_audio
         **/
        Result play_audio_file(TicketId ticket,
                               const char * filename,
                               bool warn_if_fail = true);

        /// Releases all cached input data.
        void release_cached_input ();

        // input convenience functions.
        /**
         * @brief Convenience function to read the first boolean from an input port.
         * @param port [in]; The port to read from.
         * @param output [out]; Where the data comes out, on success.
         * @return RESULT_SUCCESS on success, RESULT_FAILURE on failure.
         */
        Result input_first_boolean(PortId port, bool * output);

        /**
         * @brief Convenience function to read the first boolean from an input port.
         * @param port [in]; The port to read from.
         * @param fallback [in]; If there is no input, this is returned as a fallback.
         * @return The value on the input port, or if none there, the fallback value.
         */
        bool input_first_boolean(PortId port, bool fallback);

        /**
         * @brief Convenience function to read the first double from an input port.
         * @param port [in]; The port to read from.
         * @param output [out]; Where the data comes out, on success.
         * @return RESULT_SUCCESS on success, RESULT_FAILURE on failure.
         */
        Result input_first_double(PortId port, double * output);

        /**
         * @brief Convenience function to read the first double from an input port.
         * @param port [in]; The port to read from.
         * @param fallback [in]; If there is no input, this is returned as a fallback.
         * @return The value on the input port, or if none there, the fallback value.
         */
        double input_first_double(PortId port, double fallback);

        /**
         * @brief Convenience function to read the first enum from an input port.
         * @param port [in]; The port to read from.
         * @param output [out]; Where the data comes out, on success.
         * @return RESULT_SUCCESS on success, RESULT_FAILURE on failure.
         */
        Result input_first_enum(PortId port, long * output);

        /**
         * @brief Convenience function to read the first enum from an input port.
         * @param port [in]; The port to read from.
         * @param fallback [in]; If there is no input, this is returned as a fallback.
         * @return The value on the input port, or if none there, the fallback value.
         */
        long input_first_enum(PortId port, long fallback);

        /**
         * @brief Convenience function to read the first DoubleArray from an input port.
         * @param port [in]; The port to read from.
         * @param output [out]; Where the data comes out, on success.
         * @return RESULT_SUCCESS on success, RESULT_FAILURE on failure.
         */
        Result input_first_double_array(PortId port, DoubleArray *output);

        /**
         * @brief Convenience function to read the first DoubleArray from an input port.
         * @param port [in]; The port to read from.
         * @param fallback [in]; If there is no input, this is returned as a fallback.
         * @return The value on the input port, or if none there, the fallback value.
         */
        DoubleArray input_first_double_array(PortId port,
                                             const DoubleArray & fallback);

        /**
         * @brief Convenience function to read the first Pose2D from an input port.
         * The actual input type is DoubleArray, but this is converted
         * to a Pose2D by the standard order x, y, theta.
         *
         * @param port [in]; The port to read from.
         * @param output [out]; Where the data comes out, on success.
         *                      On failure, output will not be changed.
         * @return RESULT_SUCCESS on success, RESULT_FAILURE on failure.
         */
        Result input_first_pose2d(PortId port, Pose2D * output);

        /**
         * @brief Convenience function to read the first Pose2DT from an input port.
         * The actual input type is DoubleArray, but this is converted
         * to a Pose2DT by the standard order x, y, theta, timestamp.
         *
         * @param port [in]; The port to read from.
         * @param output [out]; Where the data comes out, on success.
         *                      On failure, output will not be changed.
         * @return RESULT_SUCCESS on success, RESULT_FAILURE on failure.
         */
        Result input_first_pose2dt(PortId port, Pose2DT * output);

        /**
         * @brief Convenience function to read the first Pose2D from an input port.
         * The actual input type is DoubleArray, but this is converted
         * to a Pose2D by the standard order x, y, theta.
         *
         * @param port [in]; The port to read from.
         * @param fallback [in]; If there is no input, this is returned as a fallback.
         * @return The DoubleArray on the input port converted to a
         *         Pose2D, or if nothing is there, the fallback value.
         */
        Pose2D input_first_pose2d(PortId port, const Pose2D & fallback);

        /**
         * @brief Convenience function to read the first Pose2DT from an input port.
         * The actual input type is DoubleArray, but this is converted
         * to a Pose2D by the standard order x, y, theta, timestamp.
         *
         * @param port [in]; The port to read from.
         * @param fallback [in]; If there is no input, this is returned as a fallback.
         * @return The DoubleArray on the input port converted to a
         *         Pose2DT, or if nothing is there, the fallback value.
         */
        Pose2DT input_first_pose2dt(PortId port, const Pose2DT & fallback);

        /**
         * @brief Convenience function to read the first IObject pointer from
         * an input port.
         *
         * Convenience function to read the first IObject pointer from
         * an input port.  This calls obtain_interface() with the
         * interface_id parameter.  If this fails, or if no input is
         * available, NULL is returned.
         *
         * If non-NULL, the resultant pointer has had one reference
         * added to it.  When the caller is finished with the returned
         * pointer, they should call remove_ref() on the result.
         *
         * @param port [in]; The port to read from.
         *
         * @param interface_id [in]; A string identifying the type of IObject
         * desired.  In practice this should be something like
         * "LandmarkMap::INTERFACE_ID" instead of a hard-coded string.
         *
         * @return A void pointer to the object on success, or NULL if
         * there is no input or if the incoming object was the wrong
         * type.  The returned pointer should be cast to the type
         * corresponding to interface_id.
         */
        void * input_first_iobject( PortId port, const char *interface_id );

        // output convenience functions.

        /**
         * @brief Send a boolean to an output port.
         * @param port [in]; The id of the port to send to.
         * @param value [in]; The value to send out.
         */
        void output_boolean(PortId port, bool value);

        /**
         * @brief Send an enum to an output port.
         * @param port [in]; The id of the port to send to.
         * @param value [in]; The value to send out.
         */
        void output_enum(PortId port, long value);

        /**
         * @brief Send a double to an output port.
         * @param port [in]; The id of the port to send to.
         * @param value [in]; The value to send out.
         */
        void output_double(PortId port, double value);

        /**
         * @brief Send a DoubleArray to an output port.
         * @param port [in]; The id of the port to send to.
         * @param value [in]; The value to send out.
         */
        void output_double_array(PortId port, const DoubleArray & value);

        /**
         * @brief Send a Pose2D to an output port as a DoubleArray,
         *        in this order: x, y, theta, timestamp.
         * @param port [in]; The id of the port to send to.
         * @param value [in]; The value to send out.
         */
        void output_pose2d(PortId port, const Pose2D & value);

        /**
         * @brief Send an IObject pointer to an output port.
         * @param port [in]; The id of the port to send to.
         * @param value [in]; The pointer to send out.
         */
        void output_iobject( PortId port, IObject * ptr );

    protected: // Internal helpers

        /**
         * @brief
         * Sets the input count, to be called only once.
         *
         * @pre  _input_count == 0 (hasn't been called yet)
         **/
        void set_input_count (PortId input_count);

        /**
         * @brief
         * Sets the output count, to be called only once.
         *
         * @pre  _output_count == 0 (hasn't been called yet)
         **/
        void set_output_count (PortId output_count);

        /**
         * @brief
         * Sets up the audio driver for playback 
         *
         * @param ticket [in]; The security ticket.
         * @param  audio_resource [in]; The name of the audio resource to use.
         * @see  play_audio_file
         **/
        Result setup_audio (TicketId ticket, const char* audio_resource);

    protected: // Propagation helpers (internal)

        /**
         * @brief
         * Helper method that pushes the stored data from the
         * specified output port to downstream behaviors connected to
         * that port.
         *
         * Before calling this method, the new output data should have
         * been computed and stored in the appropriate BehaviorData
         * object (accessed via get_output_data()).
         *
         * @param  output_port [in]; the output port from which the data is headed
         * @pre    output_port is valid
         */
        virtual Result push_output_port (PortId output_port);

        /**
         * @brief
         * Helper method that pushes a null value from the specified
         * output port to downstream behaviors connected to that
         * port.
         *
         * This method is necessary to push null values, because
         * push_output_port() always pushes a pointer to an existing
         * BehaviorData object. Null values indicate that the behavior
         * is abstaining from providing data in the current cycle;
         * effectively it means "skip me."
         *
         * @param  output_port [in]; The output port from which the data is headed.
         * @pre    output_port is valid
         */
        virtual Result push_output_port_null(PortId output_port);
#endif // DOXYGEN_INTERNAL
    protected: // Implementation

        /// Behavior security ticket
        TicketId             _behavior_ticket;

        /// Behavior container reference
        IBehaviorContainer&  _behavior_container;

        /// Memory management.
        unsigned             _behavior_ref_count;


        BehaviorConfig*      _behavior_config;   ///< Behavior configuration.
        String               _behavior_id;       ///< Behavior id
        String               _behavior_type;     ///< Behavior id

        /// Input port count
        PortCount            _input_count;

        /// Output port count
        PortCount            _output_count;

        /// Behavior active flag
        bool                 _behavior_is_active;

        /// Connection table
        ConnectionTable      _connections;

        /// Input table
        InputTable           _input_data;

        /// Output table
        OutputTable          _output_data;

        /// Valid output flag
        BitSet               _valid_output;

        /// Audio Resource 
        IAudioPlay*          _audio_resource;

    }; // end class BehaviorImpl

} // end namespace Evolution

/**
 * This macro defines the klass::create_behavior function which was declared
 * by DECLARE_BEHAVIOR.  This macro should be included in the klass.cpp file.
 * The macro also automatically registers the behavior with the behavior
 * registry.  Users who need a non-standard factory creation function can
 * manually rewrite this function instead of including this default version
 * as written in the macro.  In doing so, remember to include the
 * REGISTER_BEHAVIOR macro to register the class with the registry.
 *
 * @param klass  Name of the class (e.g. Foo)
 **/
#define IMPLEMENT_BEHAVIOR(klass)                                                   \
REGISTER_BEHAVIOR(klass)                                                            \
Evolution::Result klass::create_behavior(Evolution::TicketId ticket,                \
                                         Evolution::BehaviorConfig& config,         \
                                         Evolution::IBehaviorContainer& container,  \
                                         Evolution::IBehavior** behavior_ptr)       \
{                                                                                   \
    Evolution::BehaviorImpl *behavior = new klass(ticket, container);               \
                                                                                    \
    if (behavior == NULL)                                                           \
        return Evolution::RESULT_OUT_OF_MEMORY;                                     \
                                                                                    \
    Evolution::Result result = behavior->load_configuration(ticket, config);        \
    if (result != Evolution::RESULT_SUCCESS) {                                      \
        delete behavior;                                                            \
        return result;                                                              \
    }                                                                               \
                                                                                    \
    result = behavior->initialize (ticket);                                         \
    if (result != Evolution::RESULT_SUCCESS) {                                      \
        delete behavior;                                                            \
        return result;                                                              \
    }                                                                               \
                                                                                    \
    *behavior_ptr = behavior;                                                       \
    return Evolution::RESULT_SUCCESS;                                               \
}


/**
 * @brief
 * Declares the set_parameter() methods to be redefined in the
 * class. The other behavior param macros are used to define set
 * methods for each parameter.
 *
 * This macro must occur in the declaration of a BehaviorImpl
 * subclass. See BEGIN_BEHAVIOR_PARAMS for an example of usage.
 *
 * @see  BEGIN_BEHAVIOR_PARAMS, BEHAVIOR_PARAM, END_BEHAVIOR_PARAMS
 **/
#define DECLARE_BEHAVIOR_PARAMS \
public: \
    Evolution::Result set_parameter (Evolution::TicketId ticket, const char* param, const char* value); \


/**
 * @brief
 * Begins a list of behavior parameters and their set accessor methods.
 *
 * This macro must occur outside the class declaration and in an
 * implementation file (not a header). It must be followed immediately
 * by zero or more instances of BEHAVIOR_PARAM, and a mandatory
 * END_BEHAVIOR_PARAMS. In addition, DECLARE_BEHAVIOR_PARAMS must have
 * been used in the class declaration.
 *
 * Following is an example of usage:
 *
 * @code
 * // Header file
 * class MyClass
 * {
 * public:
 *     Result set_color (TicketId ticket, const char* color);
 *
 *     DECLARE_BEHAVIOR_PARAMS;
 * };
 *
 * // Implementation file
 * BEGIN_BEHAVIOR_PARAMS(MyClass, ParentClass);
 * BEHAVIOR_PARAM(MyClass,"color",color);  // Handled by set_color().
 * END_BEHAVIOR_PARAMS(MyClass);
 *
 * // Let's set a parameter!
 * MyClass mine;
 * mine.set_parameter (ticket, "color", "blue");  // Calls MyClass::set_color()
 * @endcode
 *
 * @param  klass  ; The name of the class
 * @param  sooper ; The name of the parent class
 *
 * @see  BEGIN_BEHAVIOR_PARAMS, BEHAVIOR_PARAM, END_BEHAVIOR_PARAMS
 **/
#define BEGIN_BEHAVIOR_PARAMS(klass, sooper)  \
\
/* The parent class */ \
static Evolution::Result \
_behavior_impl__ ## klass ## __super__set_parameter (sooper* s, Evolution::TicketId ticket, \
                                                 const char* param, const char* value) \
{ \
    return (s->sooper::set_parameter (ticket, param, value)); \
} \
\
/* The parameter table. */ \
typedef Evolution::Result (klass::* BehaviorImpl__ ## klass ## __ParamSetMethod)(Evolution::TicketId ticket, const char*); \
typedef std::map<Evolution::String, BehaviorImpl__ ## klass ## __ParamSetMethod> BehaviorImpl__ ## klass ## __ParamTable; \
static BehaviorImpl__ ## klass ## __ParamTable _behavior_impl__ ## klass ## __param_table; \
\
/* Parameter initializer function. */ \
static void \
_behavior_impl__ ## klass ## __param_initialize () \
{ \
    /* Indicates if the class parameter table is initialized. */ \
    static bool _behavior_impl__params_initialized = false; \
    if (_behavior_impl__params_initialized) \
    { \
        return; \
    } \
    _behavior_impl__params_initialized = true;


/**
 * @brief
 * Defines a behavior parameter and its set accessor method.
 *
 * Zero or more instances of BEHAVIOR_PARAM must follow immediately
 * after BEGIN_BEHAVIOR_PARAMS. See BEGIN_BEHAVIOR_PARAMS for an
 * example.
 *
 * @see  DECLARE_BEHAVIOR_PARAMS, BEGIN_BEHAVIOR_PARAMS, END_BEHAVIOR_PARAMS
 **/
#define BEHAVIOR_PARAM(klass,param,accessor) \
    _behavior_impl__ ## klass ## __param_table[param] = &klass::set_ ## accessor;


/**
 * @brief
 * Ends a list of behavior parameters and their set accessor methods.
 *
 * This macro must close a list of behavior parameters, following
 * immediately after the instances of BEHAVIOR_PARAM. See
 * BEGIN_BEHAVIOR_PARAMS for an example.
 *
 * @see  DECLARE_BEHAVIOR_PARAMS, BEGIN_BEHAVIOR_PARAMS, BEHAVIOR_PARAM
 **/
#define END_BEHAVIOR_PARAMS(klass) \
} /* end _behavior_impl_param_initialize() */ \
\
/** @see  BehaviorImpl::set_parameter() */ \
Evolution::Result klass::set_parameter (Evolution::TicketId ticket, const char* param, const char* value) \
{ \
    _behavior_impl__ ## klass ## __param_initialize (); \
    BehaviorImpl__ ## klass ## __ParamTable::iterator find_iter = \
        _behavior_impl__ ## klass ## __param_table.find (param); \
    if (find_iter == _behavior_impl__ ## klass ## __param_table.end ()) \
    { \
        return (_behavior_impl__ ## klass ## __super__set_parameter (this, ticket, param, value)); \
    } \
    return (this->*(find_iter->second))(ticket, value); \
} /* end set_parameter() */

#endif // INCLUDE_EVOLUTION_BEHAVIOR__BEHAVIOR_IMPL_HPP
