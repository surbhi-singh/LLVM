#include <iostream>
#include <stdio.h>
int getch();
void ungetch(int);
static int stack [1000];
static int *top = NULL;
int getch()
{
	int ch;
	
	if(top == NULL)
	{
		ch = getchar ();
		return ch;
	}
	if(top == stack)
	{
		ch = *top;
		top = NULL;
	}
	else
	{
		ch = *top;
		--top;
		
	}
	return ch;
	
}
void ungetch (int ch)
{
	if(top == NULL)
	{
		top = stack;
		*top = ch;
	}
	else
	{
		top++;
		*top = ch;
	}
	//*top = ch;
	//printf("unreading ch %c", ch);
	//++top;
}
