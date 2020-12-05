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

#include <string.h>

#include <file/file_path.h>
#include <string/stdstring.h>

#include "record_driver.h"

#include "../command_event.h"
#include "../general.h"
#include "../retroarch.h"
#include "../verbosity.h"
#include "../msg_hash.h"
#include "../string_list_special.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

static bool recording_enable;

static const record_driver_t *record_drivers[] = {
#ifdef HAVE_FFMPEG
   &ffemu_ffmpeg,
#endif
   &ffemu_null,
   NULL,
};

static const record_driver_t *recording_driver;
static void *recording_data;

/**
 * record_driver_find_ident:
 * @idx                : index of driver to get handle to.
 *
 * Returns: Human-readable identifier of record driver at index. Can be NULL
 * if nothing found.
 **/
const char *record_driver_find_ident(int idx)
{
   const record_driver_t *drv = record_drivers[idx];
   if (!drv)
      return NULL;
   return drv->ident;
}

/**
 * record_driver_find_handle:
 * @idx                : index of driver to get handle to.
 *
 * Returns: handle to record driver at index. Can be NULL
 * if nothing found.
 **/
const void *record_driver_find_handle(int idx)
{
   const void *drv = record_drivers[idx];
   if (!drv)
      return NULL;
   return drv;
}

/**
 * config_get_record_driver_options:
 *
 * Get an enumerated list of all record driver names, separated by '|'.
 *
 * Returns: string listing of all record driver names, separated by '|'.
 **/
const char* config_get_record_driver_options(void)
{
   return char_list_new_special(STRING_LIST_RECORD_DRIVERS, NULL);
}

void find_record_driver(void)
{
   int i;
   driver_ctx_info_t drv;
   settings_t *settings = config_get_ptr();

   drv.label = "record_driver";
   drv.s     = settings->record.driver;

   driver_ctl(RARCH_DRIVER_CTL_FIND_INDEX, &drv);

   i = drv.len;

   if (i >= 0)
      recording_driver = (const record_driver_t*)record_driver_find_handle(i);
   else
   {
      unsigned d;

      RARCH_ERR("Couldn't find any record driver named \"%s\"\n",
            settings->audio.driver);
      RARCH_LOG_OUTPUT("Available record drivers are:\n");
      for (d = 0; record_driver_find_handle(d); d++)
         RARCH_LOG_OUTPUT("\t%s\n", record_driver_find_ident(d));
      RARCH_WARN("Going to default to first record driver...\n");

      recording_driver = (const record_driver_t*)record_driver_find_handle(0);

      if (!recording_driver)
         retro_fail(1, "find_record_driver()");
   }
}

/**
 * ffemu_find_backend:
 * @ident                   : Identifier of driver to find.
 *
 * Finds a recording driver with the name @ident.
 *
 * Returns: recording driver handle if successful, otherwise
 * NULL.
 **/
const record_driver_t *ffemu_find_backend(const char *ident)
{
   unsigned i;

   for (i = 0; record_drivers[i]; i++)
   {
      if (string_is_equal(record_drivers[i]->ident, ident))
         return record_drivers[i];
   }

   return NULL;
}


/**
 * gfx_ctx_init_first:
 * @backend                 : Recording backend handle.
 * @data                    : Recording data handle.
 * @params                  : Recording info parameters.
 *
 * Finds first suitable recording context driver and initializes.
 *
 * Returns: true (1) if successful, otherwise false (0).
 **/
bool record_driver_init_first(const record_driver_t **backend, void **data,
      const struct ffemu_params *params)
{
   unsigned i;

   for (i = 0; record_drivers[i]; i++)
   {
      void *handle = record_drivers[i]->init(params);

      if (!handle)
         continue;

      *backend = record_drivers[i];
      *data = handle;
      return true;
   }

   return false;
}

void recording_dump_frame(const void *data, unsigned width,
      unsigned height, size_t pitch)
{
   struct ffemu_video_data ffemu_data = {0};
   global_t *global = global_get_ptr();

   if (!recording_data)
      return;

   ffemu_data.pitch   = pitch;
   ffemu_data.width   = width;
   ffemu_data.height  = height;
   ffemu_data.data    = data;

   if (video_driver_ctl(RARCH_DISPLAY_CTL_HAS_GPU_RECORD, NULL))
   {
      uint8_t *gpu_buf         = NULL;
      struct video_viewport vp = {0};

      video_driver_ctl(RARCH_DISPLAY_CTL_VIEWPORT_INFO, &vp);

      if (!vp.width || !vp.height)
      {
         RARCH_WARN("%s \n",
               msg_hash_to_str(MSG_VIEWPORT_SIZE_CALCULATION_FAILED));
         event_cmd_ctl(EVENT_CMD_GPU_RECORD_DEINIT, NULL);

         recording_dump_frame(data, width, height, pitch);
         return;
      }

      /* User has resized. We kinda have a problem now. */
      if (vp.width != global->record.gpu_width ||
            vp.height != global->record.gpu_height)
      {
         RARCH_WARN("%s\n", msg_hash_to_str(MSG_RECORDING_TERMINATED_DUE_TO_RESIZE));

         runloop_msg_queue_push(
               msg_hash_to_str(MSG_RECORDING_TERMINATED_DUE_TO_RESIZE),
               1, 180, true);
         event_cmd_ctl(EVENT_CMD_RECORD_DEINIT, NULL);
         return;
      }

      if (!video_driver_ctl(RARCH_DISPLAY_CTL_GPU_RECORD_GET, &gpu_buf))
         return;

      /* Big bottleneck.
       * Since we might need to do read-backs asynchronously,
       * it might take 3-4 times before this returns true. */
      if (!video_driver_ctl(RARCH_DISPLAY_CTL_READ_VIEWPORT, gpu_buf))
            return;

      ffemu_data.pitch  = global->record.gpu_width * 3;
      ffemu_data.width  = global->record.gpu_width;
      ffemu_data.height = global->record.gpu_height;
      ffemu_data.data   = gpu_buf + (ffemu_data.height - 1) * ffemu_data.pitch;

      ffemu_data.pitch  = -ffemu_data.pitch;
   }

   if (!video_driver_ctl(RARCH_DISPLAY_CTL_HAS_GPU_RECORD, NULL))
      ffemu_data.is_dupe = !data;

   if (recording_driver && recording_driver->push_video)
      recording_driver->push_video(recording_data, &ffemu_data);
}

bool recording_deinit(void)
{
   if (!recording_data || !recording_driver)
      return false;

   if (recording_driver->finalize)
      recording_driver->finalize(recording_data);

   if (recording_driver->free)
      recording_driver->free(recording_data);

   recording_data            = NULL;
   recording_driver          = NULL;

   event_cmd_ctl(EVENT_CMD_GPU_RECORD_DEINIT, NULL);

   return true;
}

bool *recording_is_enabled(void)
{
   return &recording_enable;
}

void recording_set_state(bool state)
{
   recording_enable = state;
}

void recording_push_audio(const int16_t *data, size_t samples)
{
   struct ffemu_audio_data ffemu_data;

   if (!recording_data)
      return;

   ffemu_data.data                    = data;
   ffemu_data.frames                  = samples / 2;

   if (recording_driver && recording_driver->push_audio)
      recording_driver->push_audio(recording_data, &ffemu_data);
}

/**
 * recording_init:
 *
 * Initializes recording.
 *
 * Returns: true (1) if successful, otherwise false (0).
 **/
bool recording_init(void)
{
   char recording_file[PATH_MAX_LENGTH] = {0};
   struct ffemu_params params           = {0};
   global_t *global                     = global_get_ptr();
   settings_t *settings                 = config_get_ptr();
   struct retro_system_av_info *av_info = video_viewport_get_system_av_info();
   bool *recording_enabled              = recording_is_enabled();

   if (!*recording_enabled)
      return false;

   if (rarch_ctl(RARCH_CTL_IS_DUMMY_CORE, NULL))
   {
      RARCH_WARN("%s\n",
            msg_hash_to_str(MSG_USING_LIBRETRO_DUMMY_CORE_RECORDING_SKIPPED));
      return false;
   }

   if (!settings->video.gpu_record 
         && video_driver_ctl(RARCH_DISPLAY_CTL_IS_HW_CONTEXT, NULL))
   {
      RARCH_WARN("%s.\n",
            msg_hash_to_str(MSG_HW_RENDERED_MUST_USE_POSTSHADED_RECORDING));
      return false;
   }

   RARCH_LOG("%s: FPS: %.4f, Sample rate: %.4f\n",
         msg_hash_to_str(MSG_CUSTOM_TIMING_GIVEN),
         (float)av_info->timing.fps,
         (float)av_info->timing.sample_rate);

   strlcpy(recording_file, global->record.path, sizeof(recording_file));

   if (global->record.use_output_dir)
      fill_pathname_join(recording_file,
            global->record.output_dir,
            global->record.path, sizeof(recording_file));

   params.out_width  = av_info->geometry.base_width;
   params.out_height = av_info->geometry.base_height;
   params.fb_width   = av_info->geometry.max_width;
   params.fb_height  = av_info->geometry.max_height;
   params.channels   = 2;
   params.filename   = recording_file;
   params.fps        = av_info->timing.fps;
   params.samplerate = av_info->timing.sample_rate;
   params.pix_fmt    = (video_driver_get_pixel_format() == RETRO_PIXEL_FORMAT_XRGB8888) ?
      FFEMU_PIX_ARGB8888 : FFEMU_PIX_RGB565;
   params.config     = NULL;
   
   if (*global->record.config)
      params.config = global->record.config;

   if (video_driver_ctl(RARCH_DISPLAY_CTL_SUPPORTS_RECORDING, NULL))
   {
      unsigned gpu_size;
      struct video_viewport vp = {0};

      video_driver_ctl(RARCH_DISPLAY_CTL_VIEWPORT_INFO, &vp);

      if (!vp.width || !vp.height)
      {
         RARCH_ERR("Failed to get viewport information from video driver. "
               "Cannot start recording ...\n");
         return false;
      }

      params.out_width  = vp.width;
      params.out_height = vp.height;
      params.fb_width   = next_pow2(vp.width);
      params.fb_height  = next_pow2(vp.height);

      if (settings->video.force_aspect &&
            (video_driver_get_aspect_ratio() > 0.0f))
         params.aspect_ratio  = video_driver_get_aspect_ratio();
      else
         params.aspect_ratio  = (float)vp.width / vp.height;

      params.pix_fmt             = FFEMU_PIX_BGR24;
      global->record.gpu_width   = vp.width;
      global->record.gpu_height  = vp.height;

      RARCH_LOG("%s %u x %u\n", msg_hash_to_str(MSG_DETECTED_VIEWPORT_OF),
            vp.width, vp.height);

      gpu_size = vp.width * vp.height * 3;
      if (!video_driver_ctl(RARCH_DISPLAY_CTL_GPU_RECORD_INIT, &gpu_size))
         return false;
   }
   else
   {
      if (global->record.width || global->record.height)
      {
         params.out_width  = global->record.width;
         params.out_height = global->record.height;
      }

      if (settings->video.force_aspect &&
            (video_driver_get_aspect_ratio() > 0.0f))
         params.aspect_ratio = video_driver_get_aspect_ratio();
      else
         params.aspect_ratio = (float)params.out_width / params.out_height;

      if (settings->video.post_filter_record 
            && video_driver_ctl(RARCH_DISPLAY_CTL_FRAME_FILTER_ALIVE, NULL))
      {
         unsigned max_width  = 0;
         unsigned max_height = 0;
         
         params.pix_fmt    = FFEMU_PIX_RGB565;

         if (video_driver_ctl(RARCH_DISPLAY_CTL_FRAME_FILTER_IS_32BIT, NULL))
            params.pix_fmt = FFEMU_PIX_ARGB8888;

         rarch_softfilter_get_max_output_size(
               video_driver_frame_filter_get_ptr(),
               &max_width, &max_height);
         params.fb_width  = next_pow2(max_width);
         params.fb_height = next_pow2(max_height);
      }
   }

   RARCH_LOG("%s %s @ %ux%u. (FB size: %ux%u pix_fmt: %u)\n",
         msg_hash_to_str(MSG_RECORDING_TO),
         global->record.path,
         params.out_width, params.out_height,
         params.fb_width, params.fb_height,
         (unsigned)params.pix_fmt);

   if (!record_driver_init_first(&recording_driver, &recording_data, &params))
   {
      RARCH_ERR("%s\n", msg_hash_to_str(MSG_FAILED_TO_START_RECORDING));
      event_cmd_ctl(EVENT_CMD_GPU_RECORD_DEINIT, NULL);

      return false;
   }

   return true;
}

void *recording_driver_get_data_ptr(void)
{
   return recording_data;
}

void recording_driver_clear_data_ptr(void)
{
   recording_data = NULL;
}

void recording_driver_set_data_ptr(void *data)
{
   recording_data = data;
}
