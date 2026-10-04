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

typedef LARGE_INTEGER lint;

double convert_to_sec(lint* frequency, lint* t1, lint* t2)
{
  return ((t2->QuadPart - t1->QuadPart) * 1000.0 / frequency->QuadPart) / 1000.0;
}

int get_raw_wpm(TokenizeText* tokenize_text, double seconds)
{
  // count = total words read
  return (tokenize_text->_count / seconds) * 60;
}

double get_accuracy(TokenizeText* ref_text, TokenizeText* user_text)
{
  int corrects = 0;
  int attempts = 0;

  for(size_t i = 0; i < ref_text->_count; i++)
  {
    if(strcmp(ref_text->_tokens[i], user_text->_tokens[i]) == 0)
    {
      corrects+=1;
    }
    attempts+=1;
  }
  return ( (double) corrects / (double) attempts) * 100;
}

int get_adjusted_wpm(int raw_wpm, double accuracy)
{
  return (int) raw_wpm * (accuracy / 100.0);
}

int main(void)
{
  char ref_buffer[MAX_BUFFER];
  char user_buffer[MAX_BUFFER];

  char* text = "Coffees are good, teas are mid, and water is essential for survival.";
  TokenizeText ref_tokenize_text;
  tokenize(ref_buffer, text, &ref_tokenize_text);

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
  char* user_text = scan(user_buffer, sizeof(user_buffer));
  TokenizeText user_tokenize_text;

  tokenize(user_buffer, user_text, &user_tokenize_text);

  // stops
  QueryPerformanceCounter(&t2);

  double seconds = convert_to_sec(&frequency, &t1, &t2);
  int raw_wpm = get_raw_wpm(&user_tokenize_text, seconds);
  double accuracy = get_accuracy(&ref_tokenize_text, &user_tokenize_text);
  int adjusted_wpm = get_adjusted_wpm(raw_wpm, accuracy);

  printf("WPM: %d\n", adjusted_wpm);
  printf("Accuracy: %.2f%%\n", accuracy);
  printf("Raw WPM: %d\n", raw_wpm);

  return 0;
}
