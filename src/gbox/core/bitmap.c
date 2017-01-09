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
 * @file        bitmap.c
 * @ingroup     core
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "bitmap.h"
#include "pixmap.h"
#include "bitmap/decoder.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the bitmap impl type
typedef struct __gb_bitmap_impl_t
{
	// the data
	tb_pointer_t 		data;

	// the size
	tb_size_t 			size;

	// is owner?
	tb_uint8_t 			is_owner    : 1;

	// has alpha?
	tb_uint8_t 			has_alpha   : 1;

	// the pixfmt
	tb_uint16_t         pixfmt;

	// the width
	tb_uint16_t         width;

	// the height
	tb_uint16_t         height;

	// the lpitch
	tb_uint16_t         row_bytes;

}gb_bitmap_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_bitmap_ref_t gb_bitmap_init(tb_pointer_t data, tb_size_t pixfmt, tb_size_t width, tb_size_t height, tb_size_t row_bytes, tb_bool_t has_alpha)
{
    // done
    tb_bool_t           ok = tb_false;
    gb_bitmap_impl_t*   impl = tb_null;
    do
    {
        // check
        tb_assert_and_check_break(GB_PIXFMT_OK(pixfmt));
        tb_assert_and_check_break(width && width <= GB_WIDTH_MAXN && height && height <= GB_HEIGHT_MAXN);

        // make bitmap
        impl = tb_malloc0_type(gb_bitmap_impl_t);
        tb_assert_and_check_break(impl);

        // the pixmap, only using btp
        gb_pixmap_ref_t pixmap = gb_pixmap(pixfmt, 0xff);
        tb_assert_and_check_break(pixmap);

        // the row bytes
        if (!row_bytes) row_bytes = width * pixmap->btp;
        tb_assert_and_check_break(row_bytes && row_bytes >= width * pixmap->btp);

        // init bitmap 
        impl->pixfmt        = (tb_uint16_t)pixfmt;
        impl->width 	    = (tb_uint16_t)width;
        impl->height 	    = (tb_uint16_t)height;
        impl->row_bytes 	= (tb_uint16_t)row_bytes;
        impl->size 	        = row_bytes * height;
        impl->data          = data? data : tb_malloc0(impl->size);
        impl->has_alpha     = !!has_alpha;
        impl->is_owner      = !data;
        tb_assert_and_check_break(impl->data);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) gb_bitmap_exit((gb_bitmap_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (gb_bitmap_ref_t)impl;
}
gb_bitmap_ref_t gb_bitmap_init_from_url(tb_size_t pixfmt, tb_char_t const* url)
{
    // check
    tb_assert_and_check_return_val(GB_PIXFMT_OK(pixfmt) && url, tb_null);
    
    // init stream
    tb_stream_ref_t stream = tb_stream_init_from_url(url);
    tb_assert_and_check_return_val(stream, tb_null);

    // init bitmap from stream
    gb_bitmap_ref_t bitmap = tb_null;
    if (tb_stream_open(stream)) bitmap = gb_bitmap_init_from_stream(pixfmt, stream);

    // exit stream
    tb_stream_exit(stream);

    // ok?
    return bitmap;
}
gb_bitmap_ref_t gb_bitmap_init_from_stream(tb_size_t pixfmt, tb_stream_ref_t stream)
{
    // check
    tb_assert_and_check_return_val(GB_PIXFMT_OK(pixfmt) && stream, tb_null);

    // init bitmap decoder
    gb_bitmap_decoder_ref_t decoder = gb_bitmap_decoder_init(pixfmt, stream);
    tb_assert_and_check_return_val(decoder, tb_null);

    // done bitmap decoder
    gb_bitmap_ref_t bitmap = gb_bitmap_decoder_done(decoder);
    tb_assert(bitmap);

    // exit bitmap decoder
    gb_bitmap_decoder_exit(decoder);

    // ok?
    return bitmap;
}
tb_void_t gb_bitmap_exit(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
    tb_assert_and_check_return(impl);

    // exit data
    if (impl->is_owner && impl->data) tb_free(impl->data);
    impl->data = tb_null;

    // exit it
    tb_free(impl);
}
tb_size_t gb_bitmap_size(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, 0);

    // the size
	return impl->size;
}
tb_pointer_t gb_bitmap_data(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, tb_null);

    // the data
	return impl->data;
}
tb_bool_t gb_bitmap_data_set(gb_bitmap_ref_t bitmap, tb_pointer_t data, tb_size_t pixfmt, tb_size_t width, tb_size_t height, tb_size_t row_bytes, tb_bool_t has_alpha)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl && data, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // check
        tb_assert_and_check_break(width && width <= GB_WIDTH_MAXN && height && height <= GB_HEIGHT_MAXN);

        // the pixmap, only using btp
        gb_pixmap_ref_t pixmap = gb_pixmap(pixfmt, 0xff);
        tb_assert_and_check_break(pixmap);

        // the row bytes
        if (!row_bytes) row_bytes = width * pixmap->btp;
        tb_assert_and_check_break(row_bytes && row_bytes >= width * pixmap->btp);

        // exit it first
        if (impl->data && impl->data != data && impl->is_owner) tb_free(impl->data);

        // update bitmap 
        impl->pixfmt        = (tb_uint16_t)pixfmt;
        impl->width 	    = (tb_uint16_t)width;
        impl->height 	    = (tb_uint16_t)height;
        impl->data          = data;
        impl->size          = row_bytes * height;
        impl->row_bytes 	= (tb_uint16_t)row_bytes;
        impl->is_owner      = 1;
        impl->has_alpha     = !!has_alpha;

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
tb_bool_t gb_bitmap_resize(gb_bitmap_ref_t bitmap, tb_size_t width, tb_size_t height)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl && impl->data, tb_false);

	// same?
	tb_check_return_val(impl->width != width || impl->height != height, tb_true);

    // the pixmap, only using btp
    gb_pixmap_ref_t pixmap = gb_pixmap(impl->pixfmt, 0xff);
    tb_assert_and_check_return_val(pixmap, tb_false);

    // space enough?
    if (height * width * pixmap->btp <= impl->size)
    {
        // resize
        impl->width     = (tb_uint16_t)width;
        impl->height    = (tb_uint16_t)height;
        if (impl->is_owner) impl->row_bytes = (tb_uint16_t)(width * pixmap->btp);
        impl->size      = impl->row_bytes * height;
    }
    // grow?
    else
    {
        // must be owner
        tb_assert(impl->is_owner);
        tb_check_return_val(impl->is_owner, tb_false);

		// resize 
        impl->width     = (tb_uint16_t)width;
        impl->height    = (tb_uint16_t)height;
		impl->row_bytes = (tb_uint16_t)(width * pixmap->btp);
        impl->size      = impl->row_bytes * height;
        impl->data      = tb_ralloc(impl->data, impl->size);
        tb_assert_and_check_return_val(impl->data, tb_false);
    }

	// ok
	return tb_true;
}
tb_size_t gb_bitmap_width(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, 0);

    // the width
	return impl->width;
}
tb_size_t gb_bitmap_height(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, 0);

    // the height
	return impl->height;
}
tb_size_t gb_bitmap_pixfmt(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, GB_PIXFMT_NONE);

    // the pixfmt
	return impl->pixfmt;
}
tb_bool_t gb_bitmap_has_alpha(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, tb_false);

    // has alpha?
	return impl->has_alpha? tb_true : tb_false;
}
tb_void_t gb_bitmap_set_alpha(gb_bitmap_ref_t bitmap, tb_bool_t has_alpha)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return(impl);

    // done
    impl->has_alpha = has_alpha;
}
tb_size_t gb_bitmap_row_bytes(gb_bitmap_ref_t bitmap)
{
    // check
	gb_bitmap_impl_t* impl = (gb_bitmap_impl_t*)bitmap;
	tb_assert_and_check_return_val(impl, 0);

    // the row bytes
	return impl->row_bytes;
}
