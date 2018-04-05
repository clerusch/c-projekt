#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

int main (int argc, char **argv)
{
	
	if (argc != 3) {
		printf ("Enter sourcepath and destinationpath in this order, please.\n");
		return 1;
	}

	int destexists = !(access(argv[2],F_OK));
       	//access returns zero if file exists, -1 otherwise, so we have to negate to get the correct boolean


	if (destexists) {
		char answer;
		printf ("Do you want to overwrite %s?(y/n)\n",argv[2]);
		scanf ("%c", &answer);
		if (answer == 'n') { return 1; }
	}


	FILE *source = fopen(argv[1], "r");
	FILE *destination = fopen(argv[2], "w");
	size_t n;	
	
	if (source == NULL) {
		perror("fopen");
		return 1;
	}
	
	do 
	{
		char buf[1024];

		n = fread(buf, sizeof buf[0], sizeof buf, source);

		fwrite (buf, sizeof buf[0], n, destination);
	}
	while (n != 0);



	fclose (source);
	fclose (destination);

	return 0;

}
