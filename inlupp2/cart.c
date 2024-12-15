#include "cart.h"


cart_t* cart_new(int id)
{
    cart_t* new_cart = calloc(1, sizeof(cart_t));
    new_cart->id = id;
    new_cart->items = ioopm_linked_list_create(eq_str);
    return new_cart;
}


cart_t* cart_get(ioopm_hash_table_t* cart_ht, int id)
{
    elem_t result;
    ioopm_hash_table_lookup(cart_ht, int_elem(id), &result);
    cart_t* cart = result.p;
    return cart;
}


bool cart_ID_lookup(ioopm_hash_table_t* cart_ht, int id)
{
    elem_t unused;
    return ioopm_hash_table_lookup(cart_ht, int_elem(id), &unused);
}


int cart_input_ID(ioopm_hash_table_t* cart_ht, char* message, bool exists)
{
    int id = ask_question_int(message);
    if(exists)
    {
        while(!cart_ID_lookup(cart_ht, id))
        {
            puts("No such ID!\n");
            cart_print_all(cart_ht);
            id = ask_question_int(message);
        }
    }
    else
    {
        while(cart_ID_lookup(cart_ht, id))
        {
            puts("That ID already exists!\n");
            cart_print_all(cart_ht);
            id = ask_question_int(message);
        }
    } 
    return id;
}


int cart_merch_lookup(cart_t* cart, char* merch)
{
    ioopm_list_t* cart_items = cart->items;
    if(!ioopm_linked_list_is_empty(cart_items))
    {
        int index = 0;
        ioopm_list_iterator_t* iter = ioopm_list_iterator(cart_items);
        while(ioopm_iterator_has_next(iter))
        {
            elem_t current_elem = ioopm_iterator_current(iter);
            cart_item_t* current_item = current_elem.p;
            if(strcmp(current_item->name, merch) == 0)
            {
                ioopm_iterator_destroy(iter);
                return index;
            }
            ioopm_iterator_next(iter, &current_elem);
            index++;
        }
        elem_t current_elem = ioopm_iterator_current(iter);
        cart_item_t* current_item = current_elem.p;
        if(strcmp(current_item->name, merch) == 0)
        {
            ioopm_iterator_destroy(iter);
            return index;
        }
        ioopm_iterator_destroy(iter);
    }
    return -1;
}


char* cart_input_merch(cart_t* cart, char* message, bool exists)
{
    char* merch = ask_question_string(message);
    if(exists)
    {
        while(cart_merch_lookup(cart, merch) == -1)
        {
            puts("No such merch!\n");
            free(merch);
            merch = ask_question_string(message);
        }
    }
    else
    {
        while(cart_merch_lookup(cart, merch) != -1)
        {
            puts("That mech already exists!\n");
            free(merch);
            merch = ask_question_string(message);
        }
    }
    
    return merch;
}


bool cart_is_empty(cart_t* cart)
{
    return ioopm_linked_list_is_empty(cart->items);
}


int cart_get_cost(cart_t* cart, ioopm_hash_table_t* merchandise_ht)
{
    // Fetch the names of all the merch in the cart. Use the names to find the quantity.
    // Then use the names to find the merch structs, where the price is sorted.
    int sum = 0;
    if(!ioopm_linked_list_is_empty(cart->items))
    {
        for(int i = 0; i < ioopm_linked_list_size(cart->items); i++) 
        {
            elem_t current;
            ioopm_linked_list_get(cart->items, i, &current);
            cart_item_t* current_item = current.p;
            int current_merch_price = merch_get(merchandise_ht, current_item->name)->price;
            sum += current_merch_price * current_item->amount;
        }
    }
    return sum;
}


int cart_check_reserved(ioopm_hash_table_t* cart_ht, char* merch)
{
    int sum = 0;
    ioopm_list_t* cart_list = ioopm_hash_table_values(cart_ht);  // All cart stucts
    
    for(int i = 0; i < ioopm_linked_list_size(cart_list); i++)
    {
        elem_t current_cart;
        ioopm_linked_list_get(cart_list, i, &current_cart);
        cart_t* cart_pointer = current_cart.p;

        if(cart_merch_lookup(cart_pointer, merch) != -1)  // If merch is in current cart, add quantity to sum
        {
            int current_amount = cart_get_item_amount(cart_pointer, merch);
            sum += current_amount;
        }
    }
    ioopm_linked_list_destroy(cart_list);
    return sum;
}


void cart_remove_merch(cart_t* cart, char* merch)
{
    elem_t item;
    int index = cart_merch_lookup(cart, merch);
    ioopm_linked_list_remove(cart->items, index, &item);
    cart_item_t* item_pointer = item.p;
    cart_destroy_item(item_pointer);
}


void cart_increase_merch(cart_t* cart, char* merch, int increase)
{
    if(cart_merch_lookup(cart, merch) != -1)  // Exists already, increase amount
    {
        int index = cart_merch_lookup(cart, merch);
        elem_t current_elem;
        ioopm_linked_list_get(cart->items, index, &current_elem);
        cart_item_t* current_item = current_elem.p;
        current_item->amount += increase;
    } 
    else  // Doesn't exist, create new
    {
        cart_item_t* new_item = cart_create_item(merch, increase);
        ioopm_linked_list_append(cart->items, void_elem(new_item));
    }
}


void cart_decrease_merch(cart_t* cart, char* merch, int decrease)
{
    int index = cart_merch_lookup(cart, merch);
    elem_t current_elem;
    ioopm_linked_list_get(cart->items, index, &current_elem);
    cart_item_t* current_item = current_elem.p;
    current_item->amount -= decrease;
    if(current_item->amount <= 0)
    {
        cart_remove_merch(cart, merch);
    }
}


void cart_destroy(cart_t* cart)
{
    if (cart == NULL)
    {
        return; 
    }
    while((ioopm_linked_list_size(cart->items)) > 0) { 
        elem_t current;
        ioopm_linked_list_remove(cart->items, 0, &current);
        cart_item_t* current_item = current.p;
        cart_destroy_item(current_item);
    }
    ioopm_linked_list_destroy(cart->items);
    free(cart);
}


void cart_destroy_item(cart_item_t* item)
{
    free(item->name);
    free(item);
}


cart_item_t* cart_create_item(char* name, int amount)
{
    cart_item_t* new_item = calloc(1, sizeof(cart_item_t));
    new_item->name = name;
    new_item->amount = amount;
    return new_item;
}


int cart_get_item_amount(cart_t* cart, char* merch)
{
    int index = cart_merch_lookup(cart, merch);
    elem_t current_item;
    ioopm_linked_list_get(cart->items, index, &current_item);
    cart_item_t* item_pointer = current_item.p;
    return item_pointer->amount;
}


void cart_free_all(ioopm_hash_table_t* carts_ht) 
{
    if(!ioopm_hash_table_is_empty(carts_ht))
    {
        ioopm_list_t* cart_ids = ioopm_hash_table_keys(carts_ht);
        for(int i = 0; i < ioopm_linked_list_size(cart_ids); i++) 
        {
            elem_t current_id;
            ioopm_linked_list_get(cart_ids, i, &current_id);
            cart_t* current_cart = cart_get(carts_ht, current_id.i);
            cart_destroy(current_cart);
        }
        ioopm_linked_list_destroy(cart_ids);
    }
    ioopm_hash_table_destroy(carts_ht);
}


void cart_print(cart_t* cart)
{
    printf("Cart #%d\n", cart->id);
    puts("--------------------------------");
    if(!ioopm_linked_list_is_empty(cart->items))
    {
        for(int i = 0; i < ioopm_linked_list_size(cart->items); i++)
        {
            elem_t current;
            ioopm_linked_list_get(cart->items, i, &current);
            cart_item_t* current_item = current.p;
            printf("%d.  x%d %s\n", i + 1, current_item->amount, current_item->name);
        }
    }
    else
    {
        puts("*Empty*");
    }
    puts("--------------------------------");
}


void cart_print_all(ioopm_hash_table_t* carts_ht)
{
    if(!ioopm_hash_table_is_empty(carts_ht))
    {
        printf("-------------- Available carts --------------\n");
        ioopm_list_t* cart_ids = ioopm_hash_table_keys(carts_ht);
        for(int i = 0; i < ioopm_linked_list_size(cart_ids); i++)
        {
            elem_t current_id;
            ioopm_linked_list_get(cart_ids, i, &current_id);
            printf(" *  %d\n", current_id.i);
        }
        printf("---------------------------------------------\n");
        ioopm_linked_list_destroy(cart_ids);
    }
}