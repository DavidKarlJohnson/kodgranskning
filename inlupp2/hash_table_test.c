
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// ------ Hash functions START ------
int hash_function_mod_INT(elem_t key)
{
  return key.i;
}

int hash_function_mod_INT_17(elem_t key)
{
  return key.i;
}

int hash_function_string(elem_t key)
{
  int counter = 0;
  int size = strlen(key.s);

  for(int i = 0; i < size; i++){
    counter = counter + key.s[i];
  }
  return counter % 17;
}
// ------ Hash functions END ------




// ------ EQ functions START ------
static bool eq_int(elem_t a, elem_t b) { return a.i == b.i; }

static bool eq_str(elem_t a, elem_t b) { return strcmp(a.s, b.s) == 0; }

// ------ EQ functions END ------



// ------ Predicate / apply functions START ------

static bool key_finder(elem_t key, elem_t value, void* extra) {
  elem_t *address_to_element = extra;
  elem_t element_to_find = *address_to_element;
  return key.i == element_to_find.i;
}

static bool value_finder(elem_t key, elem_t value, void* extra) {
  elem_t *address_to_element = extra;
  elem_t element_to_find = *address_to_element;
  return value.i == element_to_find.i;
}

static void replace_value_INT(elem_t key, elem_t* value, void* extra){
   elem_t *address_to_changer = extra;
  elem_t changer = *address_to_changer;

  value->i = changer.i;
  return;
}


// ------ Predicate / apply functions END ------


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
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_empty_not_empty()
{
  elem_t result1;
  elem_t result2;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1), &result1));
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  ioopm_hash_table_lookup(ht, int_elem(1), &result2);
  CU_ASSERT_EQUAL(result2.i, 100);
  ioopm_hash_table_destroy(ht);
}

void test_insert_overwrite_value()
{
  elem_t result1;
  elem_t result2;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  ioopm_hash_table_lookup(ht, int_elem(1), &result1);
  CU_ASSERT_EQUAL(result1.i, 100);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(200));
  ioopm_hash_table_lookup(ht, int_elem(1), &result2);
  CU_ASSERT_EQUAL(result2.i, 200);
  CU_ASSERT_NOT_EQUAL(result1.i, result2.i);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  ioopm_hash_table_destroy(ht);
}


void test_insert_bucket_every()
{
 elem_t result;
 ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);

 for(int i = 0; i < 17; i++) {
    ioopm_hash_table_insert(ht, int_elem(i), int_elem(i));
    ioopm_hash_table_lookup(ht, int_elem(i), &result);
    CU_ASSERT_EQUAL(result.i, i);
 }
 ioopm_hash_table_destroy(ht);
}


void test_insert_bucket_0()
{
  elem_t result1;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(0), int_elem(100));
  ioopm_hash_table_lookup(ht, int_elem(0), &result1);
  CU_ASSERT_EQUAL(100, result1.i);
  ioopm_hash_table_destroy(ht);
}

void test_lookup_negative_key()  
{
  elem_t result1;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(-1), &result1));
  ioopm_hash_table_destroy(ht);
}

void multiple_insert_test()
{
  elem_t result;
  elem_t result1;
  elem_t result2;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  ioopm_hash_table_insert(ht, int_elem(35), int_elem(300));
  ioopm_hash_table_insert(ht, int_elem(18), int_elem(200));
  ioopm_hash_table_lookup(ht, int_elem(1), &result);
  ioopm_hash_table_lookup(ht, int_elem(35), &result1);
  ioopm_hash_table_lookup(ht, int_elem(18), &result2);
  CU_ASSERT_EQUAL(result.i, 100);
  CU_ASSERT_EQUAL(result1.i, 300);
  CU_ASSERT_EQUAL(result2.i, 200);
  ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
  elem_t result1;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  for (int i = 0; i < 18; i++)
  {
      CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i), &result1));
  }
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(-1), &result1));
  ioopm_hash_table_destroy(ht);
} 

void remove_entry()
{
  elem_t result;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, int_elem(1), &result));
  CU_ASSERT_EQUAL(100, result.i);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1), &result));
  ioopm_hash_table_destroy(ht);
}

void remove_non_existing_entry()
{
  elem_t result;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  bool control = ioopm_hash_table_remove(ht, int_elem(1), &result);
  CU_ASSERT_FALSE(control);
  ioopm_hash_table_destroy(ht);
}

void correct_next_entry_after_remove()
{
  elem_t result;
  elem_t control_result2;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT_17, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(0), int_elem(100));
  ioopm_hash_table_insert(ht, int_elem(17), int_elem(200));
  ioopm_hash_table_insert(ht, int_elem(34), int_elem(300));
  ioopm_hash_table_remove(ht, int_elem(17), &result);
  elem_t comparison;
  entry_t *control = ht->buckets[0]->next;
  control = control->next;
  comparison = control->value;
  ioopm_hash_table_lookup(ht, int_elem(34), &control_result2);
  CU_ASSERT_EQUAL(comparison.i, control_result2.i);
  ioopm_hash_table_destroy(ht);
}

void removing_last_entry(){
  elem_t result;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(0), int_elem(100));
  ioopm_hash_table_insert(ht, int_elem(17), int_elem(200));
  ioopm_hash_table_remove(ht, int_elem(17), &result);
  entry_t *control = ht->buckets[0]->next;
  control = control->next;
  CU_ASSERT_PTR_NULL(control);
  ioopm_hash_table_destroy(ht);
}

void size_of_empty_hash() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  ioopm_hash_table_destroy(ht);
}

void size_of_single_entry_hash() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  ioopm_hash_table_destroy(ht);
}

void size_many_entries_hash() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(18), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(2), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(10), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(21), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(22), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(0), int_elem(404));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 7);
  ioopm_hash_table_destroy(ht);
}

void test_empty_table() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht))
  ioopm_hash_table_destroy(ht);
}

void test_non_empty_table() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(22), int_elem(404));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht))
  ioopm_hash_table_destroy(ht);
}

void clear_hashtable(){
  elem_t result;
  elem_t result1;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(404));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(1), &result));
  ioopm_hash_table_clear(ht);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1), &result1));
  ioopm_hash_table_destroy(ht);
}

void test_hashtable_keys(){
  int keys[5] = {3, 1, 42, 0, 99};
  bool found[5] = {false};
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(keys[3]), int_elem(401));
  ioopm_hash_table_insert(ht, int_elem(keys[1]), int_elem(402));
  ioopm_hash_table_insert(ht, int_elem(keys[2]), int_elem(403));
  ioopm_hash_table_insert(ht, int_elem(keys[0]), int_elem(404));
  ioopm_hash_table_insert(ht, int_elem(keys[4]), int_elem(405));

  ioopm_list_t *new_keys = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t* iter = ioopm_list_iterator(new_keys);
  
  elem_t current_index = ioopm_iterator_current(iter);
  
  for(int i = 0; i < 5; i = i + 1){
    for (int j = 0; j < 5; j = j + 1){
      if(current_index.i == keys[j]){
        found[j] = true;
        break;
        if (j == 5){
          CU_FAIL("Found a key that was never inserted!");
        }
      }
    }
    ioopm_iterator_next(iter, &current_index);
    }
      for (int i = 0; i < 5; i = i + 1){
    CU_ASSERT_TRUE(found[i]);
  }
  ioopm_linked_list_destroy(new_keys);
  ioopm_iterator_destroy(iter);
  ioopm_hash_table_destroy(ht);
  }

  void test_hashtable_keys_strings(){
  elem_t keys[5] = {str_elem("hej"), str_elem("hopp"), str_elem("kung"), str_elem("ojoj"), str_elem("a")};
  bool found[5] = {false};
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, keys[3], int_elem(401));
  ioopm_hash_table_insert(ht, keys[1], int_elem(402));
  ioopm_hash_table_insert(ht, keys[2], int_elem(403));
  ioopm_hash_table_insert(ht, keys[0], int_elem(404));
  ioopm_hash_table_insert(ht, keys[4], int_elem(405));

  ioopm_list_t *new_keys = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t* iter = ioopm_list_iterator(new_keys);
  
  elem_t current_index = ioopm_iterator_current(iter);
  
  for(int i = 0; i < 5; i = i + 1){
    for (int j = 0; j < 5; j = j + 1){
      if(strcmp(current_index.s , keys[j].s) == 0){
        found[j] = true;
        break;
        if (j == 5){
          CU_FAIL("Found a key that was never inserted!");
        }
      }
    }
    ioopm_iterator_next(iter, &current_index);
    }
      for (int i = 0; i < 5; i = i + 1){
    CU_ASSERT_TRUE(found[i]);
  }
  ioopm_linked_list_destroy(new_keys);
  ioopm_iterator_destroy(iter);
  ioopm_hash_table_destroy(ht);
  }


void test_hashtable_values(){
  int value[5] = {1, 2, 3, 4, 5};
  bool found[5] = {false};
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(value[0]));
  ioopm_hash_table_insert(ht, int_elem(2), int_elem(value[1]));
  ioopm_hash_table_insert(ht, int_elem(3), int_elem(value[2]));
  ioopm_hash_table_insert(ht, int_elem(4), int_elem(value[3]));
  ioopm_hash_table_insert(ht, int_elem(5), int_elem(value[4]));

  ioopm_list_t *alla_values = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t* iter = ioopm_list_iterator(alla_values);

  elem_t current_index = ioopm_iterator_current(iter);

  for (int i = 0; i < 5; i = i + 1){
    for (int j = 0; j < 5; j = j +1){
      if(current_index.i == value[j]){
        found[j] = true;
        break;
      }if(j == 5){
        CU_FAIL("Found a value that was never inserted!");
      }
    }
    ioopm_iterator_next(iter, &current_index);
  }
  for (int i = 0; i < 5; i = i + 1){
    CU_ASSERT_TRUE(found[i]);
  }
  ioopm_hash_table_destroy(ht);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(alla_values);
}



void test_hashtable_values_and_keys(){
  elem_t keys[5] = {int_elem(3), int_elem(10), int_elem(42), int_elem(0), int_elem(99)};
  int value[5] = {11, 22, 33, 44, 55};
  bool found[5] = {false};
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, keys[0], int_elem(value[0]));
  ioopm_hash_table_insert(ht, keys[1], int_elem(value[1]));
  ioopm_hash_table_insert(ht, keys[2], int_elem(value[2]));
  ioopm_hash_table_insert(ht, keys[3], int_elem(value[3]));
  ioopm_hash_table_insert(ht, keys[4], int_elem(value[4]));

  ioopm_list_t * alla_values = ioopm_hash_table_values(ht);
  ioopm_list_t *alla_keys = ioopm_hash_table_keys(ht);

  ioopm_list_iterator_t* iter_keys = ioopm_list_iterator(alla_keys);
  ioopm_list_iterator_t* iter_values = ioopm_list_iterator(alla_values);

  elem_t current_index_k = ioopm_iterator_current(iter_keys);
  elem_t current_index_v = ioopm_iterator_current(iter_values);


  for (int i = 0; i < 5; i = i + 1){
    for (int j = 0; j < 5; j = j +1){
      if(current_index_k.i == keys[j].i && current_index_v.i == value[j]){
        found[j] = true;
        break;
      }if(j==5){
        CU_FAIL("Found a value that was never inserted!");
      }
    }
    ioopm_iterator_next(iter_keys, &current_index_k);
    ioopm_iterator_next(iter_values, &current_index_v);

  }
  for (int i = 0; i < 5; i = i + 1){
    CU_ASSERT_TRUE(found[i]);
  }
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(alla_values);
  ioopm_linked_list_destroy(alla_keys);
  ioopm_iterator_destroy(iter_keys);
  ioopm_iterator_destroy(iter_values);
}

void hash_table_has_key() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(7), int_elem(999));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(7)));
  ioopm_hash_table_destroy(ht);
}

void hash_table_no_key() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(7)));
  ioopm_hash_table_destroy(ht);
}

void hash_table_has_value(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(7), int_elem(100));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, int_elem(100)));
  ioopm_hash_table_insert(ht, int_elem(14), int_elem(101));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, int_elem(101)));
  ioopm_hash_table_insert(ht, int_elem(80), int_elem(102));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, int_elem(102)));
  ioopm_hash_table_destroy(ht);
}

void hash_table_no_value(){
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, int_elem(1001)));
  ioopm_hash_table_destroy(ht);
}


void test_hash_table_any_find_key(){
  int find_me = 1;
  int dont_find_me = 100;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(7));
  ioopm_hash_table_insert(ht, int_elem(3), int_elem(1000000));
  ioopm_hash_table_insert(ht, int_elem(2), int_elem(0));
  CU_ASSERT_TRUE(ioopm_hash_table_any(ht, key_finder, &find_me));
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, key_finder, &dont_find_me));
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_any_find_value(){
  int find_me = 20;
  int dont_find_me = 21;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(2));
  ioopm_hash_table_insert(ht, int_elem(3), int_elem(1));
  ioopm_hash_table_insert(ht, int_elem(2), int_elem(20));
  CU_ASSERT_TRUE(ioopm_hash_table_any(ht, value_finder, &find_me));
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, value_finder, &dont_find_me));
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_find_value(){
  int find_me = 1001;
  int dont_find_me = 2001;
   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  ioopm_hash_table_insert(ht, int_elem(1), int_elem(1001));
  ioopm_hash_table_insert(ht, int_elem(3), int_elem(1001));
  ioopm_hash_table_insert(ht, int_elem(2), int_elem(1001));
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, value_finder, &find_me));
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_finder, &dont_find_me));
  ioopm_hash_table_destroy(ht);
}


void test_apply_all_value_change(){
  int test = 100;
  int test2 = 1;

   ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_mod_INT, eq_int, eq_int);
  for (int i = 0; i < 12; i = i +1){
    ioopm_hash_table_insert(ht, int_elem(i), int_elem(12332));
  }
  ioopm_hash_table_apply_to_all(ht, replace_value_INT, &test);
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, value_finder, &test));
  ioopm_hash_table_apply_to_all(ht, replace_value_INT, &test2);
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, value_finder, &test));
  ioopm_hash_table_destroy(ht);
}

void test_hash_string_key()
{
  elem_t result;
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_string, eq_str, eq_str);
  ioopm_hash_table_insert(ht, str_elem("Test"), int_elem(10));
  ioopm_hash_table_insert(ht, str_elem("test"), int_elem(11));
  ioopm_hash_table_insert(ht, str_elem("Toxts"), int_elem(22));
  ioopm_hash_table_insert(ht, str_elem("HEJHopp123"), int_elem(11));
  ioopm_hash_table_insert(ht, str_elem("789OJ"), int_elem(33));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, str_elem("Test"), &result));
  CU_ASSERT_EQUAL(10, result.i);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, str_elem("test"), &result));
  CU_ASSERT_EQUAL(11, result.i);
  ioopm_hash_table_destroy(ht);

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
    (CU_add_test(my_test_suite, "Basic tests of hash table create", test_create) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of inserting hashtable",test_insert_empty_not_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of overwriting insert hashtable",test_insert_overwrite_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of lookup on empty entry",test_lookup_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of lookup on negative key",test_lookup_negative_key) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of multiple inserts in same bucket",multiple_insert_test) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of insert in every bucket",test_insert_bucket_every) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of insert in bucket 0",test_insert_bucket_0) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of removing entry",remove_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of removing non existing entry",remove_non_existing_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of correct next pointer after removing entry",correct_next_entry_after_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of correct next pointer after removing last entry",removing_last_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of size of no entry hash",size_of_empty_hash) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of size of single entry hash",size_of_single_entry_hash) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of size of several entries hash",size_many_entries_hash) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of checking is empty on empty table",test_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of checking is empty on non empty table",test_non_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of clearing hashtable",clear_hashtable) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of finding all keys in hash",test_hashtable_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of finding all values in hash",test_hashtable_values) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of finding all keys and values in hash",test_hashtable_values_and_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of looking for a key in ht that exist",hash_table_has_key) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of looking for a key in ht that does not exist",hash_table_no_key) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of looking for a value in ht that exist",hash_table_has_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of looking for a value in ht that does not exist",hash_table_no_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of hash_any finding key",test_hash_table_any_find_key) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of hash_any finding valeu",test_hash_table_any_find_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of hash_all finding valeu",test_hash_table_any_find_value) == NULL) ||
    (CU_add_test(my_test_suite, "Basic test of applyy_all changing valeu",test_apply_all_value_change) == NULL) ||
    (CU_add_test(my_test_suite, "test_hash_string_key",test_hash_string_key) == NULL) ||
    (CU_add_test(my_test_suite, "test_hash_string_key",test_hashtable_keys_strings) == NULL) ||
    
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
