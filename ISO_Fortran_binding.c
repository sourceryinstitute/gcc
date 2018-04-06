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

// typedef struct CFI_dim_t
// {
//   // Value of the lower bound of the dimension being described.
//   CFI_index_t lower_bound;
//   // Number of elments in the dimension being described.
//   // If object is an assumed size array the value at the last dimension is -1.
//   CFI_index_t extent;
//   // The difference in bytes between the addresses of successive elements in the dimension.
//   CFI_index_t sm;
// }
// CFI_dim_t;
//
// typedef struct CFI_cdesc_t
// {
//   // NULL if unallocated allocatable or pointer.
//   // Processor dependent if object has 0 size.
//   // C address of the first element in Fortran array order.
//   void *base_addr;
//   // Storage size in bytes of object if scalar.
//   // Storage size in bytes of a single elment in object if not scalar.
//   size_t elem_len;
//   // CFI_VERSION in ISO_Fortran.h file.
//   int version;
//   // Number of dimensions of Fortran object (scalar is 0).
//   CFI_rank_t rank;
//   // Value of the specifier for the memory characteristics of the object, whether it is an allocatable, a pointer, or a nonallocatable nonpointer object.
//   CFI_attribute_t attribute;
//   // Value of the specifier for the type of the object. Each C interoperable has its own specifier.
//   CFI_type_t type;
//   // Not described in ISO/IEC 1539-1:2017
//   size_t offset;
//   // Number of elements in dim is the rank of the object.
//   // If the object is an array pointer or allocatable array, the value of dim[].lower_bound is determined by argument association, allocation or pointer association.
//   // If the object is a nonallocatable nonpointer, the value of dim[].lower_bound = 0.
//   // The N dimensions are ordered such that:
//   // n = 0, 1, 2, ..., N - 1
//   // abs( dim[n=0].sm ) >= elem_len &&
//   // abs( dim[n+1].sm ) >= abs( dim[n].sm ) * dim[n].extent && ... &&
//   // abs( dim[N-1].sm ) >= abs( dim[N-2].sm ) * dim[N-2].extent
//   // In an assumed size array the extent of the last element is equal to -1, dim[N-1].extent = -1
//   CFI_dim_t dim[];
// }
// CFI_cdesc_t;
/* Definitions */
#define type(x) _Generic((x), \
  int: "int", \
  float: "float", \
  double: "double", \
  ptrdiff_t: "ptrdiff_t", \
  size_t: "size_t", \
  int8_t: "int8_t", \
  int *: "int *", \
  float *: "float *", \
  double *: "double *", \
  ptrdiff_t *: "ptrdiff_t *", \
  size_t *: "size_t *", \
  int8_t *: "int8_t *", \
  default: "other")

/* Functions */

void *CFI_address (const CFI_cdesc_t *dv, const CFI_index_t subscripts[]){

  // C Descriptor should be allocated.
  if (dv == NULL){
    fprintf(stderr, "ISO_Fortran_binding.c: CFI_address: NULL C Descriptor. (Error No. %d).\n", CFI_ERROR_MEM_ALLOCATION);
    exit(EXIT_FAILURE);
  }

  // Base address of C Descriptor should be allocated.
  if (dv->base_addr == NULL){
    fprintf(stderr, "ISO_Fortran_binding.c: CFI_address: NULL base address of C Descriptor. (Error No. %d).\n", CFI_ERROR_BASE_ADDR_NULL);
    exit(EXIT_FAILURE);
  }

  CFI_rank_t rank = dv->rank;

  // dv is a scalar.
  if (rank == 0){
    // Base address is the C Descriptor base address.
    return dv->base_addr;
  }
  // dv is not a scalar.
  else{
    // Base address is the C address of the element of the object specified by
    // subscripts.
    void *base_addr;
    // In order to properly account for Fortran's row order we need to
    // transpose the subscripts.
    CFI_index_t *tr_subscripts;
    CFI_dim_t *tr_dim;
    tr_subscripts = malloc(rank*sizeof(CFI_index_t));
    tr_dim = malloc(rank*sizeof(CFI_dim_t));
    for (int i = 0; i < rank; i++){
      tr_subscripts[i] = subscripts[rank - i - 1];
      tr_dim[i] = dv->dim[rank - i - 1];
    }
    // We assume column major order as that is how fortran stores arrays.
    // We calculate the memory address of the specified element via the
    // canonical array dimension reduction map and multiplying by the memory
    // stride.
    CFI_index_t index = tr_subscripts[0];
    // Make sure the first subscript is in-bounds.
    if (subscripts[0] > dv->dim[0].extent - dv->dim[0].lower_bound){
      fprintf(stderr, "ISO_Fortran_binding.c: CFI_address: subscripts[%d] = %d, is out of bounds. It must not be greater than dv->dim[%d].extent - dv->dim[%d].lower_bound = %d - %d = %d. (Error No. %d).\n", 0, subscripts[0], 0, 0, dv->dim[0].extent, dv->dim[0].lower_bound, dv->dim[0].extent - dv->dim[0].lower_bound, CFI_ERROR_OUT_OF_BOUNDS);
      exit(EXIT_FAILURE);
    }
    // Start calculating the memory offset.
    CFI_index_t tmp_index = 1;
    for (int i = 1; i < rank; i++){
      // Makre sure the subscripts are in-bounds.
      if (subscripts[i] > dv->dim[i].extent - dv->dim[i].lower_bound){
        fprintf(stderr, "ISO_Fortran_binding.c: CFI_address: subscripts[%d] = %d, is out of bounds. It must not be greater than dv->dim[%d].extent - dv->dim[%d].lower_bound = %d - %d = %d. (Error No. %d).\n", i, subscripts[i], i, i, dv->dim[i].extent, dv->dim[i].lower_bound, dv->dim[i].extent - dv->dim[i].lower_bound, CFI_ERROR_OUT_OF_BOUNDS);
        exit(EXIT_FAILURE);
      }
      // Find memory location of the subscripted item by mapping the subscripts to a 1D array while taking the memory stride into account.
      tmp_index *= tr_subscripts[i] * tr_dim[i-1].extent * tr_dim[i-1].sm;
      index += tmp_index;
    }
    free(tr_subscripts);
    free(tr_dim);
    // There's no way in C to do general arithmetic on a void pointer so we
    // cast to a char pointer, do the arithmetic and cast back to a void
    // pointer.
    base_addr = (char *) dv->base_addr + index;
    return base_addr;
  }
}

void main(){
  CFI_CDESC_T(2) *dv;
  CFI_index_t subscripts[2];
  CFI_index_t test[0];
  CFI_index_t *address;

  dv = malloc(sizeof(CFI_CDESC_T(2)));
  dv->base_addr = malloc(sizeof(CFI_index_t));
  *(CFI_index_t*)dv->base_addr = 1;
  printf("%s\n", type(*dv->base_addr));
  printf("------------\n");
  printf("%ld\n", dv->base_addr);
  dv->rank = 2;
  dv->dim[0].lower_bound = 1;
  dv->dim[0].extent = 3;
  dv->dim[0].sm = 1;
  dv->dim[1].lower_bound = 1;
  dv->dim[1].extent = 2;
  dv->dim[1].sm = 1;
  printf("------------\n");
  for (int i = 0; i < dv->dim[0].extent; i++){
    for (int j = 0; j < dv->dim[1].extent; j++){
      subscripts[0] = i;
      subscripts[1] = j;
      address = (CFI_index_t*) CFI_address(dv, subscripts);
      printf("A[%d, %d] = %d\n", i+1, j+1, (char*)address);
    }
  }
  // subscripts[0] = 0;
  // subscripts[1] = 0;
  // address = (CFI_index_t*) CFI_address(dv, subscripts);
  // printf("%d\n", (char*)address);
  // printf("%ld\n", *address);
  // printf("%d, %d, %d\n", sizeof(subscripts), sizeof(CFI_index_t), sizeof(test));

  free(dv->base_addr);
  free(dv);
}
