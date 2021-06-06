#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
/*
eval
isToken
*/
int StrIndex=0;
char allString[4096];

typedef struct expression
{
    //type{0:token 1:operater}
    char *data;
    int type;
}exp;


bool isToken(char c)
{
    if(c<='9'&&c>='0')return true;
    if(c<='z'&&c>='a')return true;
    if(c<='Z'&&c>='A')return true;
    return false;
}

void parse(char *str)
{

}

int main()
{
    char testString[20];
    char *str2=&(testString[8]);
    for(int i=0;i<19;i++)testString[i]='a';
    testString[7]='\0';
    testString[19]='\0';
    scanf("%s",testString);
    printf("%s\n",testString);
    printf("%s",str2);
}