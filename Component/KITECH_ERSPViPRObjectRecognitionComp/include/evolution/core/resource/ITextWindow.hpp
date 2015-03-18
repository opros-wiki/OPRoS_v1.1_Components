//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_ITEXTWINDOW_HPP
#define INCLUDE_EVOLUTION_CORE_ITEXTWINDOW_HPP


/**
 * @file    evolution/core/resource/ITextWindow.hpp
 * @brief   This file defines a class to provide an interface to a battery.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ITextWindow 
 */


#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @interface ITextWindow 
     *
     * @brief This class provides a resource interface to a battery.
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ITextWindow 
    {
    public: // Constants

        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.ITextWindow"

    public: // Structors

        /**
         * @brief Empty constructor
         */
        ITextWindow () {}

        /**
         * @brief Empty destructor
         */
        virtual ~ITextWindow () {}


    public: // Operation
    
        /**
         * @brief Print formatted text to a window.
         *
         * This method prints formatted text to a window in
         * the same way that the printf family of method prints
         * formatted text to a console.  Each subsequent
         * call to printf prints to the next line down.
         * 
         * @param format [in]; The text formatting string.
         *
         * @return The number of characters printed.
         */
        virtual int printf(const char* format, ...) = 0;

        /**
         * @brief Print formatted text to a window.
         *
         * This method prints formatted text to a window in
         * the same way that the printf family of method prints
         * formatted text to a console.  It prints the text
         * at an x,y position in the window, where x,y are in
         * character units.
         * 
         * @param x [in]; The x position of where the text will be printed.
         * @param y [in]; The y position of where the text will be printed.
         * @param format [in]; The text formatting string.
         *
         * @return The number of characters printed.
         */
        virtual int printf(int x, int y, const char* format, ...) = 0;

        /**
         * @brief Clears the text window.
         */
        virtual void clear() = 0;
            
    }; // end class ITextWindow

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_ITEXTWINDOW_HPP
