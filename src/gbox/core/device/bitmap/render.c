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
#define TB_TRACE_MODULE_NAME            "bitmap_render"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "render.h"
#include "biltter.h"
#include "render/render.h"
#include "../../impl/bounds.h"
#include "../../impl/stroker.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_bitmap_render_apply_matrix_for_hint(gb_bitmap_device_ref_t device, gb_shape_ref_t hint, gb_shape_ref_t output)
{
    // check
    tb_assert(device && device->base.matrix && output);

    // clear output first
    output->type = GB_SHAPE_TYPE_NONE;

    // rect and no rotation?
    if (    hint
        &&  hint->type == GB_SHAPE_TYPE_RECT
        &&  0 == device->base.matrix->kx && 0 == device->base.matrix->ky)
    {
        // apply matrix to rect
        gb_rect_apply2(&hint->u.rect, &output->u.rect, device->base.matrix);

        // mark the output hint type
        output->type = GB_SHAPE_TYPE_RECT;
    }

    // ok?
    return output->type != GB_SHAPE_TYPE_NONE;
}
static tb_size_t gb_bitmap_render_apply_matrix_for_points(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_point_ref_t* output)
{
    // check
    tb_assert(device && device->points && device->base.matrix && points);

    // clear points
    tb_vector_clear(device->points);

    // done
    tb_size_t index = 0;
    for (index = 0; index < count; index++)
    {
        // apply to point
        gb_point_t point;
        gb_point_apply2(points + index, &point, device->base.matrix);

        // append point
        tb_vector_insert_tail(device->points, &point);
    }

    // save points
    if (output) *output = (gb_point_ref_t)tb_vector_data(device->points);
    tb_assert(*output);

    // the points count
    return tb_vector_size(device->points);
}
static tb_size_t gb_bitmap_render_apply_matrix_for_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_point_ref_t* output)
{
    // check
    tb_assert(device && device->points && device->base.matrix && polygon && polygon->points);

    // clear points
    tb_vector_clear(device->points);

    // done
    gb_point_ref_t  points = polygon->points;
    tb_uint16_t*    counts = polygon->counts;
    tb_uint16_t     count = *counts++;
    tb_uint16_t     index = 0;
    while (index < count)
    {
        // apply to point
        gb_point_t point;
        gb_point_apply2(points++, &point, device->base.matrix);

        // append point
        tb_vector_insert_tail(device->points, &point);
        
        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // next
            count = *counts++;
            index = 0;
        }
    }

    // save points
    if (output) *output = (gb_point_ref_t)tb_vector_data(device->points);
    tb_assert(*output);

    // the points count
    return tb_vector_size(device->points);
}
static gb_rect_ref_t gb_bitmap_render_make_bounds_for_points(gb_bitmap_device_ref_t device, gb_rect_ref_t bounds, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert(device && device->base.matrix && points && count);

    // make approximate bounds using matrix
    if (bounds)
    {
        gb_point_t pt[4];
        gb_point_make(&pt[0], bounds->x, bounds->y);
        gb_point_make(&pt[1], bounds->x, bounds->y + bounds->h);
        gb_point_make(&pt[2], bounds->x + bounds->w, bounds->y + bounds->h);
        gb_point_make(&pt[3], bounds->x + bounds->w, bounds->y);
        gb_matrix_apply_points(device->base.matrix, pt, tb_arrayn(pt));
        gb_bounds_make(&device->bounds, pt, tb_arrayn(pt));
    }
    // make accurate bounds
    else gb_bounds_make(&device->bounds, points, count);

    // ok?
    return &device->bounds;
}
static tb_void_t gb_bitmap_render_stroke_fill(gb_bitmap_device_ref_t device, gb_path_ref_t path)
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
    gb_bitmap_render_draw_path(device, path);

    // restore the mode
    gb_paint_mode_set(device->base.paint, mode);

    // restore the fill mode
    gb_paint_fill_rule_set(device->base.paint, rule);
}
static __tb_inline__ tb_bool_t gb_bitmap_render_stroke_only(gb_bitmap_device_ref_t device)
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
tb_bool_t gb_bitmap_render_init(gb_bitmap_device_ref_t device)
{
    // check
    tb_assert_and_check_return_val(device && device->base.matrix && device->base.paint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // init shader
        device->shader = gb_paint_shader(device->base.paint);

        // init biltter
        if (!gb_bitmap_biltter_init(&device->biltter, device->bitmap, device->base.paint)) break;

        // ok
        ok = tb_true;

    } while (0);

    // failed? exit it
    if (!ok) gb_bitmap_render_exit(device);

    // ok?
    return ok;
}
tb_void_t gb_bitmap_render_exit(gb_bitmap_device_ref_t device)
{
    // check
    tb_assert_and_check_return(device);

    // exit biltter
    gb_bitmap_biltter_exit(&device->biltter);
}
tb_void_t gb_bitmap_render_draw_path(gb_bitmap_device_ref_t device, gb_path_ref_t path)
{
    // check
    tb_assert(device && device->base.paint && path);

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // fill it
    if (mode & GB_PAINT_MODE_FILL)
    {
        gb_bitmap_render_draw_polygon(device, gb_path_polygon(path), gb_path_hint(path), gb_path_bounds(path));
    }

    // stroke it
    if ((mode & GB_PAINT_MODE_STROKE) && (gb_paint_stroke_width(device->base.paint) > 0))
    {
        // only stroke?
        if (gb_bitmap_render_stroke_only(device))
        {
            gb_bitmap_render_draw_polygon(device, gb_path_polygon(path), gb_path_hint(path), gb_path_bounds(path));
        }
        // fill the stroked path
        else gb_bitmap_render_stroke_fill(device, gb_stroker_done_path(device->stroker, device->base.paint, path));
    }
}
tb_void_t gb_bitmap_render_draw_lines(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint && device->base.matrix && points && count && !(count & 0x1));

    // check mode
    tb_check_return(gb_paint_mode(device->base.paint) & GB_PAINT_MODE_STROKE);

    // check width
    tb_check_return((gb_paint_stroke_width(device->base.paint) > 0));

    // only stroke?
    if (gb_bitmap_render_stroke_only(device))
    {
        // apply matrix to points
        gb_point_ref_t  stroked_points  = tb_null;
        tb_size_t       stroked_count   = gb_bitmap_render_apply_matrix_for_points(device, points, count, &stroked_points);
        tb_assert(stroked_points && stroked_count);

        // TODO: clip it
        // ...

        // stroke lines
        gb_bitmap_render_stroke_lines(device, stroked_points, stroked_count);
    }
    // fill the stroked lines
    else gb_bitmap_render_stroke_fill(device, gb_stroker_done_lines(device->stroker, device->base.paint, points, count));
}
tb_void_t gb_bitmap_render_draw_points(gb_bitmap_device_ref_t device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint && device->base.matrix && points && count);

    // check mode
    tb_check_return(gb_paint_mode(device->base.paint) & GB_PAINT_MODE_STROKE);

    // check width
    tb_check_return((gb_paint_stroke_width(device->base.paint) > 0));

    // only stroke?
    if (gb_bitmap_render_stroke_only(device))
    {
        // apply matrix to points
        gb_point_ref_t  stroked_points  = tb_null;
        tb_size_t       stroked_count   = gb_bitmap_render_apply_matrix_for_points(device, points, count, &stroked_points);
        tb_assert(stroked_points && stroked_count);

        // TODO: clip it
        // ...

        // stroke points
        gb_bitmap_render_stroke_points(device, stroked_points, stroked_count);
    }
    // fill the stroked points
    else gb_bitmap_render_stroke_fill(device, gb_stroker_done_points(device->stroker, device->base.paint, points, count));
}
tb_void_t gb_bitmap_render_draw_polygon(gb_bitmap_device_ref_t device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    tb_assert(device && device->base.paint && polygon);

    // line?
    if (hint && hint->type == GB_SHAPE_TYPE_LINE)
    {
        gb_point_t points[2];
        points[0] = hint->u.line.p0;
        points[1] = hint->u.line.p1;
        gb_bitmap_render_draw_lines(device, points, 2, bounds);
        return ;
    }
    // point?
    else if (hint && hint->type == GB_SHAPE_TYPE_POINT)
    {
        gb_bitmap_render_draw_points(device, &hint->u.point, 1, bounds);
        return ;
    }

    // the mode
    tb_size_t mode = gb_paint_mode(device->base.paint);

    // fill it
    if (mode & GB_PAINT_MODE_FILL)
    {
        // apply matrix to points
        gb_polygon_t    filled_polygon = {tb_null, polygon->counts, polygon->convex};
        tb_size_t       filled_count   = gb_bitmap_render_apply_matrix_for_polygon(device, polygon, &filled_polygon.points);
        tb_assert(filled_polygon.points && filled_count);

        // make the filled bounds
        gb_rect_ref_t   filled_bounds = gb_bitmap_render_make_bounds_for_points(device, bounds, filled_polygon.points, filled_count);
        tb_assert(filled_bounds);

        // TODO: clip it
        tb_bool_t       clipped = tb_false;

        // apply matrix to hint
        gb_shape_t      filled_hint;
        if (!clipped && gb_bitmap_render_apply_matrix_for_hint(device, hint, &filled_hint))
        {
            // check
            tb_assert(filled_hint.type == GB_SHAPE_TYPE_RECT);

            // fill rect
            gb_bitmap_render_fill_rect(device, &filled_hint.u.rect);
        }
        // fill polygon
        else gb_bitmap_render_fill_polygon(device, &filled_polygon, filled_bounds);
    }

    // stroke it
    if ((mode & GB_PAINT_MODE_STROKE) && (gb_paint_stroke_width(device->base.paint) > 0))
    {
        // only stroke?
        if (gb_bitmap_render_stroke_only(device))
        {
            // apply matrix to points
            gb_polygon_t    stroked_polygon = {tb_null, polygon->counts, polygon->convex};
            tb_size_t       stroked_count   = gb_bitmap_render_apply_matrix_for_polygon(device, polygon, &stroked_polygon.points);
            tb_assert(stroked_polygon.points && stroked_count);

            // TODO: clip it
            // ...

            // stroke polygon
            if (stroked_count) gb_bitmap_render_stroke_polygon(device, &stroked_polygon);
        }
        // fill the stroked polygon
        else gb_bitmap_render_stroke_fill(device, gb_stroker_done_polygon(device->stroker, device->base.paint, polygon, hint));
    }
}

