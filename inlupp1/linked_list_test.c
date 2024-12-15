
#include <CUnit/Basic.h>
#include <string.h>
#include "linked_list.h"
#include "common.h"

static bool integer_finder(elem_t element, void* extra){
  elem_t *address_to_element = extra;
  elem_t element_to_find = *address_to_element;
  return element.i == element_to_find.i;
}

static bool bool_finder(elem_t element, void* extra){
  elem_t *address_to_element = extra;
  elem_t element_to_find = *address_to_element;
  return element.b == element_to_find.b;
}


static bool integer_finder2(elem_t elem1, elem_t elem2){
  return elem1.i == elem2.i;
}


static bool bool_compare(elem_t bool1, elem_t bool2)
{
  return bool1.b == bool2.b;
}


static bool modulu_finder(elem_t element, void* extra){
  elem_t *address_to_mod = extra;
  elem_t mod = *address_to_mod;
  return element.i % mod.i == 0;
}

static void integer_changer(elem_t *element, void* extra){
  elem_t *address_to_changer = extra;
  elem_t changer = *address_to_changer;
  element->i = changer.i;
  return;
}

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}


void test_create()
{
   ioopm_list_t *list = ioopm_linked_list_create();
   CU_ASSERT_PTR_NOT_NULL(list);
   ioopm_linked_list_destroy(list);
}


void test_insert_and_find() {
    elem_t useless;
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_linked_list_insert(list, 0, int_elem(100));  // [100]
    ioopm_linked_list_get(list, 0, &useless);
    CU_ASSERT_EQUAL(useless.i, 100);
    ioopm_linked_list_insert(list, 1, int_elem(200));  // [100, 200] 
    ioopm_linked_list_get(list, 1, &useless);
    CU_ASSERT_EQUAL(useless.i, 200);

    ioopm_linked_list_destroy(list);
}

void test_append_empty() {  
    elem_t useless;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_append(list, int_elem(100));  // [100]
    ioopm_linked_list_get(list, 0, &useless);
    CU_ASSERT_EQUAL(useless.i, 100)
    ioopm_linked_list_destroy(list);
}

void test_append_many() {
    elem_t useless1;
    elem_t useless2;
    elem_t useless3;
    
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_append(list, int_elem(1));  // [1]
    ioopm_linked_list_append(list, int_elem(2));  // [1, 2]
    ioopm_linked_list_append(list, int_elem(3));  // [1, 2, 3]
    ioopm_linked_list_get(list, 1, &useless1);
    ioopm_linked_list_get(list, 0, &useless2);
    ioopm_linked_list_get(list, 2, &useless3);
    CU_ASSERT_EQUAL(useless1.i, 2);
    CU_ASSERT_EQUAL(useless2.i, 1);
    CU_ASSERT_EQUAL(useless3.i, 3);
    ioopm_linked_list_destroy(list);
}

void test_append_non_empty() { 
    elem_t useless;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_insert(list, 0, int_elem(10));  // [10]
    ioopm_linked_list_insert(list, 1, int_elem(20));  // [10, 20]
    ioopm_linked_list_insert(list, 2, int_elem(30));  // [10, 20, 30]
    ioopm_linked_list_append(list, int_elem(100));  // [10, 20, 30, 100]
    ioopm_linked_list_get(list, 3, &useless); 
    CU_ASSERT_EQUAL(useless.i, 100);
    ioopm_linked_list_destroy(list);
}

void test_prepend_empty() { 
    ioopm_list_t *list = ioopm_linked_list_create(); 
    elem_t get_useless;

    ioopm_linked_list_prepend(list, int_elem(100));  // [100];
    ioopm_linked_list_get(list, 0, &get_useless);
    CU_ASSERT_EQUAL(get_useless.i, 100);
    ioopm_linked_list_destroy(list);
}

void test_prepend_many() {  
    ioopm_list_t *list = ioopm_linked_list_create(); 
    elem_t get_useless;

    ioopm_linked_list_prepend(list, int_elem(3));  // [3]
    ioopm_linked_list_prepend(list, int_elem(2));  // [2, 3]
    ioopm_linked_list_prepend(list, int_elem(1));  // [1, 2, 3]
    ioopm_linked_list_get(list, 1, &get_useless);
    CU_ASSERT_EQUAL(get_useless.i, 2);
    ioopm_linked_list_get(list, 0, &get_useless);
    CU_ASSERT_EQUAL(get_useless.i, 1);
    ioopm_linked_list_get(list, 2, &get_useless);
    CU_ASSERT_EQUAL(get_useless.i, 3);
    ioopm_linked_list_destroy(list);
}

void test_prepend_non_empty() { 
    ioopm_list_t *list = ioopm_linked_list_create(); 
    elem_t get_useless;

    ioopm_linked_list_insert(list, 0, int_elem(10));  // [10]
    ioopm_linked_list_insert(list, 1, int_elem(20));  // [10, 20]
    ioopm_linked_list_insert(list, 2, int_elem(30));  // [10, 20, 30]
    ioopm_linked_list_prepend(list, int_elem(100));  // [100, 10, 20, 30];
    ioopm_linked_list_get(list, 0, &get_useless);
    CU_ASSERT_EQUAL(get_useless.i, 100); 
    ioopm_linked_list_destroy(list);
}

void test_remove() { 
    elem_t useless;
    elem_t get_useless;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_insert(list, 0, int_elem(10));  // [10]
    ioopm_linked_list_insert(list, 1, int_elem(20));  // [10, 20]
    ioopm_linked_list_insert(list, 2, int_elem(30));  // [10, 20, 30]
    ioopm_linked_list_remove(list, 1, &useless);  // [10, 30]
    ioopm_linked_list_get(list, 1, &get_useless);
    CU_ASSERT_NOT_EQUAL(get_useless.i, 20);
    ioopm_linked_list_destroy(list);
}



void test_remove_on_empty() { 
    elem_t useless;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_remove(list, 0, &useless);
    ioopm_linked_list_destroy(list);
}

void test_linked_INT_list_contain() {
    ioopm_list_t *list = ioopm_linked_list_create(integer_finder2); 
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(20));
    ioopm_linked_list_insert(list, 2, int_elem(30));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(20)));
    ioopm_linked_list_destroy(list);
}


void test_linked_BOOL_list_contain() {
    ioopm_list_t *list = ioopm_linked_list_create(bool_compare); 
    ioopm_linked_list_insert(list, 0, bool_elem(false));
    ioopm_linked_list_insert(list, 1, bool_elem(false));
    ioopm_linked_list_insert(list, 2, bool_elem(true));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, bool_elem(true)));
    ioopm_linked_list_destroy(list);
}


void test_linked_list_not_contain() {
    ioopm_list_t *list = ioopm_linked_list_create(integer_finder2); 
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(20));
    ioopm_linked_list_insert(list, 2, int_elem(30));
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(20000)));
    ioopm_linked_list_destroy(list);
}

void test_linked_list_minus_plus_contain() { 
    ioopm_list_t *list = ioopm_linked_list_create(integer_finder2); 
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(20));
    ioopm_linked_list_insert(list, 2, int_elem(30));
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(-20)));
    ioopm_linked_list_destroy(list);
}

void test_empty_size() {
    ioopm_list_t *list = ioopm_linked_list_create(); 
    int result = ioopm_linked_list_size(list);
    CU_ASSERT_EQUAL(result, 0);
    ioopm_linked_list_destroy(list);
}

void test_not_empty_size() {
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(20));
    ioopm_linked_list_insert(list, 2, int_elem(30));
    int result = ioopm_linked_list_size(list);
    CU_ASSERT_EQUAL(result, 3);
    ioopm_linked_list_destroy(list);
}

void test_making_it_empty() { 
    elem_t useless;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_insert(list, 0, int_elem(10));  // [10]
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_remove(list, 0, &useless);  // []
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_check_empty_yes() {
    ioopm_list_t *list = ioopm_linked_list_create(); 
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_check_empty_no() {
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(20));
    ioopm_linked_list_insert(list, 2, int_elem(30));
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}


void test_linked_list_clear() {

    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator));

    ioopm_linked_list_insert(list, 0, int_elem(10));  // [10]
    ioopm_linked_list_insert(list, 1, int_elem(20));  // [10, 20]
    ioopm_linked_list_insert(list, 2, int_elem(30));  // [10, 20, 30]
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));

    CU_ASSERT_TRUE(ioopm_iterator_has_next(iterator));

    ioopm_linked_list_clear(list);
    ioopm_iterator_reset(iterator);

    CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator));

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_any() {
    int find_me = 9;
    int dont_find_me = 1000;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    for (int i = 0; i < 10; i++) {
        ioopm_linked_list_insert(list, i, int_elem(i));
    }
    CU_ASSERT_TRUE(ioopm_linked_list_any(list, integer_finder, &find_me));
    CU_ASSERT_FALSE(ioopm_linked_list_any(list, integer_finder, &dont_find_me));
    ioopm_linked_list_destroy(list);
}


void test_modulol_and_bool() {
    int mod = 7;
    bool sant = true;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    ioopm_linked_list_append(list, int_elem(7));
    ioopm_linked_list_append(list, int_elem(14));
    ioopm_linked_list_append(list, int_elem(21));
    ioopm_linked_list_append(list, bool_elem(true));
    CU_ASSERT_TRUE(ioopm_linked_list_any(list, modulu_finder, &mod));
    CU_ASSERT_TRUE(ioopm_linked_list_any(list, bool_finder, &sant));

    ioopm_linked_list_destroy(list);
}



void test_all()
{
    int mod = 7;
    int mod2 = 14;
    ioopm_list_t *list = ioopm_linked_list_create(); 
    for (int i = 0; i < 10; i++) {
        int value_to_insert = 7 * i;
        ioopm_linked_list_insert(list, i, int_elem(value_to_insert));
    }
    CU_ASSERT_TRUE(ioopm_linked_list_all(list, modulu_finder, &mod));
    CU_ASSERT_FALSE(ioopm_linked_list_all(list, modulu_finder, &mod2));
    ioopm_linked_list_destroy(list);
}

void test_apply_to_all()
{
    int changer = 100;

    ioopm_list_t *list = ioopm_linked_list_create(); 
    for (int i = 0; i < 10; i++){
        int value_to_insert = 7 * i;
        ioopm_linked_list_insert(list, i, int_elem(value_to_insert));
    }
    ioopm_linked_list_apply_to_all(list, integer_changer, &changer);
    CU_ASSERT_TRUE(ioopm_linked_list_all(list, integer_finder, &changer));
    ioopm_linked_list_destroy(list);
}


// ------ ITERATOR ------


void test_linked_list_iterator_create()
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
  CU_ASSERT_PTR_NOT_NULL(list);
  CU_ASSERT_PTR_NOT_NULL(iterator);
  
  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iterator);
}

void test_linked_list_iterator_has_next()
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator));

  ioopm_linked_list_prepend(list, int_elem(1));
  ioopm_linked_list_append(list, int_elem(2));
  
  CU_ASSERT_TRUE(ioopm_iterator_has_next(iterator));
  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iterator);
}

void test_linked_list_iterator_no_next()
{
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator));
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iterator);
}

void test_iter_next_value()
{
  elem_t result;
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
  ioopm_linked_list_prepend(list, int_elem(1));  // [1]
  ioopm_linked_list_append(list, int_elem(2));  // [1, 2]
  ioopm_iterator_next(iterator, &result);
  CU_ASSERT_FALSE(ioopm_iterator_next(iterator, &result));
  CU_ASSERT_FALSE(ioopm_iterator_next(iterator, &result));
  CU_ASSERT_FALSE(ioopm_iterator_next(iterator, &result));
  CU_ASSERT_EQUAL(ioopm_iterator_current(iterator).i, 2);
  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iterator);
}


void test_iter_current_value()
{
  elem_t result1;  
  ioopm_list_t *list = ioopm_linked_list_create();

  ioopm_linked_list_prepend(list, int_elem(1));
  ioopm_linked_list_append(list, int_elem(2));
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
  elem_t result = ioopm_iterator_current(iterator);
  bool test = ioopm_iterator_next(iterator, &result1);
  elem_t result2 = ioopm_iterator_current(iterator);

  CU_ASSERT_TRUE(test);
  CU_ASSERT_EQUAL(result.i, 1);
  CU_ASSERT_EQUAL(result1.i, 2);
  CU_ASSERT_EQUAL(result1.i, result2.i);
  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iterator);  
}

void test_iter_reset() {
    elem_t result1;
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

    CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator));

    ioopm_linked_list_prepend(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(2));
    
    ioopm_iterator_reset(iterator);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(iterator));
    elem_t result = ioopm_iterator_current(iterator);
    bool test = ioopm_iterator_next(iterator, &result1);
    elem_t result2 = ioopm_iterator_current(iterator);
    CU_ASSERT_TRUE(test);
    CU_ASSERT_EQUAL(result.i, 1);
    CU_ASSERT_EQUAL(result1.i, 2);
    CU_ASSERT_EQUAL(result1.i, result2.i);
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iterator); 
}

void test_iter_remove() {
  elem_t useless;
  elem_t result;

  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);
  CU_ASSERT_FALSE(ioopm_iterator_remove(iterator, &result));

  ioopm_linked_list_prepend(list, int_elem(5));
  ioopm_linked_list_prepend(list, int_elem(4));
  ioopm_linked_list_prepend(list, int_elem(3));
  ioopm_linked_list_prepend(list, int_elem(2));
  ioopm_linked_list_prepend(list, int_elem(1));

  ioopm_iterator_next(iterator, &useless);
  CU_ASSERT_TRUE(ioopm_iterator_remove(iterator, &result));

  CU_ASSERT_EQUAL(result.i, 2);
  elem_t result2 = ioopm_iterator_current(iterator);
  ioopm_iterator_next(iterator, &result);
  CU_ASSERT_EQUAL(result2.i, 3);
  CU_ASSERT_EQUAL(result.i, 4);

  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iterator); 
}

void test_iter_insert()
{
  elem_t useless;

  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(list);

  ioopm_iterator_insert(iterator, int_elem(1));

  elem_t result = ioopm_iterator_current(iterator);
  CU_ASSERT_EQUAL(result.i, 1);
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator));
  ioopm_iterator_remove(iterator, &useless);

  ioopm_linked_list_prepend(list, int_elem(3));
  ioopm_linked_list_prepend(list, int_elem(2));

  result = ioopm_iterator_current(iterator);
  CU_ASSERT_EQUAL(result.i, 2);

  ioopm_iterator_insert(iterator, int_elem(1));

  result = ioopm_iterator_current(iterator);
  CU_ASSERT_EQUAL(result.i, 1);

  ioopm_iterator_next(iterator, &result);
  CU_ASSERT_EQUAL(result.i, 2);

  ioopm_iterator_next(iterator, &result);
  CU_ASSERT_EQUAL(result.i, 3);

  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iterator); 
}


int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "Basic tests of list create", test_create) == NULL) ||
    (CU_add_test(my_test_suite, "Tests of inserting and getting list", test_insert_and_find) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of append on empty list", test_append_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of append on non empty list", test_append_non_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of prepend on empty list", test_prepend_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of prepend several", test_prepend_many) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of append several", test_append_many) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of prepend on non empty list", test_prepend_non_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of reomve link from list", test_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of reomve on empty list", test_remove_on_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of link list contains", test_linked_INT_list_contain) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of link list contains with string", test_linked_BOOL_list_contain) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of link list not contaning", test_linked_list_not_contain) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of link list contains on plus minus(to be false)", test_linked_list_minus_plus_contain) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of getting size, empty list", test_empty_size) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of getting size, non empty list", test_not_empty_size) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of making a non empty list empty", test_making_it_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of empty list", test_check_empty_yes) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of not empty", test_check_empty_no) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of clearing linked list", test_linked_list_clear) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of find any (value)", test_any) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of find all (modulu)", test_all) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of apply to all (value changer)", test_apply_to_all) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of creating iter", test_linked_list_iterator_create) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter has next", test_linked_list_iterator_has_next) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter no next", test_linked_list_iterator_no_next) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter next", test_iter_next_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter current", test_iter_current_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter reset", test_iter_reset) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter remove", test_iter_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Basic tests of iter insert", test_iter_insert) == NULL) ||
    (CU_add_test(my_test_suite, "Modulo test", test_modulol_and_bool) == NULL) ||

    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit..
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
