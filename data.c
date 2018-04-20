#include <stdio.h>
#include <errno.h>

int main(void)
{
	unsigned long int target, a, b, prodsum = 0;

	int scanstatus;

	FILE *input = fopen("test.dat","r");

	if (input == NULL) {
		perror ("fopen");
		return 1;
	}


	//erste Zahl wird separat gespeichert
	fscanf(input, " %lu", &target);

	
	//Zeilen werden eingelesen, bis die Summe der Produkte größer als der Zielwert ist oder bis Ende der Datei
	while (scanstatus = fscanf(input, "%lu * %lu", &a, &b), scanstatus != EOF && prodsum <= target)
	{
		prodsum += a * b;
	}


	if (prodsum == target)
		printf("The sum of the products matches the target.\n");
	else 
		printf("The sum of the products doesnt match the target.\n");
	


	return 0;
}
