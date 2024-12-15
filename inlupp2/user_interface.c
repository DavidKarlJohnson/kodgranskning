#include "user_interface.h"



/*
Global variable 'Cart_counter' keeps track of all the carts that have been created.
Counter can never decrease, only increase. Thus, each cart is given its own unique ID.
A cart ID can never be reused, even if the cart is cheked out.
*/
int Cart_counter = 0;


static void UI_clear_console()
{
    system("clear");
}

/// @brief Request the user to enter a char. If the correct input is given, the function returns true
/// @param correct The correct input which will return true
/// @param question The input message presented to the user
/// @return True/false if the correct char is given.
static bool check_char_input(char correct, char* question) 
{ 
    printf("%s", question);
    char ch; 
    scanf("%c", &ch); 
    clear_input_buffer();
    return tolower(ch) == tolower(correct) ? true : false;
}


/// @brief Add merch and storage locations to the warehouse at program start.
/// @param merchandise_ht The hash table where all the merchandise information will be stored
/// @param storage_ht The hash table which keeps track of all the shelves and their content
/// @param name The name of the merchandise
/// @param desc The description of the merchandise
/// @param price The price of the merchandise
/// @param quantity The quantity of the merchandise to add to the shelf 
/// @param shelf_id The ID of the shelf where the merchandise will be stored. 
///                 Use the format *letter* *number* *number* (Ex: A01)
static void add_starting_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, char* name, char* desc, int price, int quantity, char* shelf_id)
{
    merch_t* new_merch = merch_create(strdup(name), strdup(desc), price);
    ioopm_hash_table_insert(merchandise_ht, str_elem(new_merch->name), void_elem(new_merch));
    shelf_append_to_merch(new_merch, shelf_create(strdup(shelf_id), quantity));
    ioopm_hash_table_insert(storage_ht, str_elem(strdup(shelf_id)), str_elem(strdup(name)));
}

void UI_add_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht)
{
    char* name = merch_input(merchandise_ht, "Name: ", false);
    char* desc = ask_question_string("Description: ");
    int price = input_positive_int("Price: ");
    merch_t* new_merch = merch_create(name, desc, price);
    ioopm_hash_table_insert(merchandise_ht, str_elem(name), void_elem(new_merch));
    printf("%s added!", name);
}


void UI_list_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht)
{
    if(ioopm_hash_table_is_empty(merchandise_ht))
    {
        puts("No merchandise available!");
        return;
    }

    ioopm_list_t* item_names = ioopm_hash_table_keys(merchandise_ht);

    for(int i = 0; i < ioopm_linked_list_size(item_names); i++) 
    {
        if(i % 20 == 0 && i != 0)
        {
            if(check_char_input('N', "Press *any* key to continue listing. 'N' for main menu: "))
            {
                break;
            }
        }
        elem_t element;
        ioopm_linked_list_get(item_names, i, &element);
        printf("%d.  %s\n", i + 1, element.s);
    }
    ioopm_linked_list_destroy(item_names);
}


void UI_remove_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, ioopm_hash_table_t* carts_ht)
{
    if(ioopm_hash_table_is_empty(merchandise_ht))
    {
        puts("No merchandise available!");
        return;
    }
    char* name = merch_input(merchandise_ht, "Name: ", true);   // FREE
    if(check_char_input('Y', "Merch found, would you like to remove [Y/N]?: "))
    {  
        merch_t* merch = merch_get(merchandise_ht, name);   
        
        // Remove from all storage locations
        ioopm_list_t* locations = merch_locations(merchandise_ht, name);
        for(int i = 0; i < ioopm_linked_list_size(locations); i++)
        {
            puts("LOOP1");
            elem_t current;
            ioopm_linked_list_get(locations, i, &current);
            shelf_t* shelf_pointer = current.p;
            
            elem_t unused;
            shelf_remove(storage_ht, str_elem(shelf_pointer->shelf), &unused);
        }
        
        // Remove from all carts
        ioopm_list_t* cart_ids = ioopm_hash_table_keys(carts_ht);
        for(int i = 0; i < ioopm_linked_list_size(cart_ids); i++)
        {
            elem_t current_elem;
            ioopm_linked_list_get(cart_ids, i, &current_elem);
            cart_t* current_cart = cart_get(carts_ht, current_elem.i); 
            if(cart_merch_lookup(current_cart, name) != -1)
            {
                cart_remove_merch(current_cart, name); 
            }
        }
        ioopm_linked_list_destroy(cart_ids);

       // Remove merch
        elem_t unused;
        ioopm_hash_table_remove(merchandise_ht, str_elem(name), &unused);
        merch_destroy(merch);
    }
    printf("%s removed!\n", name);
    free(name);
}


void UI_edit_merch(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* storage_ht)
{
    if(ioopm_hash_table_is_empty(merchandise_ht))
    {
        puts("No merchandise available!");
        return;
    }
    char* old_merch_name = merch_input(merchandise_ht, "Name: ", true);
    merch_t* old_merch = merch_get(merchandise_ht, old_merch_name);
    merch_t* new_merch = merch_copy(old_merch);

    bool loop = true;
    while(loop) 
    {
        printf("\n\nWhich property would you like to edit?\n");
        printf("'N' Name:  %s\n", new_merch->name);
        printf("'D' Description:  %s\n", new_merch->description);
        printf("'P' Price:  %d\n", new_merch->price);
        printf("'C' Complete\n");

        char user_input; 
        scanf("%c", &user_input); 
        clear_input_buffer();
        switch(toupper(user_input))
        {
            case 'N':
                ;
                char* new_name = merch_input(merchandise_ht, "New name: ", false);
                free(new_merch->name);
                new_merch->name = new_name;
                break;
            case 'D':
                free(new_merch->description);
                new_merch->description = ask_question_string("Description: ");
                break;
            case 'P':
                new_merch->price = input_positive_int("Price: ");
                break;
            case 'C':
                if(check_char_input('Y', "Save changes [Y/N]?: "))
                {
                    // If merch in stock, change merch name at storage locations
                    if(merch_in_stock(merchandise_ht, old_merch_name))
                    {
                        ioopm_list_t* old_merch_locations = merch_locations(merchandise_ht, old_merch_name);
                        for(int i = 0; i < ioopm_linked_list_size(old_merch_locations); i++)  // Add shelves from old merch to new
                        {
                            elem_t current;
                            elem_t unused;
                            ioopm_linked_list_get(old_merch_locations, i, &current);
                            shelf_remove(storage_ht, str_elem(current.s), &unused);
                            ioopm_hash_table_insert(storage_ht, str_elem(strdup(current.s)), str_elem(strdup(new_merch->name)));
                        }
                    }

                    // Apply merch changes to all carts, where it's stored 
                    if(!ioopm_hash_table_is_empty(carts_ht))
                    {
                        ioopm_list_t* cart_ids = ioopm_hash_table_keys(carts_ht);
                        for(int i = 0; i < ioopm_linked_list_size(cart_ids); i++)
                        {
                            elem_t current_elem;
                            ioopm_linked_list_get(cart_ids, i, &current_elem);
                            cart_t* current_cart = cart_get(carts_ht, current_elem.i); 
                            if(cart_merch_lookup(current_cart, old_merch_name) != -1)
                            {
                                int current_amount = cart_get_item_amount(current_cart, old_merch_name);
                                cart_remove_merch(current_cart, old_merch_name); 
                                cart_increase_merch(current_cart, strdup(new_merch->name), current_amount);
                            }
                        }
                        ioopm_linked_list_destroy(cart_ids);
                    }
                    elem_t unused;
                    ioopm_hash_table_remove(merchandise_ht, str_elem(old_merch_name), &unused);  // Remove old
                    ioopm_hash_table_insert(merchandise_ht, str_elem(new_merch->name), void_elem(new_merch));  // Insert new
                    merch_destroy(old_merch);
                    puts("Changes saved!");
                }
                else
                {
                    puts("...aborting");
                    merch_destroy(new_merch);
                }
                loop = false;
                break;
            default:
                puts("No such command!");
                break;
        }
    }
    free(old_merch_name);
}


void UI_show_stock(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht)
{
    if(ioopm_hash_table_is_empty(merchandise_ht))
    {
        puts("No merchandise available!");
        return;
    }
    if(ioopm_hash_table_is_empty(storage_ht))
    {
        puts("No merchandise in stock!");
        return;
    }
    char* merch_name = merch_input(merchandise_ht, "Name: ", true);
    ioopm_list_t* location_list = merch_locations(merchandise_ht, merch_name);

    if(!merch_in_stock(merchandise_ht, merch_name)) 
    {
        printf("%s is not in stock!", merch_name);
    }
    else
    {
        for(int i = 0; i < ioopm_linked_list_size(location_list); i++)
        {
            elem_t current_shelf;
            ioopm_linked_list_get(location_list, i, &current_shelf);
            shelf_t* current_shelf_pointer = current_shelf.p;
            printf("%s  |  %d\n", current_shelf_pointer->shelf, current_shelf_pointer->quantity);
        }
    }
    free(merch_name);
}


void UI_replenish(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht)
{
    char* id = shelf_input("Shelf ID: ");

    if(!shelf_lookup(storage_ht, id))  // Shelf doesn't exist, create new
    {
        if(check_char_input('Y', "That shelf is empty. Would you like to add an item [Y/N]? "))
        {
            char* merch_name = merch_input(merchandise_ht, "Item name: ", true);
            int quantity = input_positive_int("Quantity: ");
            merch_t* merch = merch_get(merchandise_ht, merch_name);
            shelf_append_to_merch(merch, shelf_create(id, quantity));

            ioopm_hash_table_insert(storage_ht, str_elem(strdup(id)), str_elem(strdup(merch_name)));
            printf("%d of '%s' added to %s!\n", quantity, merch_name, id);
            free(merch_name);
        }
        else
        {
            free(id);
            printf("...aborting\n");
        }
    }
    else  // Shelf exists already
    {
        shelf_t* shelf = shelf_get(merchandise_ht, storage_ht, id);
        elem_t merch_name;
        ioopm_hash_table_lookup(storage_ht, str_elem(id), &merch_name);
        printf("%s occupied by: %d '%s'\n", id, shelf->quantity, merch_name.s);

        if(check_char_input('Y', "Would you like to increase stock [Y/N]? "))
        {
            int amount = input_positive_int("Amount: ");
            printf("'%s' increased from %d to %d!\n", merch_name.s, shelf->quantity, shelf->quantity + amount);
            shelf->quantity += amount;
        }
        else
        {
            printf("...aborting\n");
        }
        free(id);
    }
}


void UI_create_cart(ioopm_hash_table_t* carts_ht)
{
    cart_t* new_cart = cart_new(Cart_counter);
    ioopm_hash_table_insert(carts_ht, int_elem(Cart_counter), void_elem(new_cart));
    printf("Cart ID#%d created!\n", Cart_counter);
    Cart_counter++;
}


void UI_remove_cart(ioopm_hash_table_t* carts_ht)
{
    if(ioopm_hash_table_is_empty(carts_ht))
    {
        puts("There are currently no carts!");
    }
    else 
    {
        int id = cart_input_ID(carts_ht, "Cart ID to remove: ", true);
        if(check_char_input('Y', "Are you sure you'd like to delete this cart [Y/N]? "))
        {
            printf("Cart ID#%d removed!\n", id);
            cart_destroy(cart_get(carts_ht, id));
            elem_t unused;
            ioopm_hash_table_remove(carts_ht, int_elem(id), &unused);
        }
        else
        {
            printf("...aborting\n");
        }
    }
    
}


void UI_add_to_cart(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* merchandise_ht)
{
    if(ioopm_hash_table_is_empty(carts_ht))
    {
        puts("There are currently no carts!");
    }
    else
    {
        int id = cart_input_ID(carts_ht, "Cart ID: ", true);
        cart_t* cart = cart_get(carts_ht, id);
        cart_print(cart);
        char* merch_name = merch_input(merchandise_ht, "Merch to add: ", true);
        if(merch_in_stock(merchandise_ht, merch_name))
        {
            int remaining_quantity =  merch_total_quantity(merchandise_ht, merch_name) - cart_check_reserved(carts_ht, merch_name);

            while(true)
            {
                int quantity = input_positive_int("Quantity: ");
                if(remaining_quantity == 0)  // All reserved in carts
                {
                    printf("All of '%s' in stock is reserved.\n", merch_name);
                    free(merch_name);
                    break;
                }
                else if(quantity > remaining_quantity)  // Not enough in stock
                {
                    printf("Only %d of '%s' remain in stock!\n", remaining_quantity, merch_name);
                }
                else if(quantity <= 0)  // Invalid input
                {
                    puts("Please input a quantity higher than 0!");
                }
                else
                {
                    cart_increase_merch(cart, merch_name, quantity);
                    printf("%d '%s' added to cart!\n", quantity, merch_name);
                    break;
                }
            }
        }
        else
        {
            printf("'%s' is not in stock!\n", merch_name);
            free(merch_name);
        }
    }
} 


void UI_remove_from_cart(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* merchandise_ht)
{  
    if(ioopm_hash_table_is_empty(carts_ht))
    {
        puts("There are currently no carts!");
    }
    else
    {
        int id = cart_input_ID(carts_ht, "Cart ID: ", true);
        cart_t* cart = cart_get(carts_ht, id);
        if(cart_is_empty(cart))
        {
            puts("That cart is empty!");
        }
        else
        {
            cart_print(cart);
            char* merch_name = cart_input_merch(cart, "Merch to remove: ", true);
            int quantity = input_positive_int("Quantity: ");
            cart_decrease_merch(cart, merch_name, quantity);
            printf("%d '%s' removed from cart #%d\n", quantity, merch_name, id);
            free(merch_name);
        }
    }
}


void UI_calculate_cost(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* merchandise_ht)
{
    if(ioopm_hash_table_is_empty(carts_ht))
    {
        puts("There are currently no carts!");
    }
    else
    {
        int id = cart_input_ID(carts_ht, "Cart ID: ", true);
        int cost = cart_get_cost(cart_get(carts_ht, id), merchandise_ht);
        cart_t* cart = cart_get(carts_ht, id);
        cart_print(cart);
        printf("Cost: %d\n", cost);
    }
}


void UI_checkout(ioopm_hash_table_t* carts_ht, ioopm_hash_table_t* storage_ht, ioopm_hash_table_t* merchandise_ht)
{
    if(ioopm_hash_table_is_empty(carts_ht))
    {
        puts("There are currently no carts!");
    }
    else 
    {
        int id = cart_input_ID(carts_ht, "Cart ID: ", true);
        if(check_char_input('Y', "Are you sure you'd like to checkout this cart [Y/N]? "))
        {
            cart_t* cart = cart_get(carts_ht, id);

            for(int i = 0; i < ioopm_linked_list_size(cart->items); i++)
            {
                elem_t current_item;
                ioopm_linked_list_get(cart->items, i, &current_item);
                cart_item_t* current_item_pointer = current_item.p;
                merch_decrease_stock(merchandise_ht, storage_ht, current_item_pointer->name, current_item_pointer->amount);
            }
            cart_destroy(cart);
            elem_t unused;
            ioopm_hash_table_remove(carts_ht, int_elem(id), &unused);
            printf("Cart #%i checked out successfully\n", id);
        }
        else
        {
            printf("...aborting\n");
        }
    }
}


bool UI_quit()
{
    if(check_char_input('Y', "Are you sure you'd like to quit? [Y/N]: "))
    {
        printf("...quitting\n");
        return true;
    }
    return false;
}


void UI_main_menu(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, ioopm_hash_table_t* carts_ht)
{
    bool loop = true;
    while(loop)
    {
        printf("\n\n'A' Add merchandise\n");
        printf("'L' List merchandise\n");
        printf("'D' Remove merchandise\n");
        printf("'E' Edit merchandise\n");
        printf("'S' Show stock\n");
        printf("'P' Replenish\n");
        printf("'C' Create cart\n");
        printf("'R' Remove cart\n");
        printf("'+' Add to cart\n");
        printf("'-' Remove from cart\n");
        printf("'=' Calculate cost\n");
        printf("'O' Checkout\n");
        printf("'U' Undo\n");
        printf("'Q' Quit\n\n");

        char user_input; 
        scanf("%c", &user_input); 
        clear_input_buffer();

        UI_clear_console();

        switch(toupper(user_input))
        {
            case 'A':
                UI_add_merch(merchandise_ht, storage_ht);
                break;
            case 'L':
                UI_list_merch(merchandise_ht, storage_ht);
                break;
            case 'D':
                UI_remove_merch(merchandise_ht, storage_ht, carts_ht);
                break;
            case 'E':
                UI_edit_merch(merchandise_ht, carts_ht, storage_ht);
                break;
            case 'S':
                UI_show_stock(merchandise_ht, storage_ht);
                break;
            case 'P':
                UI_replenish(merchandise_ht, storage_ht);
                break;
            case 'C':
                UI_create_cart(carts_ht);
                break;
            case 'R':
                UI_remove_cart(carts_ht);
                break;
            case '+':
                UI_add_to_cart(carts_ht, merchandise_ht);
                break;
            case '-':
                UI_remove_from_cart(carts_ht, merchandise_ht);
                break;
            case '=':
                UI_calculate_cost(carts_ht, merchandise_ht);
                break;
            case 'O':
                UI_checkout(carts_ht, storage_ht, merchandise_ht);
                break;
            case 'U':
                puts("Not yet implemented!\n");
                break;
            case 'Q':
                UI_quit() ? loop = false : true;
                break;
            default:
                puts("No such input!\n");
                break;
        }
    }
}


int main(void)
{
    // Key = {string} Merch name      Value =  {struct} Pointer to merch_t struct
    ioopm_hash_table_t* merchandise = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);  
    // Key = {string} Shelf ID        Value = {string} Merch name
    ioopm_hash_table_t* storage = ioopm_hash_table_create(hash_func_str, eq_str, eq_str);
    // Key = {int} Cart ID            Value = {struct} Pointer to cart struct
    ioopm_hash_table_t* carts = ioopm_hash_table_create(hash_func_int, eq_int, eq_str);

                                            // name, desc, price, quantity, shelf_id
    add_starting_merch(merchandise, storage, "Banana", "A banana", 10, 100, "A01");
    add_starting_merch(merchandise, storage, "Apple", "An apple", 5, 50, "A02");
    add_starting_merch(merchandise, storage, "Orange", "Arange", 7, 75, "A03");
    add_starting_merch(merchandise, storage, "Chair", "A wooden chair", 200, 200, "B55");
    add_starting_merch(merchandise, storage, "Table", "A wooden table", 500, 100, "C10");
    add_starting_merch(merchandise, storage, "Sofa", "A comfortable sofa", 1000, 50, "D01");
    add_starting_merch(merchandise, storage, "Lamp", "A bright lamp", 50, 150, "E01");
    add_starting_merch(merchandise, storage, "Milk", "Regular milk", 20, 200, "F11");
    add_starting_merch(merchandise, storage, "Bread", "White bread", 40, 300, "G66");
    add_starting_merch(merchandise, storage, "Butter", "Salted butter", 60, 500, "H15");
    add_starting_merch(merchandise, storage, "Cheese", "Cheddar cheese", 90, 300, "Y99");
    add_starting_merch(merchandise, storage, "Egg", "Medium sized chicken egg", 3, 1500, "E99");
    add_starting_merch(merchandise, storage, "Chocolate", "Dark chocolate", 25, 70, "C99");
    add_starting_merch(merchandise, storage, "Onion", "Yellow onion", 5, 900, "B99");
    add_starting_merch(merchandise, storage, "Tomato", "Red tomato", 10, 800, "A99");
    add_starting_merch(merchandise, storage, "Potato", "Sweet potato", 1, 10000, "P67");
    add_starting_merch(merchandise, storage, "Carrot", "Orange carrot", 3, 5000, "C67");
    add_starting_merch(merchandise, storage, "Cucumber", "Regular cucumber", 10, 5000, "D61");
    add_starting_merch(merchandise, storage, "Garlic", "Vampires beware", 5, 666, "H31");
    add_starting_merch(merchandise, storage, "Salt", "A pack of salt", 25, 50, "S01");
    add_starting_merch(merchandise, storage, "Pepper", "A pack of pepper", 60, 70, "S02");
    add_starting_merch(merchandise, storage, "Sugar", "A pack of sugar", 60, 100, "S03");
    add_starting_merch(merchandise, storage, "Spaghetti", "A pack of spaghetti", 30, 250, "Q70");
    add_starting_merch(merchandise, storage, "Rice", "Parboiled rice", 50, 50, "Q71");
    add_starting_merch(merchandise, storage, "Pasta", "Penne pasta", 25, 330, "Q72");

    UI_main_menu(merchandise, storage, carts);
    shelf_free_all(storage);
    cart_free_all(carts);
    merchandise_free_all(merchandise);
    return 0;
}