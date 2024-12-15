#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"
#include "linked_list.h"
#include "shelf.h"
#include "utils.h"



typedef struct merch merch_t;

struct merch
{
    char* name;
    char* description;
    int price;
    ioopm_list_t* location;  // Contains 'shelf_t'
};



/// @brief Create a new merchandise element
/// @param name Name of the merch
/// @param desc The description of the merch
/// @param price Price of the merch
/// @return A pointer to a merch element
merch_t* merch_create(char* name, char* desc, int price);

/// @brief Check a particular merchandise exists
/// NOTE: A merch existing doesn't necessarily mean that it has to be in stock
/// @param merchandise_ht The HT holding the merch
/// @param name Name of the merch
/// @return True/false if the merch exists
bool merch_lookup(ioopm_hash_table_t* merchandise_ht, char* name);

/// @brief Get a pointer to the given merch
/// @param merchandise_ht The HT where the merch is stored
/// @param name Name of the merch
/// @return A pointer to a merch element
merch_t* merch_get(ioopm_hash_table_t* merchandise_ht, char* name);

/// @brief Check a particular merchandise is in stock at any shelf
/// @param merchandise_ht The HT holding the merch
/// @param name Name of the merch
/// @return True/false if the merch is in stock 
bool merch_in_stock(ioopm_hash_table_t* merchandise_ht, char* name);

/// @brief Ask the user to enter a merch name that either already exists or doesn't
/// @param merchandise_ht The hash table to search for the merch item
/// @param message The message to display to the user
/// @param exists If the merch should exist in the hash table or not
/// @return The string given by the user if it successfully goes through the function
/// @example merch_input(test_ht, "Name of merch: ", true);
///          Will return "Chair", if the user inputs "Chair" and it exists within 'test_ht
/// @example merch_input(test_ht, "New merch: ", false);
///          Will only return if the name of a non-existing merch (within 'test_ht') is given
char* merch_input(ioopm_hash_table_t* merchandise_ht, char* message, bool exists);

/// @brief Free a merch_t struct
/// NOTE: Will also free all shelf_t structs within the merch
/// @param merch The merch to free
void merch_destroy(merch_t* merch);

/// @brief Get a list of all the locations (shelves) containing a certain item
/// @param merchandise_ht The hash table to search
/// @param item_name Name of the item to find the locations of
/// @return A list of shelves containing all the locations where the item is stored and its quantity
ioopm_list_t* merch_locations(ioopm_hash_table_t* merchandise_ht, char* item_name);

/// @brief Search all shelves and sum of the quantity of a mechandise
/// @param merchandise_ht The hash table where the merch is stored
/// @param item_name Name of the merch
/// @return To total quantity of that merch on all shelves
int merch_total_quantity(ioopm_hash_table_t* merchandise_ht, char* item_name);

/// @brief Decrease the stock of a given merchandise on any RANDOM shelves wherever it resides
/// @param merchandise_ht The hash table where the merch is stored
/// @param storage_ht The hash table which maps shelves to their merch
/// @param item_name The merch to decrease
/// @param decrease The desired quantity to remove 
/// @example If 'Chair' is stored at -> A10: 55,  G07: 10,  K78: 20,  then decreasing its quantity by 80
///          will result in the removal of shelf K78, G07, and the decrease of A10 to 5.
void merch_decrease_stock(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, char* item_name, int decrease);

/// @brief Properly free all merchandise and their respective shelf locations from the hash table
/// @param merchandise_ht The hash table to free
void merchandise_free_all(ioopm_hash_table_t* merchandise_ht);

/// @brief Create a copy of a merch element and the shelf locations where it's stored
/// @param old_merch The merch to copy. 
/// @return A copy of the argument merch
merch_t* merch_copy(merch_t* old_merch);

/// @brief Print the names of all the merchandise in the warehouse
/// @param merchandise_ht The hash table where all the merch is stored
void merch_print_all(ioopm_hash_table_t* merchandise_ht);
