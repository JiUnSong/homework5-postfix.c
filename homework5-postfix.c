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

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum {
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence;


char infixExp[MAX_EXPRESSION_SIZE];      /* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE];   /* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];   /* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];      /* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;      /* evalStack�� top */

int evalResult = 0;      /* ��� ����� ���� */

void postfixPush(char x)			// ���������� ���ÿ� �����ڸ� ����
{
	postfixStack[++postfixStackTop] = x;
}

char postfixPop()				// ���� ������ ���ÿ� ������ top�� ������ ����
{
	char x;
	if (postfixStackTop == -1)		// top == -1 �̸� 0�� ��ȯ
		return '\0';
	else {
		x = postfixStack[postfixStackTop--];
	}
	return x;
}


void evalPush(int x)		// eval�Լ����� ���ÿ� ����
{
	evalStack[++evalStackTop] = x;
}

int evalPop()			// eval�Լ������� pop
{
	if (evalStackTop == -1)
		return -1;
	else
		return evalStack[evalStackTop--];
}


/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);
}

precedence getToken(char symbol)			// �� �켱������ ��Ÿ���� ���� ����ü�Լ�
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

precedence getPriority(char x)		// �켱���� �ҷ�����
{
	return getToken(x);
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	int cnt = 0;
	int i;
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */
	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	postfixPush('N');		// top�� 'N'�� push

	for (i = 0; infixExp[i] != '\0'; i++)			// infix ���ڿ� ������ �ݺ�
	{
		x = infixExp[i];		// x�� �ӽ÷� infix ������

		if (x >= '0' && x <= '9')		// ������ ���
		{
			postfixExp[cnt++] = x;		// �ٷ� ����
		}
		else if (x == '(')			// ���ʰ�ȣ�� ������
		{
			postfixPush('(');		// ���ÿ� �װ�
		}
		else if (x == ')')			// �����ʰ�ȣ ������
		{
			while (postfixStack[postfixStackTop] != 'N' && postfixStack[postfixStackTop] != '(')		// ó���̳� '('�������� ���
			{
				postfixExp[cnt++] = postfixStack[postfixStackTop];			// postfix�� �� �����ϰ�
				postfixPop();				// �ݺ��Ҷ����� ���� �ϳ��� ����
			}
			if (postfixStack[postfixStackTop] == '(')			// '(' ������,
				postfixPop();			// ���ؿ��� ����
		}
		else
		{			/* �ƴϸ�, �켱������ ����ϰ� ���� �Ʒ����� �� ����*/
			while (postfixStack[postfixStackTop] != 'N' && getPriority(postfixStack[postfixStackTop]) >= getPriority(x)) {
				postfixExp[cnt++] = postfixStack[postfixStackTop];
				postfixPop();
			}
			postfixPush(x);
		}
	}
	while (postfixStack[postfixStackTop] != 'N')			// �� ����
	{
		postfixExp[cnt++] = postfixStack[postfixStackTop];
		postfixPop();
	}

	/* �ʿ��� ���� �ϼ� */

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
	int op1, op2, i;			//������ 2���� for���� ���� i
	/* postfixExp, evalStack�� �̿��� ��� */
	char *exp = postfixExp;
	char x;
	for(i = 0; postfixExp[i] != 0; i++) {			// ������
		x = postfixExp[i];			// x�� �� postfix ������ ����
		if (x - '0' >= 0 && x - '0' <= 9)			// ������ ���
			evalPush(x - '0');	// ���ÿ� ���ڸ� ����(int��)
		else {		// �������� ���
			op2 = evalPop();		// �� �ǿ����ڸ� �����ϰ� ������ ����
			op1 = evalPop();
			switch (x) {				//�� ���꿡 �°Բ� ������ ������ ����� ���ÿ� push
			case '+': evalPush(op2+op1); break;
			case '-': evalPush(op2 - op1); break;
			case '*': evalPush(op2*op1); break;
			case '/': evalPush(op2 / op1); break;
			}
		}
	}
	evalResult = evalPop();			// ������ ���� ����
}


int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);		// ���� ���
	printf("2019038028 -������-\n");
	char command;

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);		// ���� �Է�

		switch (command) {		//�� ���
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
