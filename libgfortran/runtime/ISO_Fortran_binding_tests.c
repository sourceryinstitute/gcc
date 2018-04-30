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

              CFI_CDESC_T (rank) test1;
              /* We do this because C sometimes doesn't make the structures with
               * a null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (test1.base_addr != NULL)
                {
                  test1.base_addr = NULL;
                  free (test1.base_addr);
                }
              ind = CFI_establish ((CFI_cdesc_t *)&test1, NULL, attribute,
                                   type[i], elem_len, rank, NULL);
              printf ("attribute = %d\ntype = %d\nbase_type = %ld\nrank = %d\n",
                      attribute, type[i], base_type, rank);
              if (ind != CFI_SUCCESS)
                {
                  printf ("CFI_establish return value = %d\n", ind);
                  errno *= 2;
                  printf ("errno = %ld\n\n", errno);
                  goto next_attribute1;
                }
              if (attribute != test1.attribute)
                {
                  printf ("Attribute fail.\n");
                  errno *= 3;
                }
              if (type[i] != test1.type)
                {
                  printf ("Type fail.\n");
                  errno *= 5;
                }
              if (rank != test1.rank)
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
              if (elem_len != test1.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld\n",
                          i, elem_len);
                  errno *= 11;
                }
              printf ("errno = %ld\n\n", errno);
            }
        next_attribute1:;
        }
    }

  printf ("Test CFI_establish: dv.base_addr != NULL.\n\n");
  CFI_index_t *extents = NULL;
  elem_len             = 0;
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
              if (extents != NULL)
                {
                  free (extents);
                }
              extents = malloc (k * sizeof (CFI_index_t));
              for (int r = 0; r < k; r++)
                {
                  extents[r] = r + 1;
                }
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

              CFI_CDESC_T (rank) test2;
              /* We do this because C sometimes doesn't make the structures with
               * a null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (test2.base_addr != NULL)
                {
                  test2.base_addr = NULL;
                  free (test2.base_addr);
                }
              ind = CFI_establish ((CFI_cdesc_t *)&test2, &ind, attribute,
                                   type[i], elem_len, rank, extents);
              printf ("attribute = %d\ntype = %d\nbase_type = %ld\nrank = %d\n",
                      attribute, type[i], base_type, rank);
              if (ind != CFI_SUCCESS)
                {
                  printf ("CFI_establish return value = %d\n", ind);
                  errno *= 2;
                  printf ("errno = %ld\n\n", errno);
                  goto next_attribute2;
                }
              if (attribute != test2.attribute)
                {
                  printf ("Attribute fail.\n");
                  errno *= 3;
                }
              if (type[i] != test2.type)
                {
                  printf ("Type fail.\n");
                  errno *= 5;
                }
              if (rank != test2.rank)
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
              if (elem_len != test2.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld\n",
                          i, elem_len);
                  errno *= 11;
                }

              printf ("extents = [ ");
              for (int r = 0; r < k; r++)
                {
                  if (extents[r] != test2.dim[r].extent)
                    {
                      printf ("Extents fail: extents[%d] = %ld., "
                              "dv.dim[%d].extent = %ld\n",
                              r, extents[r], r, test2.dim[r].extent);
                      errno *= 13;
                    }
                  printf ("%ld ", extents[r]);
                }
              printf ("]\n");

              if (attribute == CFI_attribute_pointer)
                {
                  printf ("test2.dim[].lower_bound = [ ");
                  for (int r = 0; r < k; r++)
                    {
                      if (test2.dim[r].lower_bound != 0)
                        {
                          printf (
                              "Dimension lower bound fail: if the attribute is "
                              "for a pointer, the lower bounds of every "
                              "dimension must be zero, "
                              "test2.dim[%d].lower_bound = %ld",
                              r, test2.dim[r].lower_bound);
                          errno *= 17;
                        }
                      printf ("%ld ", test2.dim[r].lower_bound);
                    }
                  printf ("]\n");
                }
              printf ("errno = %ld\n\n", errno);
            }
        next_attribute2:;
        }
    }
}
