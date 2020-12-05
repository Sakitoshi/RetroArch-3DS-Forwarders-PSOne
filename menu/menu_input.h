/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2016 - Daniel De Matteis
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MENU_INPUT_H
#define _MENU_INPUT_H

#include "../input/input_driver.h"
#include "../input/input_keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif

enum menu_action
{
   MENU_ACTION_NOOP = 0,
   MENU_ACTION_UP,
   MENU_ACTION_DOWN,
   MENU_ACTION_LEFT,
   MENU_ACTION_RIGHT,
   MENU_ACTION_OK,
   MENU_ACTION_SEARCH,
   MENU_ACTION_SCAN,
   MENU_ACTION_CANCEL,
   MENU_ACTION_INFO,
   MENU_ACTION_SELECT,
   MENU_ACTION_START,
   MENU_ACTION_SCROLL_DOWN,
   MENU_ACTION_SCROLL_UP,
   MENU_ACTION_TOGGLE,
   MENU_ACTION_POINTER_MOVED,
   MENU_ACTION_POINTER_PRESSED
};

enum menu_input_pointer_state
{
   MENU_POINTER_X_AXIS = 0,
   MENU_POINTER_Y_AXIS,
   MENU_POINTER_DELTA_X_AXIS,
   MENU_POINTER_DELTA_Y_AXIS,
   MENU_POINTER_PRESSED
};

enum menu_input_mouse_state
{
   MENU_MOUSE_X_AXIS = 0,
   MENU_MOUSE_Y_AXIS,
   MENU_MOUSE_LEFT_BUTTON,
   MENU_MOUSE_RIGHT_BUTTON,
   MENU_MOUSE_WHEEL_UP,
   MENU_MOUSE_WHEEL_DOWN,
   MENU_MOUSE_HORIZ_WHEEL_UP,
   MENU_MOUSE_HORIZ_WHEEL_DOWN
};

enum menu_input_ctl_state
{
   MENU_INPUT_CTL_NONE = 0,
   MENU_INPUT_CTL_MOUSE_PTR,
   MENU_INPUT_CTL_POINTER_PTR,
   MENU_INPUT_CTL_POINTER_ACCEL_READ,
   MENU_INPUT_CTL_POINTER_ACCEL_WRITE,
   MENU_INPUT_CTL_IS_POINTER_DRAGGED,
   MENU_INPUT_CTL_SET_POINTER_DRAGGED,
   MENU_INPUT_CTL_UNSET_POINTER_DRAGGED,
   MENU_INPUT_CTL_KEYBOARD_DISPLAY,
   MENU_INPUT_CTL_SET_KEYBOARD_DISPLAY,
   MENU_INPUT_CTL_KEYBOARD_BUFF_PTR,
   MENU_INPUT_CTL_KEYBOARD_LABEL,
   MENU_INPUT_CTL_SET_KEYBOARD_LABEL,
   MENU_INPUT_CTL_UNSET_KEYBOARD_LABEL,
   MENU_INPUT_CTL_KEYBOARD_LABEL_SETTING,
   MENU_INPUT_CTL_SET_KEYBOARD_LABEL_SETTING,
   MENU_INPUT_CTL_UNSET_KEYBOARD_LABEL_SETTING,
   MENU_INPUT_CTL_SEARCH_START,
   MENU_INPUT_CTL_DEINIT,
   MENU_INPUT_CTL_CHECK_INSIDE_HITBOX,
   MENU_INPUT_CTL_BIND_NONE,
   MENU_INPUT_CTL_BIND_SINGLE,
   MENU_INPUT_CTL_BIND_ALL,
   MENU_INPUT_CTL_BIND_ITERATE,
   MENU_INPUT_CTL_BIND_SET_MIN_MAX,
   MENU_INPUT_CTL_START_LINE
};

typedef struct menu_input_ctx_hitbox
{
   int32_t x1;
   int32_t x2;
   int32_t y1;
   int32_t y2;
} menu_input_ctx_hitbox_t;

typedef struct menu_input_ctx_bind
{
   char *s;
   size_t len;
} menu_input_ctx_bind_t;

typedef struct menu_input_ctx_line
{
   const char *label;
   const char *label_setting;
   unsigned type;
   unsigned idx;
   input_keyboard_line_complete_t cb;
} menu_input_ctx_line_t;

typedef struct menu_input_ctx_bind_limits
{
   unsigned min;
   unsigned max;
} menu_input_ctx_bind_limits_t;

/* Keyboard input callbacks */
void menu_input_st_uint_cb  (void *userdata, const char *str);
void menu_input_st_hex_cb   (void *userdata, const char *str);
void menu_input_st_string_cb(void *userdata, const char *str);
void menu_input_st_cheat_cb (void *userdata, const char *str);

unsigned menu_input_frame_retropad(retro_input_t input, retro_input_t trigger_state);

void menu_input_post_iterate(int *ret, unsigned action);

int16_t menu_input_pointer_state(enum menu_input_pointer_state state);

int16_t menu_input_mouse_state(enum menu_input_mouse_state state);

bool menu_input_ctl(enum menu_input_ctl_state state, void *data);

#ifdef __cplusplus
}
#endif

#endif
