/*
stack.h
Contains function declarations for a stack data structure
by Geoffrey Litt
*/

// -----------------------------------------------------------------------------
// struct stack
// -----------------------------------------------------------------------------
// Fields:
//   int kar - the int stored by the stack node
//   struct stack *next - a pointer to the next node in the stack

typedef struct stack{
  int kar;
  struct stack *next;
} **Stack;

// -----------------------------------------------------------------------------
// Stack stackCreate
// -----------------------------------------------------------------------------
// Description:
//   This function returns an empty, initialized Stack.
// Return values:
//   Returns a struct stack** which points to a new Stack.

Stack stackCreate(void);

// -----------------------------------------------------------------------------
// void stackPush
// -----------------------------------------------------------------------------
// Description:
//   adds a node to the top of a stack containing an int kar
// Parameters:
//   Stack s - the Stack to push a new filename onto
//   int kar - the int that the new node should contain
// External state:
//   adds a node to Stack s

void stackPush(Stack s, int kar);

// -----------------------------------------------------------------------------
// int stackPop
// -----------------------------------------------------------------------------
// Description:
//   removes a node from the top of a stack and returns the stored int
// Parameters:
//   s - the Stack to pop a off of
// Return value:
//   the int stored by the popped off node
// External state references:
//   removes a node from Stack s

int stackPop(Stack s);

// -----------------------------------------------------------------------------
// int stackEmpty
// -----------------------------------------------------------------------------
// Description:
//   Tests if a given stack is empty (has no nodes)
// Parameters:
//   s - Stack to test for emptiness
// Return values:
//   1 - indicates stack is empty
//   0 - indicates stack is not empty

int stackEmpty(Stack s);

// -----------------------------------------------------------------------------
// void stackDestroy
// -----------------------------------------------------------------------------
// Description:
//   Destroys a stack and frees all associated memory
// Parameters:
//   s - Stack to destroy

void stackDestroy(Stack s);
