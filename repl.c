#include <stdio.h>
#include "calc.h"

int main() {

  while (1) {
    printf("> ");

    char equation[255];

    fgets(equation, 255, stdin);

    printf("%g\n", evaluate(parse(lex(equation)).val.expr));
  }

  return 0;
}
