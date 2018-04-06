//Basic Terminal-Jump&Run
//Authors: Theresa, Clemens Schumann, David Reuschenberg
//After a template by Leonard König

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <termbox.h>

#define GAME_X 79
#define GAME_Y 39
#define LOG_FILENAME "/tmp/spacei.log"

int x = 0, y = 0, z = 0, steigen = 1, neu=0, answer;

time_t t3 = 0;
struct tb_event input, leave;
clock_t timeanf;
time_t t1, t2;
double timediff, random, counter1 = 0, counter2 = 0;
   
static unsigned int player_x = GAME_X/2;
static unsigned int player_y = GAME_Y - 1;

static bool obst[GAME_Y][GAME_X]; 

static bool playerpos[GAME_Y][GAME_X]; 

static const struct tb_cell empty = { 
	.ch = ' ', 
	.fg = TB_DEFAULT, 
	.bg = TB_DEFAULT, 
};

static const struct tb_cell obs = {
	.ch = '<',
	.fg = TB_DEFAULT,
	.bg = TB_RED,
};
static const struct tb_cell player = {
	.ch = 'J',
	.fg = TB_BLUE,
	.bg = TB_GREEN,
};

// Integer to ASCII Funktion
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

//Function to print text via tb_cells
static void tb_print (int posx, int posy, char *text)
{
	static struct tb_cell cellarray[GAME_X+100];

	for (int i = 0; text[i] != '\0'; i++)
	{
		cellarray[i].ch = text[i];	
		cellarray[i].fg = TB_WHITE;
		cellarray[i].bg = TB_BLACK;

		tb_put_cell(posx + i, posy, &cellarray[i]);

	}
}

//let obstacles move from right to left
static void update_obst(void)
{
	for (size_t y = 0; y < GAME_Y; y++) {
		tb_put_cell(0, 38, &empty);
		obst[38][0] = false;
	}
	for (size_t y = 1; y < sizeof(obst)/sizeof(obst[0]); y++) {
		for (size_t x = 1; x < sizeof(obst[0]); x++) {
			if (obst[y][x]) {
				obst[y][x] = false;
				tb_put_cell(x, y, &empty);
				obst[y][x-1] = true;
				tb_put_cell(x-1, y, &obs);
			}
		}
	}
}

//let player move upwards 5 tiles after pressing spacekey
int update_jump(void)
{

			if (z >= 1 && z < 6) 
			{
				for (x = 0; x < GAME_X - 1; x++) {
					for (y = 1; y < GAME_Y; y++) {
						if (playerpos[y][x])
						{
							playerpos[y][x] = false;
							playerpos[y-1][x] = true;	
							tb_put_cell(x, y, &empty);
							tb_put_cell(x, y-1, &player);
						}
					}
				}
				z++;
			}
			 
			if (z == 6)
			{
			z++;
		       	return 0;
			}

			else return 1;
}

//let player move down again to ground level
int update_land(void)
{
	if (z < 12) 
	{
		for (x = GAME_X-1; x > 0; x--)
		{
			for (y = GAME_Y-2; y >= 0; y--)
			{
				if (playerpos[y][x])
				{
					playerpos[y][x] = false;
					playerpos[y+1][x] = true;	
					tb_put_cell(x, y, &empty);
					tb_put_cell(x, y+1, &player);
				}
			}
		}
		z++;
	}
	if (z == 12)
	{
		z = 0;	
                return 1;
	}
       	else {
	        return 0;
	}
}


// Initialize board
static void re_init (void)
{
	//  Set player and obstacle arrays false false again
	for (x = 0; x < GAME_X; x++) 
	{
		for (y = 0; y < GAME_Y; y++) 
		{
			playerpos[y][x] = false;
			obst[y][x] = false;
		}
	}

	steigen = 1, neu = 0, z = 0;
	tb_clear();

	//Set playerpositions
	playerpos[player_y][player_x] = true;
	playerpos[player_y][player_x+2] = true;
        playerpos[player_y-1][player_x+1] = true;
        playerpos[player_y-2][player_x+1]=true;
        playerpos[player_y-3][player_x+1]=true;
        playerpos[player_y-2][player_x+2]=true;
        playerpos[player_y-2][player_x]=true;

       tb_put_cell(player_x, player_y, &player);
       tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);

	t1 = time(NULL);

	// Show output
	tb_present();
}
//Move left, right and jump
static int handle_key(uint16_t key)
{
	switch (key) {
	case TB_KEY_ARROW_LEFT:		
		for (y = 0; y < GAME_Y; y++)
		{
			if (playerpos[y][1] == true)
				;
			else
			{
				for (x = 1; x < GAME_X - 1; x++)
				{
					for (y = 0; y < GAME_Y; y++)
					{
						if (playerpos[y][x])
						{
							playerpos[y][x] = false;
							playerpos[y][x-1] = true;	
							tb_put_cell(x, y, &empty);
							tb_put_cell(x-1, y, &player);
						}
					}
				}
			}
		}
		break;
	case TB_KEY_ARROW_RIGHT:
		for (y = 0; y < GAME_Y; y++)
		{
			if (playerpos[y][GAME_X-2] == true)
				;
			else
			{
				for (x = GAME_X - 2; x > 0; x--)
			       	{
					for (y = 0; y < GAME_Y; y++) 
					{
						if (playerpos[y][x])
						{
							playerpos[y][x] = false;
							playerpos[y][x+1] = true;	
							tb_put_cell(x, y, &empty);
							tb_put_cell(x+1, y, &player);
						}
					}
				}
			}
		}

		break;
	case TB_KEY_SPACE:
		if (z == 0)
		       	z = 1;
		break;
	case TB_KEY_ESC:
		return 1;

	case TB_KEY_ENTER:
		return 2;

	default:
		break;
	}
	return 0;
}

//Check if any truthvalues are overlapping, if yes stop game and let user decide whether to continue or to leave
static int game_over (void)
{
	for (y = 0; y < GAME_Y; y++)
	{
		for (x = 0; x < GAME_X; x++)
		{
			if (playerpos[y][x] && obst[y][x])
			{
				tb_print (30, 28, "GAME OVER!");
				tb_present();
				sleep(2);
				tb_print (25, 32, "Press ENTER to continue or ESC to leave"); 
				tb_present();

				do {
					tb_poll_event(&leave);
					if (leave.type == TB_EVENT_KEY)
					{
						answer = handle_key(leave.key);
						if (answer == 2) 
						{		
                            if (t2-t1 > t3){
                                t3 = t2-t1;
                            }

							re_init();
							return 0;
						}			
						if (answer == 1) 
						{
							return 1;
						}
					}
				} while (true);

			}
		}
	}
	return 2;
}




int main(void)
{
	int status = EXIT_SUCCESS;

	FILE *log = fopen(LOG_FILENAME, "w+");
	if (!log) {
		perror("fopen");
		status = EXIT_FAILURE;
		goto exit;
	}
	fprintf(stderr, "Logfile: %s\n", LOG_FILENAME);

	// Initialize Termbox
	{
		int tbe;
		if ((tbe = tb_init()) < 0) {
			switch (tbe) {
			case TB_EUNSUPPORTED_TERMINAL:
			case TB_EFAILED_TO_OPEN_TTY:
			case TB_EPIPE_TRAP_ERROR:
				status = EXIT_FAILURE;
				fprintf(log, "Termbox error code: %d\n", tbe);
				break;
			default:
				status = EXIT_FAILURE;
				fprintf(log, "Unknown termbox error code: %d\n",
						tbe);
			}
			goto close_log;
		}
	}

	// Check terminal size
	{
		int tbw = tb_width();
		int tbh = tb_height();
		fprintf(log, "Dimensions: width %d, height %d\n", tbw, tbh);
		if (tbw < GAME_X || tbh < GAME_Y) {
			status = EXIT_FAILURE;
			fprintf(log, "Console too small\n");
			fprintf(log, "Needs: width: %d, height: %d\n",
					GAME_X, GAME_Y);
			goto shutdown_tb;
		}
	}
	//initialize board
	
	re_init();
	t1 = time(NULL);
    
	// Game loop

	while (true) { // give user 10ms for input
		timeanf = clock();

		// Uhrzeit block
		char kuchen[20];
		t2 = time(NULL);
		sprintf(kuchen,"Spielzeit %li:%li", (t2-t1)/60,(t2-t1)%60);
		tb_print(10,10,kuchen);
        char cake [20];
        sprintf(cake, "Highscore: %li:%li", t3/60, t3%60);
        tb_print(55,10,cake);


		// Uhrzeit block ende

		tb_peek_event(&input, 10);

		if (input.type == TB_EVENT_KEY)
	       	{
			if (handle_key(input.key) == 1) goto shutdown_tb;
		}
		
		srand(time(NULL));
		random = (double)((rand() % 3)+ 1)/130;

		timediff = (double)(clock() - timeanf)/CLOCKS_PER_SEC;
		counter1 += timediff;
		counter2 += timediff;

		if (counter1 > random)
		{                                                  
			obst[GAME_Y - 2][GAME_X-2] = true; 		
			obst[GAME_Y - 1][GAME_X-1] = true; 	//hindernis erscheint am rechten rand
			obst[GAME_Y - 1][GAME_X-2] = true;
			obst[GAME_Y - 1][GAME_X-3] = true;

		        counter1 = 0;
		}

		if (counter2 > 0.0007) { 
			update_obst();
			if (steigen)
                		steigen = update_jump();
            		else
				steigen = update_land();

			counter2 = 0;
		}

		tb_present();

		//Game over after collision
		int shutdown = game_over();
		if (shutdown == 1)
		       goto shutdown_tb;	
	}

shutdown_tb:
	fprintf(log, "exiting...\n");
	tb_shutdown();
close_log:
	if (fclose(log) != 0) {
		perror("fclose");
	}
exit:
	exit(status);
}
