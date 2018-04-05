int my_itoa (int in, char **out_str)
{
	int i, count = in, countziffern = 0, rechteziffer;

	while (count > 0) {
        count /= 10;
        countziffern++;
        }

        *out_str[countziffern] = '\0';

        for (i = countziffern - 1; i >= 0; i--)
        {
	        rechteziffer = in % 10;
                *out_str[i] = rechteziffer + 48;
                in /= 10;
        }

        return 0;
}


int main (int argc, char **argv)
{
	int integer = 42; count = integer, countsum;

	while (count > 0) {
		count /= 10;
		countsum++;
	}


	char *outstr = malloc 
