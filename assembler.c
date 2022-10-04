#include <stdio.h>

int main(int argc, char *argv[])
{
  // Incorrect number of arguments
  if (argc != 2)
  {
    printf("Usage: ./assembler [file]");
    return 1;
  }
}