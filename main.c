/*
Data Structures and Algorithms
Final Assignment
Ozlem ER
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 50
//struct for int stack
typedef struct st{
    int stack[SIZE];
    int top;//top index of stack
}STACK;
//struct for char stack
typedef struct st_c{
    char stack_c[SIZE];
    int top;
}STACK_C;
//function prototypes
int push(STACK *,int);
int pop(STACK *);
char push_c(STACK_C *,char );
char pop_c(STACK_C *);
int priority(char );
void displayStack(STACK );
char * infixToPostfix(char *);
int solveExpression(char *,int *);
/******************************************/
/*This main function manage and control other functions and enables to work the program*/
int main()
{
    FILE *fi;
    char infix[SIZE], postfix[SIZE],line[SIZE],*token;
    int unknownsArr[26],x;
    //initializing
    memset(unknownsArr,0,26);
    //open the file and check
    fi = fopen("input.txt","r"); //dosyadan sadece okuma yapilacagi icin read modunda acilir
    if(fi==NULL){
        printf("File can not be open.\n");
        exit(0);
    }
    //end of the file
    while(!feof(fi)){
        //reading one line from input file
        fgets(line,sizeof(line),fi);
        printf("Equation : %s\n",line);
        //clear arrays
        memset(infix,'\0',SIZE);
        memset(postfix,'\0',SIZE);
        //printf("%s\n",line);
        //separating left hand side and right hand side expression
        token=strtok(line," =;");
        x=*token;//left hand-side
        //printf("%c\n",x);
        token=strtok(NULL,"=;");
        strcpy(infix,token);//right hand side
        printf("INFIX:%s\n",infix);
        strcpy(postfix,infixToPostfix(infix));
        printf("POSTFIX:%s\n",postfix);
        unknownsArr[x-97]=solveExpression(postfix,unknownsArr);
        printf("RESULT: %c=%d\n",x,unknownsArr[x-97]);

   }
   //close the file
   fclose(fi);
    return 0;
}
int push(STACK *st,int newVal){
    if((st->top)==SIZE)
        return 0;
    else{
        st->stack[st->top]=newVal;
        (st->top)++;
        return 1;
    }
}
int pop(STACK *st){
    if((st->top)==0)
        return -1;
    else{
        (st->top)--;
        return st->stack[st->top];
    }
}
char push_c(STACK_C *st,char newVal){
    if((st->top)==SIZE)
        return '0';
    else{
        st->stack_c[st->top]=newVal;
        (st->top)++;
        return '1';
    }
}
char pop_c(STACK_C *st){
    if((st->top)==0)
        return '0';
    else{
        (st->top)--;
        return st->stack_c[st->top];
    }
}
/*@param char: operation sign
@ returns int: prioriyt of sign
*/
int priority(char a){
    if(a=='(')
        return 0;
    if((a=='*')||(a=='/'))
        return 2;
    if((a=='+')||(a=='-'))
        return 1;
    if(a=='\0')
        return -1;
}/*
@param char *: address of infix expression
returns char *: address of postfix expression
This function returns the infix expression to postfix expression*/
char * infixToPostfix(char *infix){
    char postfix[SIZE];
    STACK_C st_c; //char stack
    char *iter;
    char x[2];//temp for string functions
    iter=infix;
    //initializing
    st_c.top=0;
    memset(st_c.stack_c,'\0',SIZE);
    memset(postfix,'\0',SIZE);
    x[1]='\0';
    //untill end of the expression
    while(*iter!='\0'){

            if(isalnum(*iter)){
                //if iter not an operator than assign to postfix
                x[0]=*iter;
                strcat(postfix,x);
               printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);

            }
            else if((*iter)=='('){
                        push_c(&st_c,*iter);
                        printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
                    }
            else if((*iter)==')'){
                x[1]='\0';
                // unill ( comes, pop from the stack
                while((x[0]=pop_c(&st_c))!='('){
                    strcat(postfix,x);
                    printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
                }
            }
            else if((*iter)==' '){
                //spaces divides numbers , for multi-digit
                x[0]=' ';
                strcat(postfix,x);
                printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
            }
            else{

                if(st_c.top==0){
                    push_c(&st_c,*iter);
                    printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
                }else if((priority(st_c.stack_c[st_c.top-1]))>=(priority(*iter))){
                      //if new element has low priorty then stack element
                      //then pop from stack and add postfix and push new element to stack
                      x[0]=pop_c(&st_c);
                      strcat(postfix,x);
                      printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
                      push_c(&st_c,*iter);
                      //printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
                }
                else{
                    push_c(&st_c,*iter);
                    printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
                }

            }
            iter++;

    }
    //if stack is not empty then pop remainders
    while(st_c.top!=0){
        x[0]=pop_c(&st_c);
        strcat(postfix,x);
        printf("Postfix: %s\tStack: %s\n",postfix,st_c.stack_c);
    }

    return postfix;
}
/*
@param STACK: integer stack
This functions prints the stack*/
void displayStack(STACK st){
    int i;
    printf("Stack: ");
    for(i=0;i<st.top;i++){
        printf("%d ",st.stack[i]);
    }
    printf("\n");
    return;
}
/*
@param char *: address of postfix expression
@param char *: adress of unknowns values
returns int: result of equation
This function evaluates the postfix expression*/
int solveExpression(char *postfix,int *unknowns){
    STACK st;
    int solution,temp;
    int op1,op2;//operators
    char *iter;
    char multi_digit[5];//for multiple digit numbers
    char x[2],c; //temp variables
    //initializing
    iter=postfix;
    st.top=0;
    x[1]='\0';
    memset(multi_digit,'\0',5);
    memset(st.stack,0,SIZE);
    //end of the postfix exp
    while((*iter)!='\0'){
        if(isspace(*iter)){
        //if((*iter)==' '){
            //iter++;
        }
        else if(isdigit(*iter)){
            memset(multi_digit,'\0',5);
            //finding number , single digit and multi digit
            while(((*iter)!=' ') && ((*iter)!='\0')){
                x[0]=*iter;
                strcat(multi_digit,x);
                iter++;
            }//return type char to int
            temp=atoi(multi_digit);
            //printf("multii:%d\n",temp);
            //push the number
            push(&st,temp);
            displayStack(st);
        }//if there þs an unknown
        else if(isalpha(*iter)){
            //there is an unknown
            c=*iter;
            //push value of unknown
            push(&st,unknowns[c-97]);
            displayStack(st);
        }
        else{
            //calculate
            c=*iter;
            //pop operands from stack
            op2=pop(&st);
            op1=pop(&st);
            switch(c){

            case '+':
                temp=op1+op2;
                push(&st,temp);
                displayStack(st);
                break;
            case '-':
                temp=op1-op2;
                push(&st,temp);
                displayStack(st);
                break;
            case '/':
                temp=op1/op2;
                push(&st,temp);
                displayStack(st);
                break;
            case '*':
                temp=op1*op2;
                push(&st,temp);
                displayStack(st);
                break;
            case ' ':
                break;
            }
        }
        iter++;
    }
    //pop the result from stack
    if(st.top>0)
        solution=st.stack[st.top-1];
    return solution;
}
