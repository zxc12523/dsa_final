#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
/*
eval
isToken
*/

//long long bitmask
//
int StrIndex=0;
char allString[2048];


long long eval(char*token);


//==========stack============
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
//======================
//===========opQueue=======
typedef struct opQueue
{
    char op[2048];
    int parlevel[2048];
    int lv;
    int end;
    int start;
}opQueue;
opQueue opQ;
void Q_init(opQueue *oq)
{
    oq->start=1024;
    oq->end=1024;
    oq->lv=0;
}
int Q_push(opQueue *oq,char c)
{
    if(oq->end+1<2048)
    {
        oq->parlevel[oq->end]=oq->lv;
        oq->op[oq->end++]=c;
    }
    else
    {
        oq->op[oq->end]=c;
        oq->parlevel[oq->end]=oq->lv;
        oq->end=0;
    }
    if(oq->start+1==oq->end)return 0;
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
void and(char *t1,char*t2)
{
    eval(t1);
}
void or(char *t1,char*t2)
{
    eval(t1);
}
void not(char *t1)
{
    eval(t1);
}


void operation(char op)
{
    int tem=tokens.top-2;
    char* c1=tokens.data[tem+1],c2=tokens.data[tem];
    switch (op)
    {
    case '&':
        and(c1,c2);
        break;
    case '|':
        or(c1,c2);
        break;
    case '!':
        not(c2);
        break;
    default:
        break;
    }
    tokens.top--;
    return ;
}
void addOp(opQueue *oq,char c)
{
    if(c=='(')
    {
        oq->lv+=1;
        return ;
    }
    if(c==')')
    {
        oq->lv-=1;
        return ;
    }
    // if(oq->end+1<Max)
    // {
    //     oq->parlevel[oq->end]=oq->lv;
    //     oq->op[oq->end++]=c;
    // }
    // else
    // {
    //     oq->op[oq->end]=c;
    //     oq->parlevel[oq->end]=oq->lv;
    //     oq->end=0;
    // }
    if(oq->start+1==oq->end)return ;
    int start=oq->start;
    if((level(oq->op[start])<level(c)&&oq->parlevel[start]==oq->lv)||(oq->parlevel[start]<oq->lv))
    {
        // oq->start--;
        // if(oq->start<0)oq->start=Max-1;
        // oq->op[oq->start]=c;
        // oq->parlevel[oq->start]=oq->lv;
        // oq->end--;
    }
    else
    {
        start=oq->start;
        int stop=oq->end-1;
        if(stop<0)stop=2048;
        do
        {
            operation(oq->op[start]);
            start+=1;
            if(start>=2048)start=0;
        }
        while((start!=stop)&&(oq->parlevel[start]>oq->lv||
        (oq->parlevel[start]==oq->lv&&level(oq->op[start])>=level(c))));
        oq->start=start;
        if((level(oq->op[start])<level(c)&&oq->parlevel[start]==oq->lv)||(oq->parlevel[start]<oq->lv))
        {
            start--;
            if(start<0)start=2048-1;
            oq->op[start]=c;
            oq->parlevel[start]=oq->lv;
            oq->end--;
        }
        oq->start=start;
        if(oq->start==2048)oq->start=0;
    }
}



bool isToken(char c)
{
    if(c<='9'&&c>='0')return true;
    if(c<='z'&&c>='a')return true;
    if(c<='Z'&&c>='A')return true;
    return false;
}

void parse(char *str)
{
    tokens.data[tokens.top++]=allString;
    for(int i=0;str[i]!='\0';i++)
    {
        if(isToken(str[i]))
        {
            allString[StrIndex++]=str[i];
        }
        else
        {
            allString[StrIndex++]='\0';
            tokens.data[tokens.top++]=(allString+StrIndex);
            //addOP(str[i]);
        }
        
    }
}

int main()
{
    char testString[20];
    char *str2=(testString)+8;
    for(int i=0;i<19;i++)testString[i]='a';
    testString[7]='\0';
    testString[19]='\0';
    scanf("%s",testString);
    printf("%s\n",testString);
    printf("%s",str2);
}