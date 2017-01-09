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
 * @file        skia.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device_skia"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "skia/skia.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// float => skia scalar
#ifdef SK_SCALAR_IS_FLOAT
#   define gb_float_to_sk(x)   (SkScalar)gb_float_to_tb(x)
#else
#   define gb_float_to_sk(x)   (SkScalar)gb_float_to_fixed(x)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
// the skia device type
typedef struct __gb_skia_device_t
{
    // the base
    gb_device_impl_t            base;

    // the path
    SkPath*                     path;

    // the paint
    SkPaint*                    paint;

    // the bitmap
    SkBitmap*                   bitmap;

    // the canvas
    SkCanvas*                   canvas;

    // the points
    SkPoint*                    points;

    // the points count
    tb_size_t                   points_count;

}gb_skia_device_t, *gb_skia_device_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_device_skia_apply_matrix(gb_skia_device_ref_t impl)
{
    // check
    tb_assert_and_check_return(impl && impl->canvas && impl->base.matrix);

    // init matrix
	SkMatrix matrix_skia;
    matrix_skia.reset();
    matrix_skia.setScaleX(gb_float_to_sk(impl->base.matrix->sx));
    matrix_skia.setScaleY(gb_float_to_sk(impl->base.matrix->sy));
    matrix_skia.setSkewX(gb_float_to_sk(impl->base.matrix->kx));
    matrix_skia.setSkewY(gb_float_to_sk(impl->base.matrix->ky));
    matrix_skia.setTranslateX(gb_float_to_sk(impl->base.matrix->tx));
    matrix_skia.setTranslateY(gb_float_to_sk(impl->base.matrix->ty));

    // apply matrix
	impl->canvas->setMatrix(matrix_skia);
}
static tb_void_t gb_device_skia_apply_paint(gb_skia_device_ref_t impl)
{
    // check
    tb_assert_and_check_return(impl && impl->paint && impl->base.paint);

    // init paint
    impl->paint->reset();

    // init style
    tb_size_t mode = gb_paint_mode(impl->base.paint);
    switch (mode)
    {
    case GB_PAINT_MODE_FILL:
	    impl->paint->setStyle(SkPaint::kFill_Style);
        break;
    case GB_PAINT_MODE_STROKE:
	    impl->paint->setStyle(SkPaint::kStroke_Style);
        break;
    case GB_PAINT_MODE_FILL_STROKE:
	    impl->paint->setStyle(SkPaint::kStrokeAndFill_Style);
        break;
    default:
        tb_trace_e("invalid paint mode: %lu", gb_paint_mode(impl->base.paint));
        break;
    }

    // init paint for shader
    if (gb_paint_shader(impl->base.paint))
    {
    }
    // init paint for solid
    else impl->paint->setColor(gb_color_pixel(gb_paint_color(impl->base.paint)));

    // apply alpha
    impl->paint->setAlpha(gb_paint_alpha(impl->base.paint));

    // init paint for stroking
    if (mode & GB_PAINT_MODE_STROKE)
    {
        // init stroke width
        impl->paint->setStrokeWidth(gb_float_to_sk(gb_paint_stroke_width(impl->base.paint)));

        // init cap
        switch (gb_paint_stroke_cap(impl->base.paint))
        {
        case GB_PAINT_STROKE_CAP_BUTT:
            impl->paint->setStrokeCap(SkPaint::kButt_Cap);
            break;
        case GB_PAINT_STROKE_CAP_ROUND:
            impl->paint->setStrokeCap(SkPaint::kRound_Cap);
            break;
        case GB_PAINT_STROKE_CAP_SQUARE:
            impl->paint->setStrokeCap(SkPaint::kSquare_Cap);
            break;
        default:
            break;
        }

        // init join
        switch (gb_paint_stroke_join(impl->base.paint))
        {
        case GB_PAINT_STROKE_JOIN_MITER:
            impl->paint->setStrokeJoin(SkPaint::kMiter_Join);
            break;
        case GB_PAINT_STROKE_JOIN_ROUND:
            impl->paint->setStrokeJoin(SkPaint::kRound_Join);
            break;
        case GB_PAINT_STROKE_JOIN_BEVEL:
            impl->paint->setStrokeJoin(SkPaint::kBevel_Join);
            break;
        default:
            break;
        }
    }

    // init antialiasing
    tb_size_t flag = gb_paint_flag(impl->base.paint);
    if (flag & GB_PAINT_FLAG_ANTIALIASING) impl->paint->setFlags(impl->paint->getFlags() | SkPaint::kAntiAlias_Flag);
    else impl->paint->setFlags(impl->paint->getFlags() & ~SkPaint::kAntiAlias_Flag);

    // init filter bitmap
    if (flag & GB_PAINT_FLAG_FILTER_BITMAP) impl->paint->setFlags(impl->paint->getFlags() | SkPaint::kFilterBitmap_Flag);
    else impl->paint->setFlags(impl->paint->getFlags() & ~SkPaint::kFilterBitmap_Flag);
}
static tb_void_t gb_device_skia_resize(gb_device_impl_t* device, tb_size_t width, tb_size_t height)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->bitmap);

    // TODO
    tb_trace_noimpl();
}
static tb_void_t gb_device_skia_draw_clear(gb_device_impl_t* device, gb_color_t color)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas);

    // clear it
	impl->canvas->drawColor((SkColor)gb_color_pixel(color));
}
static tb_bool_t gb_device_skia_draw_hint(gb_device_impl_t* device, gb_shape_ref_t hint)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl && impl->canvas && hint, tb_false);

    // done
    tb_bool_t ok = tb_false;
    switch (hint->type)
    {
    case GB_SHAPE_TYPE_RECT:
        {
            gb_rect_ref_t rect = &hint->u.rect;
            impl->canvas->drawRect(SkRect::MakeXYWH(gb_float_to_sk(rect->x), gb_float_to_sk(rect->y), gb_float_to_sk(rect->w), gb_float_to_sk(rect->h)), *impl->paint);
            ok = tb_true;
        }
        break;
#if 0 // for lastest version
    case GB_SHAPE_TYPE_ROUND_RECT:
        {
            // the round rect
            gb_round_rect_ref_t rect = &hint->u.round_rect;

            // the bounds
            gb_rect_ref_t       bounds = &hint->u.round_rect.bounds;

            // make rrect
            SkRRect     rrect;
            SkVector    radii[4];
            radii[0] = SkVector::make(gb_float_to_sk(rect->rx[0]), gb_float_to_sk(rect->ry[0]));
            radii[1] = SkVector::make(gb_float_to_sk(rect->rx[1]), gb_float_to_sk(rect->ry[1]));
            radii[2] = SkVector::make(gb_float_to_sk(rect->rx[2]), gb_float_to_sk(rect->ry[2]));
            radii[3] = SkVector::make(gb_float_to_sk(rect->rx[3]), gb_float_to_sk(rect->ry[3]));
            rrect.setRectRadii(SkRect::MakeXYWH(gb_float_to_sk(bounds->x), gb_float_to_sk(bounds->y), gb_float_to_sk(bounds->w), gb_float_to_sk(bounds->h)));

            // draw rrect
            impl->canvas->drawRRect(rrect, *impl->paint);

            // ok
            ok = tb_true;
        }
        break;
#endif
    case GB_SHAPE_TYPE_LINE:
        {
            gb_line_ref_t line = &hint->u.line;
	        impl->canvas->drawLine(gb_float_to_sk(line->p0.x), gb_float_to_sk(line->p0.y), gb_float_to_sk(line->p1.x), gb_float_to_sk(line->p1.y), *impl->paint);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_CIRCLE:
        {
            gb_circle_ref_t circle = &hint->u.circle;
	        impl->canvas->drawCircle(gb_float_to_sk(circle->c.x), gb_float_to_sk(circle->c.y), gb_float_to_sk(circle->r), *impl->paint);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_ELLIPSE:
        {
            gb_ellipse_ref_t ellipse = &hint->u.ellipse;
	        impl->canvas->drawOval(SkRect::MakeXYWH(gb_float_to_sk(ellipse->c.x - ellipse->rx), gb_float_to_sk(ellipse->c.y - ellipse->ry), gb_float_to_sk(ellipse->rx + ellipse->rx), gb_float_to_sk(ellipse->ry + ellipse->ry)), *impl->paint);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_ARC:
        {
            gb_arc_ref_t arc = &hint->u.arc;
	        impl->canvas->drawArc(SkRect::MakeXYWH(gb_float_to_sk(arc->c.x - arc->rx), gb_float_to_sk(arc->c.y - arc->ry), SkScalarMul(gb_float_to_sk(arc->rx), SkIntToScalar(2)), SkScalarMul(gb_float_to_sk(arc->ry), SkIntToScalar(2))), gb_float_to_sk(arc->ab), gb_float_to_sk(arc->an), false, *impl->paint);
            ok = tb_true;
        }
        break;
    case GB_SHAPE_TYPE_POINT:
        {
            gb_point_ref_t point = &hint->u.point;
	        impl->canvas->drawPoint(gb_float_to_sk(point->x), gb_float_to_sk(point->y), *impl->paint);
            ok = tb_true;
        }
        break;
    default:
        break;
    }

    // ok?
    return ok;
}
static tb_void_t gb_device_skia_draw_path(gb_device_impl_t* device, gb_path_ref_t path)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && impl->path && path);

    // apply matrix
	gb_device_skia_apply_matrix(impl);

    // apply paint
    gb_device_skia_apply_paint(impl);

    // clear path
    impl->path->reset();

    // init the path fill type
    tb_size_t rule = gb_paint_fill_rule(impl->base.paint);
    switch (rule)
    {
    case GB_PAINT_FILL_RULE_ODD:
        impl->path->setFillType(SkPath::kEvenOdd_FillType);
        break;
    case GB_PAINT_FILL_RULE_NONZERO:
        impl->path->setFillType(SkPath::kWinding_FillType);
        break;
    default:
        break;
    }

    // done
    tb_for_all_if (gb_path_item_ref_t, item, path, item)
    {
        switch (item->code)
        {
        case GB_PATH_CODE_MOVE:
            impl->path->moveTo(gb_float_to_sk(item->points[0].x), gb_float_to_sk(item->points[0].y));
            break;
        case GB_PATH_CODE_LINE:
            impl->path->lineTo(gb_float_to_sk(item->points[1].x), gb_float_to_sk(item->points[1].y));
            break;
        case GB_PATH_CODE_QUAD:
            impl->path->quadTo(gb_float_to_sk(item->points[1].x), gb_float_to_sk(item->points[1].y), gb_float_to_sk(item->points[2].x), gb_float_to_sk(item->points[2].y));
            break;
        case GB_PATH_CODE_CUBIC:
            impl->path->cubicTo(gb_float_to_sk(item->points[1].x), gb_float_to_sk(item->points[1].y), gb_float_to_sk(item->points[2].x), gb_float_to_sk(item->points[2].y), gb_float_to_sk(item->points[3].x), gb_float_to_sk(item->points[3].y));
            break;
        case GB_PATH_CODE_CLOS:
            impl->path->close();
            break;
        default:
            // trace
            tb_trace_e("invalid code: %lu", item->code);
            break;
        }
    }

    // draw it
    impl->canvas->drawPath(*impl->path, *impl->paint);
}
static tb_void_t gb_device_skia_draw_lines(gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && points && count);

    // apply matrix
	gb_device_skia_apply_matrix(impl);

    // apply paint
    gb_device_skia_apply_paint(impl);

    // make points
    if (!impl->points) impl->points = tb_nalloc_type(count, SkPoint);
    // not enough? grow points
    else if (count > impl->points_count) impl->points = (SkPoint*)tb_ralloc(impl->points, count * sizeof(SkPoint));
    tb_assert_and_check_return(impl->points);

    // update points count
    if (count > impl->points_count) impl->points_count = count;

    // convert points
    tb_size_t i = 0;
    for (i = 0; i < count; i++) impl->points[i].set(gb_float_to_sk(points[i].x), gb_float_to_sk(points[i].y));

	// draw it
	impl->canvas->drawPoints(SkCanvas::kLines_PointMode, count, impl->points, *impl->paint);
}
static tb_void_t gb_device_skia_draw_points(gb_device_impl_t* device, gb_point_ref_t points, tb_size_t count, gb_rect_ref_t bounds)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && points && count);

    // apply matrix
	gb_device_skia_apply_matrix(impl);

    // apply paint
    gb_device_skia_apply_paint(impl);

    // make points
    if (!impl->points) impl->points = tb_nalloc_type(count, SkPoint);
    // not enough? grow points
    else if (count > impl->points_count) impl->points = (SkPoint*)tb_ralloc(impl->points, count * sizeof(SkPoint));
    tb_assert_and_check_return(impl->points);

    // update points count
    if (count > impl->points_count) impl->points_count = count;

    // convert points
    tb_size_t i = 0;
    for (i = 0; i < count; i++) impl->points[i].set(gb_float_to_sk(points[i].x), gb_float_to_sk(points[i].y));

	// draw it
	impl->canvas->drawPoints(SkCanvas::kPoints_PointMode, count, impl->points, *impl->paint);
}
static tb_void_t gb_device_skia_draw_polygon(gb_device_impl_t* device, gb_polygon_ref_t polygon, gb_shape_ref_t hint, gb_rect_ref_t bounds)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl && impl->canvas && impl->path && polygon);

    // the points
    gb_point_ref_t      points = polygon->points;
    tb_uint16_t const*  counts = polygon->counts;
    tb_assert_and_check_return(points && counts);

    // apply matrix
	gb_device_skia_apply_matrix(impl);

    // apply paint
    gb_device_skia_apply_paint(impl);

    // draw hint?
    if (hint && gb_device_skia_draw_hint(device, hint)) return ;

    // clear path
    impl->path->reset();

    // init path
    gb_point_ref_t  first = tb_null;
    gb_point_ref_t  point = tb_null;
    tb_uint16_t     count = *counts++;
    tb_size_t       index = 0;
    while (index < count)
    {
        // the point
        point = points++;

        // first point?
        if (!index) 
        {
            impl->path->moveTo(gb_float_to_sk(point->x), gb_float_to_sk(point->y));
            first = point;
        }
        else impl->path->lineTo(gb_float_to_sk(point->x), gb_float_to_sk(point->y));

        // next point
        index++;

        // next polygon
        if (index == count) 
        {
            // close path
            if (first && first->x == point->x && first->y == point->y) impl->path->close();

            // next
            count = *counts++;
            index = 0;
        }
    }

    // mark convex
    if (polygon->convex) impl->path->setIsConvex(true);

    // draw it
    impl->canvas->drawPath(*impl->path, *impl->paint);
}
static gb_shader_ref_t gb_device_skia_shader_linear(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO
    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_skia_shader_radial(gb_device_impl_t* device, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO
    tb_trace_noimpl();
    return tb_null;
}
static gb_shader_ref_t gb_device_skia_shader_bitmap(gb_device_impl_t* device, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return_val(impl, tb_null);

    // TODO
    tb_trace_noimpl();
    return tb_null;
}
static tb_void_t gb_device_skia_exit(gb_device_impl_t* device)
{
    // check
    gb_skia_device_ref_t impl = (gb_skia_device_ref_t)device;
    tb_assert_and_check_return(impl);

    // exit path
    if (impl->path) delete impl->path;
    impl->path = tb_null;

    // exit points
    if (impl->points) tb_free(impl->points);
    impl->points = tb_null;

    // exit paint
    if (impl->paint) delete impl->paint;
    impl->paint = tb_null;

    // exit canvas
    if (impl->canvas) delete impl->canvas;
    impl->canvas = tb_null;

    // exit bitmap
    if (impl->bitmap) delete impl->bitmap;
    impl->bitmap = tb_null;

    // exit it
    tb_free(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init_skia(gb_bitmap_ref_t bitmap)
{
    // check
    tb_assert_and_check_return_val(bitmap, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    gb_skia_device_ref_t    impl = tb_null;
    do
    {
        // the width and height
        tb_size_t width     = gb_bitmap_width(bitmap);
        tb_size_t height    = gb_bitmap_height(bitmap);
        tb_assert_and_check_break(width && height && width <= GB_WIDTH_MAXN && height <= GB_HEIGHT_MAXN);

        // make device
        impl = tb_malloc0_type(gb_skia_device_t);
        tb_assert_and_check_break(impl);

        // init base 
        impl->base.type             = GB_DEVICE_TYPE_BITMAP;
        impl->base.resize           = gb_device_skia_resize;
        impl->base.draw_clear       = gb_device_skia_draw_clear;
        impl->base.draw_path        = gb_device_skia_draw_path;
        impl->base.draw_lines       = gb_device_skia_draw_lines;
        impl->base.draw_points      = gb_device_skia_draw_points;
        impl->base.draw_polygon     = gb_device_skia_draw_polygon;
        impl->base.shader_linear    = gb_device_skia_shader_linear;
        impl->base.shader_radial    = gb_device_skia_shader_radial;
        impl->base.shader_bitmap    = gb_device_skia_shader_bitmap;
        impl->base.exit             = gb_device_skia_exit;

        // make bitmap
        impl->bitmap = new SkBitmap();
        tb_assert_and_check_break(impl->bitmap);

        // init bitmap
        impl->bitmap->setConfig(SkBitmap::kARGB_8888_Config, width, height, gb_bitmap_row_bytes(bitmap));
	    impl->bitmap->setPixels(gb_bitmap_data(bitmap));

        // make canvas
        impl->canvas = new SkCanvas(*impl->bitmap);
        tb_assert_and_check_break(impl->canvas);

        // init canvas
	    impl->canvas->resetMatrix();

        // make paint
        impl->paint = new SkPaint();
        tb_assert_and_check_break(impl->paint);

        // make path
        impl->path = new SkPath();
        tb_assert_and_check_break(impl->path);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_device_exit((gb_device_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_device_ref_t)impl;
}

