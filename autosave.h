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

#ifndef __RARCH_AUTOSAVE_H
#define __RARCH_AUTOSAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct autosave autosave_t;

/**
 * autosave_new:
 * @path            : path to autosave file
 * @data            : pointer to buffer
 * @size            : size of @data buffer
 * @interval        : interval at which saves should be performed.
 *
 * Create and initialize autosave object.
 *
 * Returns: pointer to new autosave_t object if successful, otherwise
 * NULL.
 **/
autosave_t *autosave_new(const char *path, const void *data,
      size_t size, unsigned interval);

/**
 * autosave_free:
 * @handle          : pointer to autosave object
 *
 * Frees autosave object.
 **/
void autosave_free(autosave_t *handle);

/**
 * lock_autosave:
 *
 * Lock autosave.
 **/
void lock_autosave(void);

/**
 * unlock_autosave:
 *
 * Unlocks autosave.
 **/
void unlock_autosave(void);

void autosave_event_init(void);

void autosave_event_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
