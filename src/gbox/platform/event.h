/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        event.h
 * @ingroup     platform
 *
 */
#ifndef GB_PLATFORM_EVENT_H
#define GB_PLATFORM_EVENT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "touch.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the event type enum
typedef enum __gb_event_type_e
{
    GB_EVENT_TYPE_NONE      = 0     //!< none
,   GB_EVENT_TYPE_TOUCH     = 1     //!< touch event
,   GB_EVENT_TYPE_MOUSE     = 2     //!< mouse event
,   GB_EVENT_TYPE_KEYBOARD  = 3     //!< keyboard event
,   GB_EVENT_TYPE_USER      = 4     //!< user defined event

}gb_event_type_e;

/// the keycode enum
typedef enum __gb_key_e
{
    GB_KEY_NUL              = 0x00
,   GB_KEY_BACKSPACE        = 0x08
,   GB_KEY_TAB              = 0x09
,   GB_KEY_TAB_BACK         = 0x19
,   GB_KEY_RETURN           = 0x0d
,   GB_KEY_ESCAPE           = 0x1b
,   GB_KEY_SPACE            = 0x20
,   GB_KEY_DELETE           = 0x7f

,   GB_KEY_F1               = 0x0100
,   GB_KEY_F2
,   GB_KEY_F3
,   GB_KEY_F4
,   GB_KEY_F5
,   GB_KEY_F6  
,   GB_KEY_F7 
,   GB_KEY_F8
,   GB_KEY_F9
,   GB_KEY_F10
,   GB_KEY_F11
,   GB_KEY_F12 

,   GB_KEY_LEFT   
,   GB_KEY_UP         
,   GB_KEY_RIGHT    
,   GB_KEY_DOWN   

,   GB_KEY_HOME
,   GB_KEY_END
,   GB_KEY_INSERT
,   GB_KEY_PAGE_UP  
,   GB_KEY_PAGE_DOWN 

,   GB_KEY_PRINT 
,   GB_KEY_SYSREQ
,   GB_KEY_SCROLL_LOCK
,   GB_KEY_PAUSE
,   GB_KEY_BREAK

,   GB_KEY_CAPS_LOCK
,   GB_KEY_SHIFT
,   GB_KEY_CTRL
,   GB_KEY_ALT

,   GB_KEY_MENU
,   GB_KEY_PLAY
,   GB_KEY_SEARCH
,   GB_KEY_BACK             = GB_KEY_ESCAPE
,   GB_KEY_ENTER            = GB_KEY_RETURN

}gb_key_e;

/// the keyboard event type 
typedef struct __gb_event_keyboard_t
{
    /*! the key code
     *
     * char:    [0, 0xff]
     * special: [0x0100, 0xffff]
     */
    tb_uint16_t             code;

    /// is pressed?
    tb_uint8_t              pressed;

}gb_event_keyboard_t;

/// the mouse code enum
typedef enum __gb_mouse_code_e
{
    GB_MOUSE_NONE           = 0     //!< none
,   GB_MOUSE_DOWN           = 1     //!< mouse down
,   GB_MOUSE_UP             = 2     //!< mouse up
,   GB_MOUSE_MOVE           = 3     //!< mouse move
,   GB_MOUSE_SCROLL         = 4     //!< mouse scroll

}gb_mouse_code_e;

/// the mouse button enum
typedef enum __gb_mouse_button_e
{
    GB_MOUSE_BUTTON_NONE    = 0     //!< none
,   GB_MOUSE_BUTTON_LEFT    = 1     //!< left button
,   GB_MOUSE_BUTTON_RIGHT   = 2     //!< right button
,   GB_MOUSE_BUTTON_MIDDLE  = 3     //!< middle button

}gb_mouse_button_e;

/// the mouse event type 
typedef struct __gb_event_mouse_t
{
    /// the event code
    tb_uint8_t              code;

    /// the mouse button
    tb_uint8_t              button;

    /// the cursor point
    gb_point_t              cursor;

    /// the scroll point
    gb_point_t              scroll;

}gb_event_mouse_t;

/// the touch code enum
typedef enum __gb_touch_code_e
{
    GB_TOUCH_NONE           = 0     //!< none
,   GB_TOUCH_BEGAN          = 1     //!< touch began
,   GB_TOUCH_MOVED          = 2     //!< touch moved
,   GB_TOUCH_ENDED          = 3     //!< touch ended
,   GB_TOUCH_CANCELED       = 4     //!< touch canceled

}gb_touch_code_e;

/// the touch event type 
typedef struct __gb_event_touch_t
{
    /// the event code
    tb_uint8_t              code;

    /// the touches count
    tb_uint8_t              count;

    /// the touches
    gb_touch_t*             touches;

}gb_event_touch_t;

/// the event type 
typedef struct __gb_event_t
{
    /// the type
    tb_uint8_t              type;

    /// the event
    union
    {
        /// the touch event
        gb_event_touch_t    touch;
    
        /// the mouse event
        gb_event_mouse_t    mouse;

        /// the keyboard event
        gb_event_keyboard_t keyboard;
    
    }u;

    /// the target
    tb_cpointer_t           target;

}gb_event_t;

/// the event ref type
typedef gb_event_t*         gb_event_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
__tb_extern_c_enter__

#ifdef __gb_debug__
/*! dump event 
 *
 * @param event     the event
 */
tb_void_t           gb_event_dump(gb_event_ref_t event);
#endif

__tb_extern_c_leave__
#endif
