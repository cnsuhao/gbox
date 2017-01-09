/*!The Graphic Box Library
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2017, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        rgb32.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGB32_H
#define GB_CORE_PIXMAP_RGB32_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

static __tb_inline__ tb_uint32_t gb_pixmap_rgb32_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    tb_uint32_t hs = (s >> 8) & 0x00ff00ff;
    tb_uint32_t hd = (d >> 8) & 0x00ff00ff;
    tb_uint32_t ls = s & 0x00ff00ff;
    tb_uint32_t ld = d & 0x00ff00ff;
    hd = (((a * (hs - hd)) >> 8) + hd) & 0x00ff00ff;
    ld = (((a * (ls - ld)) >> 8) + ld) & 0x00ff00ff;
    return (hd << 8) | ld;
}
static __tb_inline__ tb_uint32_t gb_pixmap_rgb32_blend2(tb_uint32_t d, tb_uint32_t hs, tb_uint32_t ls, tb_byte_t a)
{
    tb_uint32_t hd = (d >> 8) & 0x00ff00ff;
    tb_uint32_t ld = d & 0x00ff00ff;
    hd = (((a * (hs - hd)) >> 8) + hd) & 0x00ff00ff;
    ld = (((a * (ls - ld)) >> 8) + ld) & 0x00ff00ff;
    return (hd << 8) | ld;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgb32_pixel(gb_color_t color)
{
    return gb_color_pixel(color);
}
static __tb_inline__ gb_color_t gb_pixmap_rgb32_color(gb_pixel_t pixel)
{
    return gb_pixel_color(pixel);
}
static __tb_inline__ gb_pixel_t gb_pixmap_rgb32_pixel_get_l(tb_cpointer_t data)
{
    return tb_bits_get_u32_le(data);
}
static __tb_inline__ gb_pixel_t gb_pixmap_rgb32_pixel_get_b(tb_cpointer_t data)
{
    return tb_bits_get_u32_be(data);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixel_set_lo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u32_le(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixel_set_bo(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u32_be(data, pixel);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixel_cpy_o(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    *((tb_uint32_t*)data) = *((tb_uint32_t*)source);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixels_fill_lo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u32(data, tb_bits_ne_to_le_u32(pixel), count);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb32_pixels_fill_bo(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_memset_u32(data, tb_bits_ne_to_be_u32(pixel), count);
}


#endif

