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
 * @file        argb4444.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_ARGB4444_H
#define GB_CORE_PIXMAP_ARGB4444_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "rgb16.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

/* the alpha blend 
 *
 * c: 
 * 0000 0000 0000 0000 aaaa rrrr gggg bbbb
 *
 * c | c << 12:
 * 0000 aaaa rrrr gggg aaaa rrrr gggg bbbb
 *
 * 0x0f0f0f0f:
 * 0000 1111 0000 1111 0000 1111 0000 1111
 *
 * d = (c | c << 12) & 0x0f0f0f0f:
 * 0000 aaaa 0000 gggg 0000 rrrr 0000 bbbb
 *
 * (d & 0xffff) | (d >> 12):
 * 0000 0000 0000 0000 aaaa rrrr gggg bbbb
 *
 * (s * a + d * (32 - a)) >> 5 => ((s - d) * a) >> 5 + d
 */
static __tb_inline__ tb_uint16_t gb_pixmap_argb4444_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    // FIXME: s - d? overflow?
    s = (s | (s << 12)) & 0x0f0f0f0f;
    d = (d | (d << 12)) & 0x0f0f0f0f;
    d = ((((s - d) * a) >> 5) + d) & 0x0f0f0f0f;
    return (tb_uint16_t)((d & 0xffff) | (d >> 12));
}
static __tb_inline__ tb_uint16_t gb_pixmap_argb4444_blend2(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
    d = (d | (d << 12)) & 0x0f0f0f0f;
    d = ((((s - d) * a) >> 5) + d) & 0x0f0f0f0f;
    return (tb_uint16_t)((d & 0xffff) | (d >> 12));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ gb_pixel_t gb_pixmap_argb4444_pixel(gb_color_t color)
{
    return GB_ARGB_4444(color.a, color.r, color.g, color.b);
}
static __tb_inline__ gb_color_t gb_pixmap_argb4444_color(gb_pixel_t pixel)
{
    gb_color_t color;
    color.a = GB_ARGB_4444_A(pixel);
    color.r = GB_ARGB_4444_R(pixel);
    color.g = GB_ARGB_4444_G(pixel);
    color.b = GB_ARGB_4444_B(pixel);
    return color;
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_pixel_set_la(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb4444_blend(tb_bits_get_u16_le(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_pixel_set_ba(tb_pointer_t data, gb_pixel_t pixel, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb4444_blend(tb_bits_get_u16_be(data), pixel, alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_pixel_cpy_la(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb4444_blend(tb_bits_get_u16_le(data), tb_bits_get_u16_le(source), alpha >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_pixel_cpy_ba(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb4444_blend(tb_bits_get_u16_be(data), tb_bits_get_u16_be(source), alpha >> 3));
}
static __tb_inline__ gb_color_t gb_pixmap_argb4444_color_get_l(tb_cpointer_t data)
{
    return gb_pixmap_argb4444_color(tb_bits_get_u16_le(data));
}
static __tb_inline__ gb_color_t gb_pixmap_argb4444_color_get_b(tb_cpointer_t data)
{
    return gb_pixmap_argb4444_color(tb_bits_get_u16_be(data));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_color_set_lo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb4444_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_color_set_bo(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb4444_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_color_set_la(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_le(data, gb_pixmap_argb4444_blend(tb_bits_get_u16_le(data), gb_pixmap_argb4444_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_color_set_ba(tb_pointer_t data, gb_color_t color)
{
    tb_bits_set_u16_be(data, gb_pixmap_argb4444_blend(tb_bits_get_u16_be(data), gb_pixmap_argb4444_pixel(color), color.a >> 3));
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_pixels_fill_la(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x0f0f0f0f;
    while (p < e)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_argb4444_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        tb_bits_set_u16_le(&p[1], gb_pixmap_argb4444_blend2(tb_bits_get_u16_le(&p[1]), s, alpha));
        tb_bits_set_u16_le(&p[2], gb_pixmap_argb4444_blend2(tb_bits_get_u16_le(&p[2]), s, alpha));
        tb_bits_set_u16_le(&p[3], gb_pixmap_argb4444_blend2(tb_bits_get_u16_le(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_le(&p[0], gb_pixmap_argb4444_blend2(tb_bits_get_u16_le(&p[0]), s, alpha));
        p++;
    }
}
static __tb_inline__ tb_void_t gb_pixmap_argb4444_pixels_fill_ba(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
    tb_size_t       l = count & 0x3; count -= l; alpha >>= 3;
    tb_uint16_t*    p = (tb_uint16_t*)data;
    tb_uint16_t*    e = p + count;
    tb_uint32_t     s = (pixel | (pixel << 16)) & 0x0f0f0f0f;
    while (p < e)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_argb4444_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        tb_bits_set_u16_be(&p[1], gb_pixmap_argb4444_blend2(tb_bits_get_u16_be(&p[1]), s, alpha));
        tb_bits_set_u16_be(&p[2], gb_pixmap_argb4444_blend2(tb_bits_get_u16_be(&p[2]), s, alpha));
        tb_bits_set_u16_be(&p[3], gb_pixmap_argb4444_blend2(tb_bits_get_u16_be(&p[3]), s, alpha));
        p += 4;
    }
    while (l--)
    {
        tb_bits_set_u16_be(&p[0], gb_pixmap_argb4444_blend2(tb_bits_get_u16_be(&p[0]), s, alpha));
        p++;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_lo_argb4444 =
{   
    "argb4444"
,   16
,   2
,   GB_PIXFMT_ARGB4444 
,   gb_pixmap_argb4444_pixel
,   gb_pixmap_argb4444_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_rgb16_pixel_set_lo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_argb4444_color_get_l
,   gb_pixmap_argb4444_color_set_lo
,   gb_pixmap_rgb16_pixels_fill_lo
};

static gb_pixmap_t const g_pixmap_bo_argb4444 =
{   
    "argb4444"
,   16
,   2
,   GB_PIXFMT_ARGB4444 | GB_PIXFMT_BENDIAN
,   gb_pixmap_argb4444_pixel
,   gb_pixmap_argb4444_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_rgb16_pixel_set_bo
,   gb_pixmap_rgb16_pixel_cpy_o
,   gb_pixmap_argb4444_color_get_b
,   gb_pixmap_argb4444_color_set_bo
,   gb_pixmap_rgb16_pixels_fill_bo
};

static gb_pixmap_t const g_pixmap_la_argb4444 =
{   
    "argb4444"
,   16
,   2
,   GB_PIXFMT_ARGB4444 
,   gb_pixmap_argb4444_pixel
,   gb_pixmap_argb4444_color
,   gb_pixmap_rgb16_pixel_get_l
,   gb_pixmap_argb4444_pixel_set_la
,   gb_pixmap_argb4444_pixel_cpy_la
,   gb_pixmap_argb4444_color_get_l
,   gb_pixmap_argb4444_color_set_la
,   gb_pixmap_argb4444_pixels_fill_la
};

static gb_pixmap_t const g_pixmap_ba_argb4444 =
{   
    "argb4444"
,   16
,   2
,   GB_PIXFMT_ARGB4444 | GB_PIXFMT_BENDIAN
,   gb_pixmap_argb4444_pixel
,   gb_pixmap_argb4444_color
,   gb_pixmap_rgb16_pixel_get_b
,   gb_pixmap_argb4444_pixel_set_ba
,   gb_pixmap_argb4444_pixel_cpy_ba
,   gb_pixmap_argb4444_color_get_b
,   gb_pixmap_argb4444_color_set_ba
,   gb_pixmap_argb4444_pixels_fill_ba
};


#endif

