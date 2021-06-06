#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
/*
eval
isToken
*/
int StrIndex=0;
char allString[4096];

// typedef struct expression
// {
//     //type{0:token 1:operater}
//     char *data;
//     int type;
//     //result{-1:uneval,0:false,1:true}
//     int result;
// }exp;
typedef struct stack
{
    char **data;
    int top;
}stack;
stack tokens;

void st_init(stack *st)
{
    st->top=0;
    st->data=calloc(2048,sizeof(char*));

}

// ==========vector===========
#define maxlength 100
typedef struct vector
{
    int *data;
    int now,len;
    struct vector*next,*prev,*back;
}vec;

vec new_vec()
{
    vec *v=malloc(sizeof(vec));
    v->data=malloc(maxlength*sizeof(int));
    v->len=maxlength;v->now=0;
    v->prev=NULL;v->next=NULL;
    v->back=NULL;
    return *v;
}

void vec_push(vec*v,int value)
{
    vec *back=v->back;
    
    if(back==NULL)
    {
        back=v;
    }
    if(back->now<back->len)
    {
        back->data[back->now++]=value;
        return ;
    }
    //else
    int *datatemp=(int*)realloc(back->data,sizeof(int)*(back->len+maxlength));
    
    if(datatemp==NULL)
    {
        vec next=new_vec();
        back->next=&next;
        next.prev=back;
        v->back=&next;
        next.data[next.now++]=value;
    }
    else
    {
        back->data=datatemp;
        back->len+=maxlength;
        back->data[back->now++]=value;
    }
    datatemp=NULL;
}
int* vec_at(vec*v,int index)
{
    // return NULL if the index is out of range
    vec *temp=v;
    while(1)
    {
        if(temp==NULL)
        {
            return NULL;
        }
        if(index<temp->now)
        {
            return &(temp->data[index]);
        }
        index-=temp->len;
        temp=temp->next;
    }
}
//========================


bool isToken(char c)
{
    if(c<='9'&&c>='0')return true;
    if(c<='z'&&c>='a')return true;
    if(c<='Z'&&c>='A')return true;
    return false;
}

void parse(char *str)
{
    for(int i=0;str[i]!='\0';i++)
    {
        
    }
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