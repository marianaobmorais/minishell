#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *str = "123";
	printf("%d", str[10]);
	free(str);
}