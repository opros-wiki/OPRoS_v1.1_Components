//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file    evolution/core/util/StringTerminatedProtocol.hpp
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::StringTerminatedProtocol
 */

#ifndef EVOLUTION_LIBRARIES_BASE_STRING_TERMINATED_PROTOCOL__HPP
#define EVOLUTION_LIBRARIES_BASE_STRING_TERMINATED_PROTOCOL__HPP

#include <list>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/util/ISocketProtocol.hpp>

namespace Evolution
{
    /**
     * @class StringTerminatedProtocol
     * @brief
     * This class implements a protocol where a specific string is used
     * to indicate the end of a data packet. 
     *
     * MalleableBehavior uses this protocol, with just the newline
     * character '\n' as the terminating string.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL StringTerminatedProtocol : public ISocketProtocol
    {
    public:
        /**
         * @brief Construct a StringTerminatedProtocol
         *
         * @param   str [in]; the NUL-terminated string which is searched for
         * @param   chomp [in]; whether to delete that string from the 
         *          'decoded' elements
         * @param   max_len [in]; the maximum number of characters 
         *          for a line of text, or 0 to indicate that the protocol 
         *          parser should exhaust the heap before giving up.
         *          truncated strings are discarded(!)
         */                      
        StringTerminatedProtocol(const char *str, bool chomp = true, size_t max_len = 0);
        
        /**
         * @brief Construct a StringTerminatedProtocol
         *
         * @param   str [in]; the binary data which is searched for
         * @param   str_len [in]; the length of the binary data
         * @param   chomp [in]; whether to delete that string from the 
         *          'decoded' elements
         * @param   max_len [in]; the maximum number of characters for a 
         *          line of text.
         */                      
        StringTerminatedProtocol(const char *str, size_t str_len, bool chomp = true, size_t max_len = 0);
        
    public: //ISocketProtocol Interface
        virtual ProtocolResult  encode(const char* buffer, size_t buffer_size);
        virtual ProtocolResult  decode(const char* buffer, size_t buffer_size);

    protected:
        char *_str;         ///< The terminating string. 
        size_t _str_len;    ///< Size of the terminating string.
        bool _chomp;        ///< True if the terminating string should be removed from the decoded data.
        size_t _max_len;    ///< Maximum characters in the data string.
    };
}

#endif
