#pragma once
#include <stdbool.h>

#define int_elem(x) (elem_t) { .i=(x)}
#define unsigned_int_elem(x) (elem_t) { .u=(x)}
#define bool_elem(x) (elem_t) { .b=(x)}
#define float_elem(x) (elem_t) { .f=(x)}
#define void_elem(x) (elem_t) { .p=(x)}
#define str_elem(x) (elem_t) { .s=(x)}



typedef union elem elem_t;

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char *s;
};

/// Compares two elements and returns true if they are equal
typedef bool ioopm_eq_function(elem_t a, elem_t b);