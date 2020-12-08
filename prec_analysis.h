#ifndef IFJ_FINAL_PREC_ANALYSIS_H
#define IFJ_FINAL_PREC_ANALYSIS_H

#include "scanner.h"
#include "psa_stack.h"
#include "symtable.h"

void ActivateResources(HashTable* ptable);
void FreeResources();
int getIndex(Token* T);
Token* getSymbol(Token* A, Token* B);
Token* getNextToken(Token* prev_token, Token* actual_token);
Token* topTerm(Psa_stack* stack);
int checkTypes(Token* operand1, Token* operand2);
int checkRule(Psa_stack* Rulestack, Token *result_type);
Datatype getType(Token* token);
int expression(Token* prev_token, Token* act_token);
bool cg_stack_p(Token* token);

#endif //IFJ_FINAL_PARSER_H
