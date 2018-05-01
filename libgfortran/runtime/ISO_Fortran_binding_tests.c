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
  /* Fresh descriptor, base address is NULL. */
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

  /* Fresh descriptor, base address is not NULL */
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
              extents = malloc (rank * sizeof (CFI_index_t));
              for (int r = 0; r < rank; r++)
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
              for (int r = 0; r < rank; r++)
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
                  for (int r = 0; r < rank; r++)
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

  /* Fresh descriptor, base address is not NULL */
  printf ("Test CFI_allocate: dv.base_addr != NULL.\n\n");
  elem_len           = 0;
  CFI_index_t *lower = NULL;
  CFI_index_t *upper = NULL;
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
              if (lower != NULL)
                {
                  free (lower);
                }
              if (upper != NULL)
                {
                  free (upper);
                }
              extents = malloc (rank * sizeof (CFI_index_t));
              lower   = malloc (rank * sizeof (CFI_index_t));
              upper   = malloc (rank * sizeof (CFI_index_t));
              for (int r = 0; r < rank; r++)
                {
                  extents[r] = 2;
                  lower[r]   = r;
                  upper[r]   = lower[r] + extents[r];
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
              elem_len = base_type_size;
              if (base_type_size == 10)
                {
                  elem_len = 64;
                }
              if (base_type == CFI_type_Complex)
                {
                  elem_len *= 2;
                }
              CFI_CDESC_T (rank) test3;
              /* We do this because C sometimes doesn't make the structures with
               * a null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (test3.base_addr != NULL)
                {
                  test3.base_addr = NULL;
                  free (test3.base_addr);
                }
              if (attribute == CFI_attribute_other)
                {
                  goto next_attribute3;
                }
              ind = CFI_establish ((CFI_cdesc_t *)&test3, NULL, attribute,
                                   type[i], elem_len, rank, extents);
              ind = CFI_allocate ((CFI_cdesc_t *)&test3, lower, upper,
                                  base_type_size);
              printf ("type = %ld\nelem_len = %ld\n", base_type, test3.elem_len);
              for (int r = 0; r < rank; r++)
                {
                  if (lower[r] != test3.dim[r].lower_bound)
                    {
                      printf ("Dimension lower bound fail: lower[%d] = %ld, "
                              "test3.dim[%d].lower_bound = %ld\n",
                              r, lower[r], r, test3.dim[r].lower_bound);
                      errno *= 2;
                    }
                  if (upper[r] - test3.dim[r].lower_bound + 1 !=
                      test3.dim[r].extent)
                    {
                      printf ("Extent fail: upper[%d] - "
                              "test3.dim[%d].lower_bound + 1 = %ld, "
                              "test3.dim[%d].extent = %ld\n",
                              r, r, upper[r] - test3.dim[r].lower_bound + 1, r,
                              test3.dim[r].lower_bound);
                      errno *= 3;
                    }
                }
              if (elem_len != test3.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld\n",
                          i, elem_len);
                  errno *= 5;
                }
              printf ("errno = %d\n", errno);
            }
        next_attribute3:;
        }
    }

  // if (errno == 1)
  //   {
  //     return CFI_SUCCESS;
  //   }
  // else
  //   {
  //     return CFI_FAILURE;
  //   }
}
