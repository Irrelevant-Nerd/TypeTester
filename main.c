#include <stdio.h>
#include <stdlib.h>
#include <scan_input.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define MAX_BUFFER 128
#define TOKENS_CAPACITY 64

typedef struct
{
  char** _tokens;
  size_t _capacity;
  size_t _count; // how many elements are there in tokens

} TokenizeText;

void tokenize(char* buffer, const char* string, TokenizeText* tokenize_text)
{
  strcpy(buffer, string); // making a copy of string inside the buffer
  char* token = strtok(buffer, " "); // whitespace is going to be the delimiter

  while(token != NULL)
  {
    // if count is equal or exceeds the capacity, execute this
    if(tokenize_text->_count >= tokenize_text->_capacity)
    {
      size_t new_capacity = tokenize_text->_capacity * 2;
      char** temp = realloc(tokenize_text->_tokens, new_capacity * sizeof(char*));

      if(temp == NULL)
      {
        return;
      }

      tokenize_text->_tokens = temp;

      tokenize_text->_capacity = new_capacity;
    }
    tokenize_text->_tokens[tokenize_text->_count] = token;
    tokenize_text->_count += 1;
    token = strtok(NULL, " ");
  }

  for(size_t i = 0; token != NULL && i < tokenize_text->_capacity; i++)
  {
    if(tokenize_text->_count > tokenize_text->_capacity)
    {
      tokenize_text->_capacity *= 2;
      tokenize_text->_tokens = realloc(tokenize_text->_tokens, sizeof(tokenize_text->_tokens) * tokenize_text->_capacity + sizeof(tokenize_text->_tokens));
    }
    tokenize_text->_tokens[i] = token;
    tokenize_text->_count += 1;
    token = strtok(NULL, " ");
  }
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
  return ( (double) corrects / (double) attempts) * 100; // converted to percent
}

int get_adjusted_wpm(int raw_wpm, double accuracy)
{
  return (int) raw_wpm * (accuracy / 100.0); // the accuracy is converted back to decimals
}

TokenizeText initialize_tokenize_text()
{
  TokenizeText tokenize_text;

  tokenize_text._capacity = TOKENS_CAPACITY;
  tokenize_text._count = 0;

  tokenize_text._tokens = malloc(tokenize_text._capacity * sizeof(char*));

  return tokenize_text;
}
int generate_random_index(size_t text_count)
{
  return rand() % text_count;
}
bool iswhitespace(char c)
{
  return c == ' ' || c == '\t';
}
bool is_quit(const char* s)
{
  // ignore any whitespace
  while(iswhitespace(*s))
  {
    s++;
  }

  if(*s == 'n' || *s == 'N')
  {
    s++;
  }

  // if char does not equal to any of these, return false
  else
  {
    return false;
  }

  while(iswhitespace(*s))
  {
    s++;
  }

  // if end of the string, return true
  return *s == '\0';
}
char* generate_random_text(char** texts, size_t text_count)
{
  char* text = texts[generate_random_index(text_count)];
  return text;
}
int main(void)
{
  srand((unsigned) time(NULL));
  char ref_buffer[MAX_BUFFER];
  char user_buffer[MAX_BUFFER];

  // we randomly pick a sentence within this array of texts
  char* texts[] =
  {
    "Coffees are good, teas are mid, and water is essential for survival.",
    "The quick brown fox jumps over the lazy dog near the river bank.",
    "Practice makes progress, and progress makes you faster over time.",
    "A small bug can hide in a large program for a very long time.",
    "Rain tapped on the window while the old clock kept ticking softly.",
    "Never trust a computer that you cannot throw out of a window.",
    "Learning to code is like learning to ride a bike, just with more errors.",
    "The library was quiet except for the sound of turning pages.",
    "Good code is simple, readable, and does exactly what it says.",
    "She packed her bag, grabbed her keys, and walked into the cold morning.",
    "Every expert was once a beginner who refused to quit.",
    "Pizza, pasta, and ice cream make a pretty great weekend menu.",
    "The train arrived late, but the view from the window was worth the wait.",
    "Memory leaks are quiet, but they eventually make the loudest crashes.",
    "Stars filled the sky as we sat around the fire telling old stories.",
    "Debugging is twice as hard as writing code, so write simple code.",
    "The market was busy with people buying fruit, bread, and fresh flowers.",
    "Typing fast is nice, but typing accurately is what really counts.",
    "A calm mind and a steady rhythm will beat panic every single time.",
    "The cat watched the rain, yawned once, and went back to sleep."
  };
  size_t text_count = sizeof(texts) / sizeof(texts[0]);
  char* text = generate_random_text(texts, text_count);
  TokenizeText ref_tokenize_text = initialize_tokenize_text();
  tokenize(ref_buffer, text, &ref_tokenize_text);

  lint frequency;
  lint t1, t2;  // ticks - tracking system time
                // t1 - start
                // t2 - stop

  QueryPerformanceFrequency(&frequency);
  while(true)
  {
    // start
    QueryPerformanceCounter(&t1);

    // do something
    printf("START TYPING!!!\n");
    printf("%s\n", text);
    printf(">>> ");
    char* user_text = scan(user_buffer, sizeof(user_buffer));
    TokenizeText user_tokenize_text = initialize_tokenize_text();

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

    printf("\nDo you want to start typing again? [y/n]: ");
    char* choice = scan(user_buffer, sizeof(user_buffer));
    if(is_quit(choice))
    {
      break;
    }
    text = generate_random_text(texts, text_count);
  }
  return 0;
}
