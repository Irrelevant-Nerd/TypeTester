#include <stdio.h>
#include <stdlib.h>
#include <scan_input.h>
#include <string.h>

#define MAX_BUFFER 256*2
#define TOKENS_CAPACITY 256*2

typedef struct
{
  char* _tokens[TOKENS_CAPACITY];
  size_t _count; // how many elements are there in tokens
} TokenizeText;

void tokenize(char* buffer, char* string, TokenizeText* tokenize_text)
{
  size_t count = 0; // count the amount of token that is going to be stored in tokens

  strcpy(buffer, string); // making a copy of string inside the buffer
  char* token = strtok(buffer, " "); // whitespace is going to be the delimiter

  for(size_t i = 0; token != NULL && i < TOKENS_CAPACITY; i++)
  {
    tokenize_text->_tokens[i] = token;
    count++;
    token = strtok(NULL, " ");
  }

  tokenize_text->_count = count;
}

void print(TokenizeText* tokenize_text)
{
  for(size_t i = 0; i < tokenize_text->_count; i++)
  {
    if(*tokenize_text->_tokens[i] == '\0')
    {
      break;
    }
    printf("%s ", tokenize_text->_tokens[i]);
  }
}

int main(void)
{
  char buffer[MAX_BUFFER];

  TokenizeText user_tokenize_text;

  printf("START TYPING!!!\n");
  printf(">>> ");
  char* input_text = scan(buffer, sizeof(buffer));
  tokenize(buffer, input_text, &user_tokenize_text);

  print(&user_tokenize_text);

  /*
  char* text = "I love coffe so much";

  TokenizeText tokenize_text;

  tokenize(buffer, text, &tokenize_text);

  for(size_t i = 0; i < tokenize_text._count; i++)
  {
    if(*tokenize_text._tokens[i] == '\0')
    {
      break;
    }
    printf("%s\n", tokenize_text._tokens[i]);
  }
  */

  return 0;
}
