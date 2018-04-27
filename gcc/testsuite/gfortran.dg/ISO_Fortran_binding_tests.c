/* Test suite for ISO_Fortran_binding.h */
#include "ISO_Fortran_binding.h"
#include <stdio.h>
#include <stdlib.h>

int main (void)
{

  CFI_rank_t      rank;
  CFI_attribute_t attribute;
  CFI_type_t      type[10] = {CFI_type_Bool,        CFI_type_short,
                         CFI_type_ucs4_char,   CFI_type_double,
                         CFI_type_float128,    CFI_type_float128_Complex,
                         CFI_type_long_double, CFI_type_long_double_Complex,
                         CFI_type_struct,      CFI_type_other};
  size_t elem_len;
  int    ind;
  size_t base_type;
  size_t base_type_size;
  size_t errno;

  /* Test function establish. */
  /* Fresh descriptor, testing input parameters. */
  printf ("Test CFI_establish: dv.base_addr == NULL.\n\n");
  elem_len = 0;
  /* Loop through type. */
  for (int i = 0; i < 10; i++)
    {
      /* Loop through attribute. */
      for (int j = 1; j <= 3; j++)
        {
          attribute = j;
          /* Loop through rank. */
          for (int k = 0; k <= CFI_MAX_RANK; k++)
            {
              errno = 1;
              rank  = k;
              if (type[i] != CFI_type_struct && type[i] != CFI_type_other)
                {
                  base_type      = type[i] & CFI_type_mask;
                  base_type_size = (type[i] - base_type) >> CFI_type_kind_shift;
                }
              else
                {
                  base_type      = type[i];
                  base_type_size = elem_len;
                }

              CFI_CDESC_T (rank) est_test_base_addr;
              /* We do this because C sometimes doesn't make the structures with
               * a
               * null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (est_test_base_addr.base_addr != NULL)
                {
                  est_test_base_addr.base_addr = NULL;
                  free (est_test_base_addr.base_addr);
                }
              ind = CFI_establish ((CFI_cdesc_t *)&est_test_base_addr, NULL,
                                   attribute, type[i], elem_len, rank, NULL);
              printf ("attribute = %d\ntype = %d\nbase_type = %ld\nrank = %d\n",
                      attribute, type[i], base_type, rank);
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
              if (type[i] != est_test_base_addr.type)
                {
                  printf ("Type fail.\n");
                  errno *= 5;
                }
              if (rank != est_test_base_addr.rank)
                {
                  printf ("Rank fail.\n");
                  errno *= 7;
                }

              elem_len = base_type_size;
              if (base_type_size == 10)
                {
                  elem_len = 64;
                }
              if (base_type == CFI_type_Complex)
                {
                  elem_len *= 2;
                }
              if (elem_len != est_test_base_addr.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld\n",
                          i, elem_len);
                  errno *= 11;
                }
              printf ("errno = %ld\n\n", errno);
            }
        next_attribute:;
        }
    }

    for (int i=0;i<0;i++){
      printf("yehoy");
    }

  // /* Test basic types */
  // attribute = CFI_attribute_pointer;
  // rank      = 0;
  // CFI_CDESC_T (rank) est_test_int;
  //
  // /* Integers */
  // size_t int_byte_arr[5] = {1, 2, 4, 8, 16};
  // void * base_addr       = NULL;
  // for (int i = 0; i < 5; i++)
  //   {
  //     if (base_addr != NULL)
  //       {
  //         free (base_addr);
  //       }
  //     type      = CFI_type_Integer + (int_byte_arr[i] <<
  //     CFI_type_kind_shift);
  //     elem_len  = int_byte_arr[i];
  //     base_addr = NULL;
  //     base_addr = malloc (type);
  //     ind = CFI_establish ((CFI_cdesc_t *)&est_test_int, &base_addr,
  //     attribute,
  //                          type, elem_len, rank, NULL);
  //     if (ind != CFI_SUCCESS)
  //       {
  //         base_type      = type & CFI_type_mask;
  //         base_type_size = (type - base_type) >> CFI_type_kind_shift;
  //         printf ("Base type = %ld\n", base_type);
  //         printf ("Base type size (bytes) = %ld\n", base_type_size);
  //         return -1;
  //       }
  //   }
  // base_type      = type & CFI_type_mask;
  // base_type_size = (type - base_type) >> CFI_type_kind_shift;
  // printf ("Base type = %ld\n", base_type);
  // printf ("Base type size (bytes) = %ld\n", base_type_size);
}
