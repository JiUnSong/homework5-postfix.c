/*
 ============================================================================
 Name        : homework5-postfix.c
 Author      : Ji Un Song
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

/*
 * postfix.c
 *
 *  2020 Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;


char infixExp[MAX_EXPRESSION_SIZE];      /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];   /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];   /* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];      /* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;      /* evalStack용 top */

int evalResult = 0;      /* 계산 결과를 저장 */

void postfixPush(char x)			// 후위연산자 스택에 연산자를 저장
{
	postfixStack[++postfixStackTop] = x;
}

char postfixPop()				// 후위 연산자 스택에 연산자 top에 연산자 제거
{
	char x;
	if (postfixStackTop == -1)		// top == -1 이면 0을 반환
		return '\0';
	else {
		x = postfixStack[postfixStackTop--];
	}
	return x;
}


void evalPush(int x)		// eval함수에서 스택에 저장
{
	evalStack[++evalStackTop] = x;
}

int evalPop()			// eval함수에서의 pop
{
	if (evalStackTop == -1)
		return -1;
	else
		return evalStack[evalStackTop--];
}


/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);
}

precedence getToken(char symbol)			// 각 우선순위를 나타내기 위한 구조체함수
{
	switch (symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	default: return operand;
	}

}

precedence getPriority(char x)		// 우선순위 불러오기
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	int cnt = 0;
	int i;
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	postfixPush('N');		// top에 'N'을 push

	for (i = 0; infixExp[i] != '\0'; i++)			// infix 문자열 끝까지 반복
	{
		x = infixExp[i];		// x에 임시로 infix 값대입

		if (x >= '0' && x <= '9')		// 숫자일 경우
		{
			postfixExp[cnt++] = x;		// 바로 대입
		}
		else if (x == '(')			// 왼쪽괄호가 나오면
		{
			postfixPush('(');		// 스택에 쌓고
		}
		else if (x == ')')			// 오른쪽괄호 나오면
		{
			while (postfixStack[postfixStackTop] != 'N' && postfixStack[postfixStackTop] != '(')		// 처음이나 '('이전까지 계속
			{
				postfixExp[cnt++] = postfixStack[postfixStackTop];			// postfix에 값 대입하고
				postfixPop();				// 반복할때마다 스텍 하나씩 제거
			}
			if (postfixStack[postfixStackTop] == '(')			// '(' 나오면,
				postfixPop();			// 스텍에서 제거
		}
		else
		{			/* 아니면, 우선순위를 고려하고 스택 아래까지 값 대입*/
			while (postfixStack[postfixStackTop] != 'N' && getPriority(postfixStack[postfixStackTop]) >= getPriority(x)) {
				postfixExp[cnt++] = postfixStack[postfixStackTop];
				postfixPop();
			}
			postfixPush(x);
		}
	}
	while (postfixStack[postfixStackTop] != 'N')			// 값 대입
	{
		postfixExp[cnt++] = postfixStack[postfixStackTop];
		postfixPop();
	}

	/* 필요한 로직 완성 */

}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	int op1, op2, i;			//연산자 2개와 for문에 쓰일 i
	/* postfixExp, evalStack을 이용한 계산 */
	char *exp = postfixExp;
	char x;
	for(i = 0; postfixExp[i] != 0; i++) {			// 끝까지
		x = postfixExp[i];			// x에 각 postfix 값들을 대입
		if (x - '0' >= 0 && x - '0' <= 9)			// 숫자일 경우
			evalPush(x - '0');	// 스택에 숫자를 저장(int형)
		else {		// 연산자일 경우
			op2 = evalPop();		// 두 피연산자를 삭제하고 연산을 수행
			op1 = evalPop();
			switch (x) {				//각 연산에 맞게끔 연산을 수행후 결과를 스택에 push
			case '+': evalPush(op2+op1); break;
			case '-': evalPush(op2 - op1); break;
			case '*': evalPush(op2*op1); break;
			case '/': evalPush(op2 / op1); break;
			}
		}
	}
	evalResult = evalPop();			// 연산한 값을 대입
}


int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);		// 버퍼 허용
	printf("2019038028 -송지운-\n");
	char command;

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);		// 연산 입력

		switch (command) {		//각 경우
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 0;


}
