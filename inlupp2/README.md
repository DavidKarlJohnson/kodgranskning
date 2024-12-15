// @author: David Johnson  |  github.com/davidKarlJohnson
// @date:   15/11 -24



-----------------------------------
              GENERAL
-----------------------------------

This program simulates a simple warehouse which can store merchandise with a price, description and an amount. The created merchandise can be stored on shelves within the warehouse, which are used to keep track of all the available stock. Merchandise can be removed, edited and added to carts. These carts will simulate customers that are shopping in the warehouse. Carts can have any amount of merchandise added to them, their total cost can be calculated and they can be checked out from the warehouse, in which case, the stock of the merchandise witin the cart will be reduced.




FOR USERS: See within this README 'How to run' to start, and 'How the user-interface works' for more detailed information.

FOR DEVELOPERS:
    * Read 'Hash tables' below to get an overview of how data is stored witin the warehouse.
    * Read 'Ownership' below to see which part of the program takes ownership of which data. This is important, as free'ing data prematurely can cause crashes and/or memory leaks.
    * Read 'Custom data types' to see the different data types, how they're intended to be used and where they should be stored.
    * See the .png illustration witin this directory to get a high-level overview of the hash-tables and the data stored witin them.

__________________________________________________________________________________________________________________________________________________________________________________________


-----------------------------------
            HOW TO RUN
-----------------------------------

* make run              - to start the program
* make run_memtest      - to start the program and check for memory leaks during run
* make run_shelf_tests  - to run testing for the 'shelf.c' module and check for memory leaks
* make run_cart_tests   - to run testing for the 'cart.c'  module and check for memory leaks
* make run_merch_tests  - to run testing for the 'merchandise.c'  module and check for memory leaks
* make clean            - to delete temporary files


__________________________________________________________________________________________________________________________________________________________________________________________


-----------------------------------
     TEST-COVERAGE (using gcov)
-----------------------------------

FILE                                       LINES    EXEC   COVER
shelf.c                                       72      61    84%
merchandise.c                                114      89    78% 
cart.c                                       152      99    65% 
shelf_tests.c                                 98      93    94%
merchandise_tests.c                          159     154    96%
cart_tests.c                                 240     235    97%


NOTE:
* The user-interface module does not have any tests.
* Many of the tests built for the shelf, merch and cart modules are there to check for memory leaks, not code coverage.
__________________________________________________________________________________________________________________________________________________________________________________________


-----------------------------------
    HOW THE USER-INTERFACE WORKS
-----------------------------------

 -- ADD MERCHANDISE
    * Will add merchandise to the warehouse, but won't increase its stock.
    * To increase the stock of a merchandise after it has been created use the 'Replenish' sub-menu.
    * Adding merchandise that already exists is not possible.
    * Entering a merchandise with a lower-case starting letter will be capitalized automatically.

-- LIST MERCHANDISE
    * Will list 20 merchandise at a time.
    * Pressing any key other than 'N' will list the next 20 merchandise.
    * Pressing 'N' at any point will return to the main menu.

-- REMOVE MERCHANDISE
    * This sub-menu will not run if there are no merchandise at all in the warehouse.
    * If an incorrect merchandise is given (that's not in the warehouse) a list of all the available ones will be printed.
    * Removing a merchandise will remove all its stock.
    * Removing a merchandise will remove it from all carts.

-- EDIT MERCHANDISE
    * You can't change the name of a merchandise to one that already exists.
    * Changing the name of a merchandise will change the name of it in all current carts.

-- SHOW STOCK
    * Entering a merchandise that does not exist will print a list of all the available merch in the warehouse.
    * Entering a merchandise that exists, but is not in stock will notify that it's not in stock.

-- REPLENISH
    * This sub-menu is the only way to increase the stock of a merchandise.
    * A shelf can only store one type of merchandise.
    * A shelf can store an unlimited amount of one particular merchandise.
    * Entering a shelf that does not have any stock on it will ask the user if they'd like to add stock to the shel.
    * Entering a shelf that does have stock on it will ask the user if they'd like to increase the stock of that merch.
    * Negative values can't be entered to reduce stock.

-- CREATE CART
    * The ID of a cart will be generated automatically.
    * The ID of a cart will be unique for that cart. If the cart is removed or checked out, that ID is not reusable.

-- REMOVE CART
    * Removing a cart will also remove the items within it. But it won't destroy the stock of that item, nor will it remove the merch from the warehouse. Only the reserved amount within the car will be removed.

-- ADD TO CART
    * Only merch that is in stock can be added to a cart.
    * Adding merch to a cart will "reserve" the merch for that specific cart. If all available stock of a particular merchandise is added to carts, then it can't be added to more carts.
    * Adding merch to a cart won't reduce the stock of that item, only "reserve it".
    * Checking out a cart from the warehouse WILL reduce the stock of the items within it.

-- CALCULATE COST
    * If a cart does not have any merch in it, 0 will be displayed as cost.
    * If merch is removed from the cart, the cost will be adjusted.

-- CHECKOUT
    * Checking out a cart will remove that cart from the warehouse.
    * Checking out a cart will remove the stock of the merchandise within that cart. If the stock of a particular shelf is reduced below 0, then that shelf will be removed from the system.

-- UNDO
    * Unimplemented feature.

-- QUIT
    * Data won't be stored on file after the program terminates.


__________________________________________________________________________________________________________________________________________________________________________________________


-----------------------------------
            HASH TABLES
-----------------------------------

All important data is stored within three different hash tables:


    *** MERCHANDISE ***
        - Key: {string}   Name of the merchandise  (eg: "Banana")
        - Value: {struct, merch_t}   The properties of the merchandise  (eg: name: "Banana", description: "A banana", price: 10, 
                                     location = ["A10", "B55"])


    'merchandise_ht' is used to store information about the mechandise inside the warehouse. This hash table holds the 'merch_t' struct type, which is used to hold the attributes of a particular mechandise. These attributes are: name, price, description and location. The location of a merch is stored inside the 'shelf_t' struct type, which holds information about the shelf ID and the amount at that shelf. The 'shelf_t' type will be stored in a linked list belonging to the 'merch_t'.

    Example:

    merchandise_ht = [Banana, Table, Lamp]
    
    merch_t Banana {
        name: "Banana"
        desc: "A banana"
        price: 10
        location: [A10, B22]
    }

    shelf_t A10 {
        id: "A10"
        quantity: 500
    }


    * FREE 'merchandise_ht' with 'merchandise_free_all'  (will free 'merch_t' and 'shelf_t' witin)

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



    *** STORAGE ***
        - Key: {string}   Name-id of the shelf   (eg: "A10")
        - Value: {string}   Name of the merch stored at the shelf   (eg: "Chair")

    'storage_ht' is just a hash table for keeping track of all the shelves and their content. A 'merch_t' type keeps information about where ITSELF is stored (with 'shelf_t'). However, if we want to find what's stored at shelf "R55" we'd have to search every merchandise and the shelves appended to them which would lead to uneccessary computation. Therefore we use 'storage_ht' just to be able to do these quick searches. The downside to this is that we always have to keep this hash table updated whenever we increase or decrease the stock of a particular merch.

    NOTE: Since the user will be inputting information with the function 'ask_question_string' which uses 'strdup', we'll have KEY and VALUE strings within this hash table that needs to be free'ed properly whenever we remove something from the table (or terminate the program). Therefore the function 'ioopm_hash_table_destroy' won't be enough, since it doesn't free keys and values inside it. A modification of the function has been implemented to combat this issue: 'shelf_free_all'. Use this funciton whenever you want to terminate the program. And use 'shelf_remove' instead of 'ioopm_hash_table_remove' whenever you want to remove an individual item.


    * FREE 'storage_ht' with 'shelf_free_all'
    * Use 'shelf_remove' to properly REMOVE from 'storage_ht'

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    *** CARTS ***
        - Key: {int}   The number-id for the cart   (eg: 5)
        - Value: {struct, cart_t}   The properties of the cart   (eg:  id: 5,  items = ["Banana", "Chair"])


    'carts_ht' is used to store all carts that are currently active within the warehouse. This hash table holds the 'cart_t' type. Which is used to keep information about a particular cart. Inside a cart we store structs of the type 'cart_item_t', which holds the name and the amount of a particular item. 


    Example:

    carts_ht = [0, 1, 2, 3, 4, 5]

    cart_t 0 {
        id: 0
        items = [Chair, Tomato, Sugar]
    }

    cart_item_t Tomato {
        name: "Tomato"
        amount: 8
    }


    * FREE 'carts_ht' with 'cart_free_all'
__________________________________________________________________________________________________________________________________________________________________________________________


-----------------------------------
            OWNERSHIP
-----------------------------------

    * Whenever the user is asked to enter a string through the UI the function 'ask_question_string' in 'utils.c' is called at some point. 'ask_question_string' will make use of 'ask_question', which in turn use 'strdup'. Usage of 'strdup' means that at some point the string that's entered by the user will have to be properly free'd to avoid memory leaks. Therefore, whenever the user enters a string from the UI, and we store that string in a struct, hash table or list we have to remember to free it at program termination.

    Example: The user inputs "Tomato" when adding merchandise through the UI. The string "Tomato" will be stored within a 'merch_t' struct, which will be stored in the hash table 'merchandise_ht'. Later the user increases the stock of the tomato to 1000 on shelf "A10". The string "A10" will be stored in a 'shelf_t' struct which will be appended to the list witin the tomatos 'merch_t' struct. 
    Therefore we have a 'merch_t' which has taken ownership of a string (name: "Tomato"), containing a 'shelf_t' with ownership of a string (id: "A10"), which is stored in the hash table 'merchandise_ht'.

    'merchandise_ht' contains 'merch_t', which owns "Tomato" and contains 'shelf_t'. which owns "A10"


    * When the user adds a new merch with 'UI_add_merch', the 'name' and 'description' strings belongs to the 'merch_t' that's created


    * When the user adds a new shelf with 'UI_replenish', the 'id' string belongs to the 'shelf_t' which is created and appended to the merch. A copy of this id and a copy of the merch name will be created and inserted into the 'storage_ht', thus it takes ownership of its own strings and does not share ownership with the shelf and merch.


    * When the user creates a cart and add items to that cart a new struct called 'cart_item_t' will be created. This struct will take ownership of the items string name. Example: "Banana" is added to a cart. The 'cart_item_t' inside the cart will take ownership of the string "Banana" and is responsible for freeing it when the cart is checked out, deleted or the item is removed from the cart. This string is a COPY of the string stored in the 'merch_t' struct which is used to keep information about a Banana. Thus when we make changes or free this string in the cart it won't affect the merch.




    With these points in consideration - take a look in this document how to properly free a custom data type or a hash table, so that no memory leaks will occur after program termination or so that one part of the program doesn't free another part prematurely.



__________________________________________________________________________________________________________________________________________________________________________________________




-----------------------------------
       CUSTOM DATA TYPES
-----------------------------------

    *** shelf_t {struct} ***

    A shelf struct consists of two attributes:
        - shelf {*char}: This string is the "name" of the shelf, which should consist of a combination of [letter][number][number],
                         where the letter should be capitalized, eg: "A10", "G74". When the user enters a shelf through the interface
                         it will be checked for correctness with the 'shelf_input' function. NOTE: If you want to add a shelf outside interation with the UI (example: hard-coded shelves before program start) make sure they follow this format.
        - quantity {int}: Keeps track of the quantity of a particular merch stored at this shelf.

    * A 'shelf_t' struct should only be used in conjuction with the 'merch_t' type. The 'shelf_t' should be appended to the 'location' linked list within a 'merch_t'. This is the merchandises way of knowing WHERE it's currently stored and how much. 
    * A merchandise can exists without any 'shelf_t' appended to it - this means that the merch exists, but there's none of it currently in stock
    * A 'shelf_t' structs shouldn't be confused with the 'entry_t' structs that will be stored in the "storage hash table". 'shelf_t' structs are only meant to be used as a shelf identifier for a 'merch_t' struct. Thus a 'shelf_t' will NEVER be stored in any sort of hash table, only inside a linked list that's associated with a 'merch_t'


    * FREE 'shelf_t' with 'shelf_destroy'
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    *** merch_t {struct} ***

    A merch struct consists of four attributes:
        - name {*char}:  The name of the merchandise
        - description {*char}:  A short description of the merchandise
        - price {int}:  The price of the merchandise
        - location {linked list}:  A list of 'struct_t' keeping track of the shelves where the merch is currently stored and the amount

    * The 'merch_t' struct is meant to be stored in a hash table (merchandise_ht) as a value, with a key that's preferrably the name of the merch.
    * The 'merch_t' struct is NOT meant to be stored anywhere else then this hash table. To keep track of merchandise within a cart we use the 'cart_item' type. Putting a 'merch_t' in a cart will mess up the ownership of that struct when free'd.
    * The 'location' linked list of used to store 'shelf_t' structs, which keeps track of all the locations where the merch is currently stored. Destroying/freeing should also free the 'shelf_t' types within it. Therefore a 'shelf_t' should only be associated with a 'merch_t', since freeing a merch will free its 'shelf_t' structs.
    * The existence of a 'merch_t' within 'merchandise_ht' does not mean it's currently in stock. A merch can exist without any shelves appended to it.
    * Checking out a cart from the warehouse does not remove a 'merch_t' only the shelves appended to it if the stock runs out.


    * FREE 'merch_t' with 'merch_destroy' (will free 'shelf_t' within)
    * Use 'merch_decrease_stock' to REDUCE the stock of a particular merch (will free 'shelf_t' witin if reduced below 0)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    *** cart_t {struct} ***

    A cart struct consists of two attributes:
        - id {int}:  The number identifier for the cart
        - items {linked list}:  A list of all the items stored within the cart. These are of the type 'cart_item_t'

    * The cart ID is automatically selected when a cart is created through the UI. The selection of the ID uses the global variable 'Cart_counter' in the 'user_interface' module. The counter starts at 0 and is incremented for each new cart. When a cart is removed or checked out from the wareshouse, its ID can't be reused. This means that existing, removed and checked out carts will always have their own unique ID.
    * The items/merch witin a cart does NOT use the 'merch_t' type. 'merch_t' is only used to keep track of a merchansises attributes and shouldn't be stored in a cart. Carts use the 'cart_item_t' type.
    * Checking out or removing a cart will not remove any 'merch_t' type from the warehouse, only decrease the stock of that merch. If this causes the stock to go to 0, then the shelves where the merch is stored will be removed, but not the merch itself.
    * Adding merchandise to a cart will "reserve" the amount so that it can't be added to any other carts.
    Eg: 100 bananas is in stock, if 40 are added to cart #1, 60 added to cart #2 then none can be added to cart #3. The merch will still be listed as "in stock" until a cart is checked out, removed or the merch is removed from that cart.


    * FREE 'cart_t' with 'cart_destroy' (will free 'cart_item_t' witin)
    * Use 'cart_remove_merch' to properly REMOVE merch from a cart
    * Use 'cart_decrease_merch' to properly DECREASE merch from a cart (will remove if decreased to 0)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    *** cart_item_t {struct} ***

    A cart struct consists of two attributes:
        - name {char*}:  The name of the merch in the cart
        - amount {int}:  The amount of the merch stored in the cart

    * The 'cart_item_t' shouldn't be confused with 'merch_t'. 'merch_t' is used to keep track of a merchandises information within the warehouse, and is not associated with any carts. 'cart_item_t' is used to keep track of a carts inventory. This implementation avoids having different lists/hash tables taking ownership of the same struct and incorrectly freeing in one location while still being in use in another.


    * FREE 'cart_item_t' with 'cart_destoy_item'


__________________________________________________________________________________________________________________________________________________________________________________________




