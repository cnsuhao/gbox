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
 * @file        render.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "gl_render"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "render.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the vertex type
#if defined(GB_CONFIG_FLOAT_FIXED) && defined(GB_GL_FIXED)
#   define GB_GL_VERTEX_TYPE            GB_GL_FIXED
#elif defined(GB_CONFIG_FLOAT_FIXED)
#   define GB_GL_VERTEX_TYPE            GL_GL_INT
#else
#   define GB_GL_VERTEX_TYPE            GB_GL_FLOAT
#endif

// test tessellator
//#define GB_GL_TESSELLATOR_TEST_ENABLE   

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_gl_render_apply_vertices(gb_gl_device_ref_t device, gb_point_ref_t points)
{
    // check
    tb_assert(device && points);
 
    // apply vertices
    if (device->version >= 0x20) 
    {
        // check
        tb_assert(device->program);

        // apply it
        gb_glVertexAttribPointer(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_VERTICES), 2, GB_GL_VERTEX_TYPE, GB_GL_FALSE, 0, points);
    }
    else 
    {
        // apply it
        gb_glVertexPointer(2, GB_GL_VERTEX_TYPE, 0, points);
    }
}
static tb_void_t gb_gl_render_enter_solid(gb_gl_device_ref_t device)
{
    // check
    tb_assert(device);
 
    // the paint
    gb_paint_ref_t paint = device->base.paint;
    tb_assert(paint);

    // the color 
    gb_color_t color = gb_paint_color(paint);

    // the alpha
    tb_byte_t alpha = gb_paint_alpha(paint);

    // disable texture
    gb_glDisable(GB_GL_TEXTURE_2D);

    // exists alpha?
    if (alpha != 0xff)
    {
        // enable blend
        gb_glEnable(GB_GL_BLEND);
        gb_glBlendFunc(GB_GL_SRC_ALPHA, GB_GL_ONE_MINUS_SRC_ALPHA);

        // apply the alpha 
        color.a = alpha;
    }
    else
    {
        // disable blend
        gb_glDisable(GB_GL_BLEND);
    }

    // apply color
    if (device->version >= 0x20)
    {
        // check
        tb_assert(device->program);

        // apply it
        gb_glVertexAttrib4f(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_COLORS), (gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);
    }
    else
    {
        // apply it
        gb_glColor4f((gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);
    }
}
static tb_void_t gb_gl_render_leave_solid(gb_gl_device_ref_t device)
{    
    // check
    tb_assert(device);
 
    // disable blend
    gb_glDisable(GB_GL_BLEND);
}
static tb_void_t gb_gl_render_enter_shader(gb_gl_device_ref_t device)
{   
    // check
    tb_assert(device && device->base.paint);
 
    // disable blend
    gb_glDisable(GB_GL_BLEND);

    // enable texture
    gb_glEnable(GB_GL_TEXTURE_2D);
}
static tb_void_t gb_gl_render_leave_shader(gb_gl_device_ref_t device)
{   
    // check
    tb_assert(device);
 
    // disable texture
    gb_glDisable(GB_GL_TEXTURE_2D);
}
static tb_void_t gb_gl_render_enter_paint(gb_gl_device_ref_t device)
{
    // check
    tb_assert(device);

    // enter shader
    if (device->shader) gb_gl_render_enter_shader(device);
    // enter solid
    else gb_gl_render_enter_solid(device);
}
static tb_void_t gb_gl_render_leave_paint(gb_gl_device_ref_t device)
{
    // check
    tb_assert(device);

    // leave shader
    if (device->shader) gb_gl_render_leave_shader(device);
    // leave solid
    else gb_gl_render_leave_solid(device);
}
static tb_void_t gb_gl_render_fill_convex(gb_point_ref_t points, tb_uint16_t count, tb_cpointer_t priv)
{
    // check
    tb_assert(priv && points && count);

    // apply it
    gb_gl_render_apply_vertices((gb_gl_device_ref_t)priv, points);

#ifndef GB_GL_TESSELLATOR_TEST_ENABLE
    // draw it
    gb_glDrawArrays(GB_GL_TRIANGLE_FAN, 0, (gb_GLint_t)count);
#else
    // the device 
    gb_gl_device_ref_t device = (gb_gl_device_ref_t)priv;

    // make crc32
    tb_uint32_t crc32 = 0xffffffff ^ tb_crc_encode(TB_CRC_MODE_32_IEEE_LE, 0xffffffff, (tb_byte_t const*)points, count * sizeof(gb_point_t));

    // make color
    gb_color_t color;
    color.r = (tb_byte_t)crc32;
    color.g = (tb_byte_t)(crc32 >> 8);
    color.b = (tb_byte_t)(crc32 >> 16);
    color.a = 128;

    // enable blend
    gb_glEnable(GB_GL_BLEND);
    gb_glBlendFunc(GB_GL_SRC_ALPHA, GB_GL_ONE_MINUS_SRC_ALPHA);

    // apply color
    if (device->version >= 0x20) gb_glVertexAttrib4f(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_COLORS), (gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);
    else gb_glColor4f((gb_GLfloat_t)color.r / 0xff, (gb_GLfloat_t)color.g / 0xff, (gb_GLfloat_t)color.b / 0xff, (gb_GLfloat_t)color.a / 0xff);

    // draw the edges of the filled contour
    gb_glDrawArrays(GB_GL_TRIANGLE_FAN, 0, (gb_GLint_t)count);

    // disable blend
    gb_glEnable(GB_GL_BLEND);
#endif
}
static tb_void_t gb_gl_render_fill_polygon(gb_gl_device_ref_t device, gb_polygon_ref_t polygon, gb_rect_ref_t bounds, tb_size_t rule)
{
    // check
    tb_assert(device && device->tessellator);

#ifdef GB_GL_TESSELLATOR_TEST_ENABLE
    // set mode
    gb_tessellator_mode_set(device->tessellator, GB_TESSELLATOR_MODE_TRIANGULATION);
//    gb_tessellator_mode_set(device->tessellator, GB_TESSELLATOR_MODE_MONOTONE);
#endif

    // set rule
    gb_tessellator_rule_set(device->tessellator, rule);

    // set func
    gb_tessellator_func_set(device->tessellator, gb_gl_render_fill_convex, device);

    // done tessellator
    gb_tessellator_done(device->tessellator, polygon, bounds);
}
static tb_void_t gb_gl_render_stroke_lines(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert(device && points && count);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    gb_glDrawArrays(GB_GL_LINES, 0, (gb_GLint_t)count);
}
static tb_void_t gb_gl_render_stroke_points(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert(device && points && count);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    gb_glDrawArrays(GB_GL_POINTS, 0, (gb_GLint_t)count);
}
static tb_void_t gb_gl_render_stroke_polygon(gb_gl_device_ref_t device, gb_point_ref_t points, tb_uint16_t const* counts)
{
    // check
    tb_assert(device && points && counts);

    // apply vertices
    gb_gl_render_apply_vertices(device, points);

    // done
    tb_uint16_t count;
    tb_size_t   index = 0;
    while ((count = *counts++))
    {
        gb_glDrawArrays(GB_GL_LINE_STRIP, (gb_GLint_t)index, (gb_GLint_t)count);
        index += count;
    }
}
static tb_void_t gb_gl_render_stroke_fill(gb_gl_device_ref_t device, gb_path_ref_t path)
{
    // check
    tb_assert(device && device->stroker && device->base.paint && path);

    // null?
    tb_check_return(!gb_path_null(path));

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // the rule
    tb_size_t rule = gb_paint_fill_rule(device->base.paint);

    // switch to the fill mode
    gb_paint_mode_set(device->base.paint, GB_PAINT_MODE_FILL);

    // switch to the non-zero fill rule
    gb_paint_fill_rule_set(device->base.paint, GB_PAINT_FILL_RULE_NONZERO);

    // draw path
    gb_gl_render_draw_path(device, path);

    // restore the mode
    gb_paint_mode_set(device->base.paint, mode);

    // restore the fill mode
    gb_paint_fill_rule_set(device->base.paint, rule);
}
static __tb_inline__ tb_bool_t gb_gl_render_stroke_only(gb_gl_device_ref_t device)
{
    // check
    tb_assert(device && device->base.paint && device->base.matrix);

    // width == 1 and solid? only stroke it
    return (    GB_ONE == gb_paint_stroke_width(device->base.paint)
            &&  GB_ONE == gb_abs(device->base.matrix->sx)
            &&  GB_ONE == gb_abs(device->base.matrix->sy) 
            &&  !device->shader)? tb_true : tb_false;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_gl_render_init(gb_gl_device_ref_t device)
{
    // check
    tb_assert_and_check_return_val(device && device->base.matrix && device->base.paint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // init shader
        device->shader = gb_paint_shader(device->base.paint);

        // init vertex matrix
        gb_gl_matrix_convert(device->matrix_vertex, device->base.matrix);

        // apply matrix for the fixed vertex if no GB_GL_FIXED macro
#if defined(GB_CONFIG_FLOAT_FIXED) && !defined(GB_GL_FIXED)
        device->matrix_vertex[0] /= 65536.0f;
        device->matrix_vertex[1] /= 65536.0f;
        device->matrix_vertex[4] /= 65536.0f;
        device->matrix_vertex[5] /= 65536.0f;
#endif

        // init antialiasing
        if (gb_paint_flag(device->base.paint) & GB_PAINT_FLAG_ANTIALIASING) 
        {
            gb_glEnable(GB_GL_MULTISAMPLE);
#if 0
            gb_glEnable(GB_GL_LINE_SMOOTH);
            gb_glHint(GB_GL_LINE_SMOOTH_HINT, GB_GL_NICEST);
#endif
        }
        else gb_glDisable(GB_GL_MULTISAMPLE);

        // init vertex and matrix
        if (device->version >= 0x20)
        {   
            // the program type
            tb_size_t program_type = device->shader? GB_GL_PROGRAM_TYPE_BITMAP : GB_GL_PROGRAM_TYPE_COLOR;

            // program
            device->program = device->programs[program_type];
            tb_assert_and_check_break(device->program);

            // bind this program to the current gl context
            gb_gl_program_bind(device->program);

            // enable vertex
            gb_glEnableVertexAttribArray(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_VERTICES));

            // apply projection matrix
            gb_glUniformMatrix4fv(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_MATRIX_PROJECT), 1, GB_GL_FALSE, device->matrix_project);

            // apply vertex matrix
            gb_glUniformMatrix4fv(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_MATRIX_MODEL), 1, GB_GL_FALSE, device->matrix_vertex);
        }
        else
        {
            // enable vertex
            gb_glEnableClientState(GB_GL_VERTEX_ARRAY);
    
            // apply vertex matrix
            gb_glMatrixMode(GB_GL_MODELVIEW);
            gb_glPushMatrix();
            gb_glLoadIdentity();
            gb_glMultMatrixf(device->matrix_vertex);
        }

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
tb_void_t gb_gl_render_exit(gb_gl_device_ref_t device)
{
    // check
    tb_assert_and_check_return(device);

    // exit vertex and matrix
    if (device->version >= 0x20)
    {   
        // check
        tb_assert_and_check_return(device->program);

        // disable vertex
        gb_glDisableVertexAttribArray(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_VERTICES));
 
        // disable texcoord
        gb_glDisableVertexAttribArray(gb_gl_program_location(device->program, GB_GL_PROGRAM_LOCATION_TEXCOORDS));
    }
    else
    {
        // restore vertex matrix
        gb_glMatrixMode(GB_GL_MODELVIEW);
        gb_glPopMatrix();

        // disable vertex
        gb_glDisableClientState(GB_GL_VERTEX_ARRAY);
 
        // disable texcoord
        gb_glDisableClientState(GB_GL_TEXTURE_COORD_ARRAY);
    }
 
    // disable antialiasing
    gb_glDisable(GB_GL_MULTISAMPLE);
}
tb_void_t gb_gl_render_draw_path(gb_gl_device_ref_t device, gb_path_ref_t path)
{
    // check
    tb_assert(device && device->base.paint && path);

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // fill it
    if (mode & GB_PAINT_MODE_FILL)
    {
        gb_gl_render_draw_polygon(device, gb_path_polygon(path), gb_path_hint(path), gb_path_bounds(path));
    }

    // stroke it
    if ((mode & GB_PAINT_MODE_STROKE) && (gb_paint_stroke_width(device->base.paint) > 0))
    {
        // only stroke?
        if (gb_gl_render_stroke_only(device)) gb_gl_render_draw_polygon(device, gb_path_polygon(path), gb_path_hint(path), gb_path_bounds(path));
        // fill the stroked path
        else gb_gl_render_stroke_fill(device, gb_stroker_done_path(device->stroker, device->base.paint, path));
    }
}
tb_void_t gb_gl_render_draw_lines(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint && points && count);

    // check mode
    tb_check_return(gb_paint_mode(device->base.paint) & GB_PAINT_MODE_STROKE);

    // check width
    tb_check_return((gb_paint_stroke_width(device->base.paint) > 0));

    // enter paint
    gb_gl_render_enter_paint(device);

    // only stroke?
    if (gb_gl_render_stroke_only(device)) gb_gl_render_stroke_lines(device, points, count);
    // fill the stroked lines
    else gb_gl_render_stroke_fill(device, gb_stroker_done_lines(device->stroker, device->base.paint, points, count));

    // leave paint
    gb_gl_render_leave_paint(device);
}
tb_void_t gb_gl_render_draw_points(gb_gl_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint && points && count);

    // check mode
    tb_check_return(gb_paint_mode(device->base.paint) & GB_PAINT_MODE_STROKE);

    // check width
    tb_check_return((gb_paint_stroke_width(device->base.paint) > 0));

    // enter paint
    gb_gl_render_enter_paint(device);

    // only stroke?
    if (gb_gl_render_stroke_only(device)) gb_gl_render_stroke_points(device, points, count);
    // fill the stroked points
    else gb_gl_render_stroke_fill(device, gb_stroker_done_points(device->stroker, device->base.paint, points, count));

    // leave paint
    gb_gl_render_leave_paint(device);
}
tb_void_t gb_gl_render_draw_polygon(gb_gl_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint && polygon && polygon->points && polygon->counts);

    // line?
    if (hint && hint->type == GB_SHAPE_TYPE_LINE)
    {
        gb_point_t points[2];
        points[0] = hint->u.line.p0;
        points[1] = hint->u.line.p1;
        gb_gl_render_draw_lines(device, points, 2, bounds);
        return ;
    }
    // point?
    else if (hint && hint->type == GB_SHAPE_TYPE_POINT)
    {
        gb_gl_render_draw_points(device, &hint->u.point, 1, bounds);
        return ;
    }

    // enter paint
    gb_gl_render_enter_paint(device);

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // fill it
    if (mode & GB_PAINT_MODE_FILL)
    {
        // fill polygon
        gb_gl_render_fill_polygon(device, polygon, bounds, gb_paint_fill_rule(device->base.paint));
    }

    // stroke it
    if ((mode & GB_PAINT_MODE_STROKE) && (gb_paint_stroke_width(device->base.paint) > 0))
    {
        // only stroke?
        if (gb_gl_render_stroke_only(device)) gb_gl_render_stroke_polygon(device, polygon->points, polygon->counts);
        // fill the stroked polygon
        else gb_gl_render_stroke_fill(device, gb_stroker_done_polygon(device->stroker, device->base.paint, polygon, hint));
    }

    // leave paint
    gb_gl_render_leave_paint(device);
}

