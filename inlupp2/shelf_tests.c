
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "shelf.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// Create and destroy a shelf to check if it's freed properly
void test_shelf_create_destroy()
{
    shelf_t* shelf = shelf_create(strdup("A01"), 10);
    CU_ASSERT_STRING_EQUAL(shelf->shelf, "A01");
    CU_ASSERT_EQUAL(shelf->quantity, 10);
    shelf_destroy(shelf);
}


// See if 'shelf_get' returns the correct shelf
void test_shelf_get()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    shelf_append_to_merch(merch, shelf_create(strdup("A01"), 10));
    shelf_append_to_merch(merch, shelf_create(strdup("A02"), 20));
    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));
    ioopm_hash_table_insert(storage, str_elem("A01"), str_elem("Chair"));
    ioopm_hash_table_insert(storage, str_elem("A02"), str_elem("Chair"));

    shelf_t* get_shelf1 = shelf_get(merchandise, storage, "A01");
    CU_ASSERT_STRING_EQUAL(get_shelf1->shelf, "A01");
    CU_ASSERT_EQUAL(get_shelf1->quantity, 10);
    
    shelf_t* get_shelf2 = shelf_get(merchandise, storage, "A02");
    CU_ASSERT_STRING_EQUAL(get_shelf2->shelf, "A02");
    CU_ASSERT_EQUAL(get_shelf2->quantity, 20);
    CU_ASSERT_STRING_NOT_EQUAL(get_shelf2->shelf, "A01");
    CU_ASSERT_NOT_EQUAL(get_shelf2->quantity, 10);
    
    merchandise_free_all(merchandise);
    ioopm_hash_table_destroy(storage);
}


// Create a copy of a shelf, and ensure that the copy is not affected when deleting the original
void test_shelf_copy()
{
    shelf_t* shelf = shelf_create(strdup("A01"), 10);
    shelf_t* copy = shelf_copy(shelf);

    CU_ASSERT_STRING_EQUAL(copy->shelf, "A01");
    CU_ASSERT_EQUAL(copy->quantity, 10);

    shelf_destroy(shelf);

    CU_ASSERT_STRING_EQUAL(copy->shelf, "A01");
    CU_ASSERT_EQUAL(copy->quantity, 10);

    shelf_destroy(copy);
}


// Ensure that a hash table which takes ownership of its key and value strings within an 'entry_t' type is freed properly
// Use with valgrind
void test_shelf_free_all()
{
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    ioopm_hash_table_insert(storage, str_elem(strdup("A01")), str_elem(strdup("Banana")));
    ioopm_hash_table_insert(storage, str_elem(strdup("A02")), str_elem(strdup("Apple")));
    ioopm_hash_table_insert(storage, str_elem(strdup("B03")), str_elem(strdup("Orange")));
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(storage));
    shelf_free_all(storage);
}


// Ensure that a shelf is freed from a hash table which takes ownership of its key and value strings within an 'entry_t' type
void test_shelf_remove()
{
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    ioopm_hash_table_insert(storage, str_elem(strdup("A01")), str_elem(strdup("Banana")));
    ioopm_hash_table_insert(storage, str_elem(strdup("B02")), str_elem(strdup("Apple")));
    ioopm_hash_table_insert(storage, str_elem(strdup("C03")), str_elem(strdup("Orange")));  // [A01, B02, C03]
    elem_t unused;

    shelf_remove(storage, str_elem("A01"), &unused);  // [NULL, B02, C03]
    CU_ASSERT_FALSE(shelf_lookup(storage, "A01"));
    CU_ASSERT_TRUE(shelf_lookup(storage, "B02"));
    CU_ASSERT_TRUE(shelf_lookup(storage, "C03"));

    shelf_remove(storage, str_elem("C03"), &unused);  // [NULL, B02, NULL]
    CU_ASSERT_FALSE(shelf_lookup(storage, "A01"));
    CU_ASSERT_TRUE(shelf_lookup(storage, "B02"));
    CU_ASSERT_FALSE(shelf_lookup(storage, "C03"));

    shelf_remove(storage, str_elem("B02"), &unused);  // [NULL, NULL, NULL]
    CU_ASSERT_FALSE(shelf_lookup(storage, "A01"));
    CU_ASSERT_FALSE(shelf_lookup(storage, "B02"));
    CU_ASSERT_FALSE(shelf_lookup(storage, "C03"));

    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(storage));
    shelf_free_all(storage);
}


// Create a merch, a shelf and append the shelf struct to the merches' location list, check to see if list size is 1
void test_shelf_append_to_merch()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    shelf_t* shelf = shelf_create(strdup("A01"), 10);
    shelf_append_to_merch(merch, shelf);
    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));
    ioopm_list_t* locations = merch_locations(merchandise, "Chair");
    CU_ASSERT_EQUAL(ioopm_linked_list_size(locations), 1);
    merchandise_free_all(merchandise);
}



int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}


int main() {
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
  }

  if (
    (CU_add_test(my_test_suite, "Create and destroy shelf", test_shelf_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Get shelf", test_shelf_get) == NULL) ||
    (CU_add_test(my_test_suite, "Copy shelf", test_shelf_copy) == NULL) ||
    (CU_add_test(my_test_suite, "Free all shelves", test_shelf_free_all) == NULL) ||
    (CU_add_test(my_test_suite, "Remove shelf", test_shelf_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Append shelf to merch", test_shelf_append_to_merch) == NULL) ||
 



    
    0
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_NORMAL);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}