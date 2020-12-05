/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2014-2015 - Jean-André Santoni
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

#include <math.h>
#include <string.h>
#include <compat/strl.h>
#include <encodings/utf.h>
#include <retro_miscellaneous.h>

#include "menu_animation.h"
#include "../configuration.h"
#include "../performance.h"

#define IDEAL_DELTA_TIME (1.0 / 60.0 * 1000000.0)

struct tween
{
   bool        alive;
   float       duration;
   float       running_since;
   float       initial_value;
   float       target_value;
   float       *subject;
   int         tag;
   easing_cb   easing;
   tween_cb    cb;
};

struct menu_animation
{
   struct tween *list;

   size_t capacity;
   size_t size;
   size_t first_dead;
};

typedef struct menu_animation menu_animation_t;

/* from https://github.com/kikito/tween.lua/blob/master/tween.lua */

static float easing_linear(float t, float b, float c, float d)
{
   return c * t / d + b;
}

static float easing_in_out_quad(float t, float b, float c, float d)
{
   t = t / d * 2;
   if (t < 1)
      return c / 2 * pow(t, 2) + b;
   return -c / 2 * ((t - 1) * (t - 3) - 1) + b;
}

static float easing_in_quad(float t, float b, float c, float d)
{
   return c * pow(t / d, 2) + b;
}

static float easing_out_quad(float t, float b, float c, float d)
{
   t = t / d;
   return -c * t * (t - 2) + b;
}

static float easing_out_in_quad(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_quad(t * 2, b, c / 2, d);
   return easing_in_quad((t * 2) - d, b + c / 2, c / 2, d);
}

static float easing_in_cubic(float t, float b, float c, float d)
{
   return c * pow(t / d, 3) + b;
}

static float easing_out_cubic(float t, float b, float c, float d)
{
   return c * (pow(t / d - 1, 3) + 1) + b;
}

static float easing_in_out_cubic(float t, float b, float c, float d)
{
   t = t / d * 2;
   if (t < 1)
      return c / 2 * t * t * t + b;
   t = t - 2;
   return c / 2 * (t * t * t + 2) + b;
}

static float easing_out_in_cubic(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_cubic(t * 2, b, c / 2, d);
   return easing_in_cubic((t * 2) - d, b + c / 2, c / 2, d);
}

static float easing_in_quart(float t, float b, float c, float d)
{
   return c * pow(t / d, 4) + b;
}

static float easing_out_quart(float t, float b, float c, float d)
{
   return -c * (pow(t / d - 1, 4) - 1) + b;
}

static float easing_in_out_quart(float t, float b, float c, float d)
{
   t = t / d * 2;
   if (t < 1)
      return c / 2 * pow(t, 4) + b;
   return -c / 2 * (pow(t - 2, 4) - 2) + b;
}

static float easing_out_in_quart(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_quart(t * 2, b, c / 2, d);
   return easing_in_quart((t * 2) - d, b + c / 2, c / 2, d);
}

static float easing_in_quint(float t, float b, float c, float d)
{
   return c * pow(t / d, 5) + b;
}

static float easing_out_quint(float t, float b, float c, float d)
{
   return c * (pow(t / d - 1, 5) + 1) + b;
}

static float easing_in_out_quint(float t, float b, float c, float d)
{
   t = t / d * 2;
   if (t < 1)
      return c / 2 * pow(t, 5) + b;
   return c / 2 * (pow(t - 2, 5) + 2) + b;
}

static float easing_out_in_quint(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_quint(t * 2, b, c / 2, d);
   return easing_in_quint((t * 2) - d, b + c / 2, c / 2, d);
}

static float easing_in_sine(float t, float b, float c, float d)
{
   return -c * cos(t / d * (M_PI / 2)) + c + b;
}

static float easing_out_sine(float t, float b, float c, float d)
{
   return c * sin(t / d * (M_PI / 2)) + b;
}

static float easing_in_out_sine(float t, float b, float c, float d)
{
   return -c / 2 * (cos(M_PI * t / d) - 1) + b;
}

static float easing_out_in_sine(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_sine(t * 2, b, c / 2, d);
   return easing_in_sine((t * 2) -d, b + c / 2, c / 2, d);
}

static float easing_in_expo(float t, float b, float c, float d)
{
   if (t == 0)
      return b;
   return c * powf(2, 10 * (t / d - 1)) + b - c * 0.001;
}

static float easing_out_expo(float t, float b, float c, float d)
{
   if (t == d)
      return b + c;
   return c * 1.001 * (-powf(2, -10 * t / d) + 1) + b;
}

static float easing_in_out_expo(float t, float b, float c, float d)
{
   if (t == 0)
      return b;
   if (t == d)
      return b + c;
   t = t / d * 2;
   if (t < 1)
      return c / 2 * powf(2, 10 * (t - 1)) + b - c * 0.0005;
   return c / 2 * 1.0005 * (-powf(2, -10 * (t - 1)) + 2) + b;
}

static float easing_out_in_expo(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_expo(t * 2, b, c / 2, d);
   return easing_in_expo((t * 2) - d, b + c / 2, c / 2, d);
}

static float easing_in_circ(float t, float b, float c, float d)
{
   return(-c * (sqrt(1 - powf(t / d, 2)) - 1) + b);
}

static float easing_out_circ(float t, float b, float c, float d)
{
   return(c * sqrt(1 - powf(t / d - 1, 2)) + b);
}

static float easing_in_out_circ(float t, float b, float c, float d)
{
   t = t / d * 2;
   if (t < 1)
      return -c / 2 * (sqrt(1 - t * t) - 1) + b;
   t = t - 2;
   return c / 2 * (sqrt(1 - t * t) + 1) + b;
}

static float easing_out_in_circ(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_circ(t * 2, b, c / 2, d);
   return easing_in_circ((t * 2) - d, b + c / 2, c / 2, d);
}

static float easing_out_bounce(float t, float b, float c, float d)
{
   t = t / d;
   if (t < 1 / 2.75)
      return c * (7.5625 * t * t) + b;
   if (t < 2 / 2.75)
   {
      t = t - (1.5 / 2.75);
      return c * (7.5625 * t * t + 0.75) + b;
   }
   else if (t < 2.5 / 2.75)
   {
      t = t - (2.25 / 2.75);
      return c * (7.5625 * t * t + 0.9375) + b;
   }
   t = t - (2.625 / 2.75);
   return c * (7.5625 * t * t + 0.984375) + b;
}

static float easing_in_bounce(float t, float b, float c, float d)
{
   return c - easing_out_bounce(d - t, 0, c, d) + b;
}

static float easing_in_out_bounce(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_in_bounce(t * 2, 0, c, d) * 0.5 + b;
   return easing_out_bounce(t * 2 - d, 0, c, d) * 0.5 + c * .5 + b;
}

static float easing_out_in_bounce(float t, float b, float c, float d)
{
   if (t < d / 2)
      return easing_out_bounce(t * 2, b, c / 2, d);
   return easing_in_bounce((t * 2) - d, b + c / 2, c / 2, d);
}

static int menu_animation_iterate(menu_animation_t *anim,
      unsigned idx, float dt, unsigned *active_tweens)
{
   struct tween *tween = &anim->list[idx];

   if (!tween || !tween->alive)
      return -1;

   tween->running_since += dt;

   *tween->subject = tween->easing(
            tween->running_since,
            tween->initial_value,
            tween->target_value - tween->initial_value,
            tween->duration);

   if (tween->running_since >= tween->duration)
   {
      *tween->subject = tween->target_value;
      tween->alive    = false;

      if (idx < anim->first_dead)
         anim->first_dead = idx;

      if (tween->cb)
         tween->cb();
   }

   if (tween->running_since < tween->duration)
      *active_tweens += 1;

   return 0;
}

static void menu_animation_ticker_generic(uint64_t idx,
      size_t max_width, size_t *offset, size_t *width)
{
   int ticker_period, phase, phase_left_stop;
   int phase_left_moving, phase_right_stop;
   int left_offset, right_offset;

   *offset = 0;

   if (*width <= max_width)
      return;

   ticker_period     = 2 * (*width - max_width) + 4;
   phase             = idx % ticker_period;

   phase_left_stop   = 2;
   phase_left_moving = phase_left_stop + (*width - max_width);
   phase_right_stop  = phase_left_moving + 2;

   left_offset       = phase - phase_left_stop;
   right_offset      = (*width - max_width) - (phase - phase_right_stop);

   if (phase < phase_left_stop)
      *offset = 0;
   else if (phase < phase_left_moving)
      *offset = left_offset;
   else if (phase < phase_right_stop)
      *offset = *width - max_width;
   else
      *offset = right_offset;

   *width = max_width;
}

static void menu_animation_push_internal(menu_animation_t *anim,
      const struct tween *t)
{
   struct tween *target = NULL;

   if (anim->first_dead < anim->size && !anim->list[anim->first_dead].alive)
      target = &anim->list[anim->first_dead++];
   else
   {
      if (anim->size >= anim->capacity)
      {
         anim->capacity++;
         anim->list = (struct tween*)realloc(anim->list,
               anim->capacity * sizeof(struct tween));
      }

      target = &anim->list[anim->size++];
   }

   *target = *t;
}

static bool menu_animation_push(menu_animation_t *anim, void *data)
{
   struct tween t;
   menu_animation_ctx_entry_t *entry = 
      (menu_animation_ctx_entry_t*)data;

   if (!entry || !entry->subject)
      return false;

   t.alive         = true;
   t.duration      = entry->duration;
   t.running_since = 0;
   t.initial_value = *entry->subject;
   t.target_value  = entry->target_value;
   t.subject       = entry->subject;
   t.tag           = entry->tag;
   t.cb            = entry->cb;
   t.easing        = NULL;

   switch (entry->easing_enum)
   {
      case EASING_LINEAR:
         t.easing        = &easing_linear;
         break;
         /* Quad */
      case EASING_IN_QUAD:
         t.easing        = &easing_in_quad;
         break;
      case EASING_OUT_QUAD:
         t.easing        = &easing_out_quad;
         break;
      case EASING_IN_OUT_QUAD:
         t.easing        = &easing_in_out_quad;
         break;
      case EASING_OUT_IN_QUAD:
         t.easing        = &easing_out_in_quad;
         break;
         /* Cubic */
      case EASING_IN_CUBIC:
         t.easing        = &easing_in_cubic;
         break;
      case EASING_OUT_CUBIC:
         t.easing        = &easing_out_cubic;
         break;
      case EASING_IN_OUT_CUBIC:
         t.easing        = &easing_in_out_cubic;
         break;
      case EASING_OUT_IN_CUBIC:
         t.easing        = &easing_out_in_cubic;
         break;
         /* Quart */
      case EASING_IN_QUART:
         t.easing        = &easing_in_quart;
         break;
      case EASING_OUT_QUART:
         t.easing        = &easing_out_quart;
         break;
      case EASING_IN_OUT_QUART:
         t.easing        = &easing_in_out_quart;
         break;
      case EASING_OUT_IN_QUART:
         t.easing        = &easing_out_in_quart;
         break;
         /* Quint */
      case EASING_IN_QUINT:
         t.easing        = &easing_in_quint;
         break;
      case EASING_OUT_QUINT:
         t.easing        = &easing_out_quint;
         break;
      case EASING_IN_OUT_QUINT:
         t.easing        = &easing_in_out_quint;
         break;
      case EASING_OUT_IN_QUINT:
         t.easing        = &easing_out_in_quint;
         break;
         /* Sine */
      case EASING_IN_SINE:
         t.easing        = &easing_in_sine;
         break;
      case EASING_OUT_SINE:
         t.easing        = &easing_out_sine;
         break;
      case EASING_IN_OUT_SINE:
         t.easing        = &easing_in_out_sine;
         break;
      case EASING_OUT_IN_SINE:
         t.easing        = &easing_out_in_sine;
         break;
         /* Expo */
      case EASING_IN_EXPO:
         t.easing        = &easing_in_expo;
         break;
      case EASING_OUT_EXPO:
         t.easing        = &easing_out_expo;
         break;
      case EASING_IN_OUT_EXPO:
         t.easing        = &easing_in_out_expo;
         break;
      case EASING_OUT_IN_EXPO:
         t.easing        = &easing_out_in_expo;
         break;
         /* Circ */
      case EASING_IN_CIRC:
         t.easing        = &easing_in_circ;
         break;
      case EASING_OUT_CIRC:
         t.easing        = &easing_out_circ;
         break;
      case EASING_IN_OUT_CIRC:
         t.easing        = &easing_in_out_circ;
         break;
      case EASING_OUT_IN_CIRC:
         t.easing        = &easing_out_in_circ;
         break;
         /* Bounce */
      case EASING_IN_BOUNCE:
         t.easing        = &easing_in_bounce;
         break;
      case EASING_OUT_BOUNCE:
         t.easing        = &easing_out_bounce;
         break;
      case EASING_IN_OUT_BOUNCE:
         t.easing        = &easing_in_out_bounce;
         break;
      case EASING_OUT_IN_BOUNCE:
         t.easing        = &easing_out_in_bounce;
         break;
      default:
         break;
   }

   /* ignore born dead tweens */
   if (!t.easing || t.duration == 0 || t.initial_value == t.target_value)
      return false;

   menu_animation_push_internal(anim, &t);

   return true;
}


bool menu_animation_ctl(enum menu_animation_ctl_state state, void *data)
{
   static menu_animation_t anim;
   static retro_time_t cur_time    = 0;
   static retro_time_t old_time    = 0;
   static float delta_time         = 0.0f;
   static bool animation_is_active = false;

   switch (state)
   {
      case MENU_ANIMATION_CTL_DEINIT:
         {
            size_t i;

            for (i = 0; i < anim.size; i++)
            {
               if (anim.list[i].subject)
                  anim.list[i].subject = NULL;
            }

            free(anim.list);

            memset(&anim, 0, sizeof(menu_animation_t));
         }
         cur_time                  = 0;
         old_time                  = 0;
         delta_time                = 0.0f;
         break;
      case MENU_ANIMATION_CTL_IS_ACTIVE:
         return animation_is_active;
      case MENU_ANIMATION_CTL_CLEAR_ACTIVE:
         animation_is_active       = false;
         break;
      case MENU_ANIMATION_CTL_SET_ACTIVE:
         animation_is_active       = true;
         break;
      case MENU_ANIMATION_CTL_DELTA_TIME:
         {
            float *ptr = (float*)data;
            if (!ptr)
               return false;
            *ptr = delta_time;
         }
         break;
      case MENU_ANIMATION_CTL_UPDATE_TIME:
         {
            static retro_time_t last_clock_update = 0;
            settings_t *settings     = config_get_ptr();

            cur_time                 = retro_get_time_usec();
            delta_time               = cur_time - old_time;

            if (delta_time >= IDEAL_DELTA_TIME* 4)
               delta_time = IDEAL_DELTA_TIME * 4;
            if (delta_time <= IDEAL_DELTA_TIME / 4)
               delta_time = IDEAL_DELTA_TIME / 4;
            old_time      = cur_time;

            if (((cur_time - last_clock_update) > 1000000) 
                  && settings->menu.timedate_enable)
            {
               animation_is_active   = true;
               last_clock_update     = cur_time;
            }
         }
         break;
      case MENU_ANIMATION_CTL_UPDATE:
         {
            unsigned i;
            unsigned active_tweens = 0;
            float *dt = (float*)data;

            if (!dt)
               return false;

            for(i = 0; i < anim.size; i++)
               menu_animation_iterate(&anim, i, *dt, &active_tweens);

            if (!active_tweens)
            {
               anim.size       = 0;
               anim.first_dead = 0;
               return false;
            }

            animation_is_active = true;
         }
         break;
      case MENU_ANIMATION_CTL_KILL_BY_TAG:
         {
            unsigned i;
            menu_animation_ctx_tag_t *tag = (menu_animation_ctx_tag_t*)data;

            if (!tag || tag->id == -1)
               return false;

            for (i = 0; i < anim.size; ++i)
            {
               if (anim.list[i].tag != tag->id)
                  continue;

               anim.list[i].alive   = false;
               anim.list[i].subject = NULL;

               if (i < anim.first_dead)
                  anim.first_dead = i;
            }
         }
         break;
      case MENU_ANIMATION_CTL_KILL_BY_SUBJECT:
         {
            unsigned i, j,  killed = 0;
            menu_animation_ctx_subject_t *subject = 
               (menu_animation_ctx_subject_t*)data;
            float            **sub = (float**)subject->data;

            for (i = 0; i < anim.size; ++i)
            {
               if (!anim.list[i].alive)
                  continue;

               for (j = 0; j < subject->count; ++j)
               {
                  if (anim.list[i].subject != sub[j])
                     continue;

                  anim.list[i].alive   = false;
                  anim.list[i].subject = NULL;

                  if (i < anim.first_dead)
                     anim.first_dead = i;

                  killed++;
                  break;
               }
            }
         }
         break;
      case MENU_ANIMATION_CTL_TICKER:
         {
            menu_animation_ctx_ticker_t *ticker = (menu_animation_ctx_ticker_t*)
               data;
            size_t           str_len = utf8len(ticker->str);
            size_t           offset  = 0;

            if ((size_t)str_len <= ticker->len)
            {
               utf8cpy(ticker->s,
                     PATH_MAX_LENGTH,
                     ticker->str,
                     ticker->len);
               return true;
            }

            if (!ticker->selected)
            {
               utf8cpy(ticker->s, PATH_MAX_LENGTH, ticker->str, ticker->len - 3);
               strlcat(ticker->s, "...", PATH_MAX_LENGTH);
               return true;
            }

            menu_animation_ticker_generic(
                  ticker->idx,
                  ticker->len,
                  &offset,
                  &str_len);

            utf8cpy(
                  ticker->s,
                  PATH_MAX_LENGTH,
                  utf8skip(ticker->str, offset),
                  str_len);

            animation_is_active = true;
         }
         break;
      case MENU_ANIMATION_CTL_IDEAL_DELTA_TIME_GET:
         {
            menu_animation_ctx_delta_t *delta = 
               (menu_animation_ctx_delta_t*)data;
            if (!delta)
               return false;
            delta->ideal = delta->current / IDEAL_DELTA_TIME;
         }
         break;
      case MENU_ANIMATION_CTL_PUSH:
         return menu_animation_push(&anim, data);
      case MENU_ANIMATION_CTL_NONE:
      default:
         break;
   }

   return true;
}
