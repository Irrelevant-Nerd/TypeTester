#include <stdio.h>
#include <stdlib.h>
#include <scan_input.h>
#include <string.h>

#include <windows.h>

#define MAX_BUFFER 128
#define TOKENS_CAPACITY 64

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
  printf("\n");
}


typedef LARGE_INTEGER lint;

double convert_to_sec(lint* frequency, lint* t1, lint* t2)
{
  return ((t2->QuadPart - t1->QuadPart) * 1000.0 / frequency->QuadPart) / 1000.0;
}

int calculate_wpm(TokenizeText* tokenize_text, double seconds)
{
  // count = total words read
  return (tokenize_text->_count / seconds) * 60;
}

int main(void)
{
  char buffer[MAX_BUFFER];

  char* text = "We use clocks everyday to read time.\nAn analog clock is an instrument or tool used to measure time in which the hours, minutes, and seconds are indicated by hands on a dial.\nThe second hand moves around the fastest and shows the number of seconds passed in the current minute. ";

  lint frequency;
  lint t1, t2;  // ticks - tracking system time
                         // t1 - start
                         // t2 - stop

  QueryPerformanceFrequency(&frequency);

  // start
  QueryPerformanceCounter(&t1);

  // do something
  printf("START TYPING!!!\n");
  printf("%s\n", text);
  printf(">>> ");
  char* input_text = scan(buffer, sizeof(buffer));
  TokenizeText user_tokenize_text;
  tokenize(buffer, input_text, &user_tokenize_text);

  print(&user_tokenize_text);

  // stop
  QueryPerformanceCounter(&t2);

  double seconds = convert_to_sec(&frequency, &t1, &t2);
  printf("%.2f sec\n", seconds);
  printf("WPM: %d\n", calculate_wpm(&user_tokenize_text, seconds));

  return 0;
}
