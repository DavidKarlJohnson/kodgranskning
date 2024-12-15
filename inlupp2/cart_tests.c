
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "cart.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// Create a cart item, and free it properly
void test_create_destroy_item()
{
  cart_item_t *item = cart_create_item(strdup("item"), 1);
  CU_ASSERT_STRING_EQUAL(item->name, "item");
  CU_ASSERT_EQUAL(item->amount, 1);
  cart_destroy_item(item);
}


// Create and destroy an empty cart
void test_create_destroy_empty_cart()
{
  cart_t *cart = cart_new(1);
  CU_ASSERT_EQUAL(cart->id, 1);
  CU_ASSERT_PTR_NOT_NULL(cart->items);
  cart_destroy(cart);
}


// Create and destroy a non-empty cart
void test_create_destroy_non_empty_cart()
{
  cart_t *cart = cart_new(1);
  CU_ASSERT_EQUAL(cart->id, 1);
  CU_ASSERT_PTR_NOT_NULL(cart->items);
  cart_increase_merch(cart, strdup("Chair"), 1);
  cart_increase_merch(cart, strdup("Lamp"), 100);
  cart_increase_merch(cart, strdup("Table"), 50);
  cart_destroy(cart);
}


// Create a HT with several carts containing items. Then destroy the HT and ensure all items are freed
void test_create_destroy_many_non_empty_carts()
{
  ioopm_hash_table_t* carts = ioopm_hash_table_create(hash_func_int, eq_int, eq_str);
  cart_t* cart1 = cart_new(1);
  cart_t* cart2 = cart_new(2);
  cart_t* cart3 = cart_new(3);

  cart_increase_merch(cart1, strdup("Chair"), 1);
  cart_increase_merch(cart1, strdup("Lamp"), 100);
  cart_increase_merch(cart1, strdup("Table"), 50);
  cart_increase_merch(cart2, strdup("Egg"), 60);
  cart_increase_merch(cart2, strdup("Tomato"), 2);
  cart_increase_merch(cart3, strdup("Cucumber"), 100);

  ioopm_hash_table_insert(carts, int_elem(0), void_elem(cart1));
  ioopm_hash_table_insert(carts, int_elem(1), void_elem(cart2));
  ioopm_hash_table_insert(carts, int_elem(2), void_elem(cart3));
  cart_free_all(carts);
}


// Get a specific cart from a hash table
void test_cart_get()
{
  ioopm_hash_table_t* carts = ioopm_hash_table_create(hash_func_int, eq_int, eq_str);

  ioopm_hash_table_insert(carts, int_elem(1), void_elem(cart_new(1)));
  ioopm_hash_table_insert(carts, int_elem(2), void_elem(cart_new(2)));
  ioopm_hash_table_insert(carts, int_elem(3), void_elem(cart_new(3)));

  cart_t* get_cart = cart_get(carts, 2);
  CU_ASSERT_EQUAL(get_cart->id, 2);
  cart_free_all(carts);
}


// Lookup if a specific cart ID exists within a hash table
void test_cart_ID_lookup()
{
  ioopm_hash_table_t* carts = ioopm_hash_table_create(hash_func_int, eq_int, eq_str);
  ioopm_hash_table_insert(carts, int_elem(1), void_elem(cart_new(1)));
  ioopm_hash_table_insert(carts, int_elem(2), void_elem(cart_new(2)));
  ioopm_hash_table_insert(carts, int_elem(3), void_elem(cart_new(3)));
  
  CU_ASSERT_TRUE(cart_ID_lookup(carts, 2));
  CU_ASSERT_FALSE(cart_ID_lookup(carts, 8));
  cart_free_all(carts);
}


// See if 'cart_get_item_amount' returns the correct amount of a specific merchandise
void test_cart_get_amount()
{
  cart_t* cart = cart_new(1);
  char* merch1 = strdup("Chair");
  char* merch2 = strdup("Lamp");
  char* merch3 = strdup("Table");

  cart_item_t* item1 = cart_create_item(merch1, 1);
  cart_item_t* item2 = cart_create_item(merch2, 100);
  cart_item_t* item3 = cart_create_item(merch3, 50);
  ioopm_linked_list_append(cart->items, void_elem(item1));
  ioopm_linked_list_append(cart->items, void_elem(item2));
  ioopm_linked_list_append(cart->items, void_elem(item3));

  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch1), 1);
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch2), 100);
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch3), 50);
  cart_destroy(cart);
}


// Increase the amount of a merchandise in a cart, and check if the amount is correct
void test_cart_increase_existing()
{
  cart_t* cart = cart_new(1);
  char* merch1 = strdup("Chair");
  char* merch2 = strdup("Lamp");
  char* merch3 = strdup("Table");
  cart_increase_merch(cart, merch1, 1);    // [Chair: x1]
  cart_increase_merch(cart, merch2, 100);  // [Chair: x1, Lamp: x100]
  cart_increase_merch(cart, merch3, 50);   // [Chair: x1, Lamp: x100, Table: x50]
  cart_increase_merch(cart, merch1, 1);    // [Chair: x2, Lamp: x100, Table: x50]
  cart_increase_merch(cart, merch2, 100);  // [Chair: x2, Lamp: x200, Table: x50]
  cart_increase_merch(cart, merch3, 50);   // [Chair: x2, Lamp: x200, Table: x100]
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch1), 2);
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch2), 200);
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch3), 100);
  cart_destroy(cart);
}


// Decrease the amount of a merchandise in a cart, and check if the amount is correct
void test_cart_decrease_existing()
{
  cart_t* cart = cart_new(1);
  char* merch1 = strdup("Chair");
  char* merch2 = strdup("Lamp");
  char* merch3 = strdup("Table");
  cart_increase_merch(cart, merch1, 5);   // [Chair: x5]
  cart_increase_merch(cart, merch2, 100); // [Chair: x5, Lamp: x100]
  cart_increase_merch(cart, merch3, 22);  // [Chair: x5, Lamp: x100, Table: x22]

  cart_decrease_merch(cart, merch1, 2);   // [Chair: x3, Lamp: x100, Table: x22]
  cart_decrease_merch(cart, merch2, 50);  // [Chair: x3, Lamp: x50, Table: x22]
  cart_decrease_merch(cart, merch3, 2);   // [Chair: x3, Lamp: x50, Table: x20]
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch1), 3);
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch2), 50);
  CU_ASSERT_EQUAL(cart_get_item_amount(cart, merch3), 20);
  CU_ASSERT_NOT_EQUAL(cart_get_item_amount(cart, merch3), -20);
  cart_destroy(cart);
}


// Decrease the amount of a merchandise in a cart, so much so that it'll be removed
void test_cart_decrease_unitl_removal()
{
  cart_t* cart = cart_new(1);
  char* merch1 = strdup("Chair");
  char* merch2 = strdup("Lamp");
  char* merch3 = strdup("Table");
  cart_increase_merch(cart, merch1, 1);    // [Chair: x1]
  cart_increase_merch(cart, merch2, 100);  // [Chair: x1, Lamp: x100]
  cart_increase_merch(cart, merch3, 50);   // [Chair: x1, Lamp: x100, Table: x50]

  cart_decrease_merch(cart, merch1, 1);    // [NULL, Lamp: x100, Table: x50]
  cart_decrease_merch(cart, merch2, 999);  // [NULL, NULL, Table: x50]
  cart_decrease_merch(cart, merch3, 49);   // [NULL, NULL, Table: x1]
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Chair") == -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Lamp") == -1);
  CU_ASSERT_FALSE(cart_merch_lookup(cart, "Table") == -1);
  cart_destroy(cart);
}


// Test if some merchandise is in a cart, using the cart_merch_lookup function
void test_cart_merch_lookup()
{
  cart_t* cart = cart_new(1);
  cart_increase_merch(cart, strdup("Chair"), 1);
  cart_increase_merch(cart, strdup("Lamp"), 100);
  cart_increase_merch(cart, strdup("Table"), 50);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Chair") != -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Lamp") != -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Table") != -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Egg") == -1);
  cart_destroy(cart);
}


// Remove a merchandise from the cart, and check if it is removed
void test_cart_remove_merch()
{
  cart_t* cart = cart_new(1);
  char* merch1 = strdup("Chair");
  char* merch2 = strdup("Lamp");
  char* merch4 = strdup("Egg");
  char* merch3 = strdup("Table");
  cart_increase_merch(cart, merch1, 1);    // [Chair]
  cart_increase_merch(cart, merch2, 100);  // [Chair, Lamp]
  cart_increase_merch(cart, merch3, 50);   // [Chair, Lamp, Table]
  cart_increase_merch(cart, merch4, 30);   // [Chair, Lamp, Table, Egg]

  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Chair") != -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Lamp") != -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Table") != -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Egg") != -1);
  cart_remove_merch(cart, merch1);  // [NULL, Lamp, Table, Egg]
  cart_remove_merch(cart, merch2);  // [NULL, NULL, Table, Egg]
  cart_remove_merch(cart, merch3);  // [NULL, NULL, NULL, Egg]

  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Chair") == -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Lamp") == -1);
  CU_ASSERT_TRUE(cart_merch_lookup(cart, "Table") == -1);
  CU_ASSERT_FALSE(cart_merch_lookup(cart, "Egg") == -1);
  cart_destroy(cart);
}


// Check if a cart is empty
void cart_test_empty()
{
  cart_t* cart = cart_new(1);
  char* merch1 = strdup("Chair");
  char* merch2 = strdup("Lamp");

  CU_ASSERT_TRUE(cart_is_empty(cart));       // [NULL]
  cart_increase_merch(cart, merch1, 1);      // [Chair]
  CU_ASSERT_FALSE(cart_is_empty(cart));
  cart_increase_merch(cart, merch2, 100);    // [Chair, Lamp]
  cart_remove_merch(cart, merch1);           // [NULL, Lamp]
  cart_remove_merch(cart, merch2);           // [NULL, NULL]
  CU_ASSERT_TRUE(cart_is_empty(cart));

  cart_destroy(cart);
}


// Check reserved merchandise over several different carts
void cart_test_reserved()
{
  ioopm_hash_table_t* carts = ioopm_hash_table_create(hash_func_int, eq_int, eq_str); 
  cart_t* cart1 = cart_new(1);  // [Chair: 10, Lamp: 20]
  cart_t* cart2 = cart_new(2);  // [Chair: 5, Lamp: 15]
  cart_t* cart3 = cart_new(3);  // [Table: 17]

  ioopm_hash_table_insert(carts, int_elem(1), void_elem(cart1));
  ioopm_hash_table_insert(carts, int_elem(2), void_elem(cart2));
  ioopm_hash_table_insert(carts, int_elem(3), void_elem(cart3));

  char* merch1 = "Chair";
  char* merch2 = "Lamp";
  char* merch3 = "Table";

  cart_increase_merch(cart1, strdup(merch1), 10);
  cart_increase_merch(cart1, strdup(merch2), 20); 
  cart_increase_merch(cart2, strdup(merch1), 5);
  cart_increase_merch(cart2, strdup(merch2), 15);
  cart_increase_merch(cart3, strdup(merch3), 17);

  CU_ASSERT_EQUAL(cart_check_reserved(carts, "Chair"), 15);
  CU_ASSERT_EQUAL(cart_check_reserved(carts, "Lamp"), 35);
  CU_ASSERT_EQUAL(cart_check_reserved(carts, "Table"), 17);

  cart_free_all(carts);
}


// Create carts with merchandise, store them in a hash table, then free the table to see if any memory leaks occur
// Use with valgrind
void test_cart_free_all()
{
  ioopm_hash_table_t* carts = ioopm_hash_table_create(hash_func_int, eq_int, eq_str); 
  cart_t* cart1 = cart_new(1);  // [Chair: 10, Lamp: 20, Table: 30]
  cart_t* cart2 = cart_new(2);  // [Chair: 5, Lamp: 100]
  cart_t* cart3 = cart_new(3);  // [Table: 50]

  ioopm_hash_table_insert(carts, int_elem(1), void_elem(cart1));
  ioopm_hash_table_insert(carts, int_elem(2), void_elem(cart2));
  ioopm_hash_table_insert(carts, int_elem(3), void_elem(cart3));

  char* merch1 = "Chair";
  char* merch2 = "Lamp";
  char* merch3 = "Table";

  cart_increase_merch(cart1, strdup(merch1), 10);
  cart_increase_merch(cart1, strdup(merch2), 20); 
  cart_increase_merch(cart1, strdup(merch3), 30); 
  cart_increase_merch(cart2, strdup(merch1), 5);
  cart_increase_merch(cart2, strdup(merch2), 100);
  cart_increase_merch(cart3, strdup(merch3), 50);

  cart_free_all(carts);
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
    (CU_add_test(my_test_suite, "Create and destroy cart item", test_create_destroy_item) == NULL) ||
    (CU_add_test(my_test_suite, "Create and destroy an empty cart", test_create_destroy_empty_cart) == NULL) ||
    (CU_add_test(my_test_suite, "Create and destroy a non-empty cart", test_create_destroy_non_empty_cart) == NULL) ||
    (CU_add_test(my_test_suite, "Create and destroy many non-empty carts", test_create_destroy_many_non_empty_carts) == NULL) ||
    (CU_add_test(my_test_suite, "Get a specific cart from a hash table", test_cart_get) == NULL) ||
    (CU_add_test(my_test_suite, "Lookup a cart in a hash table", test_cart_ID_lookup) == NULL) ||
    (CU_add_test(my_test_suite, "Get the correct amount of a merch in a cart", test_cart_get_amount) == NULL) ||
    (CU_add_test(my_test_suite, "Increase an existing merch in a cart", test_cart_increase_existing) == NULL) ||
    (CU_add_test(my_test_suite, "Decrease an existing merch in a cart", test_cart_decrease_existing) == NULL) ||
    (CU_add_test(my_test_suite, "Decrease a merch until it's removed", test_cart_decrease_unitl_removal) == NULL) ||
    (CU_add_test(my_test_suite, "Lookup a merch in a cart", test_cart_merch_lookup) == NULL) ||
    (CU_add_test(my_test_suite, "Remove a merch from a cart", test_cart_remove_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if a cart is empty", cart_test_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Check reserved merch in carts", cart_test_reserved) == NULL) ||
    (CU_add_test(my_test_suite, "Free all carts", test_cart_free_all) == NULL) ||

    
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
