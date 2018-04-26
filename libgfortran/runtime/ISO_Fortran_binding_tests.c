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
  size_t base_type, base_type_size;

  /* Test function establish. */
  printf ("Test CFI_establish when dv.base_addr == NULL.\n\n");
  type     = CFI_type_float128;
  elem_len = 0;
  /* Loop through attribute. */
  for (int i = 1; i <= 3; i++)
    {
      attribute = i;
      /* Loop through rank. */
      for (int j = 0; j < CFI_MAX_RANK + 1; j++)
        {
          rank = j;
          CFI_CDESC_T (rank) est_test;
          /* We do this because C sometimes doesn't make the structures with a
           * null base_addr which leads to weird behaviour inside CFI_establish.
           */
          if (est_test.base_addr != NULL)
            {
              est_test.base_addr = NULL;
              free (est_test.base_addr);
            }
          ind = CFI_establish ((CFI_cdesc_t *)&est_test, NULL, attribute, type,
                               elem_len, rank, NULL);
          printf ("attribute = %d\ntype = %d\nrank = %d\n", attribute, type,
                  rank);
          if (ind != 0)
            {
              printf ("CFI_establish return value = %d\n\n", ind);
              goto next_attribute;
            }
          if (attribute != est_test.attribute)
            {
              printf ("Attribute fail.\n");
            }
          if (type != est_test.type)
            {
              printf ("Type fail.\n");
            }
          if (rank != est_test.rank)
            {
              printf ("Rank fail.\n");
            }
          if (elem_len != est_test.elem_len)
            {
              printf ("Element length fail.\n");
            }
          printf ("Test passed.\n\n");
        }
    next_attribute:;
    }

    base_type = type & CFI_type_mask;
    base_type_size = (type - base_type) >> CFI_type_kind_shift;
    printf("Base type = %ld\n", base_type);
    printf("Base type size (bytes) = %ld\n", base_type_size);
}
