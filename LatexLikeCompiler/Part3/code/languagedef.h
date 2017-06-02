#ifndef LANGUAGEDEF_H
#define LANGUAGEDEF_H

#define PRODUCTION_BASE 16
enum{
    //产生式 16-28, 与token相区别
    Production1 = PRODUCTION_BASE,
    Production2,
    Production3,
    Production4,
    Production5,
    Production6,
    Production7,
    Production8,
    Production9,
    Production10,
    Production11,
    Production12,
    Production13
};

char *ProductionString[] = {
    "S-> $B$",              //1
    "B-> INT {B}{B}{B}B'",  //2 
    "B-> SUM {B}{B}{B}B'",  //3
    "B-> ID B'",            //4
    "B-> NUM B'",           //5
    "B-> BLANK B'",         //6
    "B-> (B)B'",            //7
    "B'-> BB'",             //8
    "B'-> _C",              //9
    "B'-> ^{B}B'",          //10
    "B'-> nil",             //11
    "C-> ^{B}{B}",          //12
    "C-> {B}"               //13
};

#endif
