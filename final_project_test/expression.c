#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
/*
eval
isToken
*/

bool isToken(char c)
{
    if(c<='9'&&c>='0')return true;
    if(c<='z'&&c>='a')return true;
    if(c<='Z'&&c>='A')return true;
    return false;
}

int main()
{
    char *testString="asdjkfn | asfd";
}