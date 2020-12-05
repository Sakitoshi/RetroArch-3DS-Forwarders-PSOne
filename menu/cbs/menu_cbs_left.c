/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2015 - Daniel De Matteis
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

#include <file/file_path.h>
#include <string/stdstring.h>
#include <lists/string_list.h>

#include "../menu_driver.h"
#include "../menu_cbs.h"
#include "../menu_hash.h"
#include "../menu_input.h"
#include "../menu_setting.h"
#include "../menu_shader.h"
#include "../menu_navigation.h"

#include "../../core_info.h"
#include "../../cheats.h"
#include "../../general.h"
#include "../../retroarch.h"
#include "../../system.h"

#ifndef BIND_ACTION_LEFT
#define BIND_ACTION_LEFT(cbs, name) \
   cbs->action_left = name; \
   cbs->action_left_ident = #name;
#endif

#ifdef HAVE_SHADER_MANAGER
static int generic_shader_action_parameter_left(
      struct video_shader *shader, struct video_shader_parameter *param,
      unsigned type, const char *label, bool wraparound)
{
   if (shader)
   {
      param->current -= param->step;
      param->current  = MIN(MAX(param->minimum, param->current), param->maximum);
   }
   return 0;
}

static int shader_action_parameter_left(unsigned type, const char *label,
      bool wraparound)
{
   video_shader_ctx_t shader_info;
   struct video_shader_parameter *param = NULL;

   video_shader_driver_ctl(SHADER_CTL_GET_CURRENT_SHADER, &shader_info);

   param = &shader_info.data->parameters[type - MENU_SETTINGS_SHADER_PARAMETER_0];
   return generic_shader_action_parameter_left(shader_info.data, param, type, label, wraparound);
}

static int shader_action_parameter_preset_left(unsigned type, const char *label,
      bool wraparound)
{
   struct video_shader_parameter *param = NULL;
   struct video_shader      *shader     = NULL;

   menu_driver_ctl(RARCH_MENU_CTL_SHADER_GET,
         &shader);

   param = shader ? 
      &shader->parameters[type - MENU_SETTINGS_SHADER_PRESET_PARAMETER_0] : 
      NULL;
   return generic_shader_action_parameter_left(shader, param, type, label, wraparound);
}
#endif

static int action_left_cheat(unsigned type, const char *label,
      bool wraparound)
{
   size_t idx             = type - MENU_SETTINGS_CHEAT_BEGIN;
   return generic_action_cheat_toggle(idx, type, label,
         wraparound);
}

static int action_left_input_desc(unsigned type, const char *label,
      bool wraparound)
{
   unsigned inp_desc_index_offset = type - MENU_SETTINGS_INPUT_DESC_BEGIN;
   unsigned inp_desc_user         = inp_desc_index_offset / (RARCH_FIRST_CUSTOM_BIND + 4);
   unsigned inp_desc_button_index_offset = inp_desc_index_offset - (inp_desc_user * (RARCH_FIRST_CUSTOM_BIND + 4));
   settings_t *settings = config_get_ptr();

   if (settings->input.remap_ids[inp_desc_user][inp_desc_button_index_offset] > 0)
      settings->input.remap_ids[inp_desc_user][inp_desc_button_index_offset]--;

   return 0;
}

static int action_left_scroll(unsigned type, const char *label,
      bool wraparound)
{
   size_t selection;
   size_t scroll_accel   = 0;
   unsigned scroll_speed = 0, fast_scroll_speed = 0;
   if (!menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SELECTION, &selection))
      return menu_cbs_exit();
   if (!menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SCROLL_ACCEL, &scroll_accel))
      return false;

   scroll_speed          = (MAX(scroll_accel, 2) - 2) / 4 + 1;
   fast_scroll_speed     = 4 + 4 * scroll_speed;

   if (selection > fast_scroll_speed)
   {
      size_t idx  = selection - fast_scroll_speed;
      bool scroll = true;
      menu_navigation_ctl(MENU_NAVIGATION_CTL_SET_SELECTION, &idx);
      menu_navigation_ctl(MENU_NAVIGATION_CTL_SET, &scroll);
   }
   else
   {
      bool pending_push = false;
      menu_navigation_ctl(MENU_NAVIGATION_CTL_CLEAR, &pending_push);
   }

   return 0;
}

static int action_left_mainmenu(unsigned type, const char *label,
      bool wraparound)
{
   menu_ctx_list_t list_info;
   size_t selection          = 0;
   menu_file_list_cbs_t *cbs = NULL;
   unsigned        push_list = 0;
   file_list_t *selection_buf = menu_entries_get_selection_buf_ptr(0);
   file_list_t *menu_stack    = menu_entries_get_menu_stack_ptr(0);
   settings_t       *settings = config_get_ptr();
   menu_handle_t       *menu  = NULL;
   unsigned           action  = MENU_ACTION_LEFT;
   
   if (!menu_driver_ctl(RARCH_MENU_CTL_DRIVER_DATA_GET, &menu))
      return menu_cbs_exit();

   menu_driver_ctl(RARCH_MENU_CTL_LIST_GET_SELECTION, &list_info);

   list_info.type = MENU_LIST_PLAIN;

   menu_driver_ctl(RARCH_MENU_CTL_LIST_GET_SIZE, &list_info);

   if (list_info.size == 1)
   {
      menu_navigation_ctl(MENU_NAVIGATION_CTL_SET_SELECTION, &selection);
      if ((list_info.selection != 0)
         || settings->menu.navigation.wraparound.enable)
         push_list = 1;
   }
   else
      push_list = 2;

   menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SELECTION, &selection);

   cbs = menu_entries_get_actiondata_at_offset(selection_buf,
         selection);

   switch (push_list)
   {
      case 1:
         {
            menu_ctx_list_t list_info;

            list_info.type   = MENU_LIST_HORIZONTAL;
            list_info.action = action;

            menu_driver_ctl(RARCH_MENU_CTL_LIST_CACHE, &list_info);

            if (cbs && cbs->action_content_list_switch)
               return cbs->action_content_list_switch(
                     selection_buf, menu_stack, "", "", 0);
         }
         break;
      case 2:
         action_left_scroll(0, "", false);
         break;
      case 0:
      default:
         break;
   }

   return 0;
}

static int action_left_shader_scale_pass(unsigned type, const char *label,
      bool wraparound)
{
#ifdef HAVE_SHADER_MANAGER
   unsigned pass = type - MENU_SETTINGS_SHADER_PASS_SCALE_0;
   struct video_shader *shader           = NULL;
   struct video_shader_pass *shader_pass = NULL;

   menu_driver_ctl(RARCH_MENU_CTL_SHADER_GET,
         &shader);
   if (!shader)
      return menu_cbs_exit();
   shader_pass = &shader->pass[pass];
   if (!shader_pass)
      return menu_cbs_exit();

   {
      unsigned current_scale   = shader_pass->fbo.scale_x;
      unsigned delta           = 5;
      current_scale            = (current_scale + delta) % 6;

      shader_pass->fbo.valid   = current_scale;
      shader_pass->fbo.scale_x = shader_pass->fbo.scale_y = current_scale;

   }
#endif
   return 0;
}

static int action_left_shader_filter_pass(unsigned type, const char *label,
      bool wraparound)
{
#ifdef HAVE_SHADER_MANAGER
   unsigned delta = 2;
   unsigned pass                         = type - MENU_SETTINGS_SHADER_PASS_FILTER_0;
   struct video_shader *shader           = NULL;
   struct video_shader_pass *shader_pass = NULL;

   menu_driver_ctl(RARCH_MENU_CTL_SHADER_GET,
         &shader);
   if (!shader)
      return menu_cbs_exit();
   shader_pass = &shader->pass[pass];
   if (!shader_pass)
      return menu_cbs_exit();

   shader_pass->filter = ((shader_pass->filter + delta) % 3);
#endif
   return 0;
}

static int action_left_shader_filter_default(unsigned type, const char *label,
      bool wraparound)
{
#ifdef HAVE_SHADER_MANAGER
   rarch_setting_t *setting = menu_setting_find(menu_hash_to_str(MENU_LABEL_VIDEO_SMOOTH));
   if (!setting)
      return menu_cbs_exit();
   return menu_action_handle_setting(setting,
         menu_setting_get_type(setting), MENU_ACTION_LEFT, wraparound);
#else
   return 0;
#endif
}

static int action_left_cheat_num_passes(unsigned type, const char *label,
      bool wraparound)
{
   bool refresh      = false;
   unsigned new_size = 0;

   if (cheat_manager_get_size())
      new_size = cheat_manager_get_size() - 1;
   menu_entries_ctl(MENU_ENTRIES_CTL_SET_REFRESH, &refresh);
   menu_driver_ctl(RARCH_MENU_CTL_SET_PREVENT_POPULATE, NULL);
   cheat_manager_realloc(new_size);

   return 0;
}

static int action_left_shader_num_passes(unsigned type, const char *label,
      bool wraparound)
{
#ifdef HAVE_SHADER_MANAGER
   bool refresh      = false;
   struct video_shader *shader = NULL;

   menu_driver_ctl(RARCH_MENU_CTL_SHADER_GET,
         &shader);
   if (!shader)
      return menu_cbs_exit();

   if (shader->passes)
      shader->passes--;
   menu_entries_ctl(MENU_ENTRIES_CTL_SET_REFRESH, &refresh);
   menu_driver_ctl(RARCH_MENU_CTL_SET_PREVENT_POPULATE, NULL);
   video_shader_resolve_parameters(NULL, shader);

#endif
   return 0;
}

static int action_left_video_resolution(unsigned type, const char *label,
      bool wraparound)
{
   video_driver_ctl(RARCH_DISPLAY_CTL_GET_PREV_VIDEO_OUT, NULL);

   return 0;
}

static int playlist_association_left(unsigned type, const char *label,
      bool wraparound)
{
   unsigned i;
   int next, found, current = 0;
   core_info_t *info                = NULL;
   struct string_list *stnames      = NULL;
   struct string_list *stcores      = NULL;
   char core_path[PATH_MAX_LENGTH]  = {0};
   char new_playlist_cores[PATH_MAX_LENGTH] = {0};
   settings_t *settings             = config_get_ptr();
   const char *path                 = path_basename(label);
   core_info_list_t           *list = NULL;
   
   core_info_ctl(CORE_INFO_CTL_LIST_GET, &list);

   if (!list)
      return menu_cbs_exit();

   stnames = string_split(settings->playlist_names, ";");
   stcores = string_split(settings->playlist_cores, ";");

   if (!menu_playlist_find_associated_core(path, core_path, sizeof(core_path)))
         strlcpy(core_path, "DETECT", sizeof(core_path));

   for (i = 0; i < list->count; i++)
   {
      core_info_t *info = core_info_get(list, i);
      if (string_is_equal(info->path, core_path))
         current = i;
   }

   next = current - 1;
   if (next < 0)
   {
      if (wraparound)
         next = list->count;
      else
         next = 0;
   }

   info  = core_info_get(list, next);
   found = string_list_find_elem(stnames, path);
   if (found)
      string_list_set(stcores, found-1, info->path);

   string_list_join_concat(new_playlist_cores, sizeof(new_playlist_cores), stcores, ";");

   strlcpy(settings->playlist_cores, new_playlist_cores, sizeof(settings->playlist_cores));

   return 0;
}

static int core_setting_left(unsigned type, const char *label,
      bool wraparound)
{
   unsigned idx     = type - MENU_SETTINGS_CORE_OPTION_START;

   runloop_ctl(RUNLOOP_CTL_CORE_OPTION_PREV, &idx);

   return 0;
}

static int disk_options_disk_idx_left(unsigned type, const char *label,
      bool wraparound)
{
   event_cmd_ctl(EVENT_CMD_DISK_PREV, NULL);
   return 0;
}

static int bind_left_generic(unsigned type, const char *label,
      bool wraparound)
{
   return menu_setting_set(type, label, MENU_ACTION_LEFT, wraparound);
}

static int menu_cbs_init_bind_left_compare_label(menu_file_list_cbs_t *cbs,
      const char *label, uint32_t label_hash, uint32_t menu_label_hash, const char *elem0)
{
   unsigned i;

   if (cbs->setting)
   {
      const char *parent_group   = menu_setting_get_parent_group(cbs->setting);
      uint32_t parent_group_hash = menu_hash_calculate(parent_group);

      if ((parent_group_hash == MENU_VALUE_MAIN_MENU) && (menu_setting_get_type(cbs->setting) == ST_GROUP))
      {
         BIND_ACTION_LEFT(cbs, action_left_mainmenu);
         return 0;
      }
   }

   for (i = 0; i < MAX_USERS; i++)
   {
      uint32_t label_setting_hash;
      char label_setting[PATH_MAX_LENGTH];

      label_setting[0] = '\0';
      snprintf(label_setting, sizeof(label_setting), "input_player%d_joypad_index", i + 1);

      label_setting_hash = menu_hash_calculate(label_setting);

      if (label_hash != label_setting_hash)
         continue;

      BIND_ACTION_LEFT(cbs, bind_left_generic);
      return 0;
   }

   if (strstr(label, "rdb_entry"))
   {
      BIND_ACTION_LEFT(cbs, action_left_scroll);
   }
   else
   {
      switch (label_hash)
      {
         case MENU_LABEL_VIDEO_SHADER_SCALE_PASS:
            BIND_ACTION_LEFT(cbs, action_left_shader_scale_pass);
            break;
         case MENU_LABEL_VIDEO_SHADER_FILTER_PASS:
            BIND_ACTION_LEFT(cbs, action_left_shader_filter_pass);
            break;
         case MENU_LABEL_VIDEO_SHADER_DEFAULT_FILTER:
            BIND_ACTION_LEFT(cbs, action_left_shader_filter_default);
            break;
         case MENU_LABEL_VIDEO_SHADER_NUM_PASSES:
            BIND_ACTION_LEFT(cbs, action_left_shader_num_passes);
            break;
         case MENU_LABEL_CHEAT_NUM_PASSES:
            BIND_ACTION_LEFT(cbs, action_left_cheat_num_passes);
            break;
         case MENU_LABEL_SCREEN_RESOLUTION: 
            BIND_ACTION_LEFT(cbs, action_left_video_resolution);
            break;
         case MENU_LABEL_NO_PLAYLIST_ENTRIES_AVAILABLE:
            switch (menu_label_hash)
            {
               case MENU_VALUE_HORIZONTAL_MENU:
               case MENU_VALUE_MAIN_MENU:
                  BIND_ACTION_LEFT(cbs, action_left_mainmenu);
                  break;
            }
         default:
            return -1;
      }
   }

   return 0;
}

static int menu_cbs_init_bind_left_compare_type(menu_file_list_cbs_t *cbs,
      unsigned type, uint32_t label_hash, uint32_t menu_label_hash)
{
   if (type >= MENU_SETTINGS_CHEAT_BEGIN
         && type <= MENU_SETTINGS_CHEAT_END)
   {
      BIND_ACTION_LEFT(cbs, action_left_cheat);
   }
#ifdef HAVE_SHADER_MANAGER
   else if (type >= MENU_SETTINGS_SHADER_PARAMETER_0
         && type <= MENU_SETTINGS_SHADER_PARAMETER_LAST)
   {
      BIND_ACTION_LEFT(cbs, shader_action_parameter_left);
   }
   else if (type >= MENU_SETTINGS_SHADER_PRESET_PARAMETER_0
         && type <= MENU_SETTINGS_SHADER_PRESET_PARAMETER_LAST)
   {
      BIND_ACTION_LEFT(cbs, shader_action_parameter_preset_left);
   }
#endif
   else if (type >= MENU_SETTINGS_INPUT_DESC_BEGIN
         && type <= MENU_SETTINGS_INPUT_DESC_END)
   {
      BIND_ACTION_LEFT(cbs, action_left_input_desc);
   }
   else if ((type >= MENU_SETTINGS_PLAYLIST_ASSOCIATION_START))
   {
      BIND_ACTION_LEFT(cbs, playlist_association_left);
   }
   else if ((type >= MENU_SETTINGS_CORE_OPTION_START))
   {
      BIND_ACTION_LEFT(cbs, core_setting_left);
   }
   else
   {
      switch (type)
      {
         case MENU_SETTINGS_CORE_DISK_OPTIONS_DISK_INDEX:
            BIND_ACTION_LEFT(cbs, disk_options_disk_idx_left);
            break;
         case MENU_FILE_PLAIN:
         case MENU_FILE_DIRECTORY:
         case MENU_FILE_CARCHIVE:
         case MENU_FILE_IN_CARCHIVE:
         case MENU_FILE_CORE:
         case MENU_FILE_RDB:
         case MENU_FILE_RDB_ENTRY:
         case MENU_FILE_RPL_ENTRY:
         case MENU_FILE_CURSOR:
         case MENU_FILE_SHADER:
         case MENU_FILE_SHADER_PRESET:
         case MENU_FILE_IMAGE:
         case MENU_FILE_OVERLAY:
         case MENU_FILE_VIDEOFILTER:
         case MENU_FILE_AUDIOFILTER:
         case MENU_FILE_CONFIG:
         case MENU_FILE_USE_DIRECTORY:
         case MENU_FILE_PLAYLIST_ENTRY:
         case MENU_INFO_MESSAGE:
         case MENU_FILE_DOWNLOAD_CORE:
         case MENU_FILE_CHEAT:
         case MENU_FILE_REMAP:
         case MENU_FILE_MOVIE:
         case MENU_FILE_MUSIC:
         case MENU_FILE_IMAGEVIEWER:
         case MENU_FILE_PLAYLIST_COLLECTION:
         case MENU_FILE_DOWNLOAD_CORE_CONTENT:
         case MENU_FILE_SCAN_DIRECTORY:
         case MENU_SETTING_GROUP:
            switch (menu_label_hash)
            {
               case MENU_VALUE_HORIZONTAL_MENU:
               case MENU_VALUE_MAIN_MENU:
               case MENU_VALUE_HISTORY_TAB:
               case MENU_VALUE_ADD_TAB:
               case MENU_VALUE_PLAYLISTS_TAB:
                  BIND_ACTION_LEFT(cbs, action_left_mainmenu);
                  break;
               default:
                  BIND_ACTION_LEFT(cbs, action_left_scroll);
                  break;
            }
            break;
         case MENU_SETTING_ACTION:
         case MENU_FILE_CONTENTLIST_ENTRY:
            BIND_ACTION_LEFT(cbs, action_left_mainmenu);
            break;
         default:
            return -1;
      }
   }

   return 0;
}

int menu_cbs_init_bind_left(menu_file_list_cbs_t *cbs,
      const char *path, const char *label, unsigned type, size_t idx,
      const char *elem0, const char *elem1, const char *menu_label,
      uint32_t label_hash, uint32_t menu_label_hash)
{
   if (!cbs)
      return -1;

   BIND_ACTION_LEFT(cbs, bind_left_generic);

   if (type == MENU_SETTING_NO_ITEM)
   {
      switch (menu_label_hash)
      {
         case MENU_VALUE_HORIZONTAL_MENU:
         case MENU_VALUE_MAIN_MENU:
         case 153956705: /* TODO/FIXME - dehardcode */
            BIND_ACTION_LEFT(cbs, action_left_mainmenu);
            return 0;
         default:
            break;
      }
   }

   if (menu_cbs_init_bind_left_compare_label(cbs, label, label_hash, menu_label_hash, elem0) == 0)
      return 0;

   if (menu_cbs_init_bind_left_compare_type(cbs, type, label_hash, menu_label_hash) == 0)
      return 0;

   return -1;
}
