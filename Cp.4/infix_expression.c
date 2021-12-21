#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "Stack.h"

#define OPER 9
#define MAX_EXPR 256

char pri[OPER][OPER] = {
    //运算符优先等级 [栈顶][当前]
    /* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* |  - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* 栈 * */ '>', '>', '>', '<', '<', '<', '<', '>', '>',
    /* 顶 / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* 运 ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
    /* 算 ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /* 符 ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /*  | ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
    //           +    -    *    /    ^    !    (    )    \0
    // |-------------- 当前运算符 --------------|
};

/**
 * @brief 检索优先级数组，返回对应位置字符
 * @param A 栈顶字符
 * @param B 当前字符
 * @return char '>'/'<'/'='/' '(错误)
 */
char compare(char A, char B);

/**
 * @brief 读入多位数字，将栈顶数字乘以10之后加上digit
 * @param operand 操作数栈
 * @param digit 末尾数字
 */
void readNumber(Stack *operand, char digit);

/**
 * @brief 计算指定中缀表达式的值，同时计算其逆波兰表达式的值
 * @param input 指定中缀表达式，语法必须正确
 * @param input_len 表达式长度
 * @param RPE 此中缀表达式逆波兰表达式储存位置，仅需给与指针，对象由此函数在堆上创建
 * @return int 中缀表达式结果
 */
int computing(char *input, int input_len, char *RPE);

/**
 * @brief 判断运算符类型并使用operand中的数计算结果
 * @param operand 操作数栈
 * @param operator 操作符
 */
void computing_operator(Stack *operand, char operator);

int main(){
    printf("<=====================================================>\n\n");
    printf("此计算器会输入给定中缀表达式的结果，并输出其对应的后缀表达式(逆波兰表达式)\n\n");
    printf("注意，仅支持整数运算输入且必须保证输入算式的语法正确。\n\n以下是运算符优先级：\n\n{=,-} < {*} < {\\} < {^} < {!}\n\n");
    printf("<=====================================================>\n\n");
    char expr[MAX_EXPR];
    for(int i=0;i<MAX_EXPR;i++){
        expr[i]='\0';
    }

    char *RPE;
    printf("Pls input expression:%s\n",expr);
    scanf("%s",expr);
    printf("Expression:%s\n",expr);

    int len;
    for(len =0;expr[len]!='\0';len++);
    RPE = (char *)malloc(len+1 * 2 * sizeof(char));
    int result = computing(expr,++len,RPE);
    printf("RPE expression:%s\n",RPE);
    printf("Result:%d\n",result);
}

int computing(char *input, int input_len, char *RPE)
{
    Stack *aid = init_stack();
    push_stack(aid, '\0');
    Stack *operand = init_stack();
    int RPE_cursor = 0;
    int mult_digit = 0;

    for (int cursor = 0; cursor < input_len; cursor++)
    {
        char curr = input[cursor];
        if (isdigit(curr))
        {
            RPE[RPE_cursor++] = curr;
            RPE[RPE_cursor++] = ' ';
            if (!mult_digit)
            { // 并非多位数
                push_stack(operand, curr - '0'); // 记住这个小trick
            }
            else
            {
                readNumber(operand, curr - '0');
            }
            mult_digit = 1;
        }
        else
        {
            int keeping_gt = 1;
            while (keeping_gt)
            {
                switch (compare(top_stack(aid), curr))
                {
                case '>': // 栈顶元素大于当前元素
                    RPE[RPE_cursor++] = (char)top_stack(aid);
                    RPE[RPE_cursor++] = ' ';
                    computing_operator(operand, pop_stack(aid));
                    break;
                case '<': // 栈顶元素小于当前元素
                    push_stack(aid, curr);
                    keeping_gt = 0;
                    break;
                case '=': // 栈顶元素等于当前元素
                    pop_stack(aid);
                    keeping_gt = 0;
                    break;
                case ' ': // 发生错误
                    printf("Error,syntex illegal!\n");
                    abort;
                default: // 发生错误
                    printf("Error,syntex illegal!\n");
                    abort;
                };
            }
            mult_digit = 0;
        }
    }
    return top_stack(operand);
}

void computing_operator(Stack *operand, char operator)
{
    int A;                      // A operator
    int B = pop_stack(operand); // A operator B
    int C;                      // result
    switch (operator)
    {
    case '!':
        if(B == 0){
            C = 1;
            break;
        }
        for (int i = B - 1; i > 0; i--)
        {
            B *= i;
        }
        C = B;
        break;
    case '+':
        A = pop_stack(operand);
        C = A + B;
        break;
    case '-':
        A = pop_stack(operand);
        C = A - B;
        break;
    case '*':
        A = pop_stack(operand);
        C = A * B;
        break;
    case '/':
        A = pop_stack(operand);
        C = A / B;
        break;
    case '^':
        A = pop_stack(operand);
        C = (int)pow((double)A, (double)B);
        break;
    default:
        printf("Error,syntex illegal!\n");
        abort;
    }
    push_stack(operand, C);
}

char compare(char A, char B)
{
    int index_A;
    int index_B;
    switch (A)
    {
    case '+':
        index_A = 0;
        break;
    case '-':
        index_A = 1;
        break;
    case '*':
        index_A = 2;
        break;
    case '/':
        index_A = 3;
        break;
    case '^':
        index_A = 4;
        break;
    case '!':
        index_A = 5;
        break;
    case '(':
        index_A = 6;
        break;
    case ')':
        index_A = 7;
        break;
    case '\0':
        index_A = 8;
        break;
    default:
        printf("Error,A doesn't equal to any operator!");
        abort;
    };
    switch (B)
    {
    case '+':
        index_B = 0;
        break;
    case '-':
        index_B = 1;
        break;
    case '*':
        index_B = 2;
        break;
    case '/':
        index_B = 3;
        break;
    case '^':
        index_B = 4;
        break;
    case '!':
        index_B = 5;
        break;
    case '(':
        index_B = 6;
        break;
    case ')':
        index_B = 7;
        break;
    case '\0':
        index_B = 8;
        break;
    default:
        printf("Error,B doesn't equal to any operator!");
        abort;
    };
    return pri[index_A][index_B];
}

void readNumber(Stack *operand, char digit)
{
    int hi = pop_stack(operand) * 10 + (int)digit;
    push_stack(operand, hi);
}