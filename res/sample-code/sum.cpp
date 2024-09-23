/* Input two numbers, output the product */
#include <stdio.h>
int main()
{
	int x,y,m;
	printf("Please input x and y\n");
	scanf("%d%d",&x,&y);
	m=x*y;
	printf("%d * %d = %d\n",x,y,m);

	return 0;
}