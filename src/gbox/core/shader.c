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
 * @file        shader.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "shader.h"
#include "device.h"
#include "device/prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_shader_ref_t gb_shader_init_linear(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_line_ref_t line)
{
    // check
    tb_assert_and_check_return_val(canvas, tb_null);

    // the device 
    gb_device_impl_t* device = (gb_device_impl_t*)gb_canvas_device(canvas);
    tb_assert_and_check_return_val(device && device->shader_linear, tb_null);

    // init shader
    return device->shader_linear(device, mode, gradient, line);
}
gb_shader_ref_t gb_shader_init2_linear(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_float_t xb, gb_float_t yb, gb_float_t xe, gb_float_t ye)
{
    // make line
    gb_line_t line;
    gb_line_make(&line, xb, yb, xe, ye);

    // init
    return gb_shader_init_linear(canvas, mode, gradient, &line);
}
gb_shader_ref_t gb_shader_init2i_linear(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, tb_long_t xb, tb_long_t yb, tb_long_t xe, tb_long_t ye)
{
    // make line
    gb_line_t line;
    gb_line_imake(&line, xb, yb, xe, ye);

    // init
    return gb_shader_init_linear(canvas, mode, gradient, &line);
}
gb_shader_ref_t gb_shader_init_radial(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_circle_ref_t circle)
{
    // check
    tb_assert_and_check_return_val(canvas, tb_null);

    // the device 
    gb_device_impl_t* device = (gb_device_impl_t*)gb_canvas_device(canvas);
    tb_assert_and_check_return_val(device && device->shader_radial, tb_null);

    // init shader
    return device->shader_radial(device, mode, gradient, circle);
}
gb_shader_ref_t gb_shader_init2_radial(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    // make circle
    gb_circle_t circle;
    gb_circle_make(&circle, x0, y0, r);

    // init
    return gb_shader_init_radial(canvas, mode, gradient, &circle);
}
gb_shader_ref_t gb_shader_init2i_radial(gb_canvas_ref_t canvas, tb_size_t mode, gb_gradient_ref_t gradient, tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    // make circle
    gb_circle_t circle;
    gb_circle_imake(&circle, x0, y0, r);

    // init
    return gb_shader_init_radial(canvas, mode, gradient, &circle);
}
gb_shader_ref_t gb_shader_init_bitmap(gb_canvas_ref_t canvas, tb_size_t mode, gb_bitmap_ref_t bitmap)
{
    // check
    tb_assert_and_check_return_val(canvas, tb_null);

    // the device 
    gb_device_impl_t* device = (gb_device_impl_t*)gb_canvas_device(canvas);
    tb_assert_and_check_return_val(device && device->shader_bitmap, tb_null);

    // init shader
    return device->shader_bitmap(device, mode, bitmap);
}
tb_void_t gb_shader_exit(gb_shader_ref_t shader)
{
    // exit it
    gb_shader_dec(shader);
}
tb_size_t gb_shader_type(gb_shader_ref_t shader)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return_val(impl, GB_SHADER_TYPE_NONE);

    // the type
    return impl->type;
}
tb_size_t gb_shader_mode(gb_shader_ref_t shader)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return_val(impl, GB_SHADER_MODE_NONE);

    // the mode
    return impl->mode;
}
gb_matrix_ref_t gb_shader_matrix(gb_shader_ref_t shader)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return_val(impl, tb_null);

    // the matrix
    return &impl->matrix;
}
tb_void_t gb_shader_matrix_set(gb_shader_ref_t shader, gb_matrix_ref_t matrix)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return(impl && matrix);

    // done
    impl->matrix = *matrix;
}
tb_size_t gb_shader_ref(gb_shader_ref_t shader)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return_val(impl, 0);

    // the reference count
    return impl->refn;
}
tb_void_t gb_shader_inc(gb_shader_ref_t shader)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return(impl);

    // increase the reference count
    impl->refn++;
}
tb_void_t gb_shader_dec(gb_shader_ref_t shader)
{
    // check
    gb_shader_impl_t* impl = (gb_shader_impl_t*)shader;
    tb_assert_and_check_return(impl);

    // check refn
    tb_assert_and_check_return(impl->refn);

    // refn--
    impl->refn--;

    // exit it?
    if (!impl->refn && impl->exit) impl->exit(impl);
}
