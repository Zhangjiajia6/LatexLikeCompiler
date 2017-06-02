/* *********************************************
 *
 * Syntax Analysis for the Latex-like Compiler
 * Time:2017/4/3
 * Author: Zhang Jiajia
 *
 * ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include <errno.h>
#include "tokendef.h"
#include "languagedef.h"
#include "stack.h"
#include "LexicalAnalysis.h"

char id[20];
char num[20];

int GetNextToken(FILE *stream);
int Parser(FILE *stream, Stack *SyntaxStack);
void PrintUsg(char *procname);
void ParsingError(int token, Stack *SyntaxStack);

int main(int argc, char **argv)
{	
	int result;

	if ( argc != 2 )
	{
		PrintUsg( *argv );
		exit(EXIT_FAILURE);
	}

	FILE *fp;
	char *filename = *(argv+1);
	if ((fp=fopen(filename, "r")) == NULL)
	{
		printf("Open File %s Error!\n", filename);
		exit(EXIT_FAILURE);
	}


    Stack SyntaxStack;
	InitStack(&SyntaxStack);
	
	/*Start to get token*/
	while (1)
	{
		result = Parser(fp, &SyntaxStack);
		if (result == -1)
		{
		    printf("Parsing Done\n");
		    break;
		}
		else if (result >= Production1 && result <= Production13)
			printf("-Using Production:%s\n", ProductionString[result - PRODUCTION_BASE]);
		else if (result >= DOLLAR && result <= BLANK)
		{
		    if (result == NUM)
			    printf("    Match Token %s, Value: %d\n", TokenStr[result], atoi(num));
			else if (result == ID)
			    printf("    Match Token %s, String: %s\n", TokenStr[result], id);
			else
			    printf("    Match Token %s, Symbol: %s\n", TokenStr[result], TokenChar[result]);
		}
	}

	return 0;
}

void PrintUsg( char *procname )
{
	printf("Usg:\n");
	printf("%s FileName\n", procname);
}

int MatchedToken(Stack *stack, int token)
{
	if (getTopElem(stack) == token)
		return 1;
	else
		return 0;
}

int Parser(FILE *stream, Stack *SyntaxStack)
{
			
	static int matched = 1;
	static int token;
	static int start = 0;
	if (start == 0 && IsEmpty(SyntaxStack))
	{
		Push(SyntaxStack, S);
		start = 1;
	}	
	if (start != 0 && IsEmpty(SyntaxStack))
	{
	    return -1;
	}

	if (matched == 1)
	{
		token = GetNextToken(stream);
		matched = 0;
	}
	
	//直接和栈顶匹配
	if (MatchedToken(SyntaxStack, token))
	{
		matched =1;
		Pop(SyntaxStack);
		return token;
	}

	//不和栈顶匹配
	switch (token){
		case DOLLAR:
			if (MatchedToken(SyntaxStack, S))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, DOLLAR);
				Push(SyntaxStack, B);
				Push(SyntaxStack, DOLLAR);

				return Production1;
			}
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				return Production11;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case SUB:
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, C);
				Push(SyntaxStack, SUB);
				return Production9;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case SUP:
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, SUP);
				return Production10;
			}
			else if (MatchedToken(SyntaxStack, C))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, SUP);
				return Production12;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case LEFTBRACE:
			if (MatchedToken(SyntaxStack, C))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				return Production13;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case RIGHTBRACE:
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				return Production11;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case LEFTBRACKET:
			if (MatchedToken(SyntaxStack, B))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, RIGHTBRACKET);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACKET);
				return Production7;
			}
			else if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, B);
				return Production8;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case RIGHTBRACKET:
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				return Production11;
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case INT:
			if (MatchedToken(SyntaxStack, B))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, INT);
				return Production2;
			}
			else if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				if (!IsEmpty(SyntaxStack) && MatchedToken(SyntaxStack, B))
					return Production11;
				else
				{
					Push(SyntaxStack, B1);
					Push(SyntaxStack, B);
					return Production8;
				}
			}
			else
				ParsingError(token, SyntaxStack);

			break;
		case SUM:
			if (MatchedToken(SyntaxStack, B))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, RIGHTBRACE);
				Push(SyntaxStack, B);
				Push(SyntaxStack, LEFTBRACE);
				Push(SyntaxStack, SUM);
				return Production3;
			}
			else if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				if (!IsEmpty(SyntaxStack) && MatchedToken(SyntaxStack, B))
					return Production11;
				else
				{
					Push(SyntaxStack, B1);
					Push(SyntaxStack, B);
					return Production8;
				}
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case ID:
			if (MatchedToken(SyntaxStack, B))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, ID);
				return Production4;
			}
			else if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				if (!IsEmpty(SyntaxStack) && MatchedToken(SyntaxStack, B))
					return Production11;
				else
				{
					Push(SyntaxStack, B1);
					Push(SyntaxStack, B);
					return Production8;
				}
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case NUM:
			if (MatchedToken(SyntaxStack, B))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, NUM);
				return Production5;
			}
			else if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				if (!IsEmpty(SyntaxStack) && MatchedToken(SyntaxStack, B))
					return Production11;
				else
				{
					Push(SyntaxStack, B1);
					Push(SyntaxStack, B);
					return Production8;
				}
			}
			else
				ParsingError(token, SyntaxStack);
			break;
		case BLANK:
			if (MatchedToken(SyntaxStack, B))
			{
				Pop(SyntaxStack);
				Push(SyntaxStack, B1);
				Push(SyntaxStack, BLANK);
				return Production6;
			}
			else if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				if (!IsEmpty(SyntaxStack) && MatchedToken(SyntaxStack, B))
					return Production11;
				else
				{
					Push(SyntaxStack, B1);
					Push(SyntaxStack, B);
					return Production8;
				}
			}
			else
				ParsingError(token, SyntaxStack);
			break;
	}

}

void ParsingError(int token, Stack *SyntaxStack)
{
    printf("Error when meet token %s\n", TokenStr[token]);
    printf("Stack Status:\n");
    Traversal(SyntaxStack);
    exit(EXIT_FAILURE);
}
