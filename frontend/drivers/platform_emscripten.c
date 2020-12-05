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

#include <emscripten/emscripten.h>

#include <file/config_file.h>

#include "../../general.h"
#include "../../content.h"
#include "../frontend.h"
#include "../../retroarch.h"
#include "../../runloop.h"
#include "../frontend_driver.h"
#include "../../command_event.h"

static void emscripten_mainloop(void)
{
   unsigned sleep_ms = 0;
   int ret = runloop_iterate(&sleep_ms);
   if (ret == 1 && sleep_ms > 0)
      retro_sleep(sleep_ms);
   runloop_ctl(RUNLOOP_CTL_DATA_ITERATE, NULL);
   if (ret != -1)
      return;

   main_exit(NULL);
   exit(0);
}

void cmd_savefiles(void)
{
   event_cmd_ctl(EVENT_CMD_SAVEFILES, NULL);
}

void cmd_save_state(void)
{
   event_cmd_ctl(EVENT_CMD_SAVE_STATE, NULL);
}

void cmd_load_state(void)
{
   event_cmd_ctl(EVENT_CMD_LOAD_STATE, NULL);
}

void cmd_take_screenshot(void)
{
   event_cmd_ctl(EVENT_CMD_TAKE_SCREENSHOT, NULL);
}


int main(int argc, char *argv[])
{
   settings_t *settings = config_get_ptr();

   emscripten_set_canvas_size(800, 600);
   rarch_main(argc, argv, NULL);
   emscripten_set_main_loop(emscripten_mainloop,
         settings->video.vsync ? 0 : INT_MAX, 1);

   return 0;
}
