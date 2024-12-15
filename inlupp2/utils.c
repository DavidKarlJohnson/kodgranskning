#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"


extern char *strdup(const char *);




void clear_input_buffer()
{
    int c;
    do
    {
      c = getchar();  // Get the first character input from the console
    }
    while (c != '\n' && c != EOF);  // Keep getting the first character while it's not \n (end of user input).     
}


/*
Reads a string entered via the commandprompt, puts it 
input the parameter buffer and returns its length.
@param buf {char[]} - Char array where the input will be appended to.
@param buf_siz {int} - Max size of the buffer / Max length of the string
@return {int} - Number of characters entered by the user
*/
int read_string(char buf[], int buf_siz)
{   
  int i = 0;
  int c = getchar();
  while (c != '\n' && i < buf_siz - 1)
  { 
    buf[i] = c;
    i++;
    c = getchar();  // Get the first character input from the console
  }
  buf[i] = '\0';

  if(c != '\n' && c != EOF)  // Buffer overflow, clear input
  {
    clear_input_buffer();
  }

  return i;
}


/*
Check to see if the string argument is a number. Works with negative number.
@param str {string} - The string to check
@return bool - True if argument is a number
*/
bool is_number(char *str)
{
    for(int i = 0; i < strlen(str); i++)
    {
        if(!isdigit(str[i])) 
        {
            if(i == 0 && str[0] == '-')
            {
                if(strlen(str) == 1)  // If only '-'
                {
                    return false;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}


/*
Check to see if the string argument is a float. Works with negative float.
@param str {string} - The string to check
@return bool - True if argument is a float
*/
bool is_float(char *str)
{
    int floating_point_counter = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        if(!isdigit(str[i])) 
        {
            if(i == 0 && str[0] == '-')
            {
                if(strlen(str) == 1)  // If only '-'
                {
                    return false;
                }
                else
                {
                    continue;
                }
            }
            else if(str[i] == '.')
            {
                floating_point_counter++;
                if(floating_point_counter > 1)
                {
                  return false;
                }
            }
            else
            {
              return false;
            }
        }
        
    }
    if(floating_point_counter == 1) 
    {
      return true;
    }
    else
    {
      return false;
    }
}


/*
Creates answer_t type, set its float value to the argument and returns.
@param str {string} - The string representation of the desired float value.
@return a {answer_t} - A union type with its float value set.
*/
answer_t make_float(char *str)
{
    answer_t a;
    a.float_value = atof(str);
    return a;    
}


/*
Ask the user a question with a prompt. Check the user input with a desired function.
If check is ok, convert input with another desired function.
@param question {string} - The question to prompt the user
@param check {check_func} - A function that takes one string argument and returns a bool
@param convert {convert_func} - A function that takes one string argument and returns answer_t ,
                                which is a UNION of string, int, float.
@return buffer {answer_t} - The buffer which has been converted to the desired datatype that's
                            available in answer_t
*/
answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
  int buff_size = 255;
  char buffer[buff_size];
  do 
  {
      printf("%s", question);
      read_string(buffer, buff_size);
  }
  while(!check(buffer));

  return convert(buffer);
}
     

/*
Ask the user to input an integer with a prompt.
@param question {string} - The prompt to present to the user
@return result {int} - The integer entered by the user
*/
int ask_question_int(char *question)
{
  int result = 0;  // User input
  int successful_reads = 0;
  do 
    {
      printf("%s", question);
      successful_reads = scanf("%d", &result);  // Variable holds number of successful reads
      clear_input_buffer();
    }
  while (successful_reads < 1);  // Break if one successful read
  return result;
}


/*
Ask the user a question, whom will enter a float. A union type 'answer_t' 
will be created, with its float value set to the user input.
@param question {string} - The question to ask the user.
@return {float} - The float entered by the user.
*/
double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}


/*
Check if string length is larger than 0.
@param str {string}- The string to check.
@return {bool} - If string length is larger than 0.
*/
bool not_empty(char *str)
{
  return strlen(str) > 0;
}


/*
Ask the user a question, user enters a string. A union type 'answer_t' 
will be created, with its string value set to the user input.
@param question {string} - The question to ask the user.
@return {string} - The string entered by the user.
*/
char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}

/* OLD VERSION 
char *ask_question_string_OLD(char *question, char *buf, int buf_siz)
{
  int conversions = 0;
  do 
    {
      printf("%s\n", question);  // Ask for input, printing the string "question"
      conversions = read_string(buf, buf_siz);
    }
  while (conversions < 1);
  return buf;
}
*/


void print(char *str) 
{
  for( ; *str != '\0'; str++)
  {
    putchar(*str);
  }
}


void println(char str[])
{
    print(str);
    printf("\n");
}


/*
Trims the whitespaces at the beginning and end of a string. 
Whitespaced in the middle of a string are not trimmed.
@param str {string} - The string to be trimmed.
@return str {string} - The string without whitespaces
@example - " hello  "  returns "hello"
           "  he llo"  returns "he llo"
*/ 
char *trim(char *str)
{
  int start_index = 0;
  int end_index = 0;
  for(int i = 0; i < strlen(str); i++)
  {
    if(!isspace(str[i]))
    {
      start_index = i;
      break;
    }
  }
  for(int i = strlen(str) - 1; i >= 0; i--)
  {
    if(!isspace(str[i]))
    {
      end_index = i;
      break;
    }
  }

  int counter = 0;
  for( ; start_index <= end_index; start_index++)
  {
    str[counter] = str[start_index];
    counter++;
  }
  str[counter] = '\0';
  return str;
}


/*
Generates a number between 0 and max_numb - 1
@param max_numb {int} - Generate an int up until this number. This number is not included in the random number generation.
@return rand_num {int} - The random number generated
@example - generate_random_int(3), generates either the int 0, 1 or 2.
*/
int generate_random_int(int max_numb)
{
  srand(time(NULL));
  int rand_num = rand() % max_numb;
  return rand_num;
}


/// @brief Takes a string and capitalizes the first letter. Remaining letters will be unchanged
/// @param string The string to capitalize
/// @return The string with the fist letter capitalized
/// @example capitalize("hello") returns "Hello"
///          capitalize("strING") returns "StrING"
char* capitalize(char* string)
{
    char* new_string = strdup(string);
    new_string[0] = toupper(new_string[0]);
    free(string);
    return new_string;
}


/// @brief Puts the user in an input loop until an integer greater than 0 is entered
/// @param message The message to dislay to the user
/// @return An integer larger than 0, given by the user
int input_positive_int(char* message)
{
    int input = ask_question_int(message);
    
    while(input <= 0)
    {
        puts("Please enter a positive number!");
        input = ask_question_int(message);
    }
    return input;
}