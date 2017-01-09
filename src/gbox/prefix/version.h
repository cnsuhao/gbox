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
 * @file        version.h
 *
 */
#ifndef GB_PREFIX_VERSION_H
#define GB_PREFIX_VERSION_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "config.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// the major version
#define GB_VERSION_MAJOR            GB_CONFIG_VERSION_MAJOR

/// the minor version
#define GB_VERSION_MINOR            GB_CONFIG_VERSION_MINOR

/// the alter version
#define GB_VERSION_ALTER            GB_CONFIG_VERSION_ALTER

/// the build version
#define GB_VERSION_BUILD            GB_CONFIG_VERSION_BUILD

/// the build version string
#define GB_VERSION_BUILD_STRING     __tb_mstring_ex__(GB_CONFIG_VERSION_BUILD)

/// the version string
#define GB_VERSION_STRING           __tb_mstrcat6__("gbox_", __tb_mstring_ex__(__tb_mconcat8_ex__(v, GB_VERSION_MAJOR, _, GB_VERSION_MINOR, _, GB_VERSION_ALTER, _, GB_CONFIG_VERSION_BUILD)), "_", TB_ARCH_VERSION_STRING, " by ", TB_COMPILER_VERSION_STRING)

/// the short version string
#define GB_VERSION_SHORT_STRING     __tb_mstrcat__("gbox_", __tb_mstring_ex__(__tb_mconcat8_ex__(v, GB_VERSION_MAJOR, _, GB_VERSION_MINOR, _, GB_VERSION_ALTER, _, GB_CONFIG_VERSION_BUILD)))

#endif


