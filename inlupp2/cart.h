#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"
#include "hash_table.h"
#include "merchandise.h"
#include "utils.h"

typedef struct cart cart_t;
typedef struct cart_item cart_item_t;


struct cart 
{
    int id;
    ioopm_list_t* items;  // Contains 'cart_item_t'
};


struct cart_item
{
    char* name;
    int amount;
};




/// @brief Create a new, empty cart element
/// @param id The number ID for the cart
/// @return Pointer to a new cart element
cart_t* cart_new(int id);

/// @brief Get the pointer to a cart element
/// @param cart_ht The hash table where the cart is stored
/// @param id The number ID for the cart
/// @return Pointer to the cart element
cart_t* cart_get(ioopm_hash_table_t* cart_ht, int id);

/// @brief Search if a cart with the argument ID exists
/// @param cart_ht The hash table to search
/// @param id The number ID of the cart to search for
/// @return True/false if the cart exists within the hash table
bool cart_ID_lookup(ioopm_hash_table_t* cart_ht, int id);

/// @brief Ask the user to input a cart ID
/// @param cart_ht The hash table to search if the given cart exists/doesn't exist within
/// @param message The message to display to the user before giving an input
/// @param exists If the cart ID exists witin the hash table or not
/// @return The ID input given by the user if passed through the function correctly
/// @example cart_input_ID(test_ht, "Cart id: ", true);
///          Will return the ID of the cart input by the user iff it exists within 'test_ht'
/// @example cart_input_ID(test_ht, "New id: ", false);
///          Will return the ID of the cart input by the user iff it's a new, unique ID
int cart_input_ID(ioopm_hash_table_t* cart_ht, char* message, bool exists);

/// @brief Search if a particular merch is in a given cart
/// @param cart The cart element to search
/// @param merch The name of the merch to search for
/// @return The index position of the merch within the carts linked list.
///         Will return -1 if the merch doesn't exist within the cart.
int cart_merch_lookup(cart_t* cart, char* merch);

/// @brief Ask the user to input a merch that either does/doesn't exist in a given cart
/// @param cart The cart to search 
/// @param message The message to display to the user before giving an input
/// @param exists True/false if the merch exists in the cart
/// @return The name of the merch given by the user if passed through the function correctly
/// @example cart_input_merch(example_cart, "Merch to remove: ", true);
///          Will return the name of the merch iff it already exist in 'example_cart'
/// @example cart_input_merch(example_cart, "Merch to add: ", false);
///          Will return the name of the merch iff it's non-existing within 'example_cart'
char* cart_input_merch(cart_t* cart, char* message, bool exists);

/// @brief Check if the current cart has any merchandise
/// @param cart The cart to check
/// @return True/false if the cart contain >0 merchandise
bool cart_is_empty(cart_t* cart);

/// @brief Loop through all merch witin a cart an sum up the cost
/// @param cart The cart to sum up
/// @param merchandise_ht The hash table where all the merch is stored
/// @return To total cost of a cart (merch * quantity * price)
int cart_get_cost(cart_t* cart, ioopm_hash_table_t* merchandise_ht);

/// @brief Check how much of a particular merches' stock that's currently "reserved" in carts
/// @param cart_ht The hash table where all the carts are stored
/// @param merch The name of the merch to check
/// @return Number of 'merch' that's currently added to the carts witin the warehouse.
/// @example The merch 'Chair' has a stock of 1000. If cart #1 has 100 chairs, cart #2 has 500 chairs
///          then 600/1000 are currently "reserved" and only 400 should be available for purchase.
int cart_check_reserved(ioopm_hash_table_t* cart_ht, char* merch);


/// @brief Remove the given merch from the cart
/// NOTE: Make sure to check if the merch exists within the cart before calling this function
/// @param cart The cart to remove the merch from
/// @param merch The merch to remove from the cart
void cart_remove_merch(cart_t* cart, char* merch);

/// @brief Increase the amount of a given merch in a cart.
///        If the merch doesn't already exist within the cart, it'll be created and inserted
/// @param cart The cart to increase the merch in
/// @param merch The merch to increase
/// @param increase The amount to increase
void cart_increase_merch(cart_t* cart, char* merch, int increase);

/// @brief Decrease the amount of a given merch in a cart
///        If the decrease sets the amount below 0, that merch will be removed from the cart
/// @param cart The cart to decrease the merch in
/// @param merch The merch to decrease
/// @param decrease The amount to decrease
void cart_decrease_merch(cart_t* cart, char* merch, int decrease);

/// @brief Remove and free a given cart 
/// NOTE: The stock of the merch within the cart will not be reduced! It will only be removed from the cart
/// @param cart_ht The hash table where the cart is stored
/// @param id The number ID of the cart
void cart_destroy(cart_t* cart);

/// @brief Free a cart_item_t struct
/// @param item The item to free
void cart_destroy_item(cart_item_t* item);

/// @brief Create a cart item, with a name and amount
/// @param name The name of the merch
/// @param amount The amount of the merch
/// @return A cart item to be stored within a cart 
cart_item_t* cart_create_item(char* name, int amount);

/// @brief Get the amount of a given merch within a cart
/// @param cart The cart to search
/// @param merch The merch to search for
/// @return The amount of the given merch within the cart
int cart_get_item_amount(cart_t* cart, char* merch);

/// @brief Free all carts and their content within the warehouse
/// @param carts_ht The hash table where all carts are stored
void cart_free_all(ioopm_hash_table_t* carts_ht);

/// @brief Print a cart and all of its content
/// @param cart The cart to print
void cart_print(cart_t* cart);

/// @brief Print all carts ID's that's currently active in the warehouse
/// @param carts_ht The hash table where all the carts are stored
void cart_print_all(ioopm_hash_table_t* carts_ht);