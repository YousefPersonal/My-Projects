#include <stdlib.h>
#include <stdio.h>

#include "Header.h"
/*
typedef enum {
	VARIABLE_DECLARATION_INITIALIZATION,
	VARIABLE_DECLARATION,
	VARIABLE_INITIALIZATION,
	VARIABLE_ASSIGNMENT,
	VARIABLE_IDENTIFIER_LIST,

	ARRAY_ASSIGNMENT,
	ARRAY_DECLARATION,
	ARRAY_IDENTIFIER_WITH_SIZE,
	ARRAY_IDENTIFIER,
	ARRAY_IDENTIFIER_LIST,
	ARRAY_IDENTIFIER_WITH_POINTERS,
	ARRAY_ELEMENT_LIST,
	ARRAY_DECLARATION_INITIALIZATION,
	ARRAY_INDEX_BRACKETS,
	ARRAY_INDEX_BRACKETS_OPTIONAL_SIZE,

	ARITHMETIC_EXPRESSION,
	LITERAL_ARITHMETIC_EXPRESSION,
	LITERAL,
	VALUE,
	ASSIGNMENT,
	INCREMENT,
	IDENTIFIER,
	CAST,



} syntaxclass;

typedef struct {

	int a;

} node;


void analyzeSyntax(lexeme token);

syntaxclass variable_declaration_intialization();






/*
//For terminals (put in an array)

if (token.value() == 1 | 2 | 4 ...) {
node.syntaxClass = digit;
break;
}

//Go over each:

for (int i = 0; i < array.size; i++ {
is
}
*/
