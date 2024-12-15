#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "hash_table.h"
#include "merchandise.h"
#include "cart.h"
#include "utils.h"



/// @brief Adds new merchandise to the warehouse. Adding a new merch with the same name as an existing merch is not allowed.
///        Merchandise name will autoomatically be capitalized.
/// NOTE: Adding new merchandise does not increase its stock.
/// @param merchandise_ht The hash table where all the merch is stored
/// @param storage_ht The hash table which keeps track of all storage locations and their content
void UI_add_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht);

/// @brief Prints all the merchandise that's in the warehouse. Only 20 items are printed at a time.
///        After 20 items the user will have the option to list the next 20, or to return to the main menu.
/// @param merchandise_ht The hash table where all the merch is stored
/// @param storage_ht The hash table which keeps track of all storage locations and their content
void UI_list_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht);

/// @brief The user inputs a desired merchandise which will be removed from the warehouse.
///        Merchandise which does not exsist can't be removed.
/// @param merchandise_ht The hash table to remove the merch from.
/// @param storage_ht The hash table which keeps track of where the merch is located. When a merch is removed, the storage location is also removed.
/// @param carts_ht The hash table where all the carts are stored. If a merch is removed, it'll also be removed from all carts.
void UI_remove_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, ioopm_hash_table_t* carts_ht);

/// @brief Allows the user to edit the name, description and price properties of a given item.
///        The name of a merch can't be change to an already existing one.
/// @param merchandise_ht The hash table where all the merch is stored
/// @param storage_ht The hash table which keeps track of all storage locations and their content
/// @param carts_ht The hash table where all the carts are stored. Changing a merch will change its properties within a cart.
void UI_edit_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* storage_ht);

/// @brief The user inputs an existing merchandise which will then have its storage locations and quantity listed.
/// NOTE: Merch which is currently in a cart will also be listed and won't be delisted until the cart is checked out.
/// @param merchandise_ht The hash table where all the merch is stored
/// @param storage_ht The hash table which keeps track of all storage locations and their content
void UI_show_stock(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht);

/// @brief Allows the user to increase the stock of the given merch.
///        * If the user inputs a new shelf, that shelf will be created.
//         * If an already existing shelf is given, the user will have to option to increase the stock of the merch that's stored on that shelf.
//         * A shelf can hold an unlimited amount of a given merch, but ONLY one type of merch.
///        * Only existing merch is allowed to increase its stock. If a particular merch doesn't exist, then it has to be created first.
/// @param merchandise_ht The hash table where all the merch is stored
/// @param storage_ht The hash table which keeps track of all storage locations and their content
void UI_replenish(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht);

/// @brief Create a new, empty cart. A new cart is automatically given a number ID.
/// @param carts_ht The hash table where the cart element will be stored
void UI_create_cart(ioopm_hash_table_t* carts_ht);

/// @brief Remove a cart from the system.
/// NOTE: Removing a cart will not destroy the stock of items within it.
/// @param carts_ht The hash table where the cart is stored, and from where it'll be removed
void UI_remove_cart(ioopm_hash_table_t* carts_ht);

/// @brief Add merchandise to a cart
///        * Adding merch to a cart will not change the stock of that merch, but it'll be reserved for that cart.
///        * If a merch with a stock of 1000 are all stored witin existing carts, then that merch can no longer be added
///          to a new cart. The merch is not out of stock, all instances of it just currently "reserved".
///        * Only merch that is created and is in stock can be added to a cart.
/// @param carts_ht The hash table where the cart is stored.
/// @param merchandise_ht The hash table where all the merch is stored.
void UI_add_to_cart(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* merchandise_ht);

/// @brief Remove merchandise from a given cart
///        * Only merch that's actually stored witin the cart is removable.
///        * If a higher quantity than actually exists of a given merch is entered, then that merch will be completely removed from the cart.
///        * Removing merch from will not destroy the stock of that merch. It'll be only removed from the cart.
/// @param carts_ht The hash table where the cart is stored.
/// @param merchandise_ht The hash table where the merch is stored.
void UI_remove_from_cart(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* merchandise_ht);

/// @brief Calculate the cost of a shopping cart. 
/// @param carts_ht The hash table where the cart is stored.
/// @param merchandise_ht The hash table where all the merch is stored.
void UI_calculate_cost(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* merchandise_ht);

/// @brief Go through with a purchase of all the items in a particular shopping cart.
///        * The cart will be removed from the system when the function terminates.
///        * The stock of the merch within the cart will be decreased from random shelves. 
///          If this action causes the stock on a shelf to be 0, then that shelf will be removed.
///        * The merch will not be removed from the system, just the stock.
/// @param carts_ht The hash table where the cart is stored and from where it'll be removed.
/// @param storage_ht The shelf locations of the merch
/// @param merchandise_ht The hash table where the merch is stored.
void UI_checkout(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* storage_ht, ioopm_hash_table_t* merchandise_ht);

/// @brief Ask the user if they'd like to terminates the program. No persistance on file implemented.
/// @return True/false if the user would like to quit.
bool UI_quit();

/// @brief Main menu from where the user makes navigates the sub-menus with keyboard input.
/// @param merchandise_ht All merchandise will be stored within this hash table
/// @param storage_ht The ID for the storage shelves and their content will be stored within this hash table
/// @param carts_ht All carts and their content will be stored witin this hash table
void UI_main_menu(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, ioopm_hash_table_t* carts_ht);