#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define NUM 0

typedef enum { VALUE, EXPRESSION } NodeType;

typedef struct Node Node;

typedef union {
  double num;
  Node* expr;
} Val;

typedef struct {
  NodeType type;
  Val val;
} NodeData;

struct Node {
  char op;
  NodeData left, right;
};

typedef struct {
  int precedence; // also serves as type indication (non-zero values are ops)
  union {
    char op;
    double num;
  } val; 
} Token;

typedef struct {
  int length;
  Token* tokens;
} TokenArr;

double evaluate(Node* tree);
int get_precedence(char c);
TokenArr lex(char* str);
NodeData parse(TokenArr token_arr);
double choose_type(Val data, NodeType type);
double evaluate(Node* tree);
