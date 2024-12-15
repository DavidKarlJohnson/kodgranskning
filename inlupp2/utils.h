#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

// -------------- Types START --------------
typedef union { 
    int int_value;
    float float_value;
    char *string_value;
    char char_value;
} answer_t;

typedef bool check_func(char* str);
typedef answer_t convert_func(char* str); 
// -------------- Types END --------------

extern char *strdup(const char *);
void clear_input_buffer();
int read_string(char buf[], int buf_siz);
bool is_number(char *str);
bool is_float(char *str);
answer_t make_float(char *str);
answer_t ask_question(char *question, check_func *check, convert_func *convert);
int ask_question_int(char *question);
double ask_question_float(char *question);
bool not_empty(char *str);
char *ask_question_string(char *question);
int string_length(char str[]);
void print(char *str);
void println(char str[]);
char *trim(char *str);
int generate_random_int(int max_numb);
char* capitalize(char* string);
int input_positive_int(char* message);
#endif 

    

     
/*
Typen answer_t
Typen check_func
Typen convert_func
Deklarationen extern char *strdup(const char *);
int read_string(char *buf, int buf_siz)
bool is_number(char *str)
bool is_float(char *str) och answer_t make_float(char *) (inte obligatoriska)
bool not_empty(char *str)
answer_t ask_question(char *question, check_func *check, convert_func *convert)
char *ask_question_string(char *question)
int ask_question_int(char *question)
double ask_question_float(char *question) (inte obligatorisk)
*/