/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2016 - Hans-Kristian Arntzen
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

#ifndef GLSLANG_UTIL_HPP
#define GLSLANG_UTIL_HPP

#include <stdint.h>
#include <vector>

struct glslang_output
{
   std::vector<uint32_t> vertex;
   std::vector<uint32_t> fragment;
};

bool glslang_compile_shader(const char *shader_path, glslang_output *output);


#endif

