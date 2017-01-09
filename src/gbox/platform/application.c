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
 * @file        application.c
 * @ingroup     platform
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../gbox.h"
#include "impl/window.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the application impl type
typedef struct __gb_application_impl_t
{
    // the arguments count
    tb_size_t                   argc;

    // the arguments list
    tb_char_t**                 argv;

    // the private data
    tb_cpointer_t               priv;

    // the window info
    gb_window_info_t            info;

    // the window
    gb_window_ref_t             window;

    // loaded?
    tb_bool_t                   loaded;

}gb_application_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the application
static gb_application_ref_t     g_application = tb_null;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_application_ref_t gb_application()
{
    return g_application;
}
tb_int_t gb_application_main(tb_int_t argc, tb_char_t** argv, gb_application_init_func_t init)
{
    // done
    tb_int_t                ok = -1;
    gb_application_impl_t*  impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(init);

        // init tbox
        if (!tb_init(tb_null, tb_null)) break;

        // init gbox
        if (!gb_init()) break;

        // make application
        impl = tb_malloc0_type(gb_application_impl_t);
        tb_assert_and_check_break(impl);

        // save application
        g_application = (gb_application_ref_t)impl;

        // init arguments
        impl->argc = argc;
        impl->argv = argv;

        // init window info
        impl->info.framerate    = GB_WINDOW_DEFAULT_FRAMERATE;
        impl->info.flag         = GB_WINDOW_FLAG_NONE;

        // notify: init application
        if (!init((gb_application_ref_t)impl, &impl->info)) break;

        // init window
        impl->window = gb_window_init(&impl->info);
        tb_assert_and_check_break(impl->window);

        // loop window
        gb_window_loop(impl->window);

        // ok
        ok = 0;

    } while (0);

    // clear application 
    g_application = tb_null;

    // exit application
    if (impl) 
    {
        // exit window
        if (impl->window) gb_window_exit(impl->window);
        impl->window = tb_null;

        // exit it
        tb_free(impl);
    }
    impl = tb_null;

    // exit gbox
    gb_exit();

    // exit tbox
    tb_exit();

    // ok?
    return ok;
}
tb_size_t gb_application_argc(gb_application_ref_t application)
{
    // check
    gb_application_impl_t* impl = (gb_application_impl_t*)application;
    tb_assert_and_check_return_val(impl, 0);

    // the argc
    return impl->argc;
}
tb_char_t** gb_application_argv(gb_application_ref_t application)
{
    // check
    gb_application_impl_t* impl = (gb_application_impl_t*)application;
    tb_assert_and_check_return_val(impl, tb_null);

    // the argv
    return impl->argv;
}
tb_cpointer_t gb_application_getp(gb_application_ref_t application)
{
    // check
    gb_application_impl_t* impl = (gb_application_impl_t*)application;
    tb_assert_and_check_return_val(impl, tb_null);

    // the private data
    return impl->priv;
}
tb_void_t gb_application_setp(gb_application_ref_t application, tb_cpointer_t priv)
{
    // check
    gb_application_impl_t* impl = (gb_application_impl_t*)application;
    tb_assert_and_check_return(impl);

    // done
    impl->priv = priv;
}
gb_window_ref_t gb_application_window(gb_application_ref_t application)
{
    // check
    gb_application_impl_t* impl = (gb_application_impl_t*)application;
    tb_assert_and_check_return_val(impl, tb_null);

    // the window
    return impl->window;
}
