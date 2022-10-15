#include <stdio.h>

int main()
{
  FILE *file = fopen("test_byte_mc.txt", "wb");
  fputc(0x00, file); // PUSH 5
  fputc(0x05, file);
  fputc(0x00, file); // PUSH 4
  fputc(0x04, file);
  fputc(0xC0, file); // ADD
  fputc(0x01, file);
  fputc(0x00, file); // PUSH 48
  fputc(0x30, file);
  fputc(0xC0, file); // ADD
  fputc(0x01, file);
  fputc(0xE0, file); // PUTC
  fputc(0x01, file);
  fclose(file);
}