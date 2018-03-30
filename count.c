#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main (void)
{
	int cha, i;
	char *string = malloc (50 * sizeof *string);

	for (i = 0 ; cha = getchar(), cha != ' '; i++)
		string[i] = cha;

	string[i+1] = '\0';
		
	printf ("%s\n", string);

	return 0;
}


