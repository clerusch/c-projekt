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
#define PLAYER_Y GAME_Y - 1

static unsigned int player_x = GAME_X/2;
static bool obst[GAME_Y][GAME_X];

//Für Player braucht man auch ein Array, das die Position speichert
//static bool player[Game_Y][GAME_X];

static const struct tb_cell empty = {
	.ch = ' ',
	.fg = TB_DEFAULT,
	.bg = TB_DEFAULT,
};
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

static int handle_key(uint16_t key)
{
	switch (key) {
	case TB_KEY_ARROW_LEFT:			//Die neuen Positionen müssen auch jeweils im playerarray gespeichert werden
		if (player_x > 0) {
			tb_put_cell(player_x, PLAYER_Y, &empty);
			player_x--;
			tb_put_cell(player_x, PLAYER_Y, &player);
		}
		break;
	case TB_KEY_ARROW_RIGHT:
		if (player_x < sizeof(obst[0])) {
			tb_put_cell(player_x, PLAYER_Y, &empty);
			player_x++;
			tb_put_cell(player_x, PLAYER_Y, &player);
		}
		break;
	case TB_KEY_SPACE:
	        ;// Hier muss das Sprungevent definiert werden
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
	//   Monster layout:
	//   (4+5) | X | 5 | X | 5 | ... | (5+4)    = 79 = GAME_X
	for (int i = 1; i <= 11; i++) {
		tb_put_cell(4+ i*5 +1, MONSTER_Y, &monster);
	}
	//   Player
	tb_put_cell(player_x, PLAYER_Y, &player);

	// Show output
	tb_present();


	// Game loop
	struct tb_event e;
	time_t timeanf;
	double timediff, counter1 = 0, counter2 = 0;

	srand(time(NULL));

	while (true) {
		// give user 100ms for input
		timeanf = time(NULL);
		
		tb_peek_event(&e, 100);
		switch (e.type) {
		case TB_EVENT_KEY:
			if (handle_key(e.key) == 1) goto shutdown_tb;
			break;
		default:
			;
		}
		
		

		timediff = (double)(time(NULL) - timeanf);

		counter1 += timediff;
		counter2 += timediff;
		

		if (counter1 > rand() % 2 + 1) { //alle ein ODER zwei sek. erscheint ein neues hindernis?
			obst[38][72] = true; 	//gen_obst: hindernis erscheint am rechten rand
			counter1 = 0;
		}

	//	if (counter2 > 0.001) { leider misst timediff für kleine zeiten anscheinend nicht präzise genug, update ist viel zu langsam, vielleicht doch mit clock irgendwie?
			update_obst();
	//		counter2 = 0;
	//	}


		//update_jump(); hier muss die flugkurve des sprungs im playerarray gespeichert werden
		
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
