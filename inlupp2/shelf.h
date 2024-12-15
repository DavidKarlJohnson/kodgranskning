#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "merchandise.h"
#include "hash_table.h"
#include "linked_list.h"



typedef struct shelf shelf_t;
typedef struct merch merch_t;

struct shelf
{
    char* shelf;
    int quantity;
};


    
/// @brief Check if the given shelf is in use
/// @param storage_ht The HT holding the shelf IDs
/// @param id The string representation of the shelf
/// @return True/false if the shelf is in use
bool shelf_lookup(ioopm_hash_table_t* storage_ht, char* id);

/// @brief Given a shelf ID - return the shelf struct that's contained within that merches' location list
/// NOTE: Check if shelf is in use before calling this function! 'shelf_lookup'
/// @param merchandise_ht The HT where the merch is located
/// @param storage_ht The HT where shelf ID is located
/// @param id String representation of the shelf ID
/// @return Pointer to the shelf struct that's located within the merches' location list
shelf_t* shelf_get(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, char* shelf_id);

/// @brief Lets the user enter a valid shelf ID. If the incorrect format is given an error message
///        will be displayed, and the user will have to enter a valid one.
/// @param message The message to present to the user for input
/// @return A valid shelf ID
char* shelf_input(char* message);

/// @brief Create a new shelf element. This element should reside in the locations list
///        belonging to the merch which is stored at this shelf
/// @param id The "name" of the shelf
/// @param quantity The quantity of a particular merch which resides on this shelf
/// @return A shelf element
shelf_t* shelf_create(char* id, int quantity);

/// @brief Append a shelf_t struct to the location list of a merchandise
/// @param merch The merch to append the shelf to
/// @param shelf The shelf where the merch is stored
void shelf_append_to_merch(merch_t* merch, shelf_t* shelf);

/// @brief Free a shelf_t struct
/// @param shelf The shelf struct to free
void shelf_destroy(shelf_t* shelf);

/// @brief Free all entry_t structs within a hash table and their respective keys + values
/// NOTE: This function is a modified version of 'ioopm_hash_table_destroy' which will also free the
///       VALUE and KEY properties of all 'entry_t' structs. Since the hash table will take ownership of
///       these string values, they need to be freed properly. Thus this function should only be used for
///       the hash table keeping track of the shelf IDs and what merch is stored on them.
/// @param storage_ht The hash table where all the shelf IDs are stored and their content
void shelf_free_all(ioopm_hash_table_t* storage_ht);

/// @brief Create a copy of a shelf_t struct. Freeing the old shelf will not affect the new one.
/// @param old_shelf The old shelf to copy. The old shelf will be left untouched after function call.
/// @return A new shelf with the same attributes as the old one.
shelf_t* shelf_copy(shelf_t* old_shelf);

/// @brief Removes a shelf from the storage HT.
//// This is a modified version of 'ioopm_hash_table_remove' which properly frees the
///  VALUE and KEY properties of an 'entry_t' struct. Since the hash table will take ownership of
///  these string values, they need to be freed properly. Thus this function should only be used for
///  the hash table keeping track of the shelf IDs and what merch is stored on them.
/// @param storage_ht The HT from where the shelf will be removed
/// @param key The name of the shelf to remove. Usage: str_elem("A01")
/// @param result The removed shelf. 
/// @return True/false if shelf was removed properly
bool shelf_remove(ioopm_hash_table_t* storage_ht, elem_t key, elem_t *result);







