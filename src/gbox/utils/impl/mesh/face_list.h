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
 * @file        face_list.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_IMPL_MESH_FACE_LIST_H
#define GB_UTILS_IMPL_MESH_FACE_LIST_H

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

// the mesh face list type
typedef struct{}*           gb_mesh_face_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init the mesh face list 
 *
 * @param element           the element
 *
 * @return                  the face list
 */
gb_mesh_face_list_ref_t     gb_mesh_face_list_init(tb_element_t element);

/* exit the mesh face list
 *
 * @param list              the face list
 */
tb_void_t                   gb_mesh_face_list_exit(gb_mesh_face_list_ref_t list);

/* clear the mesh face list
 *
 * @param list              the face list
 */
tb_void_t                   gb_mesh_face_list_clear(gb_mesh_face_list_ref_t list);

/*! the mesh face iterator
 *
 * @param list              the list
 *
 * @return                  the mesh face iterator
 */
tb_iterator_ref_t           gb_mesh_face_list_itor(gb_mesh_face_list_ref_t list);

/*! the mesh face count
 *
 * @param list              the list
 *
 * @return                  the item count
 */
tb_size_t                   gb_mesh_face_list_size(gb_mesh_face_list_ref_t list);

/*! the mesh face maximum count
 *
 * @param list              the list
 *
 * @return                  the item max count
 */
tb_size_t                   gb_mesh_face_list_maxn(gb_mesh_face_list_ref_t list);

/*! make a bare face without faces
 *
 * @param list              the list
 *
 * @return                  the new face
 */
gb_mesh_face_ref_t          gb_mesh_face_list_make(gb_mesh_face_list_ref_t list);

#ifdef __gb_debug__
/*! make the face string
 *
 * @param list              the list
 * @param face              the face
 * @param data              the string data
 * @param maxn              the string maxn
 *
 * @return                  the string size
 */
tb_long_t                   gb_mesh_face_list_cstr(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face, tb_char_t* data, tb_size_t maxn);
#endif

/*! kill the face, release the storage
 *
 * @param list              the list
 * @param face              the face
 */
tb_void_t                   gb_mesh_face_list_kill(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face);

/*! the user data 
 *
 * @param list              the list
 * @param face              the face
 *
 * @return                  the user data
 */
tb_cpointer_t               gb_mesh_face_list_data(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face);

/*! set the user private data 
 *
 * @param list              the list
 * @param face              the face
 * @param data              the user data
 */
tb_void_t                   gb_mesh_face_list_data_set(gb_mesh_face_list_ref_t list, gb_mesh_face_ref_t face, tb_cpointer_t data);

/*! the list order
 *
 * @param list              the list
 *
 * @return                  the order
 */
tb_size_t                   gb_mesh_face_list_order(gb_mesh_face_list_ref_t list);

/*! set the list order
 *
 * @param list              the list
 * @param order             the order
 */
tb_void_t                   gb_mesh_face_list_order_set(gb_mesh_face_list_ref_t list, tb_size_t order);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


