/* RetroArch - A frontend for libretro.
 * Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 * Copyright (C) 2011-2016 - Daniel De Matteis
 * Copyright (C) 2012-2015 - Michael Lelli
 *
 * RetroArch is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Found-
 * ation, either version 3 of the License, or (at your option) any later version.
 *
 * RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RetroArch.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <file/file_path.h>
#include <retro_stat.h>

#ifdef HAVE_THREADS
#include <rthreads/async_job.h>
#endif

#include "frontend.h"
#include "../ui/ui_companion_driver.h"

#include "../defaults.h"
#include "../content.h"
#include "../driver.h"
#include "../system.h"
#include "../driver.h"
#include "../retroarch.h"
#include "../runloop.h"
#include "../verbosity.h"

#ifdef HAVE_MENU
#include "../menu/menu_driver.h"
#endif

#ifdef HAVE_THREADS
static async_job_t *async_jobs;

int rarch_main_async_job_add(async_task_t task, void *payload)
{
   return async_job_add(async_jobs, task, payload);
}
#endif

/**
 * main_exit:
 *
 * Cleanly exit RetroArch.
 *
 * Also saves configuration files to disk,
 * and (optionally) autosave state.
 **/
void main_exit(void *args)
{
   settings_t *settings                  = config_get_ptr();

   event_cmd_ctl(EVENT_CMD_MENU_SAVE_CURRENT_CONFIG, NULL);

#ifdef HAVE_MENU
   /* Do not want menu context to live any more. */
   menu_driver_ctl(RARCH_MENU_CTL_UNSET_OWN_DRIVER, NULL);
#endif
   rarch_ctl(RARCH_CTL_MAIN_DEINIT, NULL);

   event_cmd_ctl(EVENT_CMD_PERFCNT_REPORT_FRONTEND_LOG, NULL);

#if defined(HAVE_LOGGER) && !defined(ANDROID)
   logger_shutdown();
#endif

   frontend_driver_deinit(args);
   frontend_driver_exitspawn(settings->libretro,
         sizeof(settings->libretro));

   rarch_ctl(RARCH_CTL_DESTROY, NULL);

   ui_companion_driver_deinit();

   frontend_driver_shutdown(false);

   driver_ctl(RARCH_DRIVER_CTL_DEINIT, NULL);
   ui_companion_driver_free();
   frontend_driver_free();
}

/**
 * main_entry:
 *
 * Main function of RetroArch.
 *
 * If HAVE_MAIN is not defined, will contain main loop and will not
 * be exited from until we exit the program. Otherwise, will
 * just do initialization.
 *
 * Returns: varies per platform.
 **/
int rarch_main(int argc, char *argv[], void *data)
{
   char *fullpath                  = NULL;
   rarch_system_info_t *system     = NULL;
   void *args                      = (void*)data;
#ifndef HAVE_MAIN
   int ret                         = 0;
#endif

   rarch_ctl(RARCH_CTL_PREINIT, NULL);

   frontend_driver_init_first(args);
   rarch_ctl(RARCH_CTL_INIT, NULL);

#ifdef HAVE_THREADS
   async_jobs = async_job_new();
#endif
   
   if (frontend_driver_is_inited())
   {
      content_ctx_info_t info;

      info.argc            = argc;
      info.argv            = argv;
      info.args            = args;
      info.environ_get     = frontend_driver_environment_get_ptr();

      if (!content_ctl(CONTENT_CTL_LOAD, &info))
         return 0;
   }

   runloop_ctl(RUNLOOP_CTL_SYSTEM_INFO_GET,  &system);
   runloop_ctl(RUNLOOP_CTL_GET_CONTENT_PATH, &fullpath);

   if (content_ctl(CONTENT_CTL_IS_INITED, NULL) || content_ctl(CONTENT_CTL_DOES_NOT_NEED_CONTENT, NULL))
   {
      char tmp[PATH_MAX_LENGTH];
      struct retro_system_info *info = system ? &system->info : NULL;

      strlcpy(tmp, fullpath, sizeof(tmp));

      if (*tmp)
      {
         /* Path can be relative here.
          * Ensure we're pushing absolute path. */
         path_resolve_realpath(tmp, sizeof(tmp));
      }

      if (rarch_ctl(RARCH_CTL_IS_DUMMY_CORE, NULL) || !info)
         content_push_to_history_playlist(
               content_ctl(CONTENT_CTL_DOES_NOT_NEED_CONTENT, NULL) || *tmp,
               *tmp ? tmp : NULL,
               info);
   }

   ui_companion_driver_init_first();

#ifndef HAVE_MAIN
   do
   {
      unsigned sleep_ms = 0;
      ret = runloop_iterate(&sleep_ms);

      if (ret == 1 && sleep_ms > 0)
         retro_sleep(sleep_ms);
      runloop_ctl(RUNLOOP_CTL_DATA_ITERATE, NULL);
   }while(ret != -1);

   main_exit(args);
#endif

#ifdef HAVE_THREADS
   async_job_free(async_jobs);
   async_jobs = NULL;
#endif

   return 0;
}

#ifndef HAVE_MAIN
int main(int argc, char *argv[])
{
   return rarch_main(argc, argv, NULL);
}
#endif
