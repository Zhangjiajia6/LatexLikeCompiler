/* *********************************************
 *
 * Syntax-Directed Tranlation for the Latex-like Compiler
 * Time:2017/5/5
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
#include "SyntaxAnalysis.h"

char id[20];
char num[20];

int GetNextToken(FILE *stream);
int Parser(FILE *stream, Stack *SyntaxStack);
void PrintUsg(char *procname);
int isProduction(int type);
int isToken(int type);
void writeHtmlHead(FILE *stream);
int writeHtmlBody(FILE *instream, FILE *outstream, Stack *SyntaxStack, int top, int left, int fontsize);
void writeHtmlTail(FILE *stream);
void Translator(FILE *instream, FILE *outstream);

int main(int argc, char **argv)
{	
	if ( argc != 2 )
	{
		PrintUsg( *argv );
		exit(EXIT_FAILURE);
	}

	FILE *infp;
	FILE *outfp;
	int len;
	char *infilename = *(argv+1);
	char *outfilename;
	len = strlen(infilename);
	outfilename = (char *)malloc(len + 2);
	strncpy(outfilename, infilename, len);
	strncpy(outfilename + len - 3, "html\0", 5);

	if ((infp=fopen(infilename, "r")) == NULL)
	{
		printf("Open File %s Error!\n", infilename);
		exit(EXIT_FAILURE);
	}
	if ((outfp=fopen(outfilename, "w")) == NULL)
	{
		printf("Open File %s Error!\n", outfilename);
		exit(EXIT_FAILURE);
	}

	Translator(infp, outfp);
	
    fclose(infp);
    fclose(outfp);

	return 0;
}

void PrintUsg( char *procname )
{
	printf("Usg:\n");
	printf("%s InputFileName\n", procname);
}

void Translator(FILE *instream, FILE *outstream)
{
	Stack SyntaxStack;
	InitStack(&SyntaxStack);
	
	int TOP = 175;
	int LEFT = 500;
	int FONTSIZE = 50;
	
	writeHtmlHead(outstream);
	writeHtmlBody(instream, outstream, &SyntaxStack, TOP, LEFT, FONTSIZE);
	writeHtmlTail(outstream);

}

int isProduction(int type)
{
    return type >= Production1 && type <= Production13;
}

int isToken(int type)
{
    return type >= DOLLAR && type <= BLANK;
}

void writeHtmlHead(FILE *stream)
{
	fprintf(stream, "<html>\n<head>\n<META content=\"text/html; charset=gb2312\">\n</head>\n<body>\n");
	return;
}

void writeHtmlTail(FILE *stream)
{
	fprintf(stream, "\n</body>\n</html>");
	return;
}

int writeHtmlBody(FILE *instream, FILE *outstream, Stack *SyntaxStack, int top, int left, int fontsize)
{

	int type;
	int width = 0;
	int sub_fontsize, sub_top, sub_left, sub_width;
	int sup_fontsize, sup_top, sup_left, sup_width;

	type = Parser(instream, SyntaxStack);

    if (isProduction(type))
	{
		switch (type){
			case Production1:
				Parser(instream, SyntaxStack);
				writeHtmlBody(instream, outstream, SyntaxStack, top, left, fontsize);
				Parser(instream, SyntaxStack);
				break;
				
			case Production2:
				
			case Production3:
			    //INT or SUM
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left, fontsize);
				
				//{
				Parser(instream, SyntaxStack);
				//B1, 下标
				sub_fontsize = (int)(fontsize*0.6);
				sub_top = top + (int)(fontsize*1.2) ;
				sub_left = left + width;
				sub_width = writeHtmlBody(instream, outstream, SyntaxStack, sub_top, sub_left, sub_fontsize);
				//}
				Parser(instream, SyntaxStack);
				
				//{
				Parser(instream, SyntaxStack);
				//B2， 上标
				sup_fontsize = (int)(fontsize*0.6);
				sup_top = top - (int)(sup_fontsize*1.2);
				sup_left = left + width;
				sup_width = writeHtmlBody(instream, outstream, SyntaxStack, sup_top, sup_left, sup_fontsize);
				//}
				Parser(instream, SyntaxStack);
				
				if (sub_width >= sup_width)
				    width += sub_width;
				else
				    width += sup_width;
				
				//{
				Parser(instream, SyntaxStack);
				//B3, 中间
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				//}
				Parser(instream, SyntaxStack);
				//B'
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				return width;

			case Production4:

			case Production5:

			case Production6:
			    //ID or NUM or BLANK
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left, fontsize);
				//B'
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				return width;

			case Production7:
			    //(
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left, fontsize);
				//B1
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				//), 右括号过于靠近，需要略微调整
				width += (int)(fontsize*0.3);
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				//B'
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				return width;

			case Production8:
			    //B
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left, fontsize);
				//B'1
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				return width;

			case Production9:
			    //_
			    sub_fontsize = (int)(fontsize*0.6);
				sub_top = top + (int)(fontsize*0.6);
				sub_left = left;
				Parser(instream, SyntaxStack);
				//C
				width += writeHtmlBody(instream, outstream, SyntaxStack,sub_top, sub_left, sub_fontsize);
				return width;			    

			case Production10:
			    //^
			    sup_fontsize = (int)(fontsize*0.6);
				sup_top = top - (int)(fontsize*0.3);
				sup_left = left;
				
				Parser(instream, SyntaxStack);
				//{
				Parser(instream, SyntaxStack);
				//B
				width += writeHtmlBody(instream, outstream, SyntaxStack, sup_top, sup_left, sup_fontsize);
				//}
				Parser(instream, SyntaxStack);
				//B'
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left + width, fontsize);
				return width;

			case Production11:
			    width = 0;
			    return width; 

			case Production12:			    
			    sub_fontsize = fontsize;
				sub_top = top;
				sub_left = left;

			    sup_left = left;
			    sup_top = top - (int)(0.3*fontsize) - (int)(fontsize*1.3);
			    sup_fontsize = fontsize;
			    
			    //^
				Parser(instream, SyntaxStack);
				//{
				Parser(instream, SyntaxStack);
				//B, 下标
				sub_width = writeHtmlBody(instream, outstream, SyntaxStack, sub_top, sub_left, sub_fontsize);
				//}
				Parser(instream, SyntaxStack);
				//{
				Parser(instream, SyntaxStack);
				//B, 上标
				sup_width = writeHtmlBody(instream, outstream, SyntaxStack, sup_top, sup_left, sup_fontsize);
				//}
				Parser(instream, SyntaxStack);				
				
				if (sub_width >= sup_width)
				    return sub_width;
				else
				    return sup_width;

			case Production13:
			    //{
				Parser(instream, SyntaxStack);
				//B
				width += writeHtmlBody(instream, outstream, SyntaxStack, top, left, fontsize);
				//}
				Parser(instream, SyntaxStack);
				return width;

		}
	}
	else if (isToken(type))
	{
		switch (type){
			case LEFTBRACKET:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:100%;\">(</span></div>\n", top, left, fontsize);
				width = (int)(fontsize*0.7);
				return width;
			case RIGHTBRACKET:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:100%;\">)</span></div>\n", top, left, fontsize);
				width = (int)(fontsize*0.7);
				return width;
			case INT:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:100%;\">&int;</span></div>\n", top - (int)(0.3*fontsize), left, (int)(fontsize*1.6));
				width = (int)(fontsize*1.2);
				return width;
			case SUM:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:100%;\">&sum;</span></div>\n", top - (int)(0.3*fontsize), left, (int)(fontsize*1.6));
				width = (int)(fontsize*1.2);
				return width;
			case ID:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:oblique;line-height:100%;\">%s</span></div>\n", top, left, fontsize, id);
				width = (int)(fontsize*strlen(id)*0.55);
				return width;
			case NUM:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:100%;\">%s</span></div>\n", top, left, fontsize, num);
				width = (int)(fontsize*strlen(num)*0.55);
				return width;
			case BLANK:
				fprintf(outstream, "<div style=\"position: absolute; top:%dpx; left:%dpx;\"><span style=\"font-size:%dpx; font-style:normal;line-height:100%;\">%s</span></div>\n", top, left, fontsize, " ");
				width = fontsize*0.7;
				return width;
		}
	}
	
	else
	{
	    writeHtmlTail(outstream);
	    exit(EXIT_FAILURE);
	}
}
