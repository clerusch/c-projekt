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
#define MONSTER_Y 4


static unsigned int player_x = GAME_X/2;
static unsigned int player_y = GAME_Y - 1;

static bool obst[GAME_Y][GAME_X]; 

static bool playerpos[GAME_Y][GAME_X]; 

static const struct tb_cell empty = { .ch = ' ', .fg = TB_DEFAULT, .bg = TB_DEFAULT, };

static const struct tb_cell obs = {
	.ch = '*',
	.fg = TB_DEFAULT,
	.bg = TB_YELLOW,
};
static const struct tb_cell player = {
	.ch = '^',
	.fg = TB_DEFAULT,
	.bg = TB_GREEN,
};

static void tb_print (int posx, int posy, char *text)
{

	static struct tb_cell cellarray[50];

	for (int i = 0; text[i] != '\0'; i++)
	{
		cellarray[i].ch = text[i];	
		cellarray[i].fg = TB_DEFAULT;
		cellarray[i].bg = TB_YELLOW;

		tb_put_cell(posx + i, posy, &cellarray[i]);

	}
}

static void update_obst(void)
{
	for (size_t y = 0; y < GAME_Y; y++) {
		tb_put_cell(0, 38, &empty);
		obst[38][0] = false;
	}
	for (size_t y = 1; y < sizeof(obst)/sizeof(obst[0]); y++) {
		for (size_t x = 0; x < sizeof(obst[0]); x++) {
			if (obst[y][x]) {
				obst[y][x] = false;
				tb_put_cell(x, y, &empty);
				obst[y][x-1] = true;
				tb_put_cell(x-1, y, &obs);
			}
		}
	}
}
int update_jump(void)
{

			if (player_y < GAME_Y -1) 
			{
				if (player_y > GAME_Y - 5) {
					playerpos[player_y][player_x] = false;
					tb_put_cell(player_x, player_y, &empty);
                     tb_put_cell(player_x+2, player_y, &empty);
       tb_put_cell(player_x+1, player_y-1, &empty);
       tb_put_cell(player_x+1, player_y-2, &empty);
       tb_put_cell(player_x+1, player_y-3, &empty);
       tb_put_cell(player_x+2, player_y-2, &empty);
       tb_put_cell(player_x, player_y-2, &empty);
					player_y--;
					playerpos[player_y][player_x] = true;
					tb_put_cell(player_x, player_y, &player);
                     tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);
				if (player_y == GAME_Y - 4) return 0;
				else return 1;
                   
				}
			} 
			if (player_y == (GAME_Y - 4)){
                        return 0;}else{
                            return 1;}
}

int update_land(void)
{

			if (player_y < GAME_Y -1) 
			{
				if (player_y > GAME_Y - 5) {
					playerpos[player_y][player_x] = false;
					tb_put_cell(player_x, player_y, &empty);
                     tb_put_cell(player_x+2, player_y, &empty);
       tb_put_cell(player_x+1, player_y-1, &empty);
       tb_put_cell(player_x+1, player_y-2, &empty);
       tb_put_cell(player_x+1, player_y-3, &empty);
       tb_put_cell(player_x+2, player_y-2, &empty);
       tb_put_cell(player_x, player_y-2, &empty);
					player_y++;
					playerpos[player_y][player_x] = true;
					tb_put_cell(player_x, player_y, &player);
                    tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);
                                        
				}
			}
			if (player_y == (GAME_Y - 1)){
                        return 1;} else{
                            return 0;}
}


static int handle_key(uint16_t key)
{
	switch (key) {
	case TB_KEY_ARROW_LEFT:			//Die neuen Positionen müssen auch jeweils im playerarray gespeichert werden
		if (player_x > 0) {
			tb_put_cell(player_x, player_y, &empty);
            tb_put_cell(player_x+2, player_y, &empty);
       tb_put_cell(player_x+1, player_y-1, &empty);
       tb_put_cell(player_x+1, player_y-2, &empty);
       tb_put_cell(player_x+1, player_y-3, &empty);
       tb_put_cell(player_x+2, player_y-2, &empty);
       tb_put_cell(player_x, player_y-2, &empty);

			playerpos[player_y][player_x]= false;
			player_x--;
			playerpos[player_y][player_x]= true;

			tb_put_cell(player_x, player_y, &player);
            tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);
		}
		break;
	case TB_KEY_ARROW_RIGHT:
		if (player_x < sizeof(obst[0])) {
			tb_put_cell(player_x, player_y, &empty);
              tb_put_cell(player_x+2, player_y, &empty);
       tb_put_cell(player_x+1, player_y-1, &empty);
       tb_put_cell(player_x+1, player_y-2, &empty);
       tb_put_cell(player_x+1, player_y-3, &empty);
       tb_put_cell(player_x+2, player_y-2, &empty);
       tb_put_cell(player_x, player_y-2, &empty);

			playerpos[player_y][player_x]= false;
			player_x++;
			playerpos[player_y][player_x]= true;

			tb_put_cell(player_x, player_y, &player);
             tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);
		}
		break;
	case TB_KEY_SPACE:
		for (int i = 0; i <= GAME_X; i++)
		{
			if (playerpos[GAME_Y-1][i])
			{
				playerpos[player_y][player_x] = false;
				tb_put_cell(player_x, player_y, &empty);
                       tb_put_cell(player_x+2, player_y, &empty);
       tb_put_cell(player_x+1, player_y-1, &empty);
       tb_put_cell(player_x+1, player_y-2, &empty);
       tb_put_cell(player_x+1, player_y-3, &empty);
       tb_put_cell(player_x+2, player_y-2, &empty);
       tb_put_cell(player_x, player_y-2, &empty);
				player_y--;
				playerpos[player_y][player_x] = true;
				tb_put_cell(player_x, player_y, &player);
                 tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);
			}
		}
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
start:

	// Initialize board
	//   Player
	tb_clear();
	tb_put_cell(player_x, player_y, &player);
	playerpos[player_y][player_x] = true;
    if (playerpos[player_y][player_x]){
       /* playerpos[player_y][player_x+2] = true;
        playerpos[player_y-1][player_x+1] = true;
        playerpos[player_y-2][player_x+1]=true;
        playerpos[player_y-3][player_x+1]=true;
        playerpos[player_y-2][player_x+2]=true;
        playerpos[player_y-2][player_x]=true;
        */
       tb_put_cell(player_x+2, player_y, &player);
       tb_put_cell(player_x+1, player_y-1, &player);
       tb_put_cell(player_x+1, player_y-2, &player);
       tb_put_cell(player_x+1, player_y-3, &player);
       tb_put_cell(player_x+2, player_y-2, &player);
       tb_put_cell(player_x, player_y-2, &player);
    }
            
        
        int steigen = 1;


	// Show output
	tb_present();


	// Game loop
	struct tb_event input, leave;
	clock_t timeanf;
	double timediff, random, counter1 = 0, counter2 = 0;


	while (true) { // give user 10ms for input
		timeanf = clock();
		
		tb_peek_event(&input, 10);
		switch (input.type) {
		case TB_EVENT_KEY:
			if (handle_key(input.key) == 1) goto shutdown_tb;
			break;
		default:
			;
		}
		
		
		srand(time(NULL));
		random = (double)((rand() % 2)+ 1)/100;

		timediff = (double)(clock() - timeanf)/CLOCKS_PER_SEC;

		counter1 += timediff;
		counter2 += timediff;


		if (counter1 > random)
		{                                                    // (((rand() % 2)+ 1)/100)
			obst[GAME_Y - 1][GAME_X-8] = true; 	//hindernis erscheint am rechten rand
			counter1 = 0;
		}

		if (counter2 > 0.001) { 
			update_obst();
			if (steigen)
                		steigen = update_jump();
            		else
				steigen = update_land();

			counter2 = 0;
		}

		tb_present();


		for (int y = 0; y <= GAME_Y; y++)
			for (int x = 0; x<= GAME_X; x++)
				if (playerpos[y][x] && obst[y][x]) {
                    			tb_print (30, 28, "GAME OVER!");
					tb_present();
					sleep(3);
					tb_print (25, 32, "Press ENTER to continue or ESC to leave"); 
					tb_present();

					tb_poll_event (&leave);


					int answer = handle_key(leave.key);

					if (answer == 2) 
					{	//restart?	
						tb_clear();
						tb_present(); 
					}			
					else if (answer == 1)
						goto shutdown_tb;

					
				}

	}

shutdown_tb:
	fprintf(log, "exiting...\n");
	tb_shutdown();
close_log:
	fclose(log);
exit:
	exit(status);
}
