/*
TASKS:
LAST COMPLETED: adding handlers for possible error handlers
NEXT: Create all error possibilities and handle them
IMPORTANT NOTES:
Should add floats: so let 'f' be acceptable in number literals
When adding values to one of the arrays, ensure to update the array size in their respective analyze functions
*/

/*
ANALYZE METHODS RETURN VALUES:
1 --> lexeme completed
0 --> match
-1 --> no match
-2 --> error
-3 --> single-line comment
-4 --> multiline comment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Header.h"

//ONLY DOING int, double, void, char
char *keywords[] =
{
	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"int",
	"long",
	"NULL",
	"register",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while"
};

char *seperators[] =
{
	"{",
	"}",
	",",
	"[",
	"]",
	"(",
	")",
	";"
};

char *operators[] =
{ //Should add modulus, and remember to update the array size in analyzeOperators************************
	"+",
	"-",
	"/",
	"*",
	"=",
	">",
	"<",
	"<=",
	">=",
	"==",
	"!=",
	"++",
	"--",
	"+=",
	"-=",
	"/=",
	"*=",
	"&&",
	"||"
};

char * comments[] =
{
	"//",
	"/*",
	"*/"
};

char *literalsymbols[] =
{
	".",
	"'",
	"\""
};

/*
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
*/

/*
typedef struct {
char tempcurrentlexemevalue[50];
char *currentlexemevalue;
int charcount;
int lexemevaluecompleted;
int iserror;
lexemetypes currentlexemetype;
} lexeme;
*/

//lexemetypes currentlexemetype;
FILE * fp;
//lexeme thislexeme;
lexeme tokens[100];

lexeme getNextToken();
int analyzeComment(char letter, lexeme *thislexeme, FILE * fp);
int analyzeIdentifier(char letter, lexeme *thislexeme, FILE *fp);
int analyzeKeyword(char letter, lexeme *thislexeme, FILE *fp);
int analyzeSeperator(char letter, lexeme *thislexeme, FILE *fp);
int analyzeOperator(char letter, lexeme *thislexeme, FILE *fp);
int analyzeLiteral(char letter, lexeme *thislexeme, FILE *fp);

void printtokens();
char *lexemetypetostring(lexemetypes type);

int firstcall = 0;

int main() {
	lexeme thislexeme;
	thislexeme.iserror = 0;
	while (thislexeme.iserror != 1) {

		thislexeme = getNextToken();

		if (thislexeme.iserror != 1) {
			char *value = thislexeme.currentlexemevalue;
			char *type = lexemetypetostring(thislexeme.currentlexemetype);
			printf("( %s , %s ) \n", type, value);
		}
	}

}

lexeme getNextToken() {

	///////////////////////////////////////////////////////////////////////////////
	//thislexeme.lexemevaluecompleted = 0;
	//thislexeme.currentlexemetype = NONE;
	if (firstcall == 0) {
		fp = fopen("C:\\Users\\Owner\\Documents\\Yousef\\Programming Projects Connected To Github\\Compiler\\LexerTests\\Test2.txt", "r");
		firstcall++;
	}

	if (fp == NULL) {
		printf("DERE WAS UN ERROR WHEN OPENING FILE");
		lexeme thislexeme;
		thislexeme.iserror = 1;
		return thislexeme;
	}
	char currentchar = 0;
	int i = 0;
	static int isSingleLineComment = 0;
	static int isMultiLineComment = 0;
	static int tokenfinished = 0;
	int error = 0;

	if (currentchar != EOF) {

		if (tokenfinished != -3 && tokenfinished != -4) {
			tokenfinished = 0;
		}

		lexeme thislexeme;
		thislexeme.lexemevaluecompleted = 0;
		thislexeme.iserror = 0;
		thislexeme.currentlexemetype = NONE;
		thislexeme.charcount = 0;


		if (currentchar != 0) {
			fseek(fp, -1, SEEK_CUR);
		}
		while ((currentchar = getc(fp)) != EOF && (tokenfinished != 1 && tokenfinished != -2)) {
			////DEBUG////
			//		char *type = lexemetypetostring(thislexeme.currentlexemetype);
			//		printf("%s", type);
			///
			if (isSingleLineComment == 1 && currentchar != '\n') {
				//isSingleLineComment = 0;
				continue;
			}
			else if (isSingleLineComment == 1) {
				isSingleLineComment = 0;
				continue;
			}

			if (isMultiLineComment == 1) {

				if ((tokenfinished = analyzeComment(currentchar, &thislexeme, fp)) == -4 && currentchar == '/') {
					isMultiLineComment = 0;
				}

				continue;

			}

			if (thislexeme.currentlexemetype == NONE || thislexeme.currentlexemetype == COMMENT) {

				tokenfinished = analyzeComment(currentchar, &thislexeme, fp);
				if (tokenfinished == -3) { //single-line comment
					tokenfinished = 1;
					isSingleLineComment = 1;
				}
				else if (tokenfinished == -4 && currentchar == '*') {
					tokenfinished = 1;
					isMultiLineComment = 1;
					continue;
				}
				//else if (tokenfinished == -4 && currentchar == '/') {
				//	thislexeme.currentlexemetype = ERROR;
				//	tokenfinished = 1;
				//}
				else if (tokenfinished == -2) {

				}

			}

			if (thislexeme.currentlexemetype == NONE || thislexeme.currentlexemetype == KEYWORD) {
				tokenfinished = analyzeKeyword(currentchar, &thislexeme, fp);
			}

			if (thislexeme.currentlexemetype == IDENTIFIER) {
				tokenfinished = analyzeIdentifier(currentchar, &thislexeme, fp);
			}

			if (thislexeme.currentlexemetype == NONE || thislexeme.currentlexemetype == SEPERATOR) {
				tokenfinished = analyzeSeperator(currentchar, &thislexeme, fp);
			}

			if (thislexeme.currentlexemetype == NONE || thislexeme.currentlexemetype == OPERATOR) {
				tokenfinished = analyzeOperator(currentchar, &thislexeme, fp);
			}

			if (thislexeme.currentlexemetype == NONE || thislexeme.currentlexemetype == LITERAL) {
				tokenfinished = analyzeLiteral(currentchar, &thislexeme, fp);
			}

			//I don't think we should have this!
			if (tokenfinished == -2) {
				lexeme thislexeme;
				thislexeme.iserror = 1;
				return thislexeme;
			}
		}


		//CHECK TO MAKE IT ERROR

		char *value = malloc(sizeof(char) * (thislexeme.charcount + 1));
		for (int j = 0; j != thislexeme.charcount; j++) {
			*(value + j) = thislexeme.tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme.charcount) = '\0';
		thislexeme.currentlexemevalue = value;

		//for unended char's and strings
		if (thislexeme.currentlexemetype == LITERAL) {
			if (*thislexeme.currentlexemevalue == '"' &&
				*(thislexeme.currentlexemevalue + thislexeme.charcount - 1) != '"') {
				thislexeme.currentlexemetype = ERROR;
			}
			else if (*thislexeme.currentlexemevalue == '\'' &&
				*(thislexeme.currentlexemevalue + thislexeme.charcount - 1) != '\'') {
				thislexeme.currentlexemetype = ERROR;
			}

		}

		if (thislexeme.iserror == 1) {
			thislexeme.currentlexemetype = ERROR;
		}
		// an identifier that starts with a number: 1aa

		////////////////////////////
		if (thislexeme.currentlexemetype != NONE) {
			return thislexeme;

			//what's going on with this future me???
			//char *value = thislexeme.currentlexemevalue;
			//char *type = lexemetypetostring(thislexeme.currentlexemetype);
			//printf("( %s , %s ) \n", type, value);
		}

	}

	fclose(fp);
	//printtokens();
	lexeme thislexeme;
	thislexeme.iserror = 1;
	return thislexeme;

}

int analyzeComment(char letter, lexeme *thislexeme, FILE * fp) {

	static int ismultiline = 0;

	if (letter == '/' && thislexeme->currentlexemetype == NONE && ismultiline != 1) {

		char nextchar = getc(fp);
		if (nextchar != '*' && nextchar != '/') {
			fseek(fp, -1, SEEK_CUR);
			return -1;
		}
		fseek(fp, -1, SEEK_CUR);

		thislexeme->currentlexemetype = COMMENT;
		thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
		return 0;
	}
	else if (letter == '*' && thislexeme->currentlexemetype == COMMENT
		&& thislexeme->tempcurrentlexemevalue[0] == '/') {

		thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

		char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
		for (int j = 0; j != thislexeme->charcount; j++) {
			*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme->charcount) = '\0';
		thislexeme->currentlexemevalue = value;

		ismultiline = 1;

		thislexeme->lexemevaluecompleted = 1;
		return -4;

	}


	else if (letter == '*' && ismultiline == 1 && thislexeme->currentlexemetype == NONE) {

		thislexeme->currentlexemetype = COMMENT;
		thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
		return 0;
	}

	else if (letter == '/' && thislexeme->currentlexemetype == COMMENT &&
		thislexeme->tempcurrentlexemevalue[0] == '/') {
		thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

		char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
		for (int j = 0; j != thislexeme->charcount; j++) {
			*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme->charcount) = '\0';
		thislexeme->currentlexemevalue = value;

		thislexeme->lexemevaluecompleted = 1;
		return -3;
	}

	else if (letter == '/' && thislexeme->currentlexemetype == COMMENT &&
		thislexeme->tempcurrentlexemevalue[0] == '*') {
		thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

		char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
		for (int j = 0; j != thislexeme->charcount; j++) {
			*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme->charcount) = '\0';
		thislexeme->currentlexemevalue = value;

		ismultiline = 0;
		thislexeme->lexemevaluecompleted = 1;
		return -4;
	}

	else {
		return -1;
	}


}

int analyzeKeyword(char letter, lexeme *thislexeme, FILE *fp) {

	if (thislexeme->currentlexemetype == NONE) {

		if (isalpha(letter) || letter == '_') {
			thislexeme->currentlexemetype = KEYWORD;
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}
		else {

			return -1;
		}

	}

	if (thislexeme->currentlexemetype == KEYWORD) {

		if (isalpha(letter) || letter == '_') {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

			return 0;
		}

		else if (letter == ' ') {
			thislexeme->currentlexemetype = IDENTIFIER;
			//thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;//remove this line; dont add the space!
			char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
			for (int j = 0; j != thislexeme->charcount; j++) {
				*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
			}
			*(value + thislexeme->charcount) = '\0';
			thislexeme->currentlexemevalue = value;

			for (int i = 0; i < 32; i++) {


				if (strcmp(thislexeme->currentlexemevalue, keywords[i]) == 0) {
					//Exception thrown at 0x66A0CA20 (ucrtbased.dll) in Compiler Phases Practice.exe: 0xC0000005 : Access violation reading location 0xCCCCCCCC.
					//that was after iteration 17 thought, which should not have happened

					thislexeme->currentlexemetype = KEYWORD;
					thislexeme->lexemevaluecompleted = 1;
					return 1;
				}
			}
			return -1;
		}

		else if (isdigit(letter)) {
			thislexeme->currentlexemetype = IDENTIFIER;

			return -1;
		}

		else {                //symbols

			char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
			for (int j = 0; j != thislexeme->charcount; j++) {
				*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
			}
			*(value + thislexeme->charcount) = '\0';
			thislexeme->currentlexemevalue = value;

			thislexeme->currentlexemetype = IDENTIFIER;

			for (int i = 0; i < 32; i++) {
				if (strcmp(thislexeme->currentlexemevalue, keywords[i]) == 0) {

					thislexeme->currentlexemetype = KEYWORD;
				}
			}

			thislexeme->lexemevaluecompleted = 1;

			return 1;
		}

	}

	return -2;

}

int analyzeIdentifier(char letter, lexeme *thislexeme, FILE *fp) {

	if (isalnum(letter) || letter == '_') { //alpha or numeric
		thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
		return 0;
	}

	else if (letter == ' ') {  //space

		char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
		for (int j = 0; j != thislexeme->charcount; j++) {
			*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme->charcount) = '\0';
		thislexeme->currentlexemevalue = value;

		thislexeme->lexemevaluecompleted = 1;
		return 1;
	}

	else { //symbol

		char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
		for (int j = 0; j != thislexeme->charcount; j++) {
			*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme->charcount) = '\0';
		thislexeme->currentlexemevalue = value;

		fseek(fp, -1, SEEK_CUR);

		thislexeme->lexemevaluecompleted = 1;
		return 1;
	}

}

int analyzeLiteral(char letter, lexeme *thislexeme, FILE *fp) {

	static int ischar = 0;
	static int isstring = 0;
	static int charcount = 0;
	//static int iserror = 0;

	if (thislexeme->currentlexemetype == NONE) {
		if (isdigit(letter)) {
			thislexeme->currentlexemetype = LITERAL;
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}

		else if (letter == '\'') {
			thislexeme->currentlexemetype = LITERAL;
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			ischar = 1;
			return 0;
		}

		else if (letter == '"') {
			thislexeme->currentlexemetype = LITERAL;
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			isstring = 1;
			return 0;
		}

		return -1;

	}

	if (thislexeme->currentlexemetype == LITERAL && ischar == 0 && isstring == 0) {

		//int and float first
		if (isdigit(letter)) {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}

		else if (letter == '.') {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}

		else if (isalpha(letter) || letter == '_') {
			thislexeme->iserror = 1;
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}

		char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
		for (int j = 0; j != thislexeme->charcount; j++) {
			*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
		}
		*(value + thislexeme->charcount) = '\0';
		thislexeme->currentlexemevalue = value;

		//fseek(fp, -1, SEEK_CUR);

		if (thislexeme->iserror == 0) {
			thislexeme->lexemevaluecompleted = 1;
			return 1;
		}
		else if (thislexeme->iserror == 1) {
			thislexeme->currentlexemetype = ERROR;
			thislexeme->lexemevaluecompleted = 1;
			return 1;
		}

	}

	//for char literals
	else if (thislexeme->currentlexemetype == LITERAL && ischar == 1 && isstring == 0) {

		if (letter != '\'' && thislexeme->charcount == 1) { //all letters
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}
		else if (letter == '\'' && thislexeme->charcount == 1) { // if the literal is ''
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

			char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
			for (int j = 0; j != thislexeme->charcount; j++) {
				*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
			}
			*(value + thislexeme->charcount) = '\0';
			thislexeme->currentlexemevalue = value;

			ischar = 0;
			thislexeme->lexemevaluecompleted = 1;
			return 1;
		}
		else if (thislexeme->charcount == 2 && *(thislexeme->tempcurrentlexemevalue + 1) == '\\') {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}
		else if (letter == '\'' && (thislexeme->charcount == 2 || thislexeme->charcount == 3)) {

			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

			char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
			for (int j = 0; j != thislexeme->charcount; j++) {
				*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
			}
			*(value + thislexeme->charcount) = '\0';
			thislexeme->currentlexemevalue = value;

			ischar = 0;
			thislexeme->lexemevaluecompleted = 1;
			return 1;
		}

		thislexeme->currentlexemetype = NONE;

		ischar = 0;
		return -2;

	}


	//for string literals
	else if (thislexeme->currentlexemetype == LITERAL && ischar == 0 && isstring == 1) {

		if (letter != '"') {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}

		else if (letter == '"' && *(thislexeme->tempcurrentlexemevalue + thislexeme->charcount - 1) == '\\') {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
			return 0;
		}

		else if (letter == '"') {
			thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

			char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
			for (int j = 0; j != thislexeme->charcount; j++) {
				*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
			}
			*(value + thislexeme->charcount) = '\0';
			thislexeme->currentlexemevalue = value;

			isstring = 0;
			thislexeme->lexemevaluecompleted = 1;
			return 1;
		}

		isstring = 0;
		return -2;

	}

	return -2;
}

int analyzeOperator(char letter, lexeme *thislexeme, FILE *fp) {

	if (thislexeme->currentlexemetype == NONE) {

		//iterate through list of operators
		for (int i = 0; i < 19; i++) {
			if (letter == operators[i][0]) {

				thislexeme->currentlexemetype = OPERATOR;
				thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;
				return 0;
			}
		}

		return -1;

	}

	if (thislexeme->currentlexemetype == OPERATOR) {

		//check if operator
		//iterate through list of operators


		//check if its a 1 'digit' operator, and if not, check if letter == operators[i][1] INSTEAD:
		for (int i = 0; i < 19; i++) {
			if (letter == operators[i][1]) {

				if (thislexeme->charcount < 2) { //OPTIMIZE::: move this outside of the for loop

					thislexeme->tempcurrentlexemevalue[thislexeme->charcount++] = letter;

					char *value = malloc(sizeof(char) * thislexeme->charcount + 1);
					for (int j = 0; j < thislexeme->charcount; j++) {
						*(value + j) = thislexeme->tempcurrentlexemevalue[j]; //+		value	0xcccccccc <Error reading characters of string.>	char *
					}
					*(value + thislexeme->charcount) = '\0';
					thislexeme->currentlexemevalue = value;

					thislexeme->lexemevaluecompleted = 1;
					return 1;



				}

				return -2;

			}
		}
		//Dont add the whole for-loop/value * thing, this is good
		thislexeme->lexemevaluecompleted = 1;
		return 1;

	}

	return -2;

}

int analyzeSeperator(char letter, lexeme *thislexeme, FILE *fp) {

	if (thislexeme->currentlexemetype == NONE) {

		//iterate through list of seperators
		for (int i = 0; i < 8; i++) {
			if (letter == seperators[i][0]) {
				thislexeme->currentlexemetype = SEPERATOR;
				thislexeme->tempcurrentlexemevalue[0] = letter;
				thislexeme->currentlexemevalue = malloc(sizeof(char) * 2);
				*(thislexeme->currentlexemevalue) = letter;
				*(thislexeme->currentlexemevalue + 1) = '\0';
				thislexeme->charcount++;
				thislexeme->lexemevaluecompleted = 1;
				return 1;
			}
		}

	}

	return -1;

}

void printtokens() {

	for (int i = 0; i < 100; i++) {
		char *value = tokens[i].tempcurrentlexemevalue;
		char *type = lexemetypetostring(tokens[i].currentlexemetype);
		printf("( %s , %s ) \n", type, value);
	}

}

char *lexemetypetostring(lexemetypes type) {

	if (type == NONE) {
		return "None";
	}
	else if (type == COMMENT) {
		return "Comment";
	}
	else if (type == KEYWORD) {
		return "Keyword";
	}
	else if (type == IDENTIFIER) {
		return "Identifier";
	}
	else if (type == LITERAL) {
		return "Literal";
	}
	else if (type == OPERATOR) {
		return "Operator";
	}
	else if (type == SEPERATOR) {
		return "Seperator";
	}
	else {
		return "Error";
	}

}