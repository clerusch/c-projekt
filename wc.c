#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int wordcount(char *path)
{
	FILE *input = fopen(path, "r");

	int cha = 2;
	int wordc = 0, linec = 0, bytec = 0;
	
	do
	{
		while (cha = fgetc(input), cha != '\n' && cha != EOF)
			printf("%c",cha);

		linec++;

	} while (cha != EOF);

	printf("\n");
	printf("Zeilen: %i\n", linec);


	rewind(input);

	do
	{
		while (cha = fgetc(input), cha != ' ' && cha != EOF)
			printf("%c",cha);

		wordc++;

		if (cha = fgetc(input), cha == ' ')
			;
		else
			ungetc(cha,input);

	} while (cha != EOF);

	printf("\n");
	printf("Wörter: %i\n", wordc);




	return 0;
}
			

int main(int argc, char *argv[])
{
	pid_t status;

	if (argc == 1)
	{
		wordcount("/dev/stdin");
	}

	for (int i = 1; i < argc; i++)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
//			FILE *data2 = fopen(argv[i],"r");
//			printf("Kindprozess: erster Buchstabe von %iter Datei: %c\n", i, fgetc(data2));
			wordcount(argv[i]);
			break;
		}
		else
		{
		//	FILE *data1 = fopen(argv[1],"r");
			wait(&status);
			printf("Elternprozess: Durchlauf %i\n", i);
		}

	}
	return 0;

}
