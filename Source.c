#include<stdio.h>
#include<stdlib.h>     
#include<ctype.h>     
#include<string.h>
#include<stdbool.h>
#include<math.h>

#define SIZE 100

char separatedItems[50][50] = { NULL };

struct stack {
	int top;
	char item[SIZE];
};
typedef struct stack* stackptr;

struct intStack {
	int top;
	int item[SIZE];
};
typedef struct intStack* intstackptr;

// Function to remove all spaces from a given string 
void removeSpaces(char* str)
{

	int count = 0;

	for (int i = 0; str[i]; i++)
		if (str[i] != ' ')
			str[count++] = str[i];

	str[count] = '\0';
}

bool separate(char input[], int length)
{
	//reset separatedItems array to reuse
	memset(separatedItems, NULL, sizeof(separatedItems[0][0]) * 50 * 50);
	int r = 0, c = 0;
	for (int i = 0; i < length; i++)
	{

		if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= '0' && input[i] <= '9'))
		{
			separatedItems[r][c] = input[i];
			if ((input[i + 1] >= '0' && input[i + 1] <= '9'))
				c++;
			else if ((input[i + 1] >= 'a' && input[i + 1] <= 'z') || (input[i + 1] >= 'A' && input[i + 1] <= 'Z'))
			{
				printf("%s\n", "You have entered wrong expression! - Two or more operands appeared successively");
				return false;
			}
			else
			{
				c = 0;
				r++;
			}
		}
		else if (input[i] == '(' || input[i] == ')')
		{
			separatedItems[r][c] = input[i];
			c = 0;
			r++;;
		}
		else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^')
		{
			if (input[i + 1] == '+' || input[i + 1] == '-' || input[i + 1] == '*' || input[i + 1] == '/' || input[i + 1] == '^')
			{
				printf("%s\n", "You have entered wrong expression! - Two or more operators appeared successively");
				return false;
			}
			separatedItems[r][c] = input[i];
			c = 0;
			r++;;
		}
	}

	return true;
}

stackptr getStack() {

	stackptr stack = (stackptr)malloc(sizeof(struct stack));

	if (!stack)
		return NULL;

	stack->top = -1;
	return stack;
}

stackptr getintStack() {

	intstackptr stack = (intstackptr)malloc(sizeof(struct intStack));

	if (!stack)
		return NULL;

	stack->top = -1;
	return stack;
}

int isEmpty(stackptr s)
{
	return (s->top == -1);
}

int isintEmpty(intstackptr s)
{
	return (s->top == -1);
}

char peek(stackptr s)
{
	return s->item[s->top];
}

int ipeek(intstackptr s)
{
	return s->item[s->top];
}

char pop(stackptr s)
{
	if (!isEmpty(s))
		return s->item[s->top--];
	return '$';
}

int ipop(intstackptr s)
{
	if (!isEmpty(s))
		return s->item[s->top--];
	return '$';
}

void push(stackptr s, char inp)
{
	s->item[++s->top] = inp;
}

void ipush(intstackptr s, int inp)
{
	s->item[++s->top] = inp;
}

int isOperand(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}


int precedence(char ch)
{
	switch (ch)
	{
	case '+':
	case '-':
		return 1;

	case '*':
	case '/':
		return 2;

	case '^':
		return 3;
	}
	return -1;
}
int full(stackptr s) {

	return (s->top == SIZE - 1);

}
int size(stackptr s) {

	return strlen(s->item);
}



int Evaluate(char* postfix)
{

	intstackptr st;
	st = getintStack();
	int temp = 0;
	while (strlen(postfix) > 0)
	{
		if ((postfix[0] >= 'a' && postfix[0] <= 'z') || (postfix[0] >= 'A' && postfix[0] <= 'Z'))
		{
			printf("Can't evaluate this expression as it contains characters not numbers only!\n");
			return 0;
		}
		else if (postfix[0] == ' ')
			postfix++;
		else if (postfix[0] >= '0' && postfix[0] <= '9')
		{
			temp = atoi(postfix);
			ipush(st, temp);
			int k = 0;
			while (k < strlen(postfix))
			{
				if (postfix[k] == ' ')
				{
					//remove the first part of the string including the space
					postfix = postfix + k + 1;
					break;
				}
				else if (postfix[k] == '+' || postfix[k] == '-' || postfix[k] == '*' || postfix[k] == '/' || postfix[k] == '^')
				{
					//remove the first part of the string before operator
					postfix = postfix + k;
					break;
				}
				k++;
			}
		}
		else if (postfix[0] == '+' || postfix[0] == '-' || postfix[0] == '*' || postfix[0] == '/' || postfix[0] == '^')
		{
			temp = 0;
			int op2 = ipop(st);
			int op1 = ipop(st);

			switch (postfix[0])
			{
			case '+':
				temp = op1 + op2;
				ipush(st, temp);
				break;
			case '-':
				temp = op1 - op2;
				ipush(st, temp);
				break;
			case '*':
				temp = op1 * op2;
				ipush(st, temp);
				break;
			case '/':
				temp = op1 / op2;
				ipush(st, temp);
				break;

			case '^':
				temp = pow(op1, op2);
				ipush(st, temp);
				break;
			}

			postfix++;

			//to remove any additional spaces
			if (postfix[0] == ' ')
				postfix++;
		}

	}

	int result = ipop(st);
	return result;
}


void infixToPostfix(char* s, int l, int choice)
{
	stackptr st; //stack
	st = getStack();
	push(st, '$');
	char ns[20] = { NULL }; //output postfix new string 
	int k = 0;
	for (int i = 0; i < l; i++)
	{
		// If the scanned character is an operand, add it to output string. 
		if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9'))
		{
			ns[k] = s[i];
			k++;
		}

		else if (s[i] == '(')

			push(st, '(');

		else if (s[i] == ')')
		{
			while (peek(st) != '$' && peek(st) != '(')
			{
				char c = peek(st);
				pop(st);
				//*(ns + k) = c;

				ns[k] = c;
				k++;
			}
			if (peek(st) == '(')
			{
				char c = peek(st);
				pop(st);
			}
		}

		//If an operator is scanned 
		else {
			while (peek(st) != '$' && precedence(s[i]) <= precedence(peek(st)) && !(s[i] == '^' && peek(st) == '^'))
			{
				char c = peek(st);
				pop(st);
				ns[k] = c;
				k++;
			}
			push(st, s[i]);
			//spacing for handling multiple digits
			ns[k] = ' ';
			k++;
		}

	}

	ns[k] = ' ';
	k++;
	//Pop all the remaining elements from the stack 
	while (peek(st) != '$')
	{
		char c = peek(st);
		pop(st);
		//*(ns + k) = c;
		//k++;
		ns[k] = c;
		k++;
	}

	int evalResult = 0;
	if (choice == 2)
		evalResult = Evaluate(ns);
	removeSpaces(ns);
	printf("This is the postfix: %s\n", ns);
	if (choice == 2)
		printf("This is the evaluation result: %d\n", evalResult);

}



bool checkCorrectness(char* exp)
{
	int count = 0;
	bool operatorPosition = true;
	for (int i = 0; i < strlen(exp); i++)
	{
		if (exp[i] == '(')
			count++;
		else if (exp[i] == ')')
			count--;
	}
	if (exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/' || exp[0] == '^' || exp[strlen(exp) - 1] == '+' || exp[strlen(exp) - 1] == '-' || exp[strlen(exp) - 1] == '*' || exp[strlen(exp) - 1] == '/' || exp[strlen(exp) - 1] == '^')
		operatorPosition = false;

	if (count == 0 && operatorPosition)
		return true;
	else
	{
		printf("%s\n", "You have entered wrong expression!");
		return false;
	}
}



int main() {
	while (1)
	{
		int choice = 0;
		printf("Enter 1 for Conversion, and 2 for Conversion & Evaluation:\n");
		scanf_s("%d", &choice);
		if (choice == 1 || choice == 2)
		{
			char input[50] = { NULL };
			int length = 0;
			printf("Input the infix expresion, press enter when done:\n");
			gets(input); //btkhod endline w tmshe
			gets(input);


			removeSpaces(input);
			length = strlen(input);
			if (checkCorrectness(input) && separate(input, strlen(input)))
			{

				infixToPostfix(input, length, choice);
			}
		}
		else
			printf("Please choose a valid option!\n ");

	}
	return 0;
}
