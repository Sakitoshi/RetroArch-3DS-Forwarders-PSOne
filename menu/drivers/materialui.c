/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2016 - Daniel De Matteis
 *  Copyright (C) 2014-2015 - Jean-André Santoni
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

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <compat/posix_string.h>
#include <file/file_path.h>
#include <formats/image.h>
#include <gfx/math/matrix_4x4.h>
#include <string/stdstring.h>
#include <lists/string_list.h>

#include "menu_generic.h"

#include "../menu_driver.h"
#include "../menu_animation.h"
#include "../menu_navigation.h"
#include "../menu_hash.h"
#include "../menu_display.h"

#include "../../core_info.h"
#include "../../configuration.h"
#include "../../frontend/frontend_driver.h"
#include "../../system.h"
#include "../../runloop.h"
#include "../../verbosity.h"
#include "../../tasks/tasks_internal.h"

enum
{
   MUI_TEXTURE_POINTER = 0,
   MUI_TEXTURE_BACK,
   MUI_TEXTURE_SWITCH_ON,
   MUI_TEXTURE_SWITCH_OFF,
   MUI_TEXTURE_TAB_MAIN_ACTIVE,
   MUI_TEXTURE_TAB_PLAYLISTS_ACTIVE,
   MUI_TEXTURE_TAB_SETTINGS_ACTIVE,
   MUI_TEXTURE_TAB_MAIN_PASSIVE,
   MUI_TEXTURE_TAB_PLAYLISTS_PASSIVE,
   MUI_TEXTURE_TAB_SETTINGS_PASSIVE,
   MUI_TEXTURE_LAST
};

enum
{
   MUI_SYSTEM_TAB_MAIN = 0,
   MUI_SYSTEM_TAB_PLAYLISTS,
   MUI_SYSTEM_TAB_SETTINGS
};

#define MUI_SYSTEM_TAB_END MUI_SYSTEM_TAB_SETTINGS

typedef struct mui_handle
{
   unsigned tabs_height;
   unsigned line_height;
   unsigned shadow_height;
   unsigned scrollbar_width;
   unsigned icon_size;
   unsigned margin;
   unsigned glyph_width;
   char box_message[PATH_MAX_LENGTH];

   struct 
   {
      struct
      {
         float alpha;
      } arrow;

      menu_texture_item bg;
      menu_texture_item list[MUI_TEXTURE_LAST];
      uintptr_t white;
   } textures;

   struct
   {
      struct
      {
         unsigned idx;
         unsigned idx_old;
      } active;

      float x_pos;
      size_t selection_ptr_old;
      size_t selection_ptr;
   } categories;

   gfx_font_raster_block_t list_block;
   float scroll_y;
} mui_handle_t;

static void mui_context_reset_textures(mui_handle_t *mui,
      const char *iconpath)
{
   unsigned i;

   for (i = 0; i < MUI_TEXTURE_LAST; i++)
   {
      struct texture_image ti     = {0};
      char path[PATH_MAX_LENGTH]  = {0};

      switch(i)
      {
         case MUI_TEXTURE_POINTER:
            fill_pathname_join(path, iconpath,
                  "pointer.png", sizeof(path));
            break;
         case MUI_TEXTURE_BACK:
            fill_pathname_join(path, iconpath,
                  "back.png", sizeof(path));
            break;
         case MUI_TEXTURE_SWITCH_ON:
            fill_pathname_join(path, iconpath,
                  "on.png", sizeof(path));
            break;
         case MUI_TEXTURE_SWITCH_OFF:
            fill_pathname_join(path, iconpath,
                  "off.png", sizeof(path));
            break;
         case MUI_TEXTURE_TAB_MAIN_ACTIVE:
            fill_pathname_join(path, iconpath,
                  "main_tab_active.png", sizeof(path));
            break;
         case MUI_TEXTURE_TAB_PLAYLISTS_ACTIVE:
            fill_pathname_join(path, iconpath,
                  "playlists_tab_active.png", sizeof(path));
            break;
         case MUI_TEXTURE_TAB_SETTINGS_ACTIVE:
            fill_pathname_join(path, iconpath,
                  "settings_tab_active.png", sizeof(path));
            break;
         case MUI_TEXTURE_TAB_MAIN_PASSIVE:
            fill_pathname_join(path, iconpath,
                  "main_tab_passive.png", sizeof(path));
            break;
         case MUI_TEXTURE_TAB_PLAYLISTS_PASSIVE:
            fill_pathname_join(path, iconpath,
                  "playlists_tab_passive.png", sizeof(path));
            break;
         case MUI_TEXTURE_TAB_SETTINGS_PASSIVE:
            fill_pathname_join(path, iconpath,
                  "settings_tab_passive.png", sizeof(path));
            break;
      }

      if (string_is_empty(path) || !path_file_exists(path))
         continue;

      video_texture_image_load(&ti, path);
      video_driver_texture_load(&ti,
            TEXTURE_FILTER_MIPMAP_LINEAR, &mui->textures.list[i]);

      video_texture_image_free(&ti);
   }
}

static void mui_draw_icon(mui_handle_t *mui,
      uintptr_t texture,
      float x, float y,
      unsigned width, unsigned height,
      float rotation, float scale_factor,
      float *color)
{
   menu_display_ctx_rotate_draw_t rotate_draw;
   menu_display_ctx_draw_t draw;
   struct gfx_coords coords;
   math_matrix_4x4 mymat;

   menu_display_ctl(MENU_DISPLAY_CTL_BLEND_BEGIN, NULL);

   rotate_draw.matrix       = &mymat;
   rotate_draw.rotation     = rotation;
   rotate_draw.scale_x      = scale_factor;
   rotate_draw.scale_y      = scale_factor;
   rotate_draw.scale_z      = 1;
   rotate_draw.scale_enable = true;

   menu_display_ctl(MENU_DISPLAY_CTL_ROTATE_Z, &rotate_draw);

   coords.vertices      = 4;
   coords.vertex        = NULL;
   coords.tex_coord     = NULL;
   coords.lut_tex_coord = NULL;
   coords.color         = (const float*)color;

   draw.x           = x;
   draw.y           = height - y - mui->icon_size;
   draw.width       = mui->icon_size;
   draw.height      = mui->icon_size;
   draw.coords      = &coords;
   draw.matrix_data = &mymat;
   draw.texture     = texture;
   draw.prim_type   = MENU_DISPLAY_PRIM_TRIANGLESTRIP;

   menu_display_ctl(MENU_DISPLAY_CTL_DRAW, &draw);

   menu_display_ctl(MENU_DISPLAY_CTL_BLEND_END, NULL);
}


static void mui_draw_tab(mui_handle_t *mui,
      unsigned i,
      unsigned width, unsigned height,
      float *pure_white)
{
   unsigned tab_icon;
   switch (i)
   {
      case MUI_SYSTEM_TAB_MAIN:
         tab_icon = (i == mui->categories.selection_ptr)
            ? MUI_TEXTURE_TAB_MAIN_ACTIVE
            : MUI_TEXTURE_TAB_MAIN_PASSIVE;
         break;
      case MUI_SYSTEM_TAB_PLAYLISTS:
         tab_icon = (i == mui->categories.selection_ptr)
            ? MUI_TEXTURE_TAB_PLAYLISTS_ACTIVE
            : MUI_TEXTURE_TAB_PLAYLISTS_PASSIVE;
         break;
      case MUI_SYSTEM_TAB_SETTINGS:
         tab_icon = (i == mui->categories.selection_ptr)
            ? MUI_TEXTURE_TAB_SETTINGS_ACTIVE
            : MUI_TEXTURE_TAB_SETTINGS_PASSIVE;
         break;
   }

   mui_draw_icon(mui, mui->textures.list[tab_icon],
         width / (MUI_SYSTEM_TAB_END+1) * (i+0.5) - mui->icon_size/2,
         height - mui->tabs_height,
         width, height, 0, 1, &pure_white[0]);
}

static void mui_blit_line(float x, float y, unsigned width, unsigned height,
      const char *message, uint32_t color, enum text_alignment text_align)
{
   int font_size;
   struct font_params params;
   void *fb_buf              = NULL;

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_SIZE, &font_size);

   params.x           = x / width;
   params.y           = 1.0f - (y + font_size / 3) / height;
   params.scale       = 1.0f;
   params.drop_mod    = 0.0f;
   params.drop_x      = 0.0f;
   params.drop_y      = 0.0f;
   params.color       = color;
   params.full_screen = true;
   params.text_align  = text_align;

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_BUF, &fb_buf);

   video_driver_set_osd_msg(message, &params, fb_buf);
}

static void mui_render_quad(mui_handle_t *mui,
      int x, int y, unsigned w, unsigned h,
      unsigned width, unsigned height,
      float *coord_color)
{
   menu_display_ctx_draw_t draw;
   struct gfx_coords coords;

   coords.vertices      = 4;
   coords.vertex        = NULL;
   coords.tex_coord     = NULL;
   coords.lut_tex_coord = NULL;
   coords.color         = coord_color;

   menu_display_ctl(MENU_DISPLAY_CTL_BLEND_BEGIN, NULL);

   draw.x           = x;
   draw.y           = (int)height - y - (int)h;
   draw.width       = w;
   draw.height      = h;
   draw.coords      = &coords;
   draw.matrix_data = NULL;
   draw.texture     = mui->textures.white;
   draw.prim_type   = MENU_DISPLAY_PRIM_TRIANGLESTRIP;

   menu_display_ctl(MENU_DISPLAY_CTL_DRAW, &draw);

   menu_display_ctl(MENU_DISPLAY_CTL_BLEND_END, NULL);
}

static void mui_draw_tab_begin(mui_handle_t *mui,
      unsigned width, unsigned height,
      float *white_bg, float *grey_bg)
{
   float scale_factor;
   menu_display_ctl(MENU_DISPLAY_CTL_GET_DPI, &scale_factor);

   mui->tabs_height = scale_factor / 3;

   /* tabs background */
   mui_render_quad(mui, 0, height - mui->tabs_height, width,
         mui->tabs_height,
         width, height,
         white_bg);

   /* tabs separator */
   mui_render_quad(mui, 0, height - mui->tabs_height, width,
         1,
         width, height,
         grey_bg);
}

static void mui_draw_tab_end(mui_handle_t *mui,
      unsigned width, unsigned height,
      unsigned header_height,
      float *blue_bg)
{
   /* active tab marker */
   unsigned tab_width = width / (MUI_SYSTEM_TAB_END+1);

   mui_render_quad(mui, mui->categories.selection_ptr * tab_width,
         height - (header_height/16),
         tab_width,
         header_height/16,
         width, height,
         &blue_bg[0]);
}

static void mui_draw_scrollbar(mui_handle_t *mui, 
      unsigned width, unsigned height, float *coord_color)
{
   unsigned header_height;
   float content_height, total_height,
         scrollbar_height, scrollbar_margin, y;

   if (!mui)
      return;

   menu_display_ctl(MENU_DISPLAY_CTL_HEADER_HEIGHT, &header_height);

   content_height   = menu_entries_get_end() * mui->line_height;
   total_height     = height - header_height - mui->tabs_height;
   scrollbar_margin = mui->scrollbar_width;
   scrollbar_height = total_height / (content_height / total_height);
   y                = total_height * mui->scroll_y / content_height;

   /* apply a margin on the top and bottom of the scrollbar for aestetic */
   scrollbar_height -= scrollbar_margin * 2;
   y += scrollbar_margin;

   if (content_height >= total_height)
   {
      /* if the scrollbar is extremely short, display it as a square */
      if (scrollbar_height <= mui->scrollbar_width)
         scrollbar_height = mui->scrollbar_width;

      mui_render_quad(mui,
            width - mui->scrollbar_width - scrollbar_margin,
            header_height + y,
            mui->scrollbar_width,
            scrollbar_height,
            width, height,
            coord_color);
   }
}

static void mui_get_message(void *data, const char *message)
{
   mui_handle_t *mui   = (mui_handle_t*)data;

   if (!mui || !message || !*message)
      return;

   strlcpy(mui->box_message, message, sizeof(mui->box_message));
}

static void mui_render_messagebox(const char *message)
{
   unsigned i, width, height;
   uint32_t normal_color;
   int x, y, font_size;
   settings_t *settings     = config_get_ptr();
   struct string_list *list = (struct string_list*)
      string_split(message, "\n");

   if (!list)
      return;
   if (list->elems == 0)
      goto end;

   video_driver_get_size(&width, &height);

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_SIZE, &font_size);

   x = width  / 2;
   y = height / 2 - list->size * font_size / 2;

   normal_color = FONT_COLOR_ARGB_TO_RGBA(settings->menu.entry_normal_color);

   for (i = 0; i < list->size; i++)
   {
      const char *msg = list->elems[i].data;
      if (msg)
         mui_blit_line(x, y + i * font_size,
               width, height,
               msg, normal_color, TEXT_ALIGN_CENTER);
   }

end:
   string_list_free(list);
}

static void mui_render(void *data)
{
   size_t i             = 0;
   menu_animation_ctx_delta_t delta;
   float delta_time;
   unsigned bottom, width, height, header_height;
   mui_handle_t *mui    = (mui_handle_t*)data;
   settings_t *settings = config_get_ptr();

   if (!mui)
      return;

   video_driver_get_size(&width, &height);

   menu_animation_ctl(MENU_ANIMATION_CTL_DELTA_TIME, &delta_time);

   delta.current = delta_time;

   if (menu_animation_ctl(MENU_ANIMATION_CTL_IDEAL_DELTA_TIME_GET, &delta))
      menu_animation_ctl(MENU_ANIMATION_CTL_UPDATE, &delta.ideal);

   menu_display_ctl(MENU_DISPLAY_CTL_SET_WIDTH,  &width);
   menu_display_ctl(MENU_DISPLAY_CTL_SET_HEIGHT, &height);
   menu_display_ctl(MENU_DISPLAY_CTL_HEADER_HEIGHT, &header_height);

   if (settings->menu.pointer.enable)
   {
      int16_t pointer_y = menu_input_pointer_state(MENU_POINTER_Y_AXIS);
      float    old_accel_val, new_accel_val;
      unsigned new_pointer_val = 
         (pointer_y - mui->line_height + mui->scroll_y - 16)
         / mui->line_height;

      menu_input_ctl(MENU_INPUT_CTL_POINTER_ACCEL_READ, &old_accel_val);
      menu_input_ctl(MENU_INPUT_CTL_POINTER_PTR, &new_pointer_val);

      mui->scroll_y            -= old_accel_val / 60.0;

      new_accel_val = old_accel_val * 0.96;

      menu_input_ctl(MENU_INPUT_CTL_POINTER_ACCEL_WRITE, &new_accel_val);
   }

   if (settings->menu.mouse.enable)
   {
      int16_t mouse_y          = menu_input_mouse_state(MENU_MOUSE_Y_AXIS);

      unsigned new_pointer_val = 
         (mouse_y - mui->line_height + mui->scroll_y - 16)
         / mui->line_height;

      menu_input_ctl(MENU_INPUT_CTL_MOUSE_PTR, &new_pointer_val);
   }

   if (mui->scroll_y < 0)
      mui->scroll_y = 0;

   bottom = menu_entries_get_end() * mui->line_height
      - height + header_height + mui->tabs_height;
   if (mui->scroll_y > bottom)
      mui->scroll_y = bottom;

   if (menu_entries_get_end() * mui->line_height
         < height - header_height - mui->tabs_height)
      mui->scroll_y = 0;

   if (menu_entries_get_end() < height / mui->line_height) { }
   else
      i = mui->scroll_y / mui->line_height;

   menu_entries_ctl(MENU_ENTRIES_CTL_SET_START, &i);
}

static void mui_render_label_value(mui_handle_t *mui,
      int y, unsigned width, unsigned height,
      uint64_t index, uint32_t color, bool selected, const char *label,
      const char *value, float *pure_white)
{
   menu_animation_ctx_ticker_t ticker;
   char label_str[PATH_MAX_LENGTH];
   char value_str[PATH_MAX_LENGTH];
   int value_len            = strlen(value);
   int ticker_limit         = 0;
   size_t usable_width      = 0;
   uintptr_t texture_switch = 0;
   bool do_draw_text        = false;
   uint32_t hash_value      = 0;

   usable_width = width - (mui->margin * 2);

   if (value_len * mui->glyph_width > usable_width / 2)
      value_len = (usable_width/2) / mui->glyph_width;

   ticker_limit = (usable_width / mui->glyph_width) - (value_len + 2);

   ticker.s        = label_str;
   ticker.len      = ticker_limit;
   ticker.idx      = index;
   ticker.str      = label;
   ticker.selected = selected;

   menu_animation_ctl(MENU_ANIMATION_CTL_TICKER, &ticker);

   ticker.s        = value_str;
   ticker.len      = value_len;
   ticker.str      = value;

   menu_animation_ctl(MENU_ANIMATION_CTL_TICKER, &ticker);

   mui_blit_line(mui->margin, y + mui->line_height / 2,
         width, height, label_str, color, TEXT_ALIGN_LEFT);

   hash_value = menu_hash_calculate(value);

   if (string_is_equal(value, "disabled") || string_is_equal(value, "off"))
   {
      if (mui->textures.list[MUI_TEXTURE_SWITCH_OFF])
         texture_switch = mui->textures.list[MUI_TEXTURE_SWITCH_OFF];
      else
         do_draw_text = true;
   }
   else if (string_is_equal(value, "enabled") || string_is_equal(value, "on"))
   {
      if (mui->textures.list[MUI_TEXTURE_SWITCH_ON])
         texture_switch = mui->textures.list[MUI_TEXTURE_SWITCH_ON];
      else
         do_draw_text = true;
   }
   else
   {
      switch (hash_value)
      {
         case MENU_VALUE_COMP:
            break;
         case MENU_VALUE_MORE:
            break;
         case MENU_VALUE_CORE:
            break;
         case MENU_VALUE_RDB:
            break;
         case MENU_VALUE_CURSOR:
            break;
         case MENU_VALUE_FILE:
            break;
         case MENU_VALUE_DIR:
            break;
         case MENU_VALUE_MUSIC:
            break;
         case MENU_VALUE_IMAGE:
            break;
         case MENU_VALUE_MOVIE:
            break;
         case MENU_VALUE_ON:
            if (mui->textures.list[MUI_TEXTURE_SWITCH_ON])
               texture_switch = mui->textures.list[MUI_TEXTURE_SWITCH_ON];
            else
               do_draw_text = true;
            break;
         case MENU_VALUE_OFF:
            if (mui->textures.list[MUI_TEXTURE_SWITCH_OFF])
               texture_switch = mui->textures.list[MUI_TEXTURE_SWITCH_OFF];
            else
               do_draw_text = true;
            break;
         default:
            do_draw_text = true;
            break;
      }
   }

   if (do_draw_text)
      mui_blit_line(width - mui->margin,
            y + mui->line_height / 2,
            width, height, value_str, color, TEXT_ALIGN_RIGHT);

   if (texture_switch)
      mui_draw_icon(mui, texture_switch,
            width - mui->margin - mui->icon_size, y,
            width, height, 0, 1, &pure_white[0]);
}

static void mui_render_menu_list(mui_handle_t *mui,
      unsigned width, unsigned height,
      uint32_t normal_color,
      uint32_t hover_color,
      float *pure_white)
{
   unsigned header_height;
   uint64_t *frame_count;
   size_t i                = 0;
   size_t          end     = menu_entries_get_end();
   video_driver_ctl(RARCH_DISPLAY_CTL_GET_FRAME_COUNT, &frame_count);

   if (!menu_display_ctl(MENU_DISPLAY_CTL_UPDATE_PENDING, NULL))
      return;

   menu_display_ctl(MENU_DISPLAY_CTL_HEADER_HEIGHT, &header_height);

   mui->list_block.carr.coords.vertices = 0;

   menu_entries_ctl(MENU_ENTRIES_CTL_START_GET, &i);

   for (; i < end; i++)
   {
      int y;
      size_t selection;
      bool entry_selected;
      menu_entry_t entry;

      if (!menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SELECTION, &selection))
         continue;

      y = header_height - mui->scroll_y + (mui->line_height * i);

      if ((y - (int)mui->line_height) > (int)height
            || ((y + (int)mui->line_height) < 0))
         continue;

      menu_entry_get(&entry, 0, i, NULL, true);

      entry_selected = selection == i;

      mui_render_label_value(mui, y, width, height, *frame_count / 20,
         entry_selected ? hover_color : normal_color, entry_selected,
         entry.path, entry.value, pure_white);
   }
}

static void mui_draw_cursor(mui_handle_t *mui,
      float *color,
      float x, float y, unsigned width, unsigned height)
{
   menu_display_ctx_draw_t draw;
   struct gfx_coords coords;

   coords.vertices      = 4;
   coords.vertex        = NULL;
   coords.tex_coord     = NULL;
   coords.lut_tex_coord = NULL;
   coords.color         = (const float*)color;

   menu_display_ctl(MENU_DISPLAY_CTL_BLEND_BEGIN, NULL);

   draw.x           = x - 32;
   draw.y           = (int)height - y - 32;
   draw.width       = 64;
   draw.height      = 64;
   draw.coords      = &coords;
   draw.matrix_data = NULL;
   draw.texture     = mui->textures.list[MUI_TEXTURE_POINTER];
   draw.prim_type   = MENU_DISPLAY_PRIM_TRIANGLESTRIP;

   menu_display_ctl(MENU_DISPLAY_CTL_DRAW, &draw);

   menu_display_ctl(MENU_DISPLAY_CTL_BLEND_END, NULL);
}

static size_t mui_list_get_size(void *data, enum menu_list_type type)
{
   switch (type)
   {
      case MENU_LIST_PLAIN:
         return menu_entries_get_stack_size(0);
      case MENU_LIST_TABS:
         return MUI_SYSTEM_TAB_END;
      default:
         break;
   }

   return 0;
}

static void bgcolor_setalpha(float *bg, float alpha)
{
   bg[3]  = alpha;
   bg[7]  = alpha;
   bg[11] = alpha;
   bg[15] = alpha;
}

static int mui_get_core_title(char *s, size_t len)
{
   struct retro_system_info    *system = NULL;
   rarch_system_info_t      *info = NULL;
   settings_t *settings           = config_get_ptr();
   const char *core_name          = NULL;
   const char *core_version       = NULL;

   menu_driver_ctl(RARCH_MENU_CTL_SYSTEM_INFO_GET,
         &system);
   
   core_name    = system->library_name;
   core_version = system->library_version;

   runloop_ctl(RUNLOOP_CTL_SYSTEM_INFO_GET, &info);

   if (!settings->menu.core_enable)
      return -1; 

   if (string_is_empty(core_name))
      core_name = info->info.library_name;
   if (string_is_empty(core_name))
      core_name = menu_hash_to_str(MENU_VALUE_NO_CORE);

   if (!core_version)
      core_version = info->info.library_version;
   if (!core_version)
      core_version = "";

   snprintf(s, len, "%s %s", core_name, core_version);

   return 0;
}

static void mui_frame(void *data)
{
   unsigned header_height;
   bool display_kb;
   float black_bg[16] = {
      0, 0, 0, 0.75,
      0, 0, 0, 0.75,
      0, 0, 0, 0.75,
      0, 0, 0, 0.75,
   };
   float blue_bg[16] = {
      0.13, 0.59, 0.95, 1,
      0.13, 0.59, 0.95, 1,
      0.13, 0.59, 0.95, 1,
      0.13, 0.59, 0.95, 1,
   };
   float lightblue_bg[16] = {
      0.89, 0.95, 0.99, 1.00,
      0.89, 0.95, 0.99, 1.00,
      0.89, 0.95, 0.99, 1.00,
      0.89, 0.95, 0.99, 1.00,
   };
   float pure_white[16]=  {
      1, 1, 1, 1,
      1, 1, 1, 1,
      1, 1, 1, 1,
      1, 1, 1, 1,
   };
   float white_bg[16]=  {
      0.98, 0.98, 0.98, 1,
      0.98, 0.98, 0.98, 1,
      0.98, 0.98, 0.98, 1,
      0.98, 0.98, 0.98, 1,
   };
   float white_transp_bg[16]=  {
      0.98, 0.98, 0.98, 0.90,
      0.98, 0.98, 0.98, 0.90,
      0.98, 0.98, 0.98, 0.90,
      0.98, 0.98, 0.98, 0.90,
   };
   float grey_bg[16]=  {
      0.78, 0.78, 0.78, 1,
      0.78, 0.78, 0.78, 1,
      0.78, 0.78, 0.78, 1,
      0.78, 0.78, 0.78, 1,
   };
   float shadow_bg[16]=  {
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0.2,
      0, 0, 0, 0.2,
   };
   menu_animation_ctx_ticker_t ticker;
   unsigned width, height, ticker_limit, i;
   char msg[256];
   char title[256];
   char title_buf[256];
   char title_msg[256];
   size_t selection;
   size_t title_margin;
   uint64_t *frame_count;
   menu_display_ctx_draw_t draw;
   mui_handle_t *mui               = (mui_handle_t*)data;
   settings_t *settings            = config_get_ptr();
   const uint32_t normal_color     = 0x212121ff;
   const uint32_t hover_color      = 0x212121ff;
   const uint32_t title_color      = 0xffffffff;
   const uint32_t activetab_color  = 0x0096f2ff;
   const uint32_t passivetab_color = 0x9e9e9eff;
   bool background_rendered        = false;
   bool libretro_running           = menu_display_ctl(
         MENU_DISPLAY_CTL_LIBRETRO_RUNNING, NULL);

   video_driver_ctl(RARCH_DISPLAY_CTL_GET_FRAME_COUNT, &frame_count);

   (void)passivetab_color;
   (void)activetab_color;

   if (!mui)
      return;

   video_driver_get_size(&width, &height);

   menu_display_ctl(MENU_DISPLAY_CTL_SET_VIEWPORT, NULL);
   menu_display_ctl(MENU_DISPLAY_CTL_HEADER_HEIGHT, &header_height);

   if (libretro_running)
   {
      memset(&draw, 0, sizeof(menu_display_ctx_draw_t));

      draw.width              = width;
      draw.height             = height;
      draw.texture            = mui->textures.white;
      draw.handle_alpha       = 0.75f;
      draw.force_transparency = false;
      draw.color              = &white_transp_bg[0];
      draw.color2             = &white_bg[0];
      draw.vertex             = NULL;
      draw.tex_coord          = NULL;
      draw.vertex_count       = 4;
      draw.prim_type          = MENU_DISPLAY_PRIM_TRIANGLESTRIP;

      menu_display_ctl(MENU_DISPLAY_CTL_DRAW_BG, &draw);
   }
   else
   {
      menu_display_ctx_clearcolor_t clearcolor;

      clearcolor.r = 1.0f;
      clearcolor.g = 1.0f;
      clearcolor.b = 1.0f;
      clearcolor.a = 0.75f;

      menu_display_ctl(MENU_DISPLAY_CTL_CLEAR_COLOR, &clearcolor);

      if (mui->textures.bg)
      {
         background_rendered = true;

         /* Set new opacity for transposed white background */
         bgcolor_setalpha(white_transp_bg, 0.30);

         memset(&draw, 0, sizeof(menu_display_ctx_draw_t));

         draw.width              = width;
         draw.height             = height;
         draw.texture            = mui->textures.bg;
         draw.handle_alpha       = 0.75f;
         draw.force_transparency = true;
         draw.color              = &white_transp_bg[0];
         draw.color2             = &white_bg[0];
         draw.vertex             = NULL;
         draw.tex_coord          = NULL;
         draw.vertex_count       = 4;
         draw.prim_type          = MENU_DISPLAY_PRIM_TRIANGLESTRIP;

         menu_display_ctl(MENU_DISPLAY_CTL_DRAW_BG, &draw);

         /* Restore opacity of transposed white background */
         bgcolor_setalpha(white_transp_bg, 0.90);
      }
   }

   menu_entries_get_title(title, sizeof(title));

   if (!menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SELECTION, &selection))
      return;

   if (background_rendered || libretro_running)
      bgcolor_setalpha(lightblue_bg, 0.75);
   else
      bgcolor_setalpha(lightblue_bg, 1.0);

   /* highlighted entry */
   mui_render_quad(mui, 0,
         header_height -   mui->scroll_y + mui->line_height *
         selection, width, mui->line_height,
         width, height,
         &lightblue_bg[0]);

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_BIND_BLOCK, &mui->list_block);

   mui_render_menu_list(mui, width, height,
         normal_color, hover_color, &pure_white[0]);

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_FLUSH_BLOCK, NULL);
   menu_animation_ctl(MENU_ANIMATION_CTL_SET_ACTIVE, NULL);

   /* header */
   mui_render_quad(mui, 0, 0, width, header_height,
         width, height, &blue_bg[0]);

   mui->tabs_height = 0;

   /* display tabs if depth equal one, if not hide them */
   if (mui_list_get_size(mui, MENU_LIST_PLAIN) == 1)
   {
      mui_draw_tab_begin(mui, width, height, &white_bg[0], &grey_bg[0]);

      for (i = 0; i <= MUI_SYSTEM_TAB_END; i++)
         mui_draw_tab(mui, i, width, height, &pure_white[0]);

      mui_draw_tab_end(mui, width, height, header_height, &blue_bg[0]);
   }

   mui_render_quad(mui, 0, header_height, width,
         mui->shadow_height,
         width, height,
         &shadow_bg[0]);

   title_margin = mui->margin;

   if (menu_entries_ctl(MENU_ENTRIES_CTL_SHOW_BACK, NULL))
   {
      title_margin = mui->icon_size;
      mui_draw_icon(mui, mui->textures.list[MUI_TEXTURE_BACK],
         0, 0, width, height, 0, 1, &pure_white[0]);
   }

   ticker_limit = (width - mui->margin*2) / mui->glyph_width;

   ticker.s        = title_buf;
   ticker.len      = ticker_limit;
   ticker.idx      = *frame_count / 100;
   ticker.str      = title;
   ticker.selected = true;

   menu_animation_ctl(MENU_ANIMATION_CTL_TICKER, &ticker);

   /* Title */
   if (mui_get_core_title(title_msg, sizeof(title_msg)) == 0)
   {
      char title_buf_msg_tmp[256];
      char title_buf_msg[256];
      size_t usable_width = width - (mui->margin * 2);
      int ticker_limit, value_len;
      
      snprintf(title_buf_msg, sizeof(title_buf), "%s (%s)",
            title_buf, title_msg);
      value_len = strlen(title_buf);
      ticker_limit = (usable_width / mui->glyph_width) - (value_len + 2);

      ticker.s        = title_buf_msg_tmp;
      ticker.len      = ticker_limit;
      ticker.idx      = *frame_count / 20;
      ticker.str      = title_buf_msg;
      ticker.selected = true;

      menu_animation_ctl(MENU_ANIMATION_CTL_TICKER, &ticker);

      strlcpy(title_buf, title_buf_msg_tmp, sizeof(title_buf));
   }

   mui_blit_line(title_margin, header_height / 2, width, height,
         title_buf, title_color, TEXT_ALIGN_LEFT);

   mui_draw_scrollbar(mui, width, height, &grey_bg[0]);

   menu_input_ctl(MENU_INPUT_CTL_KEYBOARD_DISPLAY, &display_kb);

   if (display_kb)
   {
      const char *str = NULL, *label = NULL;
      menu_input_ctl(MENU_INPUT_CTL_KEYBOARD_BUFF_PTR, &str);
      menu_input_ctl(MENU_INPUT_CTL_KEYBOARD_LABEL,    &label);

      if (!str)
         str = "";
      mui_render_quad(mui, 0, 0, width, height, width, height, &black_bg[0]);
      snprintf(msg, sizeof(msg), "%s\n%s", label, str);
      mui_render_messagebox(msg);
   }

   if (!string_is_empty(mui->box_message))
   {
      mui_render_quad(mui, 0, 0, width, height, width, height, &black_bg[0]);
      mui_render_messagebox(mui->box_message);
      mui->box_message[0] = '\0';
   }

   if (settings->menu.mouse.enable && (settings->video.fullscreen 
            || !video_driver_ctl(RARCH_DISPLAY_CTL_HAS_WINDOWED, NULL)))
   {
      int16_t mouse_x = menu_input_mouse_state(MENU_MOUSE_X_AXIS);
      int16_t mouse_y = menu_input_mouse_state(MENU_MOUSE_Y_AXIS);

      mui_draw_cursor(mui, &white_bg[0], mouse_x, mouse_y, width, height);
   }

   menu_display_ctl(MENU_DISPLAY_CTL_RESTORE_CLEAR_COLOR, NULL);
   menu_display_ctl(MENU_DISPLAY_CTL_UNSET_VIEWPORT, NULL);
}

static void mui_allocate_white_texture(mui_handle_t *mui)
{
   struct texture_image ti;
   static const uint8_t white_data[] = { 0xff, 0xff, 0xff, 0xff };

   ti.width  = 1;
   ti.height = 1;
   ti.pixels = (uint32_t*)&white_data;

   video_driver_texture_load(&ti,
         TEXTURE_FILTER_NEAREST, &mui->textures.white);
}

static void mui_font(void)
{
   int font_size;
   char mediapath[PATH_MAX_LENGTH], fontpath[PATH_MAX_LENGTH];
   menu_display_ctx_font_t font_info;
   settings_t *settings = config_get_ptr();

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_SIZE, &font_size);

   fill_pathname_join(mediapath, settings->assets_directory,
         "glui", sizeof(mediapath));
   fill_pathname_join(fontpath, mediapath,
         "Roboto-Regular.ttf", sizeof(fontpath));

   font_info.path = fontpath;
   font_info.size = font_size;

   if (!menu_display_ctl(MENU_DISPLAY_CTL_FONT_MAIN_INIT, &font_info))
      RARCH_WARN("Failed to load font.");
}

static void mui_layout(mui_handle_t *mui)
{
   void *fb_buf;
   float scale_factor;
   int new_font_size;
   unsigned width, height, new_header_height;

   video_driver_get_size(&width, &height);

   /* Mobiles platforms may have very small display metrics 
    * coupled to a high resolution, so we should be DPI aware 
    * to ensure the entries hitboxes are big enough.
    *
    * On desktops, we just care about readability, with every widget
    * size proportional to the display width. */
   menu_display_ctl(MENU_DISPLAY_CTL_GET_DPI, &scale_factor);

   new_header_height    = scale_factor / 3;
   new_font_size        = scale_factor / 9;

   mui->shadow_height   = scale_factor / 36;
   mui->scrollbar_width = scale_factor / 36;
   mui->tabs_height     = scale_factor / 3;
   mui->line_height     = scale_factor / 3;
   mui->margin          = scale_factor / 9;
   mui->icon_size       = scale_factor / 3;

   menu_display_ctl(MENU_DISPLAY_CTL_SET_HEADER_HEIGHT,
         &new_header_height);
   menu_display_ctl(MENU_DISPLAY_CTL_SET_FONT_SIZE,
         &new_font_size);

   /* we assume the average glyph aspect ratio is close to 3:4 */
   mui->glyph_width = new_font_size * 3/4;

   mui_font();

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_BUF, &fb_buf);

   if (fb_buf) /* calculate a more realistic ticker_limit */
   {
      unsigned m_width = 
         font_driver_get_message_width(fb_buf, "a", 1, 1);

      if (m_width)
         mui->glyph_width = m_width;
   }
}

static void *mui_init(void **userdata)
{
   mui_handle_t   *mui = NULL;
   menu_handle_t *menu = (menu_handle_t*)
      calloc(1, sizeof(*menu));

   if (!menu)
      goto error;

   if (!menu_display_ctl(MENU_DISPLAY_CTL_INIT_FIRST_DRIVER, NULL))
      goto error;

   mui = (mui_handle_t*)calloc(1, sizeof(mui_handle_t));

   if (!mui)
      goto error;

   *userdata = mui;

   mui_layout(mui);
   mui_allocate_white_texture(mui);


   return menu;
error:
   if (menu)
      free(menu);
   return NULL;
}

static void mui_free(void *data)
{
   mui_handle_t *mui   = (mui_handle_t*)data;

   if (!mui)
      return;

   gfx_coord_array_free(&mui->list_block.carr);

   font_driver_bind_block(NULL, NULL);
}

static void mui_context_bg_destroy(mui_handle_t *mui)
{
   if (!mui)
      return;

   video_driver_texture_unload(&mui->textures.bg);
   video_driver_texture_unload(&mui->textures.white);
}

static void mui_context_destroy(void *data)
{
   unsigned i;
   mui_handle_t *mui   = (mui_handle_t*)data;

   if (!mui)
      return;

   for (i = 0; i < MUI_TEXTURE_LAST; i++)
      video_driver_texture_unload(&mui->textures.list[i]);

   menu_display_ctl(MENU_DISPLAY_CTL_FONT_MAIN_DEINIT, NULL);

   mui_context_bg_destroy(mui);
}

static bool mui_load_image(void *userdata, void *data, enum menu_image_type type)
{
   mui_handle_t *mui = (mui_handle_t*)userdata;

   switch (type)
   {
      case MENU_IMAGE_NONE:
         break;
      case MENU_IMAGE_WALLPAPER:
         mui_context_bg_destroy(mui);
         video_driver_texture_load(data,
               TEXTURE_FILTER_MIPMAP_LINEAR, &mui->textures.bg);
         mui_allocate_white_texture(mui);
         break;
      case MENU_IMAGE_BOXART:
         break;
   }

   return true;
}

static float mui_get_scroll(mui_handle_t *mui)
{
   size_t selection;
   unsigned width, height, half = 0;

   if (!mui)
      return 0;
   if (!menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SELECTION, &selection))
      return 0;

   video_driver_get_size(&width, &height);

   if (mui->line_height)
      half = (height / mui->line_height) / 2;

   if (selection < half)
      return 0;

   return ((selection + 2 - half) * mui->line_height);
}

static void mui_navigation_set(void *data, bool scroll)
{
   menu_animation_ctx_entry_t entry;
   mui_handle_t *mui    = (mui_handle_t*)data;
   float     scroll_pos = mui ? mui_get_scroll(mui) : 0.0f;

   if (!mui || !scroll)
      return;

   entry.duration     = 10;
   entry.target_value = scroll_pos;
   entry.subject      = &mui->scroll_y;
   entry.easing_enum  = EASING_IN_OUT_QUAD;
   entry.tag          = -1;
   entry.cb           = NULL;

   menu_animation_ctl(MENU_ANIMATION_CTL_PUSH, &entry);
}

static void  mui_list_set_selection(void *data, file_list_t *list)
{
   mui_navigation_set(data, true);
}

static void mui_navigation_clear(void *data, bool pending_push)
{
   size_t i             = 0;
   mui_handle_t *mui    = (mui_handle_t*)data;
   if (!mui)
      return;

   menu_entries_ctl(MENU_ENTRIES_CTL_SET_START, &i);
   mui->scroll_y = 0;
}

static void mui_navigation_set_last(void *data)
{
   mui_navigation_set(data, true);
}

static void mui_navigation_alphabet(void *data, size_t *unused)
{
   mui_navigation_set(data, true);
}

static void mui_populate_entries(
      void *data, const char *path,
      const char *label, unsigned i)
{
   mui_handle_t *mui    = (mui_handle_t*)data;
   if (!mui)
      return;

   mui->scroll_y = mui_get_scroll(mui);
}

static void mui_context_reset(void *data)
{
   char iconpath[PATH_MAX_LENGTH] = {0};
   mui_handle_t *mui              = (mui_handle_t*)data;
   settings_t *settings           = config_get_ptr();

   if (!mui || !settings)
      return;

   fill_pathname_join(iconpath, settings->assets_directory,
         "glui", sizeof(iconpath));
   fill_pathname_slash(iconpath, sizeof(iconpath));

   mui_layout(mui);
   mui_context_bg_destroy(mui);
   mui_allocate_white_texture(mui);
   mui_context_reset_textures(mui, iconpath);

   rarch_task_push_image_load(settings->menu.wallpaper, "cb_menu_wallpaper",
         menu_display_handle_wallpaper_upload, NULL);
}

static int mui_environ(enum menu_environ_cb type, void *data, void *userdata)
{
   switch (type)
   {
      case 0:
      default:
         break;
   }

   return -1;
}

static void mui_preswitch_tabs(mui_handle_t *mui, unsigned action)
{
   size_t idx              = 0;
   size_t stack_size       = 0;
   file_list_t *menu_stack = NULL;

   if (!mui)
      return;

   menu_navigation_ctl(MENU_NAVIGATION_CTL_SET_SELECTION, &idx);

   menu_stack = menu_entries_get_menu_stack_ptr(0);
   stack_size = menu_stack->size;

   if (menu_stack->list[stack_size - 1].label)
      free(menu_stack->list[stack_size - 1].label);
   menu_stack->list[stack_size - 1].label = NULL;

   switch (mui->categories.selection_ptr)
   {
      case MUI_SYSTEM_TAB_MAIN:
         menu_stack->list[stack_size - 1].label = 
            strdup(menu_hash_to_str(MENU_VALUE_MAIN_MENU));
         menu_stack->list[stack_size - 1].type = 
            MENU_SETTINGS;
         break;
      case MUI_SYSTEM_TAB_PLAYLISTS:
         menu_stack->list[stack_size - 1].label = 
            strdup(menu_hash_to_str(MENU_VALUE_PLAYLISTS_TAB));
         menu_stack->list[stack_size - 1].type = 
            MENU_PLAYLISTS_TAB;
         break;
      case MUI_SYSTEM_TAB_SETTINGS:
         menu_stack->list[stack_size - 1].label = 
            strdup(menu_hash_to_str(MENU_VALUE_SETTINGS_TAB));
         menu_stack->list[stack_size - 1].type = 
            MENU_SETTINGS;
         break;
   }
}

static void mui_list_cache(void *data,
      enum menu_list_type type, unsigned action)
{
   size_t list_size;
   mui_handle_t *mui   = (mui_handle_t*)data;

   if (!mui)
      return;

   list_size = MUI_SYSTEM_TAB_END;

   switch (type)
   {
      case MENU_LIST_PLAIN:
         break;
      case MENU_LIST_HORIZONTAL:
         mui->categories.selection_ptr_old = mui->categories.selection_ptr;

         switch (action)
         {
            case MENU_ACTION_LEFT:
               if (mui->categories.selection_ptr == 0)
               {
                  mui->categories.selection_ptr = list_size;
                  mui->categories.active.idx = list_size - 1;
               }
               else
                  mui->categories.selection_ptr--;
               break;
            default:
               if (mui->categories.selection_ptr == list_size)
               {
                  mui->categories.selection_ptr = 0;
                  mui->categories.active.idx = 1;
               }
               else
                  mui->categories.selection_ptr++;
               break;
         }

         mui_preswitch_tabs(mui, action);
         break;
      default:
         break;
   }
}

static int mui_list_push(void *data, void *userdata,
      menu_displaylist_info_t *info, unsigned type)
{
   menu_displaylist_ctx_parse_entry_t entry;
   int ret                = -1;
   core_info_list_t *list = NULL;
   menu_handle_t *menu    = (menu_handle_t*)data;

   (void)userdata;

   switch (type)
   {
      case DISPLAYLIST_LOAD_CONTENT_LIST:
         menu_entries_ctl(MENU_ENTRIES_CTL_CLEAR, info->list);
         menu_entries_push(info->list,
               menu_hash_to_str(MENU_LABEL_VALUE_LOAD_CONTENT),
               menu_hash_to_str(MENU_LABEL_LOAD_CONTENT),
               MENU_SETTING_ACTION, 0, 0);

         core_info_ctl(CORE_INFO_CTL_LIST_GET, &list);
         if (core_info_list_num_info_files(list))
         {
            menu_entries_push(info->list,
                  menu_hash_to_str(MENU_LABEL_VALUE_DETECT_CORE_LIST),
                  menu_hash_to_str(MENU_LABEL_DETECT_CORE_LIST),
                  MENU_SETTING_ACTION, 0, 0);

            menu_entries_push(info->list,
                  menu_hash_to_str(MENU_LABEL_VALUE_DOWNLOADED_FILE_DETECT_CORE_LIST),
                  menu_hash_to_str(MENU_LABEL_DOWNLOADED_FILE_DETECT_CORE_LIST),
                  MENU_SETTING_ACTION, 0, 0);
         }

         info->need_push    = true;
         info->need_refresh = true;
         ret = 0;
         break;
      case DISPLAYLIST_MAIN_MENU:
         menu_entries_ctl(MENU_ENTRIES_CTL_CLEAR, info->list);

         entry.data            = menu;
         entry.info            = info;
         entry.parse_type      = PARSE_ACTION;
         entry.add_empty_entry = false;

         if (!rarch_ctl(RARCH_CTL_IS_DUMMY_CORE, NULL))
         {
            entry.info_label      = menu_hash_to_str(MENU_LABEL_CONTENT_SETTINGS);
            menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
         }

         entry.info_label      = menu_hash_to_str(MENU_LABEL_START_CORE);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);

#ifndef HAVE_DYNAMIC
         if (frontend_driver_has_fork())
#endif
         {
            entry.info_label      = menu_hash_to_str(MENU_LABEL_CORE_LIST);
            menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
         }

         entry.info_label      = menu_hash_to_str(MENU_LABEL_LOAD_CONTENT_LIST);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);

         entry.info_label      = menu_hash_to_str(MENU_LABEL_LOAD_CONTENT_HISTORY);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);

#if defined(HAVE_NETWORKING)
#if defined(HAVE_LIBRETRODB)
         entry.info_label      = menu_hash_to_str(MENU_LABEL_ADD_CONTENT_LIST);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#endif
         entry.info_label      = menu_hash_to_str(MENU_LABEL_ONLINE_UPDATER);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#endif
         entry.info_label      = menu_hash_to_str(MENU_LABEL_INFORMATION_LIST);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#ifndef HAVE_DYNAMIC
         entry.info_label      = menu_hash_to_str(MENU_LABEL_RESTART_RETROARCH);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#endif
         entry.info_label      = menu_hash_to_str(MENU_LABEL_CONFIGURATIONS);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);

         entry.info_label      = menu_hash_to_str(MENU_LABEL_SAVE_CURRENT_CONFIG);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);

         entry.info_label      = menu_hash_to_str(MENU_LABEL_SAVE_NEW_CONFIG);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);

         entry.info_label      = menu_hash_to_str(MENU_LABEL_HELP_LIST);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#if !defined(IOS)
         entry.info_label      = menu_hash_to_str(MENU_LABEL_QUIT_RETROARCH);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#endif
#if defined(HAVE_LAKKA)
         entry.info_label      = menu_hash_to_str(MENU_LABEL_SHUTDOWN);
         menu_displaylist_ctl(DISPLAYLIST_SETTING, &entry);
#endif
         info->need_push    = true;
         ret = 0;
         break;
   }
   return ret;
}

static size_t mui_list_get_selection(void *data)
{
   mui_handle_t *mui   = (mui_handle_t*)data;

   if (!mui)
      return 0;

   return mui->categories.selection_ptr;
}

static int mui_pointer_tap(void *userdata,
      unsigned x, unsigned y, 
      unsigned ptr, menu_file_list_cbs_t *cbs,
      menu_entry_t *entry, unsigned action)
{
   size_t selection, idx;
   unsigned header_height, width, height, i;
   bool scroll                = false;
   mui_handle_t *mui          = (mui_handle_t*)userdata;
   file_list_t *menu_stack    = menu_entries_get_menu_stack_ptr(0);
   file_list_t *selection_buf = menu_entries_get_selection_buf_ptr(0);

   if (!mui)
      return 0;

   video_driver_get_size(&width, &height);

   menu_navigation_ctl(MENU_NAVIGATION_CTL_GET_SELECTION, &selection);
   menu_display_ctl(MENU_DISPLAY_CTL_HEADER_HEIGHT, &header_height);

   if (y < header_height)
   {
      menu_entries_pop_stack(&selection, 0);
      menu_navigation_ctl(MENU_NAVIGATION_CTL_SET_SELECTION, &selection);
   }
   else if (y > height - mui->tabs_height)
   {
      for (i = 0; i <= MUI_SYSTEM_TAB_END; i++)
      {
         unsigned tab_width = width / (MUI_SYSTEM_TAB_END + 1);
         unsigned start = tab_width * i;

         if ((x >= start) && (x < (start + tab_width)))
         {
            mui->categories.selection_ptr = i;

            mui_preswitch_tabs(mui, action);

            if (cbs && cbs->action_content_list_switch)
               return cbs->action_content_list_switch(selection_buf, menu_stack,
                     "", "", 0);
         }
      }
   }
   else if (ptr <= (menu_entries_get_size() - 1))
   {
      if (ptr == selection && cbs && cbs->action_select)
         return menu_entry_action(entry, selection, MENU_ACTION_SELECT);

      idx  = ptr;

      menu_navigation_ctl(MENU_NAVIGATION_CTL_SET_SELECTION, &idx);
      menu_navigation_ctl(MENU_NAVIGATION_CTL_SET, &scroll);
   }

   return 0;
}

menu_ctx_driver_t menu_ctx_mui = {
   NULL,
   mui_get_message,
   generic_menu_iterate,
   mui_render,
   mui_frame,
   mui_init,
   mui_free,
   mui_context_reset,
   mui_context_destroy,
   mui_populate_entries,
   NULL,
   mui_navigation_clear,
   NULL,
   NULL,
   mui_navigation_set,
   mui_navigation_set_last,
   mui_navigation_alphabet,
   mui_navigation_alphabet,
   generic_menu_init_list,
   NULL,
   NULL,
   NULL,
   mui_list_cache,
   mui_list_push,
   mui_list_get_selection,
   mui_list_get_size,
   NULL,
   mui_list_set_selection,
   NULL,
   mui_load_image,
   "glui",
   mui_environ,
   mui_pointer_tap,
};
