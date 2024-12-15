
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "merchandise.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// Create a merchandise and destroy it, withoyut memory leaks
void test_merch_create_destroy()
{
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    CU_ASSERT_STRING_EQUAL(merch->name, "Chair");
    CU_ASSERT_STRING_EQUAL(merch->description, "A chair");
    CU_ASSERT_EQUAL(merch->price, 100);
    CU_ASSERT_PTR_NOT_NULL(merch->location);
    merch_destroy(merch);
}


// Look up if a merchandise exists in the hash table
void test_merch_lookup()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    char* name = strdup("Chair");
    char* desc = strdup("A chair");
    CU_ASSERT_FALSE(merch_lookup(merchandise, name));
    merch_t* merch = merch_create(name, desc, 100);
    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));
    CU_ASSERT_TRUE(merch_lookup(merchandise, "Chair"));
    merchandise_free_all(merchandise);
}


// Get merchandise from the hash table and see if it's correct
void test_merch_get()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));

    merch_t* get_merch = merch_get(merchandise, "Chair");
    CU_ASSERT_STRING_EQUAL(get_merch->name, "Chair");
    CU_ASSERT_STRING_EQUAL(get_merch->description, "A chair");
    CU_ASSERT_EQUAL(get_merch->price, 100);

    merchandise_free_all(merchandise);
}


// See if a newly created merch gets freed properly without memory leaks
void test_merch_destroy()
{
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    merch_destroy(merch);
}


// Check if a merchandise is in stock
void test_merch_in_stock()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);

    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    shelf_t* shelf = shelf_create(strdup("A01"), 10);

    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));
    CU_ASSERT_FALSE(merch_in_stock(merchandise, "Chair"));  // Merch exists, but is not in stock

    shelf_append_to_merch(merch, shelf);
    CU_ASSERT_TRUE(merch_in_stock(merchandise, "Chair"));  // Now in stock

    merch_decrease_stock(merchandise, storage, "Chair", 10);  // Decrease until removal
    CU_ASSERT_FALSE(merch_in_stock(merchandise, "Chair"));

    merchandise_free_all(merchandise);
    ioopm_hash_table_destroy(storage);
}


// Check if the counting of a merchandise is correct
void test_merch_total_quantity()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    shelf_t* shelf1 = shelf_create(strdup("A01"), 10);
    shelf_t* shelf2 = shelf_create(strdup("A02"), 20);

    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));  // Chair = 0
    shelf_append_to_merch(merch, shelf1);  // Chair = 10
    shelf_append_to_merch(merch, shelf2);  // Chair = 30

    CU_ASSERT_EQUAL(merch_total_quantity(merchandise, "Chair"), 30);
    merchandise_free_all(merchandise);
}


// Check if the stock of a merchandise decreases correctly
void test_merch_decrease_stock()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);

    char* name = strdup("Chair");
    char* desc = strdup("A chair");
    char* shelf_id1 = strdup("A01");
    char* shelf_id2 = strdup("A02");
    char* shelf_id3 = strdup("A03");
    merch_t* merch = merch_create(name, desc, 100);
    shelf_t* shelf1 = shelf_create(shelf_id1, 50);
    shelf_t* shelf2 = shelf_create(shelf_id2, 20);
    shelf_t* shelf3 = shelf_create(shelf_id3, 10);
    
    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));
    shelf_append_to_merch(merch, shelf1);
    shelf_append_to_merch(merch, shelf2);
    shelf_append_to_merch(merch, shelf3);  // [50, 20, 10]
    
    ioopm_hash_table_insert(storage, str_elem(strdup(shelf_id1)), str_elem(strdup(name)));
    ioopm_hash_table_insert(storage, str_elem(strdup(shelf_id2)), str_elem(strdup(name)));
    ioopm_hash_table_insert(storage, str_elem(strdup(shelf_id3)), str_elem(strdup(name)));
    

    merch_decrease_stock(merchandise, storage, "Chair", 11);  // [50, 19, NULL]
    CU_ASSERT_EQUAL(merch_total_quantity(merchandise, "Chair"), 69);

    merch_decrease_stock(merchandise, storage, "Chair", 19);  // [50, NULL, NULL]
    CU_ASSERT_EQUAL(merch_total_quantity(merchandise, "Chair"), 50);

    merch_decrease_stock(merchandise, storage, "Chair", 1000);  // [NULL, NULL, NULL]
    CU_ASSERT_FALSE(merch_in_stock(merchandise, "Chair"));

    merchandise_free_all(merchandise);
    shelf_free_all(storage);
}


// Create a merchandise, put it on two selfs and check if locations list is of lengt 2.
// Decreasing stock by 10 should remove the first shelf and set the length to 1.
void test_merch_locations()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    shelf_t* shelf1 = shelf_create(strdup("A01"), 20);
    shelf_t* shelf2 = shelf_create(strdup("A02"), 10);

    shelf_append_to_merch(merch, shelf1);
    shelf_append_to_merch(merch, shelf2);
    ioopm_hash_table_insert(merchandise, str_elem(merch->name), void_elem(merch));

    ioopm_list_t* locations = merch_locations(merchandise, "Chair");
    CU_ASSERT_EQUAL(ioopm_linked_list_size(locations), 2);
    merch_decrease_stock(merchandise, storage, "Chair", 10);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(locations), 1);

    merchandise_free_all(merchandise);
    ioopm_hash_table_destroy(storage);
}


// Create a copy of a merch and check if it's correct, and that the copy is not affected after deletion of the original
void test_merch_copy()
{
    merch_t* merch = merch_create(strdup("Chair"), strdup("A chair"), 100);
    shelf_append_to_merch(merch, shelf_create(strdup("A01"), 10));
    shelf_append_to_merch(merch, shelf_create(strdup("A02"), 20));
    merch_t* copy = merch_copy(merch);

    // Check if copied correctly
    CU_ASSERT_STRING_EQUAL(copy->name, "Chair");
    CU_ASSERT_STRING_EQUAL(copy->description, "A chair");
    CU_ASSERT_EQUAL(copy->price, 100);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(merch->location), 2);

    // Check if copy is intact after original is removed
    merch_destroy(merch);
    CU_ASSERT_STRING_EQUAL(copy->name, "Chair");
    CU_ASSERT_STRING_EQUAL(copy->description, "A chair");
    CU_ASSERT_EQUAL(copy->price, 100);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(copy->location), 2);
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(copy->location));

    merch_destroy(copy);
}


// Create merchansie, put them on shelves, then store in a hash table. Free the table to see of an memory leaks occur
// Use with valgrind
void test_merch_free_all()
{
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);

    merch_t* chair = merch_create(strdup("Chair"), strdup("A chair"), 100);
    merch_t* lamp = merch_create(strdup("Lamp"), strdup("A lamp"), 200);
    merch_t* table = merch_create(strdup("Table"), strdup("A table"), 300);

    shelf_append_to_merch(chair, shelf_create(strdup("A11"), 10));
    shelf_append_to_merch(chair, shelf_create(strdup("B22"), 10));
    shelf_append_to_merch(lamp, shelf_create(strdup("C33"), 10));

    ioopm_hash_table_insert(merchandise, str_elem(chair->name), void_elem(chair));  // Chair   [A11: x10, B22: x10]
    ioopm_hash_table_insert(merchandise, str_elem(lamp->name), void_elem(lamp));    // Lamp    [C33: x10]
    ioopm_hash_table_insert(merchandise, str_elem(table->name), void_elem(table));  // Table   [Not in stock]
    
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
    (CU_add_test(my_test_suite, "Create and destroy merchandise", test_merch_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Look up merchandise", test_merch_lookup) == NULL) ||
    (CU_add_test(my_test_suite, "Get merchandise", test_merch_get) == NULL) ||
    (CU_add_test(my_test_suite, "Destroy merchandise", test_merch_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Check if merchandise is in stock", test_merch_in_stock) == NULL) ||
    (CU_add_test(my_test_suite, "Get total quantity of merchandise", test_merch_total_quantity) == NULL) ||
    (CU_add_test(my_test_suite, "Decrease stock of merchandise", test_merch_decrease_stock) == NULL) ||
    (CU_add_test(my_test_suite, "Get locations of merchandise", test_merch_locations) == NULL) ||
    (CU_add_test(my_test_suite, "Copy merchandise", test_merch_copy) == NULL) ||
    (CU_add_test(my_test_suite, "Free all merchandise", test_merch_free_all) == NULL) ||



    
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