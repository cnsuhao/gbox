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
 * @file        shader.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_SHADER_H
#define GB_CORE_SHADER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "canvas.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the shader type enum
typedef enum __gb_shader_type_e
{
	GB_SHADER_TYPE_NONE     = 0
,	GB_SHADER_TYPE_LINEAR   = 1     //!< the linear gradient shader
,	GB_SHADER_TYPE_RADIAL   = 2     //!< the radial gradient shader
,	GB_SHADER_TYPE_BITMAP   = 3     //!< the bitmap shader

}gb_shader_type_e;

/// the shader mode enum
typedef enum __gb_shader_mode_e
{
	GB_SHADER_MODE_NONE     = 0
,	GB_SHADER_MODE_BORDER   = 1     //!< the border mode
,	GB_SHADER_MODE_CLAMP    = 2     //!< the clamp mode
,	GB_SHADER_MODE_REPEAT   = 3     //!< the repeat mode
,	GB_SHADER_MODE_MIRROR   = 4     //!< the mirror mode

}gb_shader_mode_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
	
/*! init linear gradient shader
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param line      the line
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init_linear(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line);

/*! init linear gradient shader with line: (xb, yb), (xe, ye)
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param xb        the begin x-coordinate
 * @param yb        the begin y-coordinate
 * @param xe        the end x-coordinate
 * @param ye        the end y-coordinate
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init2_linear(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_float_t xb, gb_float_t yb, gb_float_t xe, gb_float_t ye);

/*! init linear gradient shader with integer line: (xb, yb), (xe, ye)
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param xb        the begin x-coordinate
 * @param yb        the begin y-coordinate
 * @param xe        the end x-coordinate
 * @param ye        the end y-coordinate
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init2i_linear(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye);

/*! init radial gradient shader
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param circle    the circle
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init_radial(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle);

/*! init radial gradient shader with circle: (x0, y0, r)
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init2_radial(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_float_t x0, gb_float_t y0, gb_float_t r);

/*! init radial gradient shader with integer circle: (x0, y0, r)
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param gradient  the gradient
 * @param x0        the x0-coordinate
 * @param y0        the y0-coordinate
 * @param r         the radius
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init2i_radial(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, tb_long_t x0, tb_long_t y0, tb_size_t r);

/*! init bitmap shader
 *
 * @param canvas    the canvas 
 * @param mode      the mode 
 * @param bitmap    the bitmap
 *
 * @return          the shader
 */
gb_shader_ref_t     gb_shader_init_bitmap(gb_canvas_ref_t canvas, tb_size_t mode, gb_bitmap_ref_t bitmap);

/*! exit shader
 *
 * @param shader    the shader
 */
tb_void_t 			gb_shader_exit(gb_shader_ref_t shader);

/*! the shader type
 *
 * @param shader    the shader
 *
 * @return          the type
 */
tb_size_t 	        gb_shader_type(gb_shader_ref_t shader);

/*! the shader mode
 *
 * @param shader    the shader
 *
 * @return          the mode
 */
tb_size_t 	        gb_shader_mode(gb_shader_ref_t shader);

/*! the shader matrix
 *
 * @param shader    the shader
 *
 * @return          the matrix
 */
gb_matrix_ref_t 	gb_shader_matrix(gb_shader_ref_t shader);

/*! set the shader matrix
 *
 * @param shader    the shader
 * @param matrix    the matrix
 */
tb_void_t 			gb_shader_matrix_set(gb_shader_ref_t shader, gb_matrix_ref_t matrix);

/*! the shader reference count
 *
 * @param shader    the shader
 *
 * @return          the shader reference count
 */
tb_size_t           gb_shader_ref(gb_shader_ref_t shader);

/*! increase the shader reference count
 *
 * @param shader    the shader
 */
tb_void_t           gb_shader_inc(gb_shader_ref_t shader);

/*! decrease the shader reference count, will free it if --refn == 0
 *
 * @param shader    the shader
 */
tb_void_t           gb_shader_dec(gb_shader_ref_t shader);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif
