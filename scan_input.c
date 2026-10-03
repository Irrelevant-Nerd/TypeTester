#include <stdio.h>
#include <string.h>

char* scan(char *buf, size_t capacity)
{
  if (fgets(buf, capacity, stdin) == NULL) // if buf is NULL return NULL
    return NULL;

  char *p = strchr(buf, '\n'); // *p only detects '\n', if not then it is a NULL

  if (p)
  {
    *p = '\0'; // if newline found, strip it by assigning a null
  }
  else
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // EOF when there are no more characters
  }

  return buf;
}
