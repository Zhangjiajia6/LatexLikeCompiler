#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include <errno.h>
#include "tokendef.h"
#include "languagedef.h"
#include "stack.h"
#include "LexicalAnalysis.h"

int GetNextToken(FILE *stream);
int Parser(FILE *stream, Stack *SyntaxStack);
void ParsingError(int token, Stack *SyntaxStack);

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
		if (token == -2)
		    return -1;
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
			break;
		case RIGHTBRACE:
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				return Production11;
			}
			else
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
			break;
		case RIGHTBRACKET:
			if (MatchedToken(SyntaxStack, B1))
			{
				Pop(SyntaxStack);
				return Production11;
			}
			else
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
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
			{
				ParsingError(token, SyntaxStack);
				return -1;
		    }
			break;
	}

}

void ParsingError(int token, Stack *SyntaxStack)
{
    fprintf(stderr, "Parsing Error!\n");
    fprintf(stderr, "  Error when meet token %s\n", TokenStr[token]);
    fprintf(stderr, "  Stack Status:\n");
    Traversal(SyntaxStack);
}

#endif
