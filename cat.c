#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h> //nur für perror included
#include <dirent.h>
#include <string.h>


int cat(char *sourcename)
{
	int source = open(sourcename, O_RDONLY);
	if (source == -1)
	{
		perror("open");
		close (source);
		return 1;
	}

	ssize_t readvalue, writevalue;

	do {
	void * buf = malloc(1000);

	readvalue = read(source, buf, sizeof(buf));
	writevalue = write(2, buf, readvalue); 

	free(buf);
	}
	while(readvalue > 0 && writevalue > 0);

	if (readvalue < 0)
	{
		perror("read");
		close(source);
		return 1;
	}

	if (writevalue < 0)
	{
		perror("read");
		close(source);
		return 1;
	}


	close(source);

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i<argc; i++)
		{
			cat(argv[i]);
		}
	}
	else 
	{
		cat("/dev/stdin");
	}


	return 0;
}
