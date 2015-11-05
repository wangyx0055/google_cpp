/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#ifndef LIBRARIES_NACL_IO_OSMMAN_H
#define LIBRARIES_NACL_IO_OSMMAN_H

#if defined(WIN32)

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4
#define PROT_NONE  0x0

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_FIXED     0x10
#define MAP_ANONYMOUS 0x20
#define MAP_FAILED (void*)-1

#else

#include <sys/mman.h>

#endif

#endif  /* LIBRARIES_NACL_IO_OSMMAN_H */
