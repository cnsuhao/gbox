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
 * @file        mesh.h
 * @ingroup     utils
 */
#ifndef GB_UTILS_IMPL_TESSELLATOR_MESH_H
#define GB_UTILS_IMPL_TESSELLATOR_MESH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* make mesh from the given polygon
 *
 * @param impl      the tessellator impl
 * @param polygon   the polygon
 *
 * @return          tb_true or tb_false
 */
tb_bool_t           gb_tessellator_mesh_make(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon);

/* make an empty non-loop edge and initialize it
 *
 * @param impl      the tessellator impl
 * @param org       the original point, do not initialize it if be null
 * @param dst       the destinate point, do not initialize it if be null
 *
 * @return          the edge
 */
gb_mesh_edge_ref_t  gb_tessellator_mesh_make_edge(gb_tessellator_impl_t* impl, gb_point_ref_t org, gb_point_ref_t dst);

#endif


