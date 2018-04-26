/* Test suite for ISO_Fortran_binding.h */
#include "ISO_Fortran_binding.h"
#include <stdio.h>
#include <stdlib.h>

int main (void)
{

  CFI_rank_t      rank;
  CFI_attribute_t attribute;
  CFI_type_t      type;
  size_t          elem_len;
  int             ind;
  size_t          base_type;
  size_t          base_type_size;
  size_t          errno = 1;

  /* Test function establish. */
  printf ("Test CFI_establish: dv.base_addr == NULL.\n\n");
  type     = CFI_type_other;
  elem_len = 0;
  /* Loop through attribute. */
  for (int i = 1; i <= 3; i++)
    {
      attribute = i;
      /* Loop through rank. */
      for (int j = 0; j < CFI_MAX_RANK + 1; j++)
        {
          rank = j;
          if (type != CFI_type_struct && type != CFI_type_other)
            {
              base_type      = type & CFI_type_mask;
              base_type_size = (type - base_type) >> CFI_type_kind_shift;
            }
          else
            {
              base_type_size = 0;
            }

          CFI_CDESC_T (rank) est_test_base_addr;
          /* We do this because C sometimes doesn't make the structures with a
           * null base_addr which leads to weird behaviour inside CFI_establish.
           */
          if (est_test_base_addr.base_addr != NULL)
            {
              est_test_base_addr.base_addr = NULL;
              free (est_test_base_addr.base_addr);
            }
          ind = CFI_establish ((CFI_cdesc_t *)&est_test_base_addr, NULL,
                               attribute, type, elem_len, rank, NULL);
          printf ("attribute = %d\ntype = %d\nrank = %d\n", attribute, type,
                  rank);
          if (ind != CFI_SUCCESS)
            {
              printf ("CFI_establish return value = %d\n\n", ind);
              errno *= 2;
              goto next_attribute;
            }
          if (attribute != est_test_base_addr.attribute)
            {
              printf ("Attribute fail.\n");
              errno *= 3;
            }
          if (type != est_test_base_addr.type)
            {
              printf ("Type fail.\n");
              errno *= 5;
            }
          if (rank != est_test_base_addr.rank)
            {
              printf ("Rank fail.\n");
              errno *= 7;
            }
          if ((type == CFI_type_struct &&
               elem_len != est_test_base_addr.elem_len) ||
              (type == CFI_type_other &&
               elem_len != est_test_base_addr.elem_len) ||
              (type != CFI_type_struct && type != CFI_type_other &&
               base_type_size != est_test_base_addr.elem_len))
            {
              printf ("Element length fail.\n");
              errno *= 11;
            }
          printf ("errno = %d\n\n", errno);
        }
    next_attribute:;
    }

  /* Test basic types */
  attribute = CFI_attribute_other;
  rank      = 0;
  CFI_CDESC_T (rank) est_test_int;

  /* Integers */
  size_t int_byte_arr[5] = {1, 2, 4, 8, 16};
  void * base_addr       = NULL;
  for (int i = 0; i < 5; i++)
    {
      if (base_addr != NULL)
        {
          free (base_addr);
        }
      type      = CFI_type_Integer + (int_byte_arr[i] << CFI_type_kind_shift);
      elem_len  = int_byte_arr[i];
      base_addr = NULL;
      base_addr = malloc (type);
      ind = CFI_establish ((CFI_cdesc_t *)&est_test_int, &base_addr, attribute,
                           type, elem_len, rank, NULL);
      if (ind != CFI_SUCCESS)
        {
          base_type      = type & CFI_type_mask;
          base_type_size = (type - base_type) >> CFI_type_kind_shift;
          printf ("Base type = %ld\n", base_type);
          printf ("Base type size (bytes) = %ld\n", base_type_size);
          return -1;
        }
    }
  base_type      = type & CFI_type_mask;
  base_type_size = (type - base_type) >> CFI_type_kind_shift;
  printf ("Base type = %ld\n", base_type);
  printf ("Base type size (bytes) = %ld\n", base_type_size);
}
