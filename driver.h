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

#ifndef __RARCH_DRIVER__H
#define __RARCH_DRIVER__H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include <boolean.h>
#include <compat/posix_string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define AUDIO_CHUNK_SIZE_BLOCKING      512

/* So we don't get complete line-noise when fast-forwarding audio. */
#define AUDIO_CHUNK_SIZE_NONBLOCKING   2048

#define AUDIO_MAX_RATIO                16

/* Specialized _MOUSE that targets the full screen regardless of viewport.
 */
#define RARCH_DEVICE_MOUSE_SCREEN      (RETRO_DEVICE_MOUSE | 0x10000)

/* Specialized _POINTER that targets the full screen regardless of viewport.
 * Should not be used by a libretro implementation as coordinates returned
 * make no sense.
 *
 * It is only used internally for overlays. */
#define RARCH_DEVICE_POINTER_SCREEN    (RETRO_DEVICE_POINTER | 0x10000)

#define RARCH_DEVICE_ID_POINTER_BACK   (RETRO_DEVICE_ID_POINTER_PRESSED | 0x10000)

/* libretro has 16 buttons from 0-15 (libretro.h)
 * Analog binds use RETRO_DEVICE_ANALOG, but we follow the same scheme
 * internally in RetroArch for simplicity, so they are mapped into [16, 23].
 */
#define RARCH_FIRST_CUSTOM_BIND        16
#define RARCH_FIRST_META_KEY           RARCH_CUSTOM_BIND_LIST_END

#define AXIS_NEG(x)        (((uint32_t)(x) << 16) | UINT16_C(0xFFFF))
#define AXIS_POS(x)        ((uint32_t)(x) | UINT32_C(0xFFFF0000))
#define AXIS_NONE          UINT32_C(0xFFFFFFFF)
#define AXIS_DIR_NONE      UINT16_C(0xFFFF)

#define AXIS_NEG_GET(x)    (((uint32_t)(x) >> 16) & UINT16_C(0xFFFF))
#define AXIS_POS_GET(x)    ((uint32_t)(x) & UINT16_C(0xFFFF))

#define NO_BTN             UINT16_C(0xFFFF)

#define HAT_UP_SHIFT       15
#define HAT_DOWN_SHIFT     14
#define HAT_LEFT_SHIFT     13
#define HAT_RIGHT_SHIFT    12
#define HAT_UP_MASK        (1 << HAT_UP_SHIFT)
#define HAT_DOWN_MASK      (1 << HAT_DOWN_SHIFT)
#define HAT_LEFT_MASK      (1 << HAT_LEFT_SHIFT)
#define HAT_RIGHT_MASK     (1 << HAT_RIGHT_SHIFT)
#define HAT_MAP(x, hat)    ((x & ((1 << 12) - 1)) | hat)

#define HAT_MASK           (HAT_UP_MASK | HAT_DOWN_MASK | HAT_LEFT_MASK | HAT_RIGHT_MASK)
#define GET_HAT_DIR(x)     (x & HAT_MASK)
#define GET_HAT(x)         (x & (~HAT_MASK))

#define DRIVERS_CMD_ALL \
      ( DRIVER_AUDIO \
      | DRIVER_VIDEO \
      | DRIVER_INPUT \
      | DRIVER_CAMERA \
      | DRIVER_LOCATION \
      | DRIVER_MENU \
      | DRIVERS_VIDEO_INPUT )

/* RetroArch specific bind IDs. */
enum
{
   /* Custom binds that extend the scope of RETRO_DEVICE_JOYPAD for
    * RetroArch specifically.
    * Analogs (RETRO_DEVICE_ANALOG) */
   RARCH_ANALOG_LEFT_X_PLUS = RARCH_FIRST_CUSTOM_BIND,
   RARCH_ANALOG_LEFT_X_MINUS,
   RARCH_ANALOG_LEFT_Y_PLUS,
   RARCH_ANALOG_LEFT_Y_MINUS,
   RARCH_ANALOG_RIGHT_X_PLUS,
   RARCH_ANALOG_RIGHT_X_MINUS,
   RARCH_ANALOG_RIGHT_Y_PLUS,
   RARCH_ANALOG_RIGHT_Y_MINUS,

   /* Turbo */
   RARCH_TURBO_ENABLE,

   RARCH_CUSTOM_BIND_LIST_END,

   /* Command binds. Not related to game input,
    * only usable for port 0. */
   RARCH_FAST_FORWARD_KEY = RARCH_FIRST_META_KEY,
   RARCH_FAST_FORWARD_HOLD_KEY,
   RARCH_LOAD_STATE_KEY,
   RARCH_SAVE_STATE_KEY,
   RARCH_FULLSCREEN_TOGGLE_KEY,
   RARCH_QUIT_KEY,
   RARCH_STATE_SLOT_PLUS,
   RARCH_STATE_SLOT_MINUS,
   RARCH_REWIND,
   RARCH_MOVIE_RECORD_TOGGLE,
   RARCH_PAUSE_TOGGLE,
   RARCH_FRAMEADVANCE,
   RARCH_RESET,
   RARCH_SHADER_NEXT,
   RARCH_SHADER_PREV,
   RARCH_CHEAT_INDEX_PLUS,
   RARCH_CHEAT_INDEX_MINUS,
   RARCH_CHEAT_TOGGLE,
   RARCH_SCREENSHOT,
   RARCH_MUTE,
   RARCH_OSK,
   RARCH_NETPLAY_FLIP,
   RARCH_SLOWMOTION,
   RARCH_ENABLE_HOTKEY,
   RARCH_VOLUME_UP,
   RARCH_VOLUME_DOWN,
   RARCH_OVERLAY_NEXT,
   RARCH_DISK_EJECT_TOGGLE,
   RARCH_DISK_NEXT,
   RARCH_DISK_PREV,
   RARCH_GRAB_MOUSE_TOGGLE,

   RARCH_MENU_TOGGLE,

   RARCH_BIND_LIST_END,
   RARCH_BIND_LIST_END_NULL
};

enum analog_dpad_mode
{
   ANALOG_DPAD_NONE = 0,
   ANALOG_DPAD_LSTICK,
   ANALOG_DPAD_RSTICK,
   ANALOG_DPAD_LAST
};

enum
{
   DRIVER_AUDIO        = 1 << 0,
   DRIVER_VIDEO        = 1 << 1,
   DRIVER_INPUT        = 1 << 2,
   DRIVER_CAMERA       = 1 << 3,
   DRIVER_LOCATION     = 1 << 4,
   DRIVER_MENU         = 1 << 5,
   DRIVERS_VIDEO_INPUT = 1 << 6
};

enum driver_ctl_state
{
   RARCH_DRIVER_CTL_NONE = 0,
   RARCH_DRIVER_CTL_DEINIT,
   /* Deinitializes drivers.
    * @data is a bitmask which determines 
    * which drivers get deinitialized. */
   RARCH_DRIVER_CTL_UNINIT,
   /* Initializes drivers.
    * @data is a bitmask which determines 
    * which drivers get initialized. */
   RARCH_DRIVER_CTL_INIT,
   /* Attempts to find a default driver for 
    * all driver types.
    *
    * Should be run before RARCH_DRIVER_CTL_INIT.
    */
   RARCH_DRIVER_CTL_INIT_PRE,
   /* Sets monitor refresh rate to new value by calling
    * video_monitor_set_refresh_rate(). Subsequently
    * calls audio_monitor_set_refresh_rate(). */
   RARCH_DRIVER_CTL_SET_REFRESH_RATE,
   /* Sets audio and video drivers to nonblock state.
    *
    * If nonblock state is false, sets blocking state for both
    * audio and video drivers instead. */
   RARCH_DRIVER_CTL_SET_NONBLOCK_STATE,
   /* Update the system Audio/Video information. 
    * Will reinitialize audio/video drivers.
    * Used by RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO. */
   RARCH_DRIVER_CTL_UPDATE_SYSTEM_AV_INFO,

   RARCH_DRIVER_CTL_FIND_FIRST,

   RARCH_DRIVER_CTL_FIND_PREV,

   RARCH_DRIVER_CTL_FIND_NEXT,

   /* Find index of the driver, based on @label. */
   RARCH_DRIVER_CTL_FIND_INDEX

};

typedef struct driver_ctx_info
{
   const char *label;
   char *s;
   ssize_t len;
} driver_ctx_info_t;

bool driver_ctl(enum driver_ctl_state state, void *data);

#ifdef __cplusplus
}
#endif

#endif
