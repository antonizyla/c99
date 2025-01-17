#include "parser.h"
#include "../ast/empty_block.h"
#include "../ast/function_def.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AstNode* parse() {
  return new_FunctionDefNode((char*) "main", new_EmptyBlockNode());
}

char* removeAllComments(char* str) {
  if (str == NULL) {
    return NULL;
  }
  int length = strlen(str);
  for (int i = 1; i < length; i++) {
    if (str[i - 1] == '/' && str[i] == '*') {
      int j = i;
      while (j != length && !(str[j - 1] == '*' && str[j] == '/')) {
        j++;
      }
      memset(str + i - 1, '`', j - i + 2);
      i = j;
    } else if (str[i - 1] == '/' && str[i] == '/') {
      int j = i;
      while (j != length && !(str[j] == '\n' || str[j] == '\0')) {
        j++;
      }
      memset(str + i - 1, '`', j - i + 1);
      i = j;
    }
  }
  int contigousIndex = 0;
  for (int i = 0; i < length; i++) {
    if (str[i] != '`' && str[i] != '\t') {
      str[contigousIndex] = str[i];
      contigousIndex++;
    }
  }
  str[contigousIndex] = '\0';
  str = (char*) realloc(str, contigousIndex + 1);
  return str;
}

char* readFileToString(char* filename) {
  if (filename == NULL) {
    return NULL;
  }
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("File cannot be opened or does not exist\n");
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char* string = (char*) malloc(fsize + 1);
  if (string == NULL) {
    printf("Error allocating memory\n");
    return NULL;
  }

  if (fread(string, fsize, 1, fp) == 0) {
    printf("Error reading file\n");
    return NULL;
  }

  fclose(fp);
  string[fsize] = 0;
  return string;
}
