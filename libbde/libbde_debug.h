/*
 * Debug functions
 *
 * Copyright (C) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBBDE_DEBUG_H )
#define _LIBBDE_DEBUG_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

const char *libbde_debug_print_encryption_method(
             uint32_t encryption_method );

const char *libbde_debug_print_entry_type(
             uint16_t entry_type );

const char *libbde_debug_print_value_type(
             uint16_t value_type );

const char *libbde_debug_print_volume_master_key_type(
             uint32_t volume_master_key_type );

int libbde_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif
