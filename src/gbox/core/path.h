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
 * @file        path.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PATH_H
#define GB_CORE_PATH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the path code enum
 *
 * the point count: |code - 1|: 1 0 1 2 3
 */
typedef enum __gb_path_code_e
{
    GB_PATH_CODE_MOVE       = 0 //!< the move-to code
,   GB_PATH_CODE_CLOS       = 1 //!< the clos code
,   GB_PATH_CODE_LINE       = 2 //!< the line-to code
,   GB_PATH_CODE_QUAD       = 3 //!< the quad-to code
,   GB_PATH_CODE_CUBIC      = 4 //!< the cubic-to code
,   GB_PATH_CODE_MAXN       = 5 //!< the code max count

}gb_path_code_e;

/// the path item for the iterator
typedef struct __gb_path_item_t
{
    /// the code
    tb_size_t       code;

    /*! the points[4]
     *
     * move-to:     points[0]
     * line-to:     points[0], points[1]
     * quad-to:     points[0], points[1], points[2]
     * cubic-to:    points[0], points[1], points[2], points[3]
     * close:       points[0]
     */
    gb_point_ref_t  points;

}gb_path_item_t, *gb_path_item_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init path
 *
 * @return          the path
 */
gb_path_ref_t       gb_path_init(tb_noarg_t);

/*! exit path
 *
 * @param path      the path
 */
tb_void_t           gb_path_exit(gb_path_ref_t path);

/*! clear path
 *
 * @param path      the path
 */
tb_void_t           gb_path_clear(gb_path_ref_t path);

/*! copy path
 *
 * @param path      the path
 * @param copied    the copied path
 */
tb_void_t           gb_path_copy(gb_path_ref_t path, gb_path_ref_t copied);

/*! is null path?
 *
 * @param path      the path
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_path_null(gb_path_ref_t path);

/*! the path bounds
 *
 * @param path      the path
 *
 * @return          the bounds
 */
gb_rect_ref_t       gb_path_bounds(gb_path_ref_t path);

/*! the path convex?
 *
 * @param path      the path
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_path_convex(gb_path_ref_t path);

/*! set to be convex path, convex path may draw faster
 *
 * @param path      the path
 * @param convex    is convex?
 *
 */
tb_void_t           gb_path_convex_set(gb_path_ref_t path, tb_bool_t convex);

/*! get the last point
 *
 * @param path      the path
 * @param point     the last point
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_path_last(gb_path_ref_t path, gb_point_ref_t point);

/*! set the last point
 *
 * @param path      the path
 * @param point     the last point
 */
tb_void_t           gb_path_last_set(gb_path_ref_t path, gb_point_ref_t point);

/*! the path hint shape
 *
 * @param path      the path
 *
 * @return          the hint shape 
 */
gb_shape_ref_t      gb_path_hint(gb_path_ref_t path);

/*! the path polygon 
 *
 * @param path      the path
 *
 * @return          the polygon
 */
gb_polygon_ref_t    gb_path_polygon(gb_path_ref_t path);

/*! apply the matrix to the path 
 *
 * @param path      the path
 * @param matrix    the matrix
 */
tb_void_t           gb_path_apply(gb_path_ref_t path, gb_matrix_ref_t matrix);

/*! close the current contour
 *
 * if the current point is not equal to the first point of the contour
 * patch a line segment automatically
 *
 * @param path      the path
 */
tb_void_t           gb_path_clos(gb_path_ref_t path);

/*! move to the point
 *
 * @param path      the path
 * @param point     the point
 */
tb_void_t           gb_path_move_to(gb_path_ref_t path, gb_point_ref_t point);

/*! move to the point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_move2_to(gb_path_ref_t path, gb_float_t x, gb_float_t y);

/*! move to the integer point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_move2i_to(gb_path_ref_t path, tb_long_t x, tb_long_t y);

/*! line to the point
 *
 * @param path      the path
 * @param point     the point
 */
tb_void_t           gb_path_line_to(gb_path_ref_t path, gb_point_ref_t point);

/*! line to the point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_line2_to(gb_path_ref_t path, gb_float_t x, gb_float_t y);

/*! line to the integer point(x, y)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_line2i_to(gb_path_ref_t path, tb_long_t x, tb_long_t y);

/*! quad to the ctrl, point
 *
 * @param path      the path
 * @param ctrl      the control point
 * @param point     the point
 */
tb_void_t           gb_path_quad_to(gb_path_ref_t path, gb_point_ref_t ctrl, gb_point_ref_t point);

/*! quad to the ctrl(cx, cy), point(x, y)
 *
 * @param path      the path
 * @param cx        the control x-coordinate
 * @param cy        the control y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_quad2_to(gb_path_ref_t path, gb_float_t cx, gb_float_t cy, gb_float_t x, gb_float_t y);

/*! quad to the integer ctrl(cx, cy), point(x, y)
 *
 * @param path      the path
 * @param cx        the control x-coordinate
 * @param cy        the control y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_quad2i_to(gb_path_ref_t path, tb_long_t cx, tb_long_t cy, tb_long_t x, tb_long_t y);

/*! cubic to the ctrl0, ctrl1, point
 *
 * @param path      the path
 * @param ctrl0     the control0 point
 * @param ctrl1     the control1 point
 * @param point     the point
 */
tb_void_t           gb_path_cubic_to(gb_path_ref_t path, gb_point_ref_t ctrl0, gb_point_ref_t ctrl1, gb_point_ref_t point);

/*! cubic to the ctrl0(cx0, cy0), ctrl1(cx1, cy1), point(x, y)
 *
 * @param path      the path
 * @param cx0       the control0 x-coordinate
 * @param cy0       the control0 y-coordinate
 * @param cx1       the control1 x-coordinate
 * @param cy1       the control1 y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_cubic2_to(gb_path_ref_t path, gb_float_t cx0, gb_float_t cy0, gb_float_t cx1, gb_float_t cy1, gb_float_t x, gb_float_t y);

/*! cubic to the integer ctrl0(cx0, cy0), ctrl1(cx1, cy1), point(x, y)
 *
 * @param path      the path
 * @param cx0       the control0 x-coordinate
 * @param cy0       the control0 y-coordinate
 * @param cx1       the control1 x-coordinate
 * @param cy1       the control1 y-coordinate
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 */
tb_void_t           gb_path_cubic2i_to(gb_path_ref_t path, tb_long_t cx0, tb_long_t cy0, tb_long_t cx1, tb_long_t cy1, tb_long_t x, tb_long_t y);

/*! arc to the arc
 *
 * @param path      the path
 * @param arc       the arc
 */
tb_void_t           gb_path_arc_to(gb_path_ref_t path, gb_arc_ref_t arc);

/*! arc to the arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_arc2_to(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! arc to the integer arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_arc2i_to(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/*! add path and ignore the first point
 *
 * @param path      the path
 * @param added     the added path
 */
tb_void_t           gb_path_path_to(gb_path_ref_t path, gb_path_ref_t added);

/*! add path in reverse order and ignore the last point
 *
 * @param path      the path
 * @param added     the added path
 */
tb_void_t           gb_path_rpath_to(gb_path_ref_t path, gb_path_ref_t added);

/*! add path
 *
 * @param path      the path
 * @param added     the added path
 */
tb_void_t           gb_path_add_path(gb_path_ref_t path, gb_path_ref_t added);

/*! add path in reverse order
 *
 * @param path      the path
 * @param added     the added path
 */
tb_void_t           gb_path_add_rpath(gb_path_ref_t path, gb_path_ref_t added);

/*! add line
 *
 * @param path      the path
 * @param line      the line
 */
tb_void_t           gb_path_add_line(gb_path_ref_t path, gb_line_ref_t line);

/*! add line(x0, y0, x1, y1)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 */
tb_void_t           gb_path_add_line2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1);

/*! add integer line(x0, y0, x1, y1)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 */
tb_void_t           gb_path_add_line2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1);

/*! add arc
 *
 * @param path      the path
 * @param arc       the arc
 */
tb_void_t           gb_path_add_arc(gb_path_ref_t path, gb_arc_ref_t arc);

/*! add arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_add_arc2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an);

/*! add integer arc(x0, y0, rx, ry, ab, an)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param ab        the start angle
 * @param an        the sweep angle, 0 - 360
 */
tb_void_t           gb_path_add_arc2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an);

/*! add triangle
 *
 * @param path      the path
 * @param triangle  the triangle
 */
tb_void_t           gb_path_add_triangle(gb_path_ref_t path, gb_triangle_ref_t triangle);

/*! add triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_path_add_triangle2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2);

/*! add integer triangle(x0, y0, x1, y1, x2, y2)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param x1        the x1-coordinate
 * @param y1        the y1-coordinate
 * @param x2        the x2-coordinate
 * @param y2        the y2-coordinate
 */
tb_void_t           gb_path_add_triangle2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2);

/*! add rect
 *
 * @param path      the path
 * @param rect      the rect
 * @param direction the direction
 */
tb_void_t           gb_path_add_rect(gb_path_ref_t path, gb_rect_ref_t rect, tb_size_t direction);

/*! add rect(x, y, w, h)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 * @param direction the direction
 */
tb_void_t           gb_path_add_rect2(gb_path_ref_t path, gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h, tb_size_t direction);

/*! add integer rect(x, y, w, h)
 *
 * @param path      the path
 * @param x         the x-coordinate
 * @param y         the y-coordinate
 * @param w         the width
 * @param h         the height
 * @param direction the direction
 */
tb_void_t           gb_path_add_rect2i(gb_path_ref_t path, tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h, tb_size_t direction);

/*! add round rect
 *
 * @param path      the path
 * @param rect      the rect
 * @param direction the direction
 */
tb_void_t           gb_path_add_round_rect(gb_path_ref_t path, gb_round_rect_ref_t rect, tb_size_t direction);

/*! add round rect(x, y, w, h)
 *
 * @param path      the path
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_round_rect2(gb_path_ref_t path, gb_rect_ref_t bounds, gb_float_t rx, gb_float_t ry, tb_size_t direction);

/*! add integer round rect(x, y, w, h)
 *
 * @param path      the path
 * @param bounds    the bounds
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_round_rect2i(gb_path_ref_t path, gb_rect_ref_t bounds, tb_size_t rx, tb_size_t ry, tb_size_t direction);

/*! add circle
 *
 * @param path      the path
 * @param circle    the circle
 * @param direction the direction
 */
tb_void_t           gb_path_add_circle(gb_path_ref_t path, gb_circle_ref_t circle, tb_size_t direction);

/*! add circle(x0, y0, r)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_circle2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t r, tb_size_t direction);

/*! add integer circle(x0, y0, r)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_circle2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t r, tb_size_t direction);

/*! add ellipse
 *
 * @param path      the path
 * @param ellipse   the ellipse
 * @param direction the direction
 */
tb_void_t           gb_path_add_ellipse(gb_path_ref_t path, gb_ellipse_ref_t ellipse, tb_size_t direction);

/*! add ellipse(x0, y0, rx, ry)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_ellipse2(gb_path_ref_t path, gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, tb_size_t direction);

/*! add integer ellipse(x0, y0, rx, ry)
 *
 * @param path      the path
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param rx        the x-radius
 * @param ry        the y-radius
 * @param direction the direction
 */
tb_void_t           gb_path_add_ellipse2i(gb_path_ref_t path, tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_size_t direction);

#ifdef __gb_debug__
/*! dump path
 *
 * @param path      the path
 */
tb_void_t           gb_path_dump(gb_path_ref_t path);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
