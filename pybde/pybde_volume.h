/*
 * Python object definition of the libbde volume
 *
 * Copyright (c) 2011-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYBDE_VOLUME_H )
#define _PYBDE_VOLUME_H

#include <common.h>
#include <types.h>

#include "pybde_libbde.h"
#include "pybde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pybde_volume pybde_volume_t;

struct pybde_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libbde volume
	 */
	libbde_volume_t *volume;
};

extern PyMethodDef pybde_volume_object_methods[];
extern PyTypeObject pybde_volume_type_object;

PyObject *pybde_volume_new(
           void );

PyObject *pybde_volume_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_new_open_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pybde_volume_init(
     pybde_volume_t *pybde_volume );

void pybde_volume_free(
      pybde_volume_t *pybde_volume );

PyObject *pybde_volume_signal_abort(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_open(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_open_file_object(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_close(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_read_buffer(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_read_random(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_seek_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_get_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_size(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_set_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_set_recovery_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif
