#ifndef STACK_H
#define STACK_H
/********************************************************************** 
** 
**--------------------------file infomation---------------------------- 
**file name:stack.h 
** 
**last modified date:2010-7-30 
** 
**descriptioin:对于栈的各种操作------any operation of the stack 
** 
**created by:Ethan Wei 
***********************************************************************/  
  
/********************************************************************** 
头文件------head file 
**********************************************************************/  
#include <stdio.h>  
#include <stdlib.h>  
#include "tokendef.h"
#include "languagedef.h"
  
/********************************************************************** 
宏定义------macro definition 
**********************************************************************/  
#define Type int  
#define INITIAL_SIZE 30                //栈的初始化大小---initial size of the stack  
#define INCREASE_SIZE 20                //栈大小的增量-----increase size of the stack  
#define ROW 10                //打印栈中元素的每行个数---the number of row    
/************************************************************************* 
结构体的定义-----------struct definition 
*************************************************************************/  
typedef struct stack  
{  
    Type *data;  
    int top;                            //栈顶指针----top stack point  
    int StackSize;                      //栈的大小----size of the stack  
}Stack;  
  
/************************************************************************* 
函数定义--------function definition 
*************************************************************************/  
  
/************************************************************************* 
函数名称：InitStack 
函数表述: 对栈进行初始化 
输入值：  Stack * 
返回值：  初始化成功返回0，否则退出 
*************************************************************************/  
int InitStack(Stack *stack)  
{  
    stack->data = (Type *)malloc(INITIAL_SIZE*sizeof(Type)); //为元素开辟空间--alloc the space for element  
    if(stack->data == NULL)  
    {  
        fprintf(stderr,"开辟空间失败");  
        exit(1);  
    }  
    stack->top  = -1;                                        //栈顶指针为-1--set top point -1  
    stack->StackSize = INITIAL_SIZE;                     //初始化大小----set initial size  
  
    #ifdef debug  
        puts("初始化栈成功！");  
    #endif  
  
    return 0;  
}  
  
/************************************************************************* 
函数名称：Push 
函数表述: 压栈操作 
输入值：  Stack *,Type 
返回值：  初始化成功返回0，否则返回1 
*************************************************************************/  
int Push(Stack *stack,Type elem)  
{  
    if(stack->top >= stack->StackSize - 1)  
    {  
        //如果空间不够了，则重新开辟新的空间--if the space is not enough,realloc the space  
        stack->data = (Type *)realloc(stack->data,(stack->StackSize + INCREASE_SIZE)*sizeof(Type));  
        stack->StackSize  += INCREASE_SIZE;  
  
        #ifdef debug  
            puts("开辟新空间成功！");  
        #endif  
  
        if(stack->data == NULL)  
        {  
            fprintf(stderr,"开辟空间失败，栈已经满了！/n");  
            return 1;  
        }  
    }  
    stack->top +=  1;  
    stack->data[stack->top] = elem;  
  
    #ifdef debug  
        puts("压栈成功！");  
    #endif  
  
    return 0;  
}  
  
/************************************************************************* 
函数名称：Pop 
函数表述: 弹栈操作 
输入值：  Stack *,Type 
返回值：  返回弹出的值 
*************************************************************************/  
Type Pop(Stack *stack)  
{  
    Type elem;  
    if(stack->top == -1)  
    {  
        fprintf(stderr,"栈已经空，弹栈失败！");  
        exit(1);  
    }  
    elem = stack->data[stack->top];  
  
    stack->top --;  
    #ifdef debug  
        puts("弹栈成功！");  
    #endif  
    return elem;  
}  
  
/************************************************************************* 
函数名称：IsEmpty 
函数表述: 判断栈是否为空 
输入值：  Stack *,Type 
返回值：  为空返回1，否则返回0 
*************************************************************************/  
int IsEmpty(Stack *stack)  
{  
    if(stack->top == -1)  
    {  
        return 1;  
    }  
  
    return 0;  
}  
  
/************************************************************************* 
函数名称：Traversal 
函数表述: 遍历栈 
输入值：  Stack *,Type 
返回值：  无 
*************************************************************************/  
void Traversal(Stack *stack)  
{  
    int i;  
  
    puts("栈中元素：top -> bottom");  
    for(i = stack->top;i >= 0;i --)  
    {  
        if (stack->data[i] >= DOLLAR && stack->data[i] <= C)
            printf("%s  ", TokenChar[stack->data[i]]);  
  
        if(i % ROW == 0)  
        {  
            puts("");  
        }  
    }  
    puts("");  
}  
  
/************************************************************************* 
函数名称：getTopElem 
函数表述: 获得栈顶元素 
输入值：  Stack *,Type 
返回值：  返回栈顶元素 
*************************************************************************/  
Type getTopElem(Stack *stack)  
{  
    Type top;  
    top = stack->data[stack->top];  
    return top;  
}  

#endif
