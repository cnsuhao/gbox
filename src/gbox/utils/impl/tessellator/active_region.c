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
 * @file        active_region.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "active_region"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "active_region.h"
#include "geometry.h"
#include "mesh.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// enable test?
#define GB_ACTIVE_REGION_TEST_ENABLE    (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_bool_t gb_tessellator_active_region_leq(gb_tessellator_active_region_ref_t lregion, gb_tessellator_active_region_ref_t rregion)
{
    // check
    tb_assert(lregion && lregion->edge && rregion && rregion->edge);

    // the origin and destination of the left edge
    gb_mesh_vertex_ref_t ledge_org = gb_mesh_edge_org(lregion->edge);
    gb_mesh_vertex_ref_t ledge_dst = gb_mesh_edge_dst(lregion->edge);

    // the origin and destination of the right edge
    gb_mesh_vertex_ref_t redge_org = gb_mesh_edge_org(rregion->edge);
    gb_mesh_vertex_ref_t redge_dst = gb_mesh_edge_dst(rregion->edge);

    /* two edges must go up
     *
     *  / \     / \
     *   |       |
     */
    tb_assertf_abort(gb_tessellator_edge_go_up(lregion->edge), "%{mesh_edge}", lregion->edge);
    tb_assertf_abort(gb_tessellator_edge_go_up(rregion->edge), "%{mesh_edge}", rregion->edge);

    /* 
     *             .
     *     ledge .   .
     *         .       . redge
     *       .           .
     */
    if (ledge_dst == redge_dst)
    {
        /* 
         *             .
         *             ..
         *             .. 
         *       ledge .. redge
         *             ..
         *             ..
         *             .
         */
        if (ledge_org == redge_org) return 1;
        /* 
         *             .
         *     ledge .   .
         *         .       . redge
         *       . --------  .
         *                     .
         *                       .
         *
         */
        else if (gb_tessellator_vertex_in_top_or_hleft_or_eq(ledge_org, redge_org)) 
            return gb_tessellator_vertex_on_edge_or_left(ledge_org, redge_dst, redge_org);
        /* 
         *           .                      
         *         .   . redge      
         * ledge .       .           
         *     . --------- .       
         *   .                                 
         * .                                         
         *
         */
        else return gb_tessellator_vertex_on_edge_or_right(redge_org, ledge_dst, ledge_org);
    }
    /* 
     *      .                  .
     *        .              .
     *    ledge .          . redge
     *            .      .
     *
     * or
     *
     *      .                   .
     *        .               .
     *    ledge .           . redge
     *            .       .
     *              .   .
     *                .
     *
     * or
     *          .           . 
     *            .       .
     *        ledge .   . redge
     *                .
     *              .   .
     *            .       .
     *          .           .
     */
    else
    {
        /* 
         *  .
         *    .
         *      . ---------------- .
         *        .              .
         *    ledge .          . redge
         *            .      .
         *
         * or
         *
         *  .
         *    .
         *      . ----------------- .
         *        .               .
         *    ledge .           . redge
         *            .       .
         *              .   .
         *                .
         *
         * or
         *      .
         *        .
         *          . --------- . 
         *            .       .
         *        ledge .   . redge
         *                .
         *              .   .
         *            .       .
         *          .           .
         */
        if (gb_tessellator_vertex_in_top_or_hleft_or_eq(ledge_dst, redge_dst))
            return gb_tessellator_vertex_on_edge_or_right(redge_dst, ledge_dst, ledge_org);
        /* 
         *                             .
         *                           .
         *      . ---------------- .
         *        .              .
         *    ledge .          . redge
         *            .      .
         *
         * or
         *
         *                              .
         *                            .
         *      . ----------------- .
         *        .               .
         *    ledge .           . redge
         *            .       .
         *              .   .
         *                .
         *
         * or
         *                          .
         *                        .
         *          . --------- . 
         *            .       .
         *        ledge .   . redge
         *                .
         *              .   .
         *            .       .
         *          .           .
         */
        else return gb_tessellator_vertex_on_edge_or_left(ledge_dst, redge_dst, redge_org);
    }

    // failed
    tb_assert(0);
    return 0;
}
static tb_long_t gb_tessellator_active_region_comp(tb_element_ref_t element, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
    // lregion <= rregion ? -1 : 1
    return (!gb_tessellator_active_region_leq((gb_tessellator_active_region_ref_t)ldata, (gb_tessellator_active_region_ref_t)rdata) << 1) - 1;
}
#ifdef __gb_debug__
static tb_char_t const* gb_tessellator_active_region_cstr(tb_element_ref_t element, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    gb_tessellator_active_region_ref_t region = (gb_tessellator_active_region_ref_t)data;
    tb_assert_and_check_return_val(region, tb_null);

    // make info
    tb_long_t size = tb_snprintf(cstr, maxn, "%{tess_region}.%{mesh_edge}", region, region->edge);
    if (size >= 0) cstr[size] = '\0';

    // ok?
    return cstr;
}
static tb_long_t gb_tessellator_active_region_printf(tb_cpointer_t object, tb_char_t* cstr, tb_size_t maxn)
{
    // check
    tb_assert_and_check_return_val(object && cstr && maxn, -1);

    // the region
    gb_tessellator_active_region_ref_t region = (gb_tessellator_active_region_ref_t)object;

    // is bounds region?
    if (region->bounds) return tb_snprintf(cstr, maxn, "(e%lu.bounds)", region->edge->id);

    // is fixable edge region?
    if (region->fixedge) return tb_snprintf(cstr, maxn, "(e%lu.fixable)", region->edge->id);

    // make info
    return tb_snprintf( cstr
                    ,   maxn
                    ,   "(e%lu.w%d, winding: %ld, inside: %d)"
                    ,   region->edge->id
                    ,   gb_tessellator_edge_winding(region->edge)
                    ,   region->winding
                    ,   region->inside);
}
#endif
/* insert region in ascending order and save the region position
 *
 * r0 ----> r1 ------> r2 -------> r3 ---> ... ---->
 *                 region_prev
 *                     |----------------->
 *                           insert
 *
 */
static gb_tessellator_active_region_ref_t gb_tessellator_active_regions_insert_done(gb_tessellator_impl_t* impl, tb_size_t prev, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert(impl && impl->active_regions && region && region->edge);

    // the edge must go up
    tb_assertf_abort(gb_tessellator_edge_go_up(region->edge), "%{mesh_edge}", region->edge);

    // trace
    tb_trace_d("insert: %{mesh_edge}", region->edge);

    // reverse to find the inserted position
    tb_size_t itor = tb_find_if(impl->active_regions, prev, tb_iterator_tail(impl->active_regions), tb_predicate_beq, region);

    // trace
    tb_trace_d("insert: find count: %lu", tb_distance(impl->active_regions, prev, itor));

    // insert the region to the next position
    itor = tb_list_insert_prev(impl->active_regions, itor, region);
    tb_assert(itor != tb_iterator_tail(impl->active_regions));

    // get the real region reference 
    region = (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, itor);
    tb_assert(region);

    // save the region position
    region->position = itor;

    // save the region reference to the edge
    gb_tessellator_edge_region_set(region->edge, region);

    // ok
    return region;
}
/* insert region for the bounds in ascending order
 *
 * dst(event)
 * / \
 *  |  region
 *  |
 * org
 */
static tb_void_t gb_tessellator_active_regions_insert_bounds(gb_tessellator_impl_t* impl, gb_float_t x, gb_float_t y_org, gb_float_t y_dst)
{
    // check
    tb_assert(impl && impl->mesh && impl->active_regions);

    // init two points of the new edge
    gb_point_t org;
    gb_point_t dst;
    gb_point_make(&org, x, y_org);
    gb_point_make(&dst, x, y_dst);

    // make edge
    gb_mesh_edge_ref_t edge = gb_tessellator_mesh_make_edge(impl, &org, &dst);
    tb_assert(edge);

    // update the current sweep event for inserting region in ascending order
    impl->event = gb_mesh_edge_dst(edge);

    // make region
    gb_tessellator_active_region_t region;
    region.edge     = edge;
    region.winding  = 0;
    region.inside   = 0;
    region.fixedge  = 0;
    region.bounds   = 1;

    // insert region
    gb_tessellator_active_regions_insert(impl, &region);
}
#if GB_ACTIVE_REGION_TEST_ENABLE && defined(__gb_debug__)
static tb_void_t gb_tessellator_active_regions_test_insert(gb_tessellator_impl_t* impl, gb_float_t sweep_xb, gb_float_t sweep_xe, gb_float_t sweep_y)
{
    // check
    tb_assert(impl && impl->mesh && impl->active_regions);

    // init coordinates
    tb_long_t xb = tb_random_range(tb_null, gb_float_to_long(sweep_xb), gb_float_to_long(sweep_xe));
    tb_long_t xe = tb_random_range(tb_null, gb_float_to_long(sweep_xb), gb_float_to_long(sweep_xe));
    tb_long_t yb = tb_random_range(tb_null, 1, 200);
    tb_long_t ye = tb_random_range(tb_null, 1, 200);

    // init two points of the new edge
    gb_point_t org;
    gb_point_t dst;
    gb_point_make(&org, gb_long_to_float(xb), sweep_y + gb_long_to_float(yb));
    gb_point_make(&dst, gb_long_to_float(xe), sweep_y - gb_long_to_float(ye));

    // make edge
    gb_mesh_edge_ref_t edge = gb_tessellator_mesh_make_edge(impl, &org, &dst);
    tb_assert(edge);

    // make region
    gb_tessellator_active_region_t region;
    region.edge     = edge;
    region.winding  = 0;
    region.inside   = 0;
    region.bounds   = 1;

    // insert region
    gb_tessellator_active_regions_insert(impl, &region);
}
static tb_void_t gb_tessellator_active_regions_test(gb_tessellator_impl_t* impl, gb_float_t sweep_xb, gb_float_t sweep_xe, gb_float_t sweep_y)
{
    // check
    tb_assert(impl && impl->active_regions);

    // make the current sweep event point
    gb_point_t point;
    gb_point_make(&point, gb_avg(sweep_xb, sweep_xe), sweep_y);

    // insert some regions 
    __tb_volatile__ tb_size_t count = 20;
    while (count--) gb_tessellator_active_regions_test_insert(impl, sweep_xb, sweep_xe, sweep_y);

    // make the codes for drawing sweep line
    tb_printf(  "    gb_canvas_color_set(canvas, GB_COLOR_BLACK);\n");
    tb_printf(  "    gb_canvas_draw_line2i(canvas, %ld, %ld, %ld, %ld);\n"
            ,   gb_float_to_long(sweep_xb)
            ,   gb_float_to_long(sweep_y)
            ,   gb_float_to_long(sweep_xe)
            ,   gb_float_to_long(sweep_y));

    // dump the codes for drawing
    tb_size_t index = 1;
    tb_for_all_if (gb_tessellator_active_region_ref_t, region, impl->active_regions, region)
    {
        // the edge
        gb_mesh_edge_ref_t edge = region->edge;

        // the points
        gb_point_ref_t org = gb_tessellator_vertex_point(gb_mesh_edge_org(edge));
        gb_point_ref_t dst = gb_tessellator_vertex_point(gb_mesh_edge_dst(edge));

        // make the codes
        tb_printf(  "    gb_canvas_color_set(canvas, gb_color_from_index(%lu));\n", index++);
        tb_printf(  "    gb_canvas_draw_line2i(canvas, %ld, %ld, %ld, %ld);\n"
                ,   gb_float_to_long(org->x)
                ,   gb_float_to_long(org->y)
                ,   gb_float_to_long(dst->x)
                ,   gb_float_to_long(dst->y));
    }
}
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t gb_tessellator_active_regions_make(gb_tessellator_impl_t* impl, gb_rect_ref_t bounds)
{
    // check
    tb_assert(impl && bounds && bounds->w > 0 && bounds->h > 0);

    // init active regions
    if (!impl->active_regions) 
    {
        // make active region element
        tb_element_t element = tb_element_mem(sizeof(gb_tessellator_active_region_t), tb_null, tb_null);

        // init the comparator 
        element.comp = gb_tessellator_active_region_comp;

#ifdef __gb_debug__
        // init the c-string function for tb_list_dump
        element.cstr = gb_tessellator_active_region_cstr;

        // register printf("%{tess_region}", region);
        static tb_bool_t s_is_registered = tb_false;
        if (!s_is_registered)
        {
            // register it
            tb_printf_object_register("tess_region", gb_tessellator_active_region_printf);

            // ok
            s_is_registered = tb_true;
        }
#endif

        // make active regions
        impl->active_regions = tb_list_init(0, element);
    }
    tb_assert_abort_and_check_return_val(impl->active_regions, tb_false);

    // clear active regions first
    tb_list_clear(impl->active_regions);

    /* insert two regions for the bounds to avoid special cases
     *
     * their coordinates are big enough that they will never be merged with real input features.
     * 
     * (xb, yb)
     *   / \         .        .    .     / \
     *    |  region    .     .     .      |  region
     *    |              .  .    .        |
     *                                 (xe, ye)
     */
    gb_float_t xb = bounds->x - GB_ONE;
    gb_float_t yb = bounds->y - GB_ONE;
    gb_float_t xe = bounds->x + bounds->w + GB_ONE;
    gb_float_t ye = bounds->y + bounds->h + GB_ONE;
    gb_tessellator_active_regions_insert_bounds(impl, xb, ye, yb);
    gb_tessellator_active_regions_insert_bounds(impl, xe, ye, yb);

#if GB_ACTIVE_REGION_TEST_ENABLE && defined(__gb_debug__)
    // test the active regions
    gb_tessellator_active_regions_test(impl, xb, xe, gb_avg(yb, ye));
#endif

    // ok
    return tb_list_size(impl->active_regions);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_find(gb_tessellator_impl_t* impl, gb_mesh_edge_ref_t edge)
{
    // check
    tb_assert(impl && impl->active_regions && edge);

    // make a temporary region with the given edge for finding the real region containing it
    gb_tessellator_active_region_t region_temp;
    region_temp.edge = edge;

    // the edge must go up
    tb_assertf_abort(gb_tessellator_edge_go_up(edge), "%{mesh_edge}", edge);

    /* reverse to find the region containing the given edge from the regions
     *
     * region.edge
     * |
     * .                .     .
     * .                .      .
     * .     region1    .       .
     * .                .        . region3
     * .         .      .         .
     * .       .        . region2  .
     * .     . edge     .
     *          |
     *        found                <= find direction
     *
     *
     */
    tb_size_t itor = tb_rfind_all_if(impl->active_regions, tb_predicate_le, &region_temp);

    // get the found item
    return (itor != tb_iterator_tail(impl->active_regions))? (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, itor) : tb_null;
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_left(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert(impl && impl->active_regions);
    tb_assert(region && region->position != tb_iterator_tail(impl->active_regions));
    
    // get the prev position
    tb_size_t position = tb_iterator_prev(impl->active_regions, region->position);

    // no left region?
    tb_check_return_val(position != tb_iterator_tail(impl->active_regions), tb_null);

    // ok
    return (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, position);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_right(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert(impl && impl->active_regions);
    tb_assert(region && region->position != tb_iterator_tail(impl->active_regions));
    
    // get the next position
    tb_size_t position = tb_iterator_next(impl->active_regions, region->position);

    // no right region?
    tb_check_return_val(position != tb_iterator_tail(impl->active_regions), tb_null);

    // ok
    return (gb_tessellator_active_region_ref_t)tb_iterator_item(impl->active_regions, position);
}
tb_bool_t gb_tessellator_active_regions_in_left(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region1, gb_tessellator_active_region_ref_t region2)
{
    // region1 <= region2
    return gb_tessellator_active_region_leq(region1, region2);
}
tb_void_t gb_tessellator_active_regions_remove(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert(impl && impl->active_regions && region && region->edge);
    tb_assert(region->position != tb_iterator_tail(impl->active_regions));

    // it can not be a real edge if the left edge need fix, then we will remove it 
    tb_assert(!region->fixedge || !gb_tessellator_edge_winding(region->edge));

    // clear the region reference for the edge
    gb_tessellator_edge_region_set(region->edge, tb_null);

    // remove it
    tb_list_remove(impl->active_regions, region->position);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_insert(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert(impl && impl->active_regions && region);

    // insert it
    return gb_tessellator_active_regions_insert_done(impl, tb_iterator_head(impl->active_regions), region);
}
gb_tessellator_active_region_ref_t gb_tessellator_active_regions_insert_after(gb_tessellator_impl_t* impl, gb_tessellator_active_region_ref_t region_prev, gb_tessellator_active_region_ref_t region)
{
    // check
    tb_assert(impl && impl->active_regions && region_prev && region);
    tb_assert(region_prev->position != tb_iterator_tail(impl->active_regions));

    // region_prev <= region
    tb_assert(tb_iterator_comp(impl->active_regions, region_prev, region) <= 0);

    // insert it
    return gb_tessellator_active_regions_insert_done(impl, region_prev->position, region);
}
#ifdef __gb_debug__
tb_void_t gb_tessellator_active_regions_check(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert(impl && impl->active_regions);

    // done
    gb_tessellator_active_region_ref_t region_prev = tb_null;
    tb_for_all_if (gb_tessellator_active_region_ref_t, region, impl->active_regions, region)
    {
        // check order
        if (region_prev)
        {
            // the order is error?
            if (tb_iterator_comp(impl->active_regions, region_prev, region) > 0)
            {
                // trace
                tb_trace_i("the order of the active regions is error with event: %{mesh_vertex}", impl->event);

                // dump it
#           ifdef __tb_debug__
                tb_list_dump(impl->active_regions);
#           endif

                // trace
                tb_trace_i("%{mesh_edge}", region_prev->edge);
                tb_trace_i("<?=");
                tb_trace_i("%{mesh_edge}", region->edge);

                // abort it
                tb_assert(0);
            }
        }

        // the edge must go up
        tb_assertf_abort(gb_tessellator_edge_go_up(region->edge), "%{mesh_edge}", region->edge);

        // update the previous region
        region_prev = region;
    }
}
#endif

