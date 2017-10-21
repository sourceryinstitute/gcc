/* Header for functions resolving DATA statements.
   Copyright (C) 2007-2017 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* Version number of this file (increment when the file is incompatible with a previous version)
 */
#define CFI_VERSION 0

/* Maximum supported array rank
 */
#define CFI_MAX_RANK 8

/* C address of an object described by a C descriptor.
 *
 *    dv          address of a C descriptor describing the object.
 *   subscripts  a null pointer or
 *               the address of an array of type CFI_index_t wherein subscripts is the address
 *               of an array of CFI_index_t with at least n elements, where n is the rank of the object.
 *               (subscripts[i] shall be within the bounds of dimension i specified by the
 *               C descriptor's dim member.)
 *   result      If the object is
 *                 - an array of rank n, the result is the C address of the element of the object that
 *                   the first n elements of the subscripts argument would specify if used as subscripts.
 *                 - scalar, the result is its C address.
 */
void *CFI_address(const CFI_cdesc_t *dv, const CFI_index_t subscripts[]);

/* Allocate memory for an object described by a C descriptor.
 */
int CFI_allocate(CFI_cdesc_t *dv, const CFI_index_t lower_bounds[],
                 const CFI_index_t upper_bounds[], size_t elem_len);

/* Deallocate memory for an object described by a C descriptor.
 */
int CFI_deallocate(CFI_cdesc_t *dv);

/* Establish a C descriptor.
 */
int CFI_establish(CFI_cdesc_t *dv, void *base_addr, CFI_attribute_t attribute,
                  CFI_type_t type, size_t elem_len, CFI_rank_t rank,
                  const CFI_index_t extents[]);

/* Update a C descriptor for an array section for which each element is an element of a given array.
 */
int CFI_section(CFI_cdesc_t *result, const CFI_cdesc_t *source,
                const CFI_index_t lower_bounds[], const CFI_index_t upper_bounds[],
                const CFI_index_t strides[]);

/* Update a C descriptor for an array section for which each element is a part of the corresponding element
 * of an array.
 */
int CFI_select_part(CFI_cdesc_t *result, const CFI_cdesc_t *source, size_t displacement, size_t elem_len);


/* Update a C descriptor for a Fortran pointer to be associated with the whole of a given object or to be
 * disassociated.
 */
int CFI_setpointer(CFI_cdesc_t *result, CFI_cdesc_t *source, const CFI_index_t lower_bounds[]);

/* standard signed integer type capable of representing the result of subtracting two pointers.
 */
typedef CFI_index_t int;

/* Lower bound, extent, and memory stride for one dimension of an array.
 */
typedef struct CFI_dim_t {
  CFI_index_t lower_bound; /* Lower bound for the dimension being described. */
  CFI_index_t extent;      /* Number of elements in the dimension being described, or −1 for the final dimension of an assumed-size array. */
  CFI_index_t sm;          /* Memory stride for a dimension (difference in bytes between successive elements' adresses in the dimension being described. */
};

/* Largest supported rank
 */
typedef CFI_rank_t int;

/* Supported type specifiers
 */
typedef CFI_type_t int;

/* Attribute codes
 */
typedef CFI_attribute_t int;

/* Flexible array member
 */
typedef struct CFI_cdesc_t {
  /* Fortran standard: the first three members must be base_addr, elem_len, and version in that order.
   */
  base_addr;
  elem_len;
  version;

    /* Fortran standard: any additional members must be positioned between "version" and "dim" */

  CFI_rank_t rank;           /* Number of dimensions of the Fortran object being described; if the object is scalar, the value is zero. */
  CFI_type_t type;           /* Specifier for the object's type. */
  CFI_attribute_t attribute; /* Attribute indicating whether object is allocatable, a data pointer, or nonallocatable nonpointer . */

  /* Fortran standard: dim must be the final member.
   */
  CFI_dim_t dim;
};
