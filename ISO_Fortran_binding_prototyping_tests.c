// This file is to be deleted at a future date.
// It serves only as a disposible testing platform for prototyping and testing miscellaneous pieces of the actual implementation ISO_Fortran_binding.c

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
//#include <string.h>
#include <stdlib.h>


typedef ptrdiff_t CFI_index_t;
typedef int8_t CFI_rank_t;
typedef int8_t CFI_attribute_t;
typedef int16_t CFI_type_t;


typedef struct CFI_dim_t
{
  CFI_index_t lower_bound;
  CFI_index_t extent;
  CFI_index_t sm;
}
CFI_dim_t;



typedef struct CFI_cdesc_t
{
  void *base_addr;
  size_t elem_len;
  int version;
  CFI_rank_t rank;
  CFI_attribute_t attribute;
  CFI_type_t type;
  size_t offset;
  CFI_dim_t dim[];
}
CFI_cdesc_t;

int main () {

  CFI_cdesc_t *dv;
  CFI_index_t subscripts[12];
  printf("%td", sizeof(subscripts)/sizeof(CFI_index_t));
  do {
    if (test == NULL){
      fprintf(stderr, "Error ISO_Fortran_binding.c: CFI_address: Unallocated C Descriptor.\n");
      exit(EXIT_FAILURE);
     }
  } while(1==1);
  return 0;

}
