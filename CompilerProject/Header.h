#pragma once

typedef enum {
	IDENTIFIER,
	KEYWORD,
	SEPERATOR,
	OPERATOR,
	COMMENT,
	LITERAL,
	NONE,
	ERROR
} lexemetypes;

typedef struct {
	char tempcurrentlexemevalue[50];
	char *currentlexemevalue;
	int charcount;
	int lexemevaluecompleted;
	int iserror;
	lexemetypes currentlexemetype;
} lexeme;