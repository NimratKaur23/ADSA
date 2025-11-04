#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// Stack for operators (char stack)
char stack[MAX];
int top = -1;

void push(char c) {
    if (top == MAX - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack[++top] = c;
}

char pop() {
    if (top == -1) {
        return -1;
    }
    return stack[top--];
}

char peek() {
    if (top == -1) return -1;
    return stack[top];
}

int precedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

// Function to convert Infix -> Postfix
void infixToPostfix(char infix[], char postfix[]) {
    int i, k = 0;
    char symbol;
    for (i = 0; i < strlen(infix); i++) {
        symbol = infix[i];

        if (isalnum(symbol)) {   // Operand
            postfix[k++] = symbol;
        }
        else if (symbol == '(') {
            push(symbol);
        }
        else if (symbol == ')') {
            while (peek() != '(' && top != -1) {
                postfix[k++] = pop();
            }
            pop(); // Remove '('
        }
        else { // Operator
            while (precedence(peek()) >= precedence(symbol)) {
                postfix[k++] = pop();
            }
            push(symbol);
        }
    }
    // Pop remaining operators
    while (top != -1) {
        postfix[k++] = pop();
    }
    postfix[k] = '\0';
}

// Stack for integers (for postfix evaluation)
int intStack[MAX];
int intTop = -1;

void intPush(int val) {
    if (intTop == MAX - 1) {
        printf("Stack Overflow\n");
        return;
    }
    intStack[++intTop] = val;
}

int intPop() {
    if (intTop == -1) {
        printf("Stack Underflow\n");
        return -1;
    }
    return intStack[intTop--];
}

// Function to evaluate Postfix expression
int evaluatePostfix(char postfix[]) {
    int i, op1, op2, result;
    char symbol;

    for (i = 0; i < strlen(postfix); i++) {
        symbol = postfix[i];

        if (isdigit(symbol)) {
            intPush(symbol - '0');  // Convert char digit to int
        }
        else {
            op2 = intPop();
            op1 = intPop();
            switch (symbol) {
                case '+': result = op1 + op2; break;
                case '-': result = op1 - op2; break;
                case '*': result = op1 * op2; break;
                case '/': result = op1 / op2; break;
                case '^': result = 1;
                          for (int j = 0; j < op2; j++)
                              result *= op1;
                          break;
                default: result = 0;
            }
            intPush(result);
        }
    }
    return intPop();
}

// Driver function
int main() {
    char infix[MAX], postfix[MAX];
    int result;

    printf("Enter Infix Expression (only single-digit operands): ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);
    printf("Postfix Expression: %s\n", postfix);

    result = evaluatePostfix(postfix);
    printf("Evaluation Result: %d\n", result);

    return 0;
}
