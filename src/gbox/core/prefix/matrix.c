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
 * @file        matrix.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "matrix"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "matrix.h"
#include "point.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
#ifdef GB_CONFIG_FLOAT_FIXED
typedef gb_float_t      gb_invdet_t;
#else
typedef tb_double_t     gb_invdet_t;
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

#ifdef GB_CONFIG_FLOAT_FIXED
static __tb_inline__ gb_float_t gb_matrix_mul_add(gb_float_t a, gb_float_t b, gb_float_t c, gb_float_t d)
{
    return (gb_float_t)(((tb_hong_t)a * b + (tb_hong_t)c * d) >> 16);
}
static __tb_inline__ gb_float_t gb_matrix_mul_sub(gb_float_t a, gb_float_t b, gb_float_t c, gb_float_t d)
{
    return (gb_float_t)(((tb_hong_t)a * b - (tb_hong_t)c * d) >> 16);
}
static __tb_inline__ gb_float_t gb_matrix_det(gb_float_t sx, gb_float_t sy, gb_float_t kx, gb_float_t ky)
{
    tb_hong_t det = (tb_hong_t)sx * sy - (tb_hong_t)kx * ky;
    if (!det) return 0;
    return (gb_float_t)(((tb_hong_t)1 << 48) / det);
}
#else
static __tb_inline__ gb_float_t gb_matrix_mul_add(gb_float_t a, gb_float_t b, gb_float_t c, gb_float_t d)
{
    return (gb_float_t)((tb_double_t)a * b + (tb_double_t)c * d);
}
static __tb_inline__ tb_double_t gb_matrix_det(gb_float_t sx, gb_float_t sy, gb_float_t kx, gb_float_t ky)
{
    tb_double_t det = (tb_double_t)sx * sy - (tb_double_t)kx * ky;
    if (gb_abs(det) <= GB_NEAR0 * GB_NEAR0 * GB_NEAR0) return 0.;
    return 1. / det;
}
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tb_void_t gb_matrix_init(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t kx, gb_float_t ky, gb_float_t sy, gb_float_t tx, gb_float_t ty)
{
    // check
    tb_assert(matrix);

    // init
    matrix->sx = sx;
    matrix->kx = kx;
    matrix->ky = ky;
    matrix->sy = sy;
    matrix->tx = tx;
    matrix->ty = ty;
}
tb_void_t gb_matrix_init_rotate(gb_matrix_ref_t matrix, gb_float_t degrees)
{
    gb_float_t s;
    gb_float_t c;
    gb_sincos(gb_degree_to_radian(degrees), &s, &c);
    gb_matrix_init_sincos(matrix, s, c);
}
tb_void_t gb_matrix_init_rotatep(gb_matrix_ref_t matrix, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    gb_float_t s;
    gb_float_t c;
    gb_sincos(gb_degree_to_radian(degrees), &s, &c);
    gb_matrix_init_sincosp(matrix, s, c, px, py);
}
tb_void_t gb_matrix_init_sincos(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos)
{
    gb_matrix_init(matrix, cos, -sin, sin, cos, 0, 0);  
}
tb_void_t gb_matrix_init_sincosp(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    gb_float_t const one_cos = GB_ONE - cos;
    gb_matrix_init(matrix, cos, -sin, sin, cos, gb_mul(sin, py) + gb_mul(one_cos, px), gb_mul(-sin, px) + gb_mul(one_cos, py)); 
}
tb_void_t gb_matrix_init_skew(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky)
{
    gb_matrix_init(matrix, GB_ONE, kx, ky, GB_ONE, 0, 0);
}
tb_void_t gb_matrix_init_skewp(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    gb_matrix_init(matrix, GB_ONE, kx, ky, GB_ONE, gb_mul(-kx, py), gb_mul(-ky, px));
}
tb_void_t gb_matrix_init_scale(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy)
{
    gb_matrix_init(matrix, sx, 0, 0, sy, 0, 0);
}
tb_void_t gb_matrix_init_scalep(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    gb_matrix_init(matrix, sx, 0, 0, sy, px - gb_mul(sx, px), py - gb_mul(sy, py));
}
tb_void_t gb_matrix_init_translate(gb_matrix_ref_t matrix, gb_float_t tx, gb_float_t ty)
{
    gb_matrix_init(matrix, GB_ONE, 0, 0, GB_ONE, tx, ty);
}
tb_void_t gb_matrix_clear(gb_matrix_ref_t matrix)
{
    gb_matrix_init(matrix, GB_ONE, 0, 0, GB_ONE, 0, 0);
}
tb_void_t gb_matrix_copy(gb_matrix_ref_t matrix, gb_matrix_ref_t copied)
{
    // check
    tb_assert(matrix);

    // copy it
    if (copied) *matrix = *copied;
    // clear it
    else gb_matrix_clear(matrix);
}
tb_bool_t gb_matrix_invert(gb_matrix_ref_t matrix)
{
    // check
    tb_assert(matrix);

    // identity?
    if (gb_matrix_identity(matrix)) return tb_true;

    // no rotate?
    gb_matrix_t mx = *matrix;
    if (0 == matrix->kx && 0 == matrix->ky)
    {
        // invert it if sx != 1.0
        if (GB_ONE != matrix->sx)
        {
            // check
            tb_assert_and_check_return_val(!gb_near0(matrix->sx), tb_false);

            // compute sx and tx
            mx.sx = gb_invert(matrix->sx);
            mx.tx = gb_div(-matrix->tx, matrix->sx);
        }

        // invert it if sy != 1.0
        if (GB_ONE != matrix->sy)
        {
            // check
            tb_assert_and_check_return_val(!gb_near0(matrix->sy), tb_false);

            // compute sy and ty
            mx.sy = gb_invert(matrix->sy);
            mx.ty = gb_div(-matrix->ty, matrix->sy);
        }
    }
    else
    {
        /* |A|
         *
         * adj(A)
         *
         * (sx, kx)     (sy, -kx)
         *          => 
         * (ky, sy)     (-ky, sx)
         */
        gb_invdet_t det = gb_matrix_det(matrix->sx, matrix->sy, matrix->kx, matrix->ky);
        tb_assert_and_check_return_val(det, tb_false);

        // invert
#ifdef GB_CONFIG_FLOAT_FIXED
        mx.sx = tb_fixed_mul(matrix->sy, det);
        mx.sy = tb_fixed_mul(matrix->sx, det);
        mx.kx = tb_fixed_mul(-matrix->kx, det);
        mx.ky = tb_fixed_mul(-matrix->ky, det);
        mx.tx = gb_mul(gb_matrix_mul_sub(matrix->kx, matrix->ty, matrix->sy, matrix->tx), det);
        mx.ty = gb_mul(gb_matrix_mul_sub(matrix->ky, matrix->tx, matrix->sx, matrix->ty), det);
#else
        mx.sx = (tb_float_t)(matrix->sy * det);
        mx.sy = (tb_float_t)(matrix->sx * det);
        mx.kx = (tb_float_t)(-matrix->kx * det);
        mx.ky = (tb_float_t)(-matrix->ky * det);
        mx.tx = (tb_float_t)(((tb_double_t)matrix->kx * matrix->ty - (tb_double_t)matrix->sy * matrix->tx) * det);
        mx.ty = (tb_float_t)(((tb_double_t)matrix->ky * matrix->tx - (tb_double_t)matrix->sx * matrix->ty) * det);
#endif
    }

    // update matrix
    *matrix = mx;

    // ok
    return tb_true;
}
tb_bool_t gb_matrix_identity(gb_matrix_ref_t matrix)
{
    // check
    tb_assert(matrix);

    // is identity?
    return (    GB_ONE == matrix->sx
            &&  GB_ONE == matrix->sy
            &&  0 == matrix->kx
            &&  0 == matrix->ky
            &&  0 == matrix->tx 
            &&  0 == matrix->ty)? tb_true : tb_false;
}
tb_bool_t gb_matrix_rotate(gb_matrix_ref_t matrix, gb_float_t degrees)
{
    // 0 ?
    tb_check_return_val(degrees != 0, tb_true);

    // rotate
    gb_matrix_t mx;
    gb_matrix_init_rotate(&mx, degrees);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_rotate_lhs(gb_matrix_ref_t matrix, gb_float_t degrees)
{
    // 0 ?
    tb_check_return_val(degrees != 0, tb_true);

    // rotate
    gb_matrix_t mx;
    gb_matrix_init_rotate(&mx, degrees);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_rotatep(gb_matrix_ref_t matrix, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    // 0 ?
    tb_check_return_val(degrees != 0, tb_true);

    // rotate
    gb_matrix_t mx;
    gb_matrix_init_rotatep(&mx, degrees, px, py);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_rotatep_lhs(gb_matrix_ref_t matrix, gb_float_t degrees, gb_float_t px, gb_float_t py)
{
    // 0 ?
    tb_check_return_val(degrees != 0, tb_true);

    // rotate
    gb_matrix_t mx;
    gb_matrix_init_rotatep(&mx, degrees, px, py);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_scale(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy)
{
    // check
    tb_assert(matrix);

    // 1/1 ?
    tb_check_return_val(GB_ONE != sx || GB_ONE != sy, tb_true);

#if 0
    // scale
    gb_matrix_t mx;
    gb_matrix_init_scale(&mx, sx, sy);
    return gb_matrix_multiply(matrix, &mx);
#else

    matrix->sx = gb_mul(matrix->sx, sx);
    matrix->ky = gb_mul(matrix->ky, sx);
    matrix->kx = gb_mul(matrix->kx, sy);
    matrix->sy = gb_mul(matrix->sy, sy);

    return tb_true;
#endif
}
tb_bool_t gb_matrix_scale_lhs(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy)
{
    // 1/1 ?
    tb_check_return_val(GB_ONE != sx || GB_ONE != sy, tb_true);

    // scale
    gb_matrix_t mx;
    gb_matrix_init_scale(&mx, sx, sy);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_scalep(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    // 1/1 ?
    tb_check_return_val(GB_ONE != sx || GB_ONE != sy, tb_true);

    // scale
    gb_matrix_t mx;
    gb_matrix_init_scalep(&mx, sx, sy, px, py);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_scalep_lhs(gb_matrix_ref_t matrix, gb_float_t sx, gb_float_t sy, gb_float_t px, gb_float_t py)
{
    // 1/1 ?
    tb_check_return_val(GB_ONE != sx || GB_ONE != sy, tb_true);

    // scale
    gb_matrix_t mx;
    gb_matrix_init_scalep(&mx, sx, sy, px, py);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_translate(gb_matrix_ref_t matrix, gb_float_t dx, gb_float_t dy)
{
    // check
    tb_assert(matrix);

    // 0, 0 ?
    tb_check_return_val(dx != 0 || dy != 0, tb_true);

#if 0
    // translate
    gb_matrix_t mx;
    gb_matrix_init_translate(&mx, dx, dy);
    return gb_matrix_multiply(matrix, &mx);
#else
    // translate
    matrix->tx = gb_matrix_mul_add(matrix->sx, dx, matrix->kx, dy) + matrix->tx;
    matrix->ty = gb_matrix_mul_add(matrix->ky, dx, matrix->sy, dy) + matrix->ty;

    // ok
    return tb_true;
#endif
}
tb_bool_t gb_matrix_translate_lhs(gb_matrix_ref_t matrix, gb_float_t dx, gb_float_t dy)
{
    // check
    tb_assert(matrix);

    // 0, 0 ?
    tb_check_return_val(dx != 0 || dy != 0, tb_true);

#if 0
    // translate
    gb_matrix_t mx;
    gb_matrix_init_translate(&mx, dx, dy);
    return gb_matrix_multiply_lhs(matrix, &mx);
#else
    // translate
    matrix->tx += dx;
    matrix->ty += dy;

    // ok
    return tb_true;
#endif
}
tb_bool_t gb_matrix_skew(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky)
{
    // skew
    gb_matrix_t mx;
    gb_matrix_init_skew(&mx, kx, ky);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_skew_lhs(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky)
{
    // skew
    gb_matrix_t mx;
    gb_matrix_init_skew(&mx, kx, ky);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_skewp(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    // skew
    gb_matrix_t mx;
    gb_matrix_init_skewp(&mx, kx, ky, px, py);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_skewp_lhs(gb_matrix_ref_t matrix, gb_float_t kx, gb_float_t ky, gb_float_t px, gb_float_t py)
{
    // skew
    gb_matrix_t mx;
    gb_matrix_init_skewp(&mx, kx, ky, px, py);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_sincos(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos)
{
    // sincos
    gb_matrix_t mx;
    gb_matrix_init_sincos(&mx, sin, cos);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_sincos_lhs(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos)
{
    // sincos
    gb_matrix_t mx;
    gb_matrix_init_sincos(&mx, sin, cos);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_sincosp(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    // sincos
    gb_matrix_t mx;
    gb_matrix_init_sincosp(&mx, sin, cos, px, py);
    return gb_matrix_multiply(matrix, &mx);
}
tb_bool_t gb_matrix_sincosp_lhs(gb_matrix_ref_t matrix, gb_float_t sin, gb_float_t cos, gb_float_t px, gb_float_t py)
{
    // sincos
    gb_matrix_t mx;
    gb_matrix_init_sincosp(&mx, sin, cos, px, py);
    return gb_matrix_multiply_lhs(matrix, &mx);
}
tb_bool_t gb_matrix_multiply(gb_matrix_ref_t matrix, gb_matrix_ref_t factor)
{
    // check
    tb_assert(matrix && factor);

    // identity?
    if (gb_matrix_identity(factor)) return tb_true;

    /* multiply
     * 
     * | lsx lkx ltx |   | rsx rkx rtx |
     * | lky lsy lty | * | rky rsy rty | 
     * |   0   0   1 |   |   0   0   1 |
     *
     * =>
     *
     * | lsx * rsx + lkx * rky | lsx * rkx + lkx * rsy | lsx * rtx + lkx * rty + ltx |
     * | lky * rsx + lsy * rky | lky * rkx + lsy * rsy | lky * rtx + lsy * rty + lty |
     * |                     0 |                     0 |                           1 |
     *
     * @note path * (A * B * C) != ((path * A) * B) * C
     */
    gb_matrix_t mx;
    mx.sx = gb_matrix_mul_add(matrix->sx, factor->sx, matrix->kx, factor->ky);
    mx.ky = gb_matrix_mul_add(matrix->ky, factor->sx, matrix->sy, factor->ky);

    mx.kx = gb_matrix_mul_add(matrix->sx, factor->kx, matrix->kx, factor->sy);
    mx.sy = gb_matrix_mul_add(matrix->ky, factor->kx, matrix->sy, factor->sy);

    mx.tx = gb_matrix_mul_add(matrix->sx, factor->tx, matrix->kx, factor->ty) + matrix->tx;
    mx.ty = gb_matrix_mul_add(matrix->ky, factor->tx, matrix->sy, factor->ty) + matrix->ty;

    // update matrix
    *matrix = mx;

    // ok
    return tb_true;
}
tb_bool_t gb_matrix_multiply_lhs(gb_matrix_ref_t matrix, gb_matrix_ref_t factor)
{
    // identity?
    if (gb_matrix_identity(factor)) return tb_true;
    
    // save it
    gb_matrix_t mx = *factor;

    // multiply
    tb_bool_t ok = gb_matrix_multiply(&mx, matrix);

    // update matrix
    *matrix = mx;

    // ok?
    return ok;
}
tb_void_t gb_matrix_apply_points(gb_matrix_ref_t matrix, gb_point_ref_t points, tb_size_t count)
{
    // check
    tb_assert_and_check_return(matrix && points && count);

    // apply it
    tb_size_t i = 0;
    for (i = 0; i < count; i++) gb_point_apply(points + i, matrix);
}
