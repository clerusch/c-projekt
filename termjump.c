#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>

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
static struct tb_cell monster = {
	.ch = 'X',
	.fg = TB_BOLD,
	.bg = TB_RED,
};

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
static void update_jump(void)
{

			if (player_y < GAME_Y -1) 
			{
				if (player_y > GAME_Y - 5) {
					playerpos[player_y][player_x] = false;
					tb_put_cell(player_x, player_y, &empty);
					player_y--;
					playerpos[player_y][player_x] = true;
					tb_put_cell(player_x, player_y, &player);
				}
			}
}


















static int handle_key(uint16_t key)
{
	switch (key) {
	case TB_KEY_ARROW_LEFT:			//Die neuen Positionen müssen auch jeweils im playerarray gespeichert werden
		if (player_x > 0) {
			tb_put_cell(player_x, player_y, &empty);

			playerpos[player_y][player_x]= false;
			player_x--;
			playerpos[player_y][player_x]= true;

			tb_put_cell(player_x, player_y, &player);
		}
		break;
	case TB_KEY_ARROW_RIGHT:
		if (player_x < sizeof(obst[0])) {
			tb_put_cell(player_x, player_y, &empty);

			playerpos[player_y][player_x]= false;
			player_x++;
			playerpos[player_y][player_x]= true;

			tb_put_cell(player_x, player_y, &player);
		}
		break;
	case TB_KEY_SPACE:
		for (int i = 0; i <= GAME_X; i++)
		{
			if (playerpos[GAME_Y-1][i])
			{
				playerpos[player_y][player_x] = false;
				tb_put_cell(player_x, player_y, &empty);
				player_y--;
				playerpos[player_y][player_x] = true;
				tb_put_cell(player_x, player_y, &player);
			}
		}
		break;
	case TB_KEY_ESC:
		return 1;
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

	// Initialize board
	//   Player
	tb_put_cell(player_x, player_y, &player);
	playerpos[player_y][player_x] = true;


	// Show output
	tb_present();


	// Game loop
	struct tb_event e;
	clock_t timeanf;
	double timediff, random, counter1 = 0, counter2 = 0;


	while (true) { // give user 100ms for input
		timeanf = clock();
		
		tb_peek_event(&e, 10);
		switch (e.type) {
		case TB_EVENT_KEY:
			if (handle_key(e.key) == 1) goto shutdown_tb;
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
			obst[GAME_Y - 1][GAME_X-8] = true; 	//gen_obst: hindernis erscheint am rechten rand
			counter1 = 0;
		}

		if (counter2 > 0.001) { 
			update_obst();
			update_jump();

			counter2 = 0;
		}

















		//checken, ob kollision: sehr naiv, weil testet einfach jede runde sämtliche wahrheitswerte gegeneinander hier ist ein besserer algorithmus gefragt
		//for (i = 0; i < GAME_Y; i++)
		//	for (j=0; j< GAME_X; j++)
		//		if (player[x][y] && obst[x][y]) {
		//			tb_print ("GAME OVER!");
		//			sleep(3);
		//			goto shutdown_tb;
		//		}


		tb_present();
	}

shutdown_tb:
	fprintf(log, "exiting...\n");
	tb_shutdown();
close_log:
	fclose(log);
exit:
	exit(status);
}
