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
 * @file        float.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_FLOAT_H
#define GB_CORE_PREFIX_FLOAT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

#ifdef GB_CONFIG_FLOAT_FIXED

// constant
#define GB_ONE                  TB_FIXED_ONE
#define GB_TWO                  (TB_FIXED_ONE + TB_FIXED_ONE)
#define GB_HALF                 TB_FIXED_HALF
#define GB_MAF                  TB_FIXED_MAX
#define GB_MIF                  TB_FIXED_MIN
#define GB_NAN                  TB_FIXED_NAN
#define GB_INF                  TB_FIXED_INF
#define GB_PI                   TB_FIXED_PI
#define GB_NEAR0                TB_FIXED_NEAR0
#define GB_SQRT2                TB_FIXED_SQRT2      //!< sqrt(2)
#define GB_ONEOVER_SQRT2        (46341)             //!< 1/sqrt(2)
#define GB_TAN_PIOVER8          (27146)             //!< tan(pi/8)
#define GB_SQRT2_OVER2          (46341)             //!< sqrt(2)/2
#define GB_PIOVER180            (1144)              //<! pi/180
#define GB_180OVERPI            (3754936)           //!< 180/pi
#define GB_DEGREE_45            (2949120)
#define GB_DEGREE_90            (5898240)
#define GB_DEGREE_180           (11796480)
#define GB_DEGREE_360           (23592960)

// conversion
#ifdef TB_CONFIG_TYPE_HAVE_FLOAT
#   define gb_float_to_tb(x)    tb_fixed_to_float(x)
#   define tb_float_to_gb(x)    tb_float_to_fixed(x)
#endif

#define gb_long_to_float(x)     tb_long_to_fixed(x)
#define gb_float_to_long(x)     tb_fixed_to_long(x)

#define gb_fixed_to_float(x)    (x)
#define gb_float_to_fixed(x)    (x)

#define gb_fixed6_to_float(x)   tb_fixed6_to_fixed(x)
#define gb_float_to_fixed6(x)   tb_fixed_to_fixed6(x)

#define gb_fixed30_to_float(x)  tb_fixed30_to_fixed(x)
#define gb_float_to_fixed30(x)  tb_fixed_to_fixed30(x)

#define gb_degree_to_radian(x)  (tb_fixed_mul((x), GB_PIOVER180))
#define gb_radian_to_degree(x)  (tb_fixed_mul((x), GB_180OVERPI))

// round
#define gb_round(x)             tb_fixed_round(x)
#define gb_ceil(x)              tb_fixed_ceil(x)
#define gb_floor(x)             tb_fixed_floor(x)

// is nan?
#define gb_isnan(x)             ((x) == TB_FIXED_NAN)

// is finite?
#define gb_isfinite(x)          ((x) != TB_FIXED_NAN)

// interpolation
#define gb_interp(x, y, f)      ((x) + gb_mul((y) - (x), (f)))

// functions
#define gb_abs(x)               tb_fixed_abs(x)
#define gb_avg(x, y)            tb_fixed_avg(x, y)
#define gb_mul(x, y)            tb_fixed_mul(x, y)
#define gb_div(x, y)            tb_fixed_div(x, y)
#define gb_lsh(x, y)            tb_fixed_lsh(x, y)
#define gb_rsh(x, y)            tb_fixed_rsh(x, y)
#define gb_imul(x, y)           tb_fixed_imul(x, y)
#define gb_idiv(x, y)           tb_fixed_idiv(x, y)
#define gb_imuldiv(x, y, z)     tb_fixed_imuldiv(x, y, z)
#define gb_imulsub(x, y, z)     tb_fixed_imulsub(x, y, z)
#define gb_invert(x)            tb_fixed_invert(x)
#define gb_half(x)              ((x) >> 1)
#define gb_sqre(x)              tb_fixed_sqre(x)
#define gb_sqrt(x)              tb_fixed_sqrt(x)
#define gb_sin(x)               tb_fixed_sin(x)
#define gb_cos(x)               tb_fixed_cos(x)
#define gb_sincos(x, s, c)      tb_fixed_sincos(x, s, c)
#define gb_tan(x)               tb_fixed_tan(x)
#define gb_asin(x)              tb_fixed_asin(x)
#define gb_acos(x)              tb_fixed_acos(x)
#define gb_atan(x)              tb_fixed_atan(x)
#define gb_atan2(y, x)          tb_fixed_atan2(y, x)
#define gb_exp(x)               tb_fixed_exp(x)
#define gb_exp1(x)              tb_fixed_exp1(x)
#define gb_expi(x)              tb_fixed_expi(x)

#elif defined(TB_CONFIG_TYPE_HAVE_FLOAT)

// constant
#define GB_ONE                  (1.0f)
#define GB_TWO                  (2.0f)
#define GB_HALF                 (0.5f)
#define GB_MAF                  TB_MAF
#define GB_MIF                  TB_MIF
#define GB_NAN                  TB_NAN
#define GB_INF                  TB_INF
#define GB_PI                   TB_PI
#define GB_NEAR0                (1.0f / (1 << 12))
#define GB_SQRT2                (1.414213562f)      //!< sqrt(2)
#define GB_ONEOVER_SQRT2        (0.707106781f)      //!< 1/sqrt(2)
#define GB_TAN_PIOVER8          (0.414213562f)      //!< tan(pi/8)
#define GB_SQRT2_OVER2          (0.707106781f)      //!< sqrt(2)/2
#define GB_PIOVER180            (0.017453293f)      //<! pi/180
#define GB_180OVERPI            (57.29577951f)      //!< 180/pi
#define GB_DEGREE_45            (45.0f)
#define GB_DEGREE_90            (90.0f)
#define GB_DEGREE_180           (180.0f)
#define GB_DEGREE_360           (360.0f)

// conversion
#define gb_float_to_tb(x)       (x)
#define tb_float_to_gb(x)       (x)

#define gb_long_to_float(x)     (tb_float_t)(x)
#define gb_float_to_long(x)     (tb_long_t)(x)

#define gb_fixed_to_float(x)    tb_fixed_to_float(x)
#define gb_float_to_fixed(x)    tb_float_to_fixed(x)

#define gb_fixed6_to_float(x)   tb_fixed6_to_float(x)
#define gb_float_to_fixed6(x)   tb_float_to_fixed6(x)

#define gb_fixed30_to_float(x)  tb_fixed30_to_float(x)
#define gb_float_to_fixed30(x)  tb_float_to_fixed30(x)

#define gb_degree_to_radian(x)  ((x) * GB_PIOVER180)
#define gb_radian_to_degree(x)  ((x) * GB_180OVERPI)

// round
#define gb_round(x)             tb_round(x)
#define gb_ceil(x)              tb_ceil(x)
#define gb_floor(x)             tb_floor(x)

// is nan?
#define gb_isnan(x)             tb_isnanf(x)

// is finite?
#define gb_isfinite(x)          tb_isfinf(x)

// interpolation
#define gb_interp(x, y, f)      ((x) + ((y) - (x)) * (f))

// functions
#define gb_abs(x)               tb_fabs(x)
#define gb_avg(x, y)            (((x) + (y)) * 0.5f)
#define gb_lsh(x, y)            ((x) * (tb_float_t)(1 << (y)))
#define gb_rsh(x, y)            ((x) / (tb_float_t)(1 << (y)))
#define gb_mul(x, y)            ((x) * (y))
#define gb_div(x, y)            ((x) / (y))
#define gb_imul(x, y)           ((x) * (tb_float_t)(y))
#define gb_idiv(x, y)           ((x) / (tb_float_t)(y))
#define gb_imuldiv(x, y, z)     (((x) * (tb_float_t)(y)) / (tb_float_t)(z))
#define gb_imulsub(x, y, z)     (((x) * (tb_float_t)(y)) - (tb_float_t)(z))
#define gb_invert(x)            (1.0f / (x))
#define gb_half(x)              ((x) * 0.5f)
#define gb_sqre(x)              ((x) * (x))
#define gb_sqrt(x)              tb_sqrtf(x)
#define gb_sin(x)               tb_sinf(x)
#define gb_cos(x)               tb_cosf(x)
#define gb_sincos(x, s, c)      tb_sincosf(x, s, c)
#define gb_tan(x)               tb_tanf(x)
#define gb_asin(x)              tb_asinf(x)
#define gb_acos(x)              tb_acosf(x)
#define gb_atan(x)              tb_atanf(x)
#define gb_atan2(y, x)          tb_atan2f(y, x)
#define gb_exp(x)               tb_expf(x)
#define gb_exp1(x)              tb_exp1f(x)
#define gb_expi(x)              tb_expif(x)

#else
#   error float is not supported.
#endif

/// nearly zero?
#define gb_near0(x)             (gb_abs(x) <= GB_NEAR0)

/// nearly equal?
#define gb_near_eq(x, y)        (gb_abs((x) - (y)) <= GB_NEAR0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* compute the sign of the float value 
 *
 * @param x                     the value
 *
 * @return                      -GB_ONE or 0 or GB_ONE
 */
static __tb_inline__ gb_float_t  gb_sign(gb_float_t x)
{
    return ((x < 0)? -GB_ONE : (x > 0)? GB_ONE : 0);
}

/* compute the sign of the float value to long
 *
 * @param x                     the value
 *
 * @return                      -1 or 0 or 1
 */
static __tb_inline__ tb_long_t  gb_sign_to_long(gb_float_t x)
{
    return ((x < 0)? -1 : (x > 0));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif

