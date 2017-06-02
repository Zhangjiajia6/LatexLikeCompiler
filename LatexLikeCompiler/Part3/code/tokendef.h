#ifndef TOKENDEF_H
#define TOKENDEF_H

enum{
    //终结符号 0-11
    DOLLAR,
	SUB,
	SUP,
	LEFTBRACE,
	RIGHTBRACE,
	LEFTBRACKET,
	RIGHTBRACKET,
	INT,
	SUM,
	ID,
	NUM,
	BLANK,
	
	//非终结符号 12-15
	S,
	B,
	B1,
	C
};
char *TokenStr[] = {
    "DOLLAR",
	"SUB",
	"SUP",
	"LEFTBRACE",
	"RIGHTBRACE",
	"LEFTBRACKET",
	"RIGHTBRACKET",
	"INT",
	"SUM",
	"ID",
	"NUM",
	"BLANK",
	"S",
	"B",
	"B'",
	"C"
};
char *TokenChar[] = {
    "$",
    "-",
    "^",
    "{",
    "}",
    "(",
    ")",
    "INT",
    "SUM",
    "ID",
    "NUM",
    "BLANK",
    "S",
    "B",
    "B'",
    "C"
};
#endif
