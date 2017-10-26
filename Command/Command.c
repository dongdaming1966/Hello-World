#include <stdio.h>
#include <string.h>

int main(void)
{
	char string[50];	//storage the input command

	printf("***************************\n");
	printf("     command supported     \n");
	printf(" clock                     \n");
	printf("***************************\n");
	while(1)
	{	
		printf("command:");
		scanf("%s",string);
		if(!strcmp(string,"clock"))
			printf("clock");
		else
			printf("unidentified command!\n");
	}
	return 0;

}
