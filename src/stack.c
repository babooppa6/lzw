/*
stack.c
Contains implementation code for a stack data structure
by Geoffrey Litt
*/

#define _GNU_SOURCE
#include "stack.h"
#include "globals.h"

Stack stackCreate(void)
{
  Stack s;

  s = malloc(sizeof(struct stack *));
  *s = 0;

  return s;
}

void stackPush(Stack s, int kar)
{
  struct stack *new;

  new = malloc(sizeof(*new));

  new->next = *s;
  new->kar = kar;

  *s = new;
  return;
}

int stackPop(Stack s)
{
  int kar;
  struct stack *new;

  if (*s == 0){
    return -1;
  }
  else {
    kar = (*s)->kar;
  }

  new = (*s)->next;
  free(*s);
  *s = new;

  return kar;
}

int stackEmpty(Stack s)
{
  return (*s) == 0;
}

void stackDestroy(Stack s)
{
  while(!stackEmpty(s)){
    stackPop(s);
  }

  free(s);
}
