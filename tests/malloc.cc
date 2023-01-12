#include "ft_malloc.h"
#include "gtest/gtest.h"

TEST(AllocDefault, SuccessiveAllocTiny) {
  int *ptr1 = ft_malloc(sizeof(int));
  int *ptr2 = ft_malloc(sizeof(int));
  struct s_alloc_info info;
  t_ptr_alloc ptr_info = alloc_info(ptr, &info);
}
