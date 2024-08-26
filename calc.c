#include "calc.h"

int get_precedence(char c) {
    switch (c) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^':           return 3;
    }
    return 0;
}

TokenArr lex(char* str) {
    char num_buffer[16];
    char* crnt_digit = num_buffer;

    int prec_depth = 0;
    int length = 0;
    Token* tokens = NULL;

    do {
        switch (*str) {
            case '.':
            case '0'...'9': *crnt_digit++ = *str; break;
            case '(':       prec_depth += 3;      break;
            case ')':       prec_depth -= 3;      break;

            case '+': case '-': case  '*':
            case '/': case '^': case '\0':    
                *crnt_digit = '\0';
                tokens = realloc(tokens, sizeof(Token) * ++length);
                tokens[length - 1] = (Token){NUM, {.num = atof(num_buffer)}};
                crnt_digit = num_buffer;
        
                if (*str == '\0') break;

                tokens = realloc(tokens, sizeof(Token) * ++length);
                tokens[length - 1] = (Token){get_precedence(*str) + prec_depth, {*str}};
        }
    
        str++;
    } while (str[-1] != '\0');
  
    return (TokenArr){length, tokens};
}

NodeData parse(TokenArr token_arr) {
    int length = token_arr.length;
    Token* tokens = token_arr.tokens;

    if (length == 1) return (NodeData){VALUE, tokens[0].val.num};

    int low_prec = INT_MAX; // low precedence
    for (int i = 0; i < length; i++) {
        int crnt_prec = tokens[i].precedence;
        if (crnt_prec > 0 && crnt_prec < low_prec)
            low_prec = crnt_prec;
    }

    for (int j = length - 1; j >= 0; j--) {
        Token crnt = tokens[j]; // current
        if (crnt.precedence == 0 || crnt.precedence != low_prec) continue;
        
        Node* node = malloc(sizeof(Node));
        node->op = tokens[j].val.op;
        node->left = parse((TokenArr){j, tokens});
        node->right = parse((TokenArr){ length - j - 1, tokens + j + 1});

        return (NodeData){EXPRESSION, {.expr = node}};
    }

    return (NodeData){VALUE, 0};
}

double choose_type(Val data, NodeType type) {
    return type == EXPRESSION
        ? evaluate(data.expr)
        : data.num;
}

double evaluate(Node* tree) {
    double left  = choose_type(tree->left.val, tree->left.type),
           right = choose_type(tree->right.val, tree->right.type);

    switch (tree->op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return left / right;
        case '^': return pow(left, right);
    }

    return -1;
}
