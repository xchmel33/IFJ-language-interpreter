#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

FILE* source;

void procces_id_key_data(char S_Attribute[10], Token* token) {

    token->type = TT_KEYWORD;
    if (!strcmp(S_Attribute, "else")) token->attribute.keyword = ELSE;
    else if (!strcmp(S_Attribute, "for")) token->attribute.keyword = FOR;
    else if (!strcmp(S_Attribute, "func")) token->attribute.keyword = FUNC;
    else if (!strcmp(S_Attribute, "if")) token->attribute.keyword = IF;
    else if (!strcmp(S_Attribute, "package")) token->attribute.keyword = PACKAGE;
    else if (!strcmp(S_Attribute, "return"))  token->attribute.keyword = RETURN;
    else if (!strcmp(S_Attribute, "int")) {
        token->type = TT_DATATYPE;
        token->attribute.datatype = INT;
    }
    else if (!strcmp(S_Attribute, "float64")) {
        token->type = TT_DATATYPE;
        token->attribute.datatype = FLOAT64;
    }
    else if (!strcmp(S_Attribute, "string")) {
        token->type = TT_DATATYPE;
        token->attribute.datatype = STRING;
    }
    else {
        token->type = TT_IDENTIFIER;
        strcpy(token->attribute.string, S_Attribute);
    }
}

void procces_decimal(char S_Attribute[10], Token* token) {

    char* end;
    double value = strtod(S_Attribute, &end);
    if (*end) {
        return;
    }
    token->type = TT_DECIMAL;
    token->attribute.decimal = value;
}

void SetSource(FILE* f) {
    source = f;
}

int GetToken(Token* token) {

    token->type = TT_EMPTY;
    memset(token->attribute.string, 0, sizeof(token->attribute.string));
    ScannerState state = SS_START;
    char c = { 0 };
    char S_Attribute[100] = "";
    char strhex[2];

    while (state != SS_FINISHED) {
        c = getc(source);
        switch (state)
        {
        case(SS_START):
            strcpy(S_Attribute, "");
            if (c == '\n' || c == '\r') {
                token->type = TT_EOL;
                strcpy(token->attribute.string, "NULL");
                state = SS_FINISHED;
            }
            else if (isdigit(c)) {
                strncat(S_Attribute, &c, 1);
                state = SS_NUMBER;
            }
            else if (isspace(c)) {
                state = SS_START;
            }
            else if (isalpha(c) || c == '_') {
                strncat(S_Attribute, &c, 1);
                state = SS_ID_KEY_DATA;
            }
            else if (c == EOF) {
                token->type = TT_EOF;
                strcpy(token->attribute.string, "NULL");
                state = SS_FINISHED;
            }
            else if (c == ',') {
                token->type = TT_COMMA;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == ';') {
                token->type = TT_SEMICOLON;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '(') {
                token->type = TT_L_BRACKET;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == ')') {
                token->type = TT_R_BRACKET;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '{') {
                token->type = TT_BLOCK_BEGIN;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '}') {
                token->type = TT_BLOCK_END;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '+') {
                token->type = TT_ADD;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '-') {
                token->type = TT_SUB;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '*') {
                token->type = TT_MUL;
                token->attribute.string[0] = c;
                state = SS_FINISHED;
            }
            else if (c == '/') {
                state = SS_BACKSLASH;
            }
            else if (c == '<') {
                strncat(S_Attribute, &c, 1);
                state = SS_LESS_THAN;
            }
            else if (c == '>') {
                strncat(S_Attribute, &c, 1);
                state = SS_MORE_THAN;
            }
            else if (c == '=') {
                strncat(S_Attribute, &c, 1);
                state = SS_EQUAL;
            }
            else if (c == '!') {
                strncat(S_Attribute, &c, 1);
                state = SS_NOT;
            }
            else if (c == ':') {
                strncat(S_Attribute, &c, 1);
                state = SS_INIT;
            }
            else if (c == '"') {
                //strncat(S_Attribute, &c, 1);
                state = SS_STRING;
            }
            else {
                state = SS_ERROR;
            }
            break;
        case(SS_STRING):
            if (c == '"') {
                //strncat(S_Attribute, &c, 1);
                token->type = TT_STRING;
                strcpy(token->attribute.string, S_Attribute);
                state = SS_FINISHED;
            }
            else if (c < 32) {
                ungetc(c, source);
                state = SS_ERROR;
            }
            else if (c == '\\') {
                state = SS_ESCAPE_SEQUENCE;
            }
            else {
                strncat(S_Attribute, &c, 1);
            }
            break;
        case(SS_ESCAPE_SEQUENCE):
            if (c == 'n') {
                c = '\n';
                strncat(S_Attribute, &c, 1);
                state = SS_STRING;
            }
            else if (c == 't') {
                c = '\t';
                strncat(S_Attribute, &c, 1);
                state = SS_STRING;
            }
            else if (c == 'x') {
                state = SS_ESCAPE_SEQUENCE_HEX;
            }
            else if (c == '"') {
                c = '\"';
                strncat(S_Attribute, &c, 1);
                state = SS_STRING;
            }
            else if (c == '\\') {
                strncat(S_Attribute, &c, 1);
                state = SS_STRING;
            }
            break;
        case(SS_ESCAPE_SEQUENCE_HEX):
            if ((isdigit(c)) || (tolower(c) >= 97 && tolower(c) <= 102)) {
                strhex[0] = c;
                state = SS_ESCAPE_SEQUENCE_HEX_2;
            }
            else {
                state = SS_ERROR;
            }
            break;
        case(SS_ESCAPE_SEQUENCE_HEX_2):
            if ((isdigit(c)) || (tolower(c) >= 97 && tolower(c) <= 102)) {
                strhex[1] = c;
                c = (char)strtol(strhex, NULL, 16);
                strncat(S_Attribute, &c, 1);
                state = SS_STRING;
            }
            else {
                state = SS_ERROR;
            }
            break;
        case(SS_INIT):
            if (c == '=') {
                strncat(S_Attribute, &c, 1);
                token->type = TT_INIT;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            else {
                state = SS_ERROR;
            }
            break;
        case(SS_LESS_THAN):
            if (c == '=') {
                strncat(S_Attribute, &c, 1);
                token->type = TT_LESS_OR_EQUAL;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            else {
                ungetc(c, source);
                token->type = TT_LESS_THAN;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            break;
        case(SS_MORE_THAN):
            if (c == '=') {
                strncat(S_Attribute, &c, 1);
                token->type = TT_MORE_OR_EQUAL;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            else {
                ungetc(c, source);
                token->type = TT_MORE_THAN;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            break;
        case(SS_NOT):
            if (c == '=') {
                strncat(S_Attribute, &c, 1);
                token->type = TT_NOT_EQUAL;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            else {
                state = SS_ERROR;
            }
            break;
        case(SS_EQUAL):
            if (c == '=') {
                strncat(S_Attribute, &c, 1);
                token->type = TT_EQUAL;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            else {
                ungetc(c, source);
                token->type = TT_ASSIGN;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            break;
        case(SS_BACKSLASH):
            if (c == '/') {
                state = SS_LINE_COMMENTARY;
            }
            else if (c == '*') {
                state = SS_BLOCK_COMMENTARY;
            }
            else {
                ungetc(c, source);
                S_Attribute[0] = '/';
                token->type = TT_DIV;
                strcpy(token->attribute.string,S_Attribute);
                state = SS_FINISHED;
            }
            break;
        case(SS_LINE_COMMENTARY):
            if (c == '\n' || c == '\r') {
                token->type = TT_EOL;
                strcpy(token->attribute.string, "NULL");
                state = SS_FINISHED;
            }
            else if (c == EOF) {
                token->type = TT_EOF;
                strcpy(token->attribute.string, "NULL");
                state = SS_FINISHED;
            }
            break;
        case(SS_BLOCK_COMMENTARY):
            strcpy(S_Attribute, "");
            while (strcmp(S_Attribute, "*/") != 0) {
                if (c == '*' || c == '/') {
                    strncat(S_Attribute, &c, 1);
                }
                c = getc(source);
            }
            state = SS_START;
            break;
        case(SS_ID_KEY_DATA):
            if ((isalpha(c) || isdigit(c) || c == '_')) {
                strncat(S_Attribute, &c, 1);
            }
            else {
                ungetc(c, source);
                procces_id_key_data(S_Attribute, token);
                state = SS_FINISHED;
            }
            break;
        case(SS_NUMBER):
            if (isdigit(c)) {
                strncat(S_Attribute, &c, 1);
            }
            else if (c == '.') {
                strncat(S_Attribute, &c, 1);
                state = SS_NUMBER_DECIMAL;
            }
            else if (tolower(c) == 'e') {
                strncat(S_Attribute, &c, 1);
                state = SS_NUMBER_EXPONENT;
            }
            else {
                ungetc(c, source);
                token->type = TT_INTEGER;
                token->attribute.integer = atoi(S_Attribute);
                state = SS_FINISHED;
            }
            break;
        case(SS_NUMBER_DECIMAL):
            if (isdigit(c)) {
                strncat(S_Attribute, &c, 1);
            }
            else if (tolower(c) == 'e') {
                strncat(S_Attribute, &c, 1);
                state = SS_NUMBER_EXPONENT;
            }
            else {
                procces_decimal(S_Attribute, token);
                state = SS_FINISHED;
            }
            break;
        case(SS_NUMBER_EXPONENT):
            if (isdigit(c) || c == '+' || c == '-') {
                strncat(S_Attribute, &c, 1);
            }
            else {
                procces_decimal(S_Attribute, token);
                state = SS_FINISHED;
            }
            break;
        case(SS_ERROR):
            return 1;
            break;
    }
    }
    return token->type == TT_EMPTY ? -1 : 0;

}

void printToken(Token* token) {

    switch (token->type)
    {
    case(TT_EOL):
        printf("Token: EOL, %s\n", token->attribute.string);
        break;
    case(TT_EOF):
        printf("Token: EOF, %s\n", token->attribute.string);
        break;
    case(TT_IDENTIFIER):
        printf("Token: IDENTIFIER, %s\n", token->attribute.string);
        break;
    case(TT_KEYWORD):
        printf("Token: KEYWORD, ");
        switch (token->attribute.keyword)
        {
        case(ELSE):
            printf("else\n");
            break;
        case(FOR):
            printf("for\n");
            break;
        case(FUNC):
            printf("func\n");
            break;
        case(IF):
            printf("if\n");
            break;
        case(PACKAGE):
            printf("package\n");
            break;
        case(RETURN):
            printf("return\n");
            break;
        }
        break;
    case(TT_DATATYPE):
        printf("Token: DATATYPE, ");
        switch (token->attribute.datatype)
        {
        case(INT):
            printf("int\n");
            break;
        case(FLOAT64):
            printf("float64\n");
            break;
        case(STRING):
            printf("string\n");
            break;
        }
        break;
    case(TT_INTEGER):
        printf("Token: INTEGER, %d\n", token->attribute.integer);
        break;
    case(TT_DECIMAL):
        printf("Token: DECIMAL, %f\n", token->attribute.decimal);
        break;
    case(TT_STRING):
        printf("Token: STRING, %s\n", token->attribute.string);
        break;
    case(TT_EQUAL):
        printf("Token: EQUAL, %s\n", token->attribute.string);
        break;
    case(TT_NOT_EQUAL):
        printf("Token: NOT EQUAL, %s\n", token->attribute.string);
        break;
    case(TT_LESS_THAN):
        printf("Token: LESS THAN, %s\n", token->attribute.string);
        break;
    case(TT_LESS_OR_EQUAL):
        printf("Token: LESS OR EQUAL, %s\n", token->attribute.string);
        break;
    case(TT_MORE_THAN):
        printf("Token: MORE THAN, %s\n", token->attribute.string);
        break;
    case(TT_MORE_OR_EQUAL):
        printf("Token: MORE OR EQUAL, %s\n", token->attribute.string);
        break;
    case(TT_ASSIGN):
        printf("Token: ASSIGN, %s\n", token->attribute.string);
        break;
    case(TT_INIT):
        printf("Token: INIT, %s\n", token->attribute.string);
        break;
    case(TT_ADD):
        printf("Token: ADD, %s\n", token->attribute.string);
        break;
    case(TT_SUB):
        printf("Token: SUB, %s\n", token->attribute.string);
        break;
    case(TT_MUL):
        printf("Token: MUL, %s\n", token->attribute.string);
        break;
    case(TT_DIV):
        printf("Token: DIV, %s\n", token->attribute.string);
        break;
    case(TT_L_BRACKET):
        printf("Token: LEFT BRACKET, %s\n", token->attribute.string);
        break;
    case(TT_R_BRACKET):
        printf("Token: RIGHT BRACKET, %s\n", token->attribute.string);
        break;
    case(TT_BLOCK_BEGIN):
        printf("Token: BLOCK BEGIN, %s\n", token->attribute.string);
        break;
    case(TT_BLOCK_END):
        printf("Token: BLOCK END, %s\n", token->attribute.string);
        break;
    case(TT_COMMA):
        printf("Token: COMMA, %s\n", token->attribute.string);
        break;
    case(TT_SEMICOLON):
        printf("Token: SEMICOLON, %s\n", token->attribute.string);
        break;
    case(TT_TABLESYM):
        printf("Token: TABLESYM, %s\n", token->attribute.string);
        break;
    case(TT_EMPTY):
        printf("Token: EMPTY, %s\n", token->attribute.string);
        break;
    case(TT_EXPRESSION):
        printf("Token: EXPRESSION, %s\n", token->attribute.string);
        break;
    }
}