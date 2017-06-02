#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokendef.h"

extern char id[20];
extern char num[20];

void LexicalError(void)
{
    printf("Lexical error: Unrecognized token\n");
	exit(EXIT_FAILURE);
}

int GetNextToken( FILE *stream )
{
	char ch;
	ch = fgetc(stream);
	if ( ch == EOF)
	    return -1;
	switch (ch){
		/*ignore blanks*/
		case ' ':
		case '\n':
		case '\t':
			return GetNextToken(stream);
			break;

		case '$':
			return DOLLAR;
			break;

		case '_':
			return SUB;
			break;
		case '^':
			return SUP;
			break;
		case '{':
			return LEFTBRACE;
			break;
		case '}':
			return RIGHTBRACE;
			break;
		case '(':
			return LEFTBRACKET;
			break;
		case ')':
			return RIGHTBRACKET;
			break;

		case '\\':
			/*Test \int \sum or \blank*/
			/*code here*/
			ch = fgetc(stream);

			if (ch == 'i')
			{
				char str[4];
				str[0] = ch;
				str[1] = fgetc(stream);
				str[2] = fgetc(stream);
				str[3] = '\0';

				if ( !strcmp(str,"int") )
					return INT;
				else
				{
					LexicalError();
				}
			}
			else if (ch == 's')
			{
				char str[4];
				str[0] = ch;
				str[1] = fgetc(stream);
				str[2] = fgetc(stream);
				str[3] = '\0';

				if ( !strcmp(str,"sum") )
					return SUM;
				else
				{
					LexicalError();
				}
			}
			else if (ch == 'b')
			{
				char str[6];
				str[0] = ch;
				str[1] = fgetc(stream);
				str[2] = fgetc(stream);
				str[3] = fgetc(stream);
				str[4] = fgetc(stream);
				str[5] = '\0';

				if ( !strcmp(str,"blank") )
					return BLANK;
				else
				{
					LexicalError();
				}
			}
			else
			{
				LexicalError();
			}

		default:
			/*Test NUM or ID*/
			if ( isdigit(ch) )
			{
				num[0] = ch;
				int count = 1;
				while ( (num[count] = fgetc(stream)) != EOF )
				{
					if ( isdigit(num[count]) )
						count++;
					else
					{
						fseek(stream, -1L, SEEK_CUR);
						break;
					}
				}
				num[count] = '\0';
				return NUM;
			}

			else if ( isalpha(ch) )
			{
				id[0] = ch;
				int count = 1;
				while ( (id[count] = fgetc(stream)) != EOF )
				{
					if ( isalnum(id[count]) )
						count++;
					else
					{
						fseek(stream, -1L, SEEK_CUR);
						break;
					}
				}
				id[count] = '\0';
				return ID;
			}

			else
			{
				LexicalError();
			}
		}
}
#endif
