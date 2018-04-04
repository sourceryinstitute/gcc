/* ISO_Fortran_binding.c of GCC's GNU Fortran compiler.
   Copyright (c) 2018 Free Software Foundation, Inc.

   This file is part of the GNU Fortran runtime library (libgfortran)
   Libgfortran is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   Libgfortran is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with libquadmath; see the file COPYING.LIB.  If
   not, write to the Free Software Foundation, Inc., 51 Franklin Street
   - Fifth Floor, Boston, MA 02110-1301, USA. */

/* Functions as defined by ISO/IEC 1539-1:2017
   on Further Interoperability of Fortran with C.

   Note: This file implements the functions defined in the ISO_Fortran_binding.h
   header file.
*/

/* This file is a work in progress, it is not meant to compile and test features, that is what ISO_Fortran_binding_prototyping_tests.c is for. I'm currently annotating the structures according to the standard just to have them in-file so I know what to do with them. */

#include "ISO_Fortran_binding.h"
#include <stdio.h>
#include <stdlib.h>
/*
typedef struct CFI_dim_t
{
  // Value of the lower bound of the dimension being described.
  CFI_index_t lower_bound;
  // Number of elments in the dimension being described.
  // If object is an assumed size array the value at the last dimension is -1.
  CFI_index_t extent;
  // The difference in bytes between the addresses of successive elements in the dimension.
  CFI_index_t sm;
}
CFI_dim_t;

typedef struct CFI_cdesc_t
{
  // NULL if unallocated allocatable or pointer.
  // Processor dependent if object has 0 size.
  // C address of the first element in Fortran array order.
  void *base_addr;
  // Storage size in bytes of object if scalar.
  // Storage size in bytes of a single elment in object if not scalar.
  size_t elem_len;
  // CFI_VERSION in ISO_Fortran.h file.
  int version;
  // Number of dimensions of Fortran object (scalar is 0).
  CFI_rank_t rank;
  // Value of the specifier for the memory characteristics of the object, whether it is an allocatable, a pointer, or a nonallocatable nonpointer object.
  CFI_attribute_t attribute;
  // Value of the specifier for the type of the object. Each C interoperable has its own specifier.
  CFI_type_t type;
  // Not described in ISO/IEC 1539-1:2017
  size_t offset;
  // Number of elements in dim is the rank of the object.
  // If the object is an array pointer or allocatable array, the value of dim[].lower_bound is determined by argument association, allocation or pointer association.
  // If the object is a nonallocatable nonpointer, the value of dim[].lower_bound = 0.
  // The N dimensions are ordered such that:
  // n = 0, 1, 2, ..., N - 1
  // abs( dim[n=0].sm ) >= elem_len &&
  // abs( dim[n+1].sm ) >= abs( dim[n].sm ) * dim[n].extent && ... &&
  // abs( dim[N-1].sm ) >= abs( dim[N-2].sm ) * dim[N-2].extent
  // In an assumed size array the extent of the last element is equal to -1, dim[N-1].extent = -1
  CFI_dim_t dim[];
}
CFI_cdesc_t;
/*
/* Functions */

void *CFI_address (const CFI_cdesc_t *dv, const CFI_index_t subscripts[]){
  if (dv == NULL){
    fprintf(stderr, "ISO_Fortran_binding.c: CFI_address: ERROR BASE ADDRESS NULL (%d).\n", CFI_ERROR_BASE_ADDR_NULL);
    exit(EXIT_FAILURE);
  }
  return *dv->base_addr;
}

void main(){
  CFI_CDESC_TYPE_T(5, float) *dv;
  CFI_index_t subscripts[2];
  float *address;

  dv = malloc(sizeof(CFI_CDESC_TYPE_T(5, float)));
  dv->base_addr = malloc(sizeof(float));
  *dv->base_addr = 3.14159;
  printf("%f\n",*dv->base_addr);

  subscripts[0] = 4;
  subscripts[1] = 9;
  address = CFI_address(dv, subscripts);
  printf("%f\n", address);

  free(dv);
}
