#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

unsigned long filesize (FILE *filename)
	//returns size of inputfile in bytes
{
	int i = 0, cipher;
	

	while (cipher = fgetc(filename), cipher != EOF)
		i++;

	return i;
}

int main (int argc, char **argv)
{
	
	if (argc != 3) {
		printf ("Enter sourcepath and destinationpath in this order, please.\n");
		return 1;
	}

	

	//printf ("Read: %.*s\n", n, buf);

	int destexists = !(access(argv[2],F_OK));
	//printf ("destexists: %d\n", destexists);
	

	if (destexists) {
		char answer;
		printf ("Do you want to overwrite %s?(y/n)\n",argv[2]);
		scanf ("%c", &answer);
		if (answer != 'y') { return 1; }
	}


	FILE *source = fopen(argv[1], "rw");
	FILE *destination = fopen(argv[2], "w");
	size_t n;	
	do 
	{
		char buf[1024];


		if (source == NULL) {
			perror("fopen");
			return 1;
		}


		n = fread(buf, sizeof buf[0], sizeof buf, source);

		fwrite (buf, sizeof buf[0], n, destination);
	}
	while (n != 0);



	fclose (source);
	fclose (destination);

	return 0;

}
