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

/* This file is a work in progress, it is not meant to compile and test
* features, that is what ISO_Fortran_binding_prototyping_tests.c is for. I'm
* currently annotating the structures according to the standard just to have
* them in-file so I know what to do with them. */
#pragma GCC diagnostic ignored "-Wvla"
// #include "libgfortran.h"
#include "ISO_Fortran_binding.h"
#include <stdio.h>
#include <stdlib.h>

// typedef struct CFI_dim_t
// {
//   // Value of the lower bound of the dimension being described.
//   CFI_index_t lower_bound;
//   // Number of elments in the dimension being described.
//   // If object is an assumed size array the value at the last dimension is
//   -1.
//   CFI_index_t extent;
//   // The difference in bytes between the addresses of successive elements in
//   the dimension.
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
//   // Storage size in bytes of a single element in object if not scalar.
//   size_t elem_len;
//   // CFI_VERSION in ISO_Fortran.h file.
//   int version;
//   // Number of dimensions of Fortran object (scalar is 0).
//   CFI_rank_t rank;
//   // Value of the specifier for the memory characteristics of the object,
//   whether it is an allocatable, a pointer, or a nonallocatable nonpointer
//   object.
//   CFI_attribute_t attribute;
//   // Value of the specifier for the type of the object. Each C interoperable
//   has its own specifier.
//   CFI_type_t type;
//   // Not described in ISO/IEC 1539-1:2017
//   size_t offset;
//   // Number of elements in dim is the rank of the object.
//   // If the object is an array pointer or allocatable array, the value of
//   dim[].lower_bound is determined by argument association, allocation or
//   pointer association.
//   // If the object is a nonallocatable nonpointer, the value of
//   dim[].lower_bound = 0.
//   // The N dimensions are ordered such that:
//   // n = 0, 1, 2, ..., N - 1
//   // abs( dim[n=0].sm ) >= elem_len &&
//   // abs( dim[n+1].sm ) >= abs( dim[n].sm ) * dim[n].extent && ... &&
//   // abs( dim[N-1].sm ) >= abs( dim[N-2].sm ) * dim[N-2].extent
//   // In an assumed size array the extent of the last element is equal to -1,
//   dim[N-1].extent = -1
//   CFI_dim_t dim[];
// }
// CFI_cdesc_t;
/* Definitions */
#define type(x)                                                                \
  _Generic((x), \
int : "int", \
float : "float", \
double : "double", \
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

int CFI_establish (CFI_cdesc_t *dv, void *base_addr, CFI_attribute_t attribute,
                   CFI_type_t type, size_t elem_len, CFI_rank_t rank,
                   const CFI_index_t extents[])
{

  // C Descriptor should be allocated.
  if (dv == NULL)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_establish: NULL C Descriptor. "
               "(Error No. %d).\n",
               CFI_INVALID_DESCRIPTOR);
      return CFI_INVALID_DESCRIPTOR;
    }

  /*// C Descriptor must be big enough to hold an object of a specified rank.
  if (sizeof (dv) < sizeof (CFI_CDESC_T (rank)))
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_establish: C Descriptor is not "
               "big enough to hold an object of rank %d. (Error No. "
               "%d).\n",
               rank, CFI_INVALID_DESCRIPTOR);
      return CFI_INVALID_DESCRIPTOR;
    }*/

  // C Descriptor must not be an allocated allocatable.
  if (dv->attribute == CFI_attribute_allocatable && dv->base_addr != NULL)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_establish: If the C Descriptor "
               "represents an allocatable variable (dv->attribute == "
               "%d), its base address must be NULL "
               "(dv->base_addr == NULL). (Error No. %d).\n",
               CFI_attribute_allocatable, CFI_INVALID_DESCRIPTOR);
      return CFI_INVALID_DESCRIPTOR;
    }

  /* base_addr should be NULL or an appropriately aligned address for an object
   * of the specified type. If it is not NULL the types and elem_len must be
   * consitent with the type and type parameters of the Fortran data. In order
   * to find out if that's the case, we must invert the type definitions in
   * ISO_Fortran_binding.h. The answer tells us the size in bytes of each data
   * type element. */
  // base_addr is NULL

  // If C Descripor will be established as an unallocated allocatable,
  // attribute must be CFI_attribute_allocatable.
  if (attribute == CFI_attribute_pointer && base_addr == NULL)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_establish: If the base "
                       "address is NULL (base_addr == NULL) then the "
                       "attribute "
                       "must be for an allocatable (attribute == "
                       "%d) or other (attribute == "
                       "%d). (Error No. %d).\n",
               CFI_attribute_allocatable, CFI_attribute_other,
               CFI_INVALID_ATTRIBUTE);
      return CFI_INVALID_ATTRIBUTE;
    }

  if (type == CFI_type_struct || type == CFI_type_other)
    {
      dv->elem_len = elem_len;
    }
  else
    {
      dv->elem_len = (type - (type & CFI_type_mask)) >> CFI_type_kind_shift;
    }
  dv->version   = CFI_VERSION;
  dv->rank      = rank;
  dv->attribute = attribute;
  dv->type      = type;

  if (rank > 0 && base_addr != NULL)
    {
      if (extents == NULL)
        {
          fprintf (stderr, "ISO_Fortran_binding.c: CFI_establish: Extents "
                           "must not "
                           "be NULL, extents != NULL. (Error No. %d).\n",
                   CFI_INVALID_EXTENT);
          return CFI_INVALID_ATTRIBUTE;
        }
      for (int i = 0; i < rank; i++)
        {
          dv->dim[i].extent = extents[i];
        }
    }

  return CFI_SUCCESS;
}

int CFI_setpointer (CFI_cdesc_t *result, CFI_cdesc_t *source,
                    const CFI_index_t lower_bounds[])
{

  // If source is NULL, the result is a C Descriptor that describes a
  // disassociated pointer.
  if (source == NULL)
    {
      result->base_addr = NULL;
      result->version   = CFI_VERSION;
      result->attribute = CFI_attribute_pointer;
    }
  // If source is a disassociated pointer, the result is a C Descriptor that
  // describes a disassociated pointer but with the characteristics of source.
  else if (source->base_addr == NULL &&
           source->attribute == CFI_attribute_pointer)
    {
      result->base_addr = NULL;
      result->elem_len  = source->elem_len;
      result->version   = source->version;
      result->rank      = source->rank;
      result->attribute = source->attribute;
      result->type      = source->type;
      result->offset    = source->offset;
      for (int i = 0; i < source->rank; i++)
        {
          result->dim[i].lower_bound = source->dim[i].lower_bound;
          result->dim[i].extent      = source->dim[i].extent;
          result->dim[i].sm          = source->dim[i].sm;
        }
    }
  else
    {
      if (source->rank > 0 && lower_bounds != NULL)
        {
          result->base_addr = source->base_addr;
          result->elem_len  = source->elem_len;
          result->version   = source->version;
          result->rank      = source->rank;
          result->attribute = source->attribute;
          result->type      = source->type;
          result->offset    = source->offset;
          for (int i = 0; i < source->rank; i++)
            {
              result->dim[i].lower_bound = lower_bounds[i];
              result->dim[i].extent      = source->dim[i].extent;
              result->dim[i].sm          = source->dim[i].sm;
            }
        }
      else
        {
          result->base_addr = source->base_addr;
          result->elem_len  = source->elem_len;
          result->version   = source->version;
          result->rank      = source->rank;
          result->attribute = source->attribute;
          result->type      = source->type;
          result->offset    = source->offset;
          for (int i = 0; i < source->rank; i++)
            {
              result->dim[i].lower_bound = source->dim[i].lower_bound;
              result->dim[i].extent      = source->dim[i].extent;
              result->dim[i].sm          = source->dim[i].sm;
            }
        }
    }

  return CFI_SUCCESS;
}

void *CFI_address (const CFI_cdesc_t *dv, const CFI_index_t subscripts[])
{

  // C Descriptor should be allocated.
  if (dv == NULL)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_address: NULL C Descriptor. "
                       "(Error No. %d).\n",
               CFI_INVALID_DESCRIPTOR);
      exit (EXIT_FAILURE);
    }

  // Base address of C Descriptor should be allocated.
  if (dv->base_addr == NULL)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_address: NULL base address of "
               "C Descriptor. (Error No. %d).\n",
               CFI_ERROR_BASE_ADDR_NULL);
      exit (EXIT_FAILURE);
    }

  CFI_rank_t rank = dv->rank;

  // dv is a scalar.
  if (rank == 0)
    {
      // Base address is the C Descriptor base address.
      return dv->base_addr;
    }
  // dv is not a scalar.
  else
    {
      // Base address is the C address of the element of the object
      // specified by
      // subscripts.
      void *base_addr;
      // In order to properly account for Fortran's row order we need to
      // transpose the subscripts.
      CFI_index_t *tr_subscripts;
      CFI_dim_t *  tr_dim;
      tr_subscripts = malloc (rank * sizeof (CFI_index_t));
      tr_dim        = malloc (rank * sizeof (CFI_dim_t));
      for (int i = 0; i < rank; i++)
        {
          tr_subscripts[i] = subscripts[rank - i - 1];
          tr_dim[i]        = dv->dim[rank - i - 1];
        }
      // We assume column major order as that is how fortran stores
      // arrays.
      // We calculate the memory address of the specified element via the
      // canonical array dimension reduction map and multiplying by the
      // memory
      // stride.
      CFI_index_t index = tr_subscripts[0];
      // Make sure the first subscript is in-bounds.
      if (subscripts[0] > dv->dim[0].extent - dv->dim[0].lower_bound)
        {
          fprintf (
              stderr, "ISO_Fortran_binding.c: CFI_address: subscripts[%d] = "
                      "%d, is out of bounds. It must not be greater than "
                      "dv->dim[%d].extent - dv->dim[%d].lower_bound = %d - "
                      "%d "
                      "= %d. (Error No. %d).\n",
              0, subscripts[0], 0, 0, dv->dim[0].extent, dv->dim[0].lower_bound,
              dv->dim[0].extent - dv->dim[0].lower_bound,
              CFI_ERROR_OUT_OF_BOUNDS);
          exit (EXIT_FAILURE);
        }
      // Start calculating the memory offset.
      CFI_index_t tmp_index = 1;
      for (int i = 1; i < rank; i++)
        {
          // Make sure the subscripts are in-bounds.
          if (subscripts[i] > dv->dim[i].extent - dv->dim[i].lower_bound)
            {
              fprintf (stderr, "ISO_Fortran_binding.c: "
                               "CFI_address: subscripts[%d] = "
                               "%d, is out of bounds. It must "
                               "not be greater than "
                               "dv->dim[%d].extent - "
                               "dv->dim[%d].lower_bound = %d - "
                               "%d = %d. (Error No. %d).\n",
                       i, subscripts[i], i, i, dv->dim[i].extent,
                       dv->dim[i].lower_bound,
                       dv->dim[i].extent - dv->dim[i].lower_bound,
                       CFI_ERROR_OUT_OF_BOUNDS);
              exit (EXIT_FAILURE);
            }
          // Find memory location of the subscripted item by mapping
          // the
          // subscripts
          // to a 1D array while taking the memory stride into
          // account.
          tmp_index *=
              tr_subscripts[i] * tr_dim[i - 1].extent * tr_dim[i - 1].sm;
          index += tmp_index;
        }
      free (tr_subscripts);
      free (tr_dim);
      // There's no way in C to do general arithmetic on a void pointer so
      // we
      // cast to a char pointer, do the arithmetic and cast back to a void
      // pointer.
      base_addr = (char *)dv->base_addr + index;
      return base_addr;
    }
}

int CFI_is_contiguous (const CFI_cdesc_t *dv)
{
  if (dv->base_addr == NULL)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_is_contiguous: NULL base "
                       "address of C Descriptor. (Error No. %d).\n",
               CFI_ERROR_BASE_ADDR_NULL);
      exit (EXIT_FAILURE);
    }

  if (dv->rank == 0)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_is_contiguous: C Descriptor "
                       "must describe an array (rank > 0). (Error No. %d).\n",
               CFI_INVALID_RANK);
      exit (EXIT_FAILURE);
    }

  // There is no guarantee other arrays are contiguous.
  if (dv->attribute == CFI_attribute_pointer)
    {
      return 0;
    }
  // Allocatable, assume shape and assumed size arrays are always contiguous.
  else
    {
      return 1;
    }
}

int CFI_allocate (CFI_cdesc_t *dv, const CFI_index_t lower_bounds[],
                  const CFI_index_t upper_bounds[], size_t elem_len)
{

  // C Descriptor should be allocated.
  if (dv == NULL)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_allocate: NULL C Descriptor. "
               "(Error No. %d).\n",
               CFI_INVALID_DESCRIPTOR);
      return CFI_INVALID_DESCRIPTOR;
    }

  // Base address of C Descriptor should be NULL.
  if (dv->base_addr != NULL)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_allocate: Base address of C "
                       "Descriptor should be NULL. (Error No. %d).\n",
               CFI_ERROR_BASE_ADDR_NOT_NULL);
      return CFI_ERROR_BASE_ADDR_NOT_NULL;
    }

  // The C Descriptor must be for an allocatable or pointer object.
  if (dv->attribute != CFI_attribute_pointer ||
      dv->attribute != CFI_attribute_allocatable)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_allocate: The object of the C "
               "Descriptor must be a pointer or allocatable variable. "
               "(Error No. %d).\n",
               CFI_INVALID_ATTRIBUTE);
      return CFI_INVALID_ATTRIBUTE;
    }

  // If the type is a character, the descriptor's elemenent length is replaced
  // by the elem_len argument.
  if (dv->type == CFI_type_char || dv->type == CFI_type_ucs4_char ||
      dv->type != CFI_type_signed_char)
    {
      dv->elem_len = elem_len;
    }

  size_t arr_len = 1;

  // If rank is greater than 0, lower_bounds and upper_bounds are used.
  // They're
  // ignored otherwhise.
  if (dv->rank > 0)
    {
      if (lower_bounds == NULL || upper_bounds == NULL)
        {
          fprintf (stderr,
                   "ISO_Fortran_binding.c: CFI_allocate: If rank > 0, "
                   "rank "
                   "= %d, the upper and lower bounds arguments, "
                   "upper_bounds[] and lower_bounds[], must not be NULL. "
                   "(Error No. %d).\n",
                   dv->rank, CFI_INVALID_EXTENT);
          return CFI_INVALID_EXTENT;
        }
      for (int i = 0; dv->rank; i++)
        {
          if (dv->dim[i].extent != upper_bounds[i] - lower_bounds[i] + 1)
            {
              fprintf (
                  stderr, "ISO_Fortran_binding.c: CFI_allocate: The lower and "
                          "upper bounds must be consistent with the extent of "
                          "the dimension described, dv->dim[%d].extent = %d, "
                          "must be equal to upper_bounds[%d] - "
                          "lower_bounds[%d] + 1 = %d - %d + 1 = %d. "
                          "(Error No. %d).\n",
                  i, dv->dim[i].extent, i, i, upper_bounds[i], lower_bounds[i],
                  upper_bounds[i] - lower_bounds[i] + 1, CFI_INVALID_EXTENT);
              return CFI_INVALID_EXTENT;
            }
          dv->dim[i].lower_bound = lower_bounds[i];
          dv->dim[i].sm          = dv->elem_len;
          arr_len *= dv->dim[i].extent;
        }
    }

  dv->base_addr = malloc (arr_len * dv->elem_len);

  return CFI_SUCCESS;
}

int CFI_deallocate (CFI_cdesc_t *dv)
{

  // C Descriptor should be allocated.
  if (dv == NULL)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_deallocate: NULL C Descriptor. "
               "(Error No. %d).\n",
               CFI_INVALID_DESCRIPTOR);
      return CFI_INVALID_DESCRIPTOR;
    }

  // C Descriptor must be for an allocatable or pointer variable.
  if (dv->attribute == CFI_attribute_other)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_deallocate: C Descriptor must "
               "describe a pointer or allocatabale object. (Error No. "
               "%d).\n",
               CFI_INVALID_ATTRIBUTE);
      return CFI_INVALID_ATTRIBUTE;
    }

  free (dv->base_addr);
  return CFI_SUCCESS;
}

int CFI_section (CFI_cdesc_t *result, const CFI_cdesc_t *source,
                 const CFI_index_t lower_bounds[],
                 const CFI_index_t upper_bounds[], const CFI_index_t strides[])
{

  // Result must not be an allocatable array.
  if (result->attribute == CFI_attribute_allocatable)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_section: Result must describe "
               "a pointer, CFI_attribute_pointer, or other, "
               "CFI_attribute_other. (Error No. %d).\n",
               CFI_INVALID_ATTRIBUTE);
      return CFI_INVALID_ATTRIBUTE;
    }

  // Base address of source must not be NULL.
  if (source->base_addr == NULL)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_section: Base address of "
                       "source must be allocated. (Error No. %d).\n",
               CFI_ERROR_BASE_ADDR_NULL);
      return CFI_ERROR_BASE_ADDR_NULL;
    }

  // Source must be some form of array (nonallocatable nonpointer array,
  // allocated allocatable array or an associated pointer array).
  if (source->rank <= 0)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_section: Source must have rank "
               "greater than zero. (Error No. %d).\n",
               CFI_INVALID_RANK);
      return CFI_INVALID_RANK;
    }

  if (result->elem_len != source->elem_len)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_section: The element lengths "
               "of source, source->elem_len = %d, and result, "
               "result->elem_len = %d, must be the same. (Error No. "
               "%d).\n",
               source->elem_len, result->elem_len, CFI_INVALID_ELEM_LEN);
      return CFI_INVALID_ELEM_LEN;
    }

  if (result->type != source->type)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_section: The type of source, "
               "source->type = %d, and result, result->type = %d, must be "
               "the same. (Error No. %d).\n",
               source->type, result->type, CFI_INVALID_TYPE);
      return CFI_INVALID_TYPE;
    }

  int zero_count = 0;
  for (int i = 0; i < source->rank; i++)
    {
      zero_count++;
    }

  if (result->rank != source->rank - zero_count)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_section: Rank of result, "
                       "source->rank = %d, must be equal to the rank of source "
                       "minus the number of zeros in strides = %d - %d = %d. "
                       "(Error No. %d).\n",
               result->rank, source->rank, zero_count,
               source->rank - zero_count, CFI_INVALID_RANK);
      return CFI_INVALID_RANK;
    }

  CFI_index_t *lower;
  CFI_index_t *upper;
  CFI_index_t *stride;
  lower  = malloc (source->rank * sizeof (CFI_index_t));
  upper  = malloc (source->rank * sizeof (CFI_index_t));
  stride = malloc (source->rank * sizeof (CFI_index_t));

  // Lower bounds.
  if (lower_bounds == NULL)
    {
      for (int i = 0; i < source->rank; i++)
        {
          lower[i] = source->dim[i].lower_bound;
        }
    }
  else
    {
      for (int i = 0; i < source->rank; i++)
        {
          lower[i] = lower_bounds[i];
        }
    }

  // Upper bounds.
  if (upper_bounds == NULL)
    {
      if (source->dim[source->rank].extent == -1)
        {
          fprintf (stderr,
                   "ISO_Fortran_binding.c: CFI_section: Source must not "
                   "be "
                   "an assumed size array if upper_bounds is NULL. (Error "
                   "No. %d).\n",
                   CFI_INVALID_EXTENT);
          return CFI_INVALID_EXTENT;
        }
      for (int i = 0; i < source->rank; i++)
        {
          upper[i] = source->dim[i].lower_bound + source->dim[i].extent - 1;
        }
    }
  else
    {
      for (int i = 0; i < source->rank; i++)
        {
          upper[i] = upper_bounds[i];
        }
    }

  // Stride
  if (strides == NULL)
    {
      for (int i = 0; i < source->rank; i++)
        {
          stride[i] = 1;
        }
    }
  else
    {
      for (int i = 0; i < source->rank; i++)
        {
          stride[i] = strides[i];
          // If stride[i] = then lower[i] and upper[i] must be equal.
          if (stride[i] == 0 && lower[i] != upper[i])
            {
              fprintf (stderr, "ISO_Fortran_binding.c: "
                               "CFI_section: If strides[%d] = 0, "
                               "then the lower bounds, "
                               "lower_bounds[%d] = %d, and "
                               "upper_bounds[%d] = %d, must be "
                               "equal. (Error No. %d).\n",
                       i, i, lower_bounds[i], i, upper_bounds[i],
                       CFI_ERROR_OUT_OF_BOUNDS);
              return CFI_ERROR_OUT_OF_BOUNDS;
            }
        }
    }

  // Lower bounds.
  if (lower_bounds != NULL)
    {
      for (int i = 0; i < source->rank; i++)
        {
          if (stride[i] == 0 || (upper[i] - lower[i] + stride[i]) / stride[i])
            {
              if (lower[i] < source->dim[i].lower_bound ||
                  lower[i] >
                      source->dim[i].lower_bound + source->dim[i].extent - 1)
                {
                  fprintf (stderr, "ISO_Fortran_binding.c: "
                                   "CFI_section: If stride[%d] = "
                                   "0, or (upper[%d] - lower[%d] + "
                                   "stride[%d])/stride[%d] = (%d - "
                                   "%d + %d)/%d = %d. "
                                   "(Error No. %d).\nIf upper_bounds "
                                   "is not NULL, then "
                                   "upper[i] = upper_bounds[i] for "
                                   "all i, otherwhise "
                                   "upper[i] is the upper bound of "
                                   "the Fortran array. "
                                   "If lower_bounds is not NULL, "
                                   "then lower[i] = "
                                   "lower_bounds[i] for all i, "
                                   "otherwhise lower[i] is "
                                   "the lower bound of the Fortran "
                                   "array.\n",
                           i, i, i, i, i, upper[i], lower[i], stride[i],
                           stride[i],
                           (upper[i] - lower[i] + stride[i]) / stride[i],
                           CFI_ERROR_OUT_OF_BOUNDS);
                  return CFI_ERROR_OUT_OF_BOUNDS;
                }
            }
        }
    }

  // upper bounds.
  if (upper_bounds != NULL)
    {
      for (int i = 0; i < source->rank; i++)
        {
          if (stride[i] == 0 || (upper[i] - lower[i] + stride[i]) / stride[i])
            {
              if (lower[i] < source->dim[i].lower_bound ||
                  lower[i] >
                      source->dim[i].lower_bound + source->dim[i].extent - 1)
                {
                  fprintf (stderr, "ISO_Fortran_binding.c: CFI_section: If "
                                   "stride[%d] = 0, or (upper[%d] - lower[%d] "
                                   "+ stride[%d])/stride[%d] = (%d - %d + "
                                   "%d)/%d = %d. (Error No. %d).\nIf "
                                   "upper_bounds is not NULL, then upper[i] = "
                                   "upper_bounds[i] for all i, otherwhise "
                                   "upper[i] is the upper bound of "
                                   "the Fortran array. "
                                   "If lower_bounds is not NULL, "
                                   "then lower[i] = "
                                   "lower_bounds[i] for all i, "
                                   "otherwhise lower[i] is "
                                   "the lower bound of the Fortran "
                                   "array.\n",
                           i, i, i, i, i, upper[i], lower[i], stride[i],
                           stride[i],
                           (upper[i] - lower[i] + stride[i]) / stride[i],
                           CFI_ERROR_OUT_OF_BOUNDS);
                  return CFI_ERROR_OUT_OF_BOUNDS;
                }
            }
        }
    }

  // Update the result to describe the array section.
  result->base_addr = CFI_address (source, lower);
  for (int i = 0; i < result->rank; i++)
    {
      result->dim[i].lower_bound = lower[i];
      result->dim[i].extent      = upper[i] - lower[i] + 1;
      result->dim[i].sm          = stride[i] * result->elem_len;
    }
  free (lower);
  free (upper);
  free (stride);

  return CFI_SUCCESS;
}

int CFI_select_part (CFI_cdesc_t *result, const CFI_cdesc_t *source,
                     size_t displacement, size_t elem_len)
{

  if (result->attribute == CFI_attribute_allocatable)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_select_part: Result must "
                       "not describe an allocatabale object. (Error No. %d).\n",
               CFI_INVALID_ATTRIBUTE);
      return CFI_INVALID_ATTRIBUTE;
    }

  // Base address of source must not be NULL.
  if (source->base_addr == NULL)
    {
      fprintf (stderr,
               "ISO_Fortran_binding.c: CFI_select_part: Base address of "
               "source must be allocated. (Error No. %d).\n",
               CFI_ERROR_BASE_ADDR_NULL);
      return CFI_ERROR_BASE_ADDR_NULL;
    }

  /* Nonallocatable nonpointer must not be an assumed size array */
  if (source->dim[source->rank].extent == -1)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_select_part: Source must "
                       "not describe an assumed size array. (Error No. %d).\n",
               CFI_INVALID_DESCRIPTOR);
      return CFI_INVALID_DESCRIPTOR;
    }

  /* Check the element length */
  size_t base_type_size =
      (result->type - CFI_type_Character) >> CFI_type_kind_shift;
  if (base_type_size == 1 || base_type_size == 4)
    {
      if (elem_len != base_type_size)
        {
          fprintf (stderr, "ISO_Fortran_binding.c: "
                           "CFI_select_part: Element length, elem_len = %d, "
                           "must be equal to the size in bytes of a Fortran "
                           "character, base_type_size = %d. (Error "
                           "No. %d).\n",
                   elem_len, base_type_size, CFI_INVALID_ELEM_LEN);
          return CFI_INVALID_ELEM_LEN;
        }
      result->elem_len = elem_len;
      for (int i = 0; i < result->rank; i++)
        {
          result->dim[i].sm = result->elem_len;
        }
    }
  else
    {
      base_type_size = (result->type - CFI_type_Integer) >> CFI_type_kind_shift;
      if (base_type_size == 1 && elem_len != base_type_size)
        {
          fprintf (stderr, "ISO_Fortran_binding.c: "
                           "CFI_select_part: Element length, elem_len = %d, "
                           "must be equal to the size in bytes of a Fortran "
                           "character, base_type_size = %d. (Error "
                           "No. %d).\n",
                   elem_len, base_type_size, CFI_INVALID_ELEM_LEN);
          return CFI_INVALID_ELEM_LEN;
        }
      result->elem_len = elem_len;
      for (int i = 0; i < result->rank; i++)
        {
          result->dim[i].sm = result->elem_len;
        }
    }

  /* Check displacement */
  if (displacement < 0 || displacement > source->elem_len - 1)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_select_part: Displacement "
                       "must be within the bounds of source, 0 <= displacement "
                       "(=%d) <= source->elem_len - 1 (=%d). (Error No. %d).\n",
               displacement, source->elem_len - 1, CFI_ERROR_OUT_OF_BOUNDS);
      return CFI_ERROR_OUT_OF_BOUNDS;
    }

  if (displacement + result->elem_len > source->elem_len)
    {
      fprintf (stderr, "ISO_Fortran_binding.c: CFI_select_part: Displacement "
                       "plus the element length of the result must be less "
                       "than or equal to the element length of the source, "
                       "displacement + result->elem_len (=%d+%d=%d) <= "
                       "source->elem_len (=%d). This ensures consistency in "
                       "picking part of the source (Error No. %d).\n",
               displacement, source->elem_len, displacement + source->elem_len,
               source->elem_len, CFI_ERROR_OUT_OF_BOUNDS);
      return CFI_ERROR_OUT_OF_BOUNDS;
    }

  result->base_addr = (char *)source->base_addr + displacement;

  return CFI_SUCCESS;
}
/*
void main ()
{
  CFI_CDESC_T (2) * dv;
  CFI_index_t  subscripts[2];
  CFI_index_t  test[0];
  CFI_index_t *address;

  dv                             = malloc (sizeof (CFI_CDESC_T (2)));
  dv->base_addr                  = malloc (sizeof (CFI_index_t));
  *(CFI_index_t *) dv->base_addr = 1;
  printf ("%s\n", type (*dv->base_addr));
  printf ("------------\n");
  printf ("%ld\n", dv->base_addr);
  dv->rank               = 2;
  dv->dim[0].lower_bound = 1;
  dv->dim[0].extent      = 3;
  dv->dim[0].sm          = 1;
  dv->dim[1].lower_bound = 1;
  dv->dim[1].extent      = 2;
  dv->dim[1].sm          = 1;
  printf ("------------\n");
  for (int i = 0; i < dv->dim[0].extent; i++)
    {
      for (int j = 0; j < dv->dim[1].extent; j++)
        {
          subscripts[0] = i;
          subscripts[1] = j;
          address       = (CFI_index_t *) CFI_address (dv, subscripts);
          printf ("A[%d, %d] = %d\n", i + 1, j + 1, (char *) address);
        }
    }
  // subscripts[0] = 0;
  // subscripts[1] = 0;
  // address = (CFI_index_t*) CFI_address(dv, subscripts);
  // printf("%d\n", (char*)address);
  // printf("%ld\n", *address);
  // printf("%d, %d, %d\n", sizeof(subscripts), sizeof(CFI_index_t),
  // sizeof(test));
  printf("type size in bytes = %d\n", (CFI_type_int128_t - (CFI_type_int128_t &
CFI_type_mask)) >> CFI_type_kind_shift);
  printf("base type = %d\n", CFI_type_int128_t & CFI_type_mask);

  free (dv->base_addr);
  free (dv);
}
*/
