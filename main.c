/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

/* Global declarations */ /* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token = 21;
int nextToken = 21;
FILE *in_fp, *fopen();
/* Function declarations */ void addChar();
void getChar();
void getNonBlank();
int lex();
int checkError();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26


/******************************************************/
/* main driver */
int main() {
  /* Open the input data file and process its contents */
  if ((in_fp = fopen("front.in", "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
  else {
    getChar();
    do {
      lex();
    } while (nextToken != EOF);
  }
}


/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
token */
int lookup(char ch) {
  switch (ch) {
    case '+':
      addChar();
      token = nextToken;
      nextToken = ADD_OP;
      break;
    case '-':
      addChar();
      token = nextToken;
      nextToken = SUB_OP;
      break;
    case '*':
      addChar();
      token = nextToken;
      nextToken = MULT_OP;
      break;
    case '/':
      addChar();
      token = nextToken;
      nextToken = DIV_OP;
      break;
    default:
      addChar();
      token = nextToken;
      nextToken = EOF;
    break;
  }
  return nextToken;
}


/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
    printf("Error - lexeme is too long \n");
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
character class */
void getChar() {
  if ((nextChar = getc(in_fp)) != EOF) {
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else charClass = UNKNOWN;
  }
  else
    charClass = EOF;
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
  while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* Check if an operand was placed next to an operand or if an operator was
 placed next to an operator*/
int checkError(){
  int isTokenOpperand = 1;
  int isNextTokenOpperand = 1;
  if(token > 20 && token < 25){
    isTokenOpperand = 0;
  }
  if(nextToken > 20 && nextToken < 25){
    isNextTokenOpperand = 0;
  }
  if(isNextTokenOpperand == isTokenOpperand){
    return 1;
  }
  return 0;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
  lexLen = 0;
  getNonBlank();
  switch (charClass) {
    /* Parse identifiers */
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT) {
        addChar();
        getChar();
      }
      token = nextToken;
      nextToken = IDENT;
      break;
    /* Parse integer literals */
    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
        addChar();
        getChar();
      }
      token = nextToken;
      nextToken = INT_LIT;
      break;
    /* Parentheses and operators */
    case UNKNOWN:
      lookup(nextChar);
      getChar();
      break;
    /* EOF */
    case EOF:
      nextToken = EOF;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      break;
  } /* End of switch */
  if(checkError()){
    printf("Error\n");
  }
  printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
  return nextToken;
}  /* End of function lex */
