
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"

#define WIDTH 800 
#define HEIGHT 600
#define E_WIDTH 30
#define E_HEIGHT 30
#define P_WIDTH 50
#define P_HEIGHT 10
#define B_WIDTH 4
#define B_HEIGHT 15
#define P_BULLETS 5
#define E_BULLETS 15

enum colour_t {red, green, purple};
enum direction_t {left, right};

struct enemy_t {

	SDL_Rect dimentions;
	enum colour_t colour;
	int alive;
};

struct invaders_t {

	struct enemy_t enemy[5][10];
	enum direction_t direction;
	int speed;
};

struct player_t {

	SDL_Rect dimentions;
};

struct bullet_t {
	
	SDL_Rect dimentions;
	int alive;
};

//global variables, for convenience.
static SDL_Surface *screen;
struct invaders_t invaders;
struct player_t player;
struct bullet_t bullets[P_BULLETS];
struct bullet_t e_bullets[E_BULLETS];

//initilize the enemys starting positions, direction, speed and colour
void init_invaders() {
	
	invaders.direction = left;
	invaders.speed = 1;

	int i,j;
	int x = 100;
	int y = 40;
	
	for (i = 0; i < 5; i++) {
	
		for (j = 0; j < 10; j++) {
		
			invaders.enemy[i][j].alive = 1;
			invaders.enemy[i][j].dimentions.x = x;
			invaders.enemy[i][j].dimentions.y = y;
			invaders.enemy[i][j].dimentions.w = E_WIDTH;
			invaders.enemy[i][j].dimentions.h = E_HEIGHT;
			
			x += E_WIDTH + 5; // gap size
			
			if (i == 0) {
				
				invaders.enemy[i][j].colour = purple;
	
			} else if (i >= 1 && i < 3) {
			
				invaders.enemy[i][j].colour = green;
		
			} else {
		
				invaders.enemy[i][j].colour = red;
			}
		}
		
		x = 100;
		y += E_HEIGHT + 5;
	}
}

void init_player() {

	player.dimentions.x = (WIDTH / 2) - (P_WIDTH / 2);
	player.dimentions.y = HEIGHT - (P_HEIGHT + 10);
	player.dimentions.w = P_WIDTH;
	player.dimentions.h = P_HEIGHT;
}

void init_bullets(struct bullet_t b[], int max) {

	int i;

	for (i = 0; i < max; i++) {
	
		b[i].alive = 0;
		b[i].dimentions.x = 0;
		b[i].dimentions.y = 0;
		b[i].dimentions.w = B_WIDTH;
		b[i].dimentions.h = B_HEIGHT;
	}
}

void draw_invaders(int x, int y) {

	SDL_Rect src;
	int i,j;

	src.w = E_WIDTH;
	src.h = E_HEIGHT;
	
	Uint8 c;
	
	for (i = 0; i < 5; i++) {
		
		switch (invaders.enemy[i][0].colour) {
		
			case green:
				
				c = SDL_MapRGB(screen->format, 0, 234, 0);
				break;
			
			case red:
				
				c = SDL_MapRGB(screen->format, 234, 0, 0);
				break;
			
			case purple:
				
				c = SDL_MapRGB(screen->format, 234, 0, 234);
				break;
		}

		for (j = 0; j < 10; j++) {
			
			src.x = invaders.enemy[i][j].dimentions.x;
			src.y = invaders.enemy[i][j].dimentions.y;

			if (invaders.enemy[i][j].alive == 0) {
				
				continue;
			}
			
			SDL_FillRect(screen, &src, c);
		}
	}
}

void draw_player() {

	SDL_Rect src;
	Uint8 c = SDL_MapRGB(screen->format, 255, 255, 0);

	src.x = player.dimentions.x;
	src.y = player.dimentions.y;
	src.w = player.dimentions.w;
	src.h = player.dimentions.h;
	
	SDL_FillRect(screen, &src, c);
}

void draw_bullets(struct bullet_t b[], int max) {

	SDL_Rect src;
	Uint8 c = SDL_MapRGB(screen->format, 255, 255, 255);
	int i;


	for (i = 0; i < max; i++) {
	
		if (b[i].alive == 1) {
		
			src.x = b[i].dimentions.x;
			src.y = b[i].dimentions.y;
			src.w = b[i].dimentions.w;
			src.h = b[i].dimentions.h;
			SDL_FillRect(screen, &src, c);
		}
	}
}

int move_bullets(struct bullet_t b[], int max, int speed) {
	
	int i;

	for(i = 0; i < max; i++) {
	
		if (b[i].alive == 1) {
			
			b[i].dimentions.y += speed;
			
			if (b[i].dimentions.y <= 0) {
		
				b[i].alive = 0;	
			}
		}
	}

	return 0;
}

void move_invaders_down() {

	int i,j;

	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
		
			invaders.enemy[i][j].dimentions.y += E_HEIGHT;
		}
	}
			
	invaders.speed++;
}

int move_invaders(int speed) {
	
	int i,j;

	if (invaders.direction == left) {
	
		for (i = 0; i < 10; i++) {
		
			for (j = 0; j < 5; j++) {
			
				if (invaders.enemy[j][i].alive == 1) {
	
					if (invaders.enemy[j][i].dimentions.x <= 0) {
					
						invaders.direction = right;
						move_invaders_down();
						return 0;
					}

					invaders.enemy[j][i].dimentions.x -= invaders.speed;
				}
			}
		}

	} else if (invaders.direction == right) {
	
		for (i = 9; i >= 0; i--) {
		
			for (j = 0; j < 5; j++) {
			
				if (invaders.enemy[j][i].alive == 1) {
				
					if (invaders.enemy[j][i].dimentions.x + E_WIDTH >= WIDTH) {
				
						invaders.direction = left;
						move_invaders_down();
						return 0;
					}
	
					invaders.enemy[j][i].dimentions.x += invaders.speed;
				}
			}
		}
	}

	return 0;
}

void move_player(int direction) {

	if (direction == 0) {
			
		if (player.dimentions.x > 0) {
			
			player.dimentions.x -= 10;
		}

	} else if (direction == 1) {

		if (player.dimentions.x + player.dimentions.w < WIDTH){

			player.dimentions.x += 10;
		}
	}
}

int collision(SDL_Rect a, SDL_Rect b) {

	if (a.y + a.h < b.y) {
					
		return 0;
	}
				
	if (a.y > b.y + b.h) {
					
		return 0;
	}
				
	if (a.x > b.x + b.w) {
					
		return 0;
	}
			
	if (a.x + a.w < b.x) {
					
		return 0;
	}

	return 1;
}

void enemy_collision() {

	int i,j,k,c;
	
	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
			
			if (invaders.enemy[i][j].alive == 1) {
			
				for (k = 0; k < P_BULLETS; k++) {
			
					if (bullets[k].alive == 1) {
						
						c = collision(bullets[k].dimentions, invaders.enemy[i][j].dimentions);
				
						if (c == 1) {
				
							invaders.enemy[i][j].alive = 0;
							bullets[k].alive = 0;
							bullets[k].dimentions.x = 0;
							bullets[k].dimentions.y = 0;
						}
					}
				}
			}
		}
	}
}

void enemy_player_collision() {

	int i,j,c;

	for(i = 0; i < 5; i++) {

		for(j = 0; j < 10; j++) {
		
			if (invaders.enemy[i][j].alive == 1) {
					
				c = collision(player.dimentions, invaders.enemy[i][j].dimentions);

				if (c == 1) {
				
					invaders.speed = 0;
				}
			}
		}
	}
}

void player_shoot() {

	int i;

	for (i = 0; i < P_BULLETS; i++) {
				
		if (bullets[i].alive == 0) {
			
			bullets[i].dimentions.x = player.dimentions.x + (P_WIDTH / 2);
			bullets[i].dimentions.y = player.dimentions.y;
			bullets[i].alive = 1;
			break;
		}
	}
}

void enemy_ai() {

	int i,j,k;

	//enemy bullet ai
	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
			
			if (invaders.enemy[i][j].alive == 1 && player.dimentions.x == invaders.enemy[i][j].dimentions.x) {
				 
				for (k = 0; k < E_BULLETS; k++) {
		
					if (e_bullets[i].alive == 0) {
			
						e_bullets[k].dimentions.x = invaders.enemy[i][j].dimentions.x;
						e_bullets[k].dimentions.y = invaders.enemy[i][j].dimentions.y;
						e_bullets[k].alive = 1;
						break;
					}
				}		
			}
		}
	}
}

int main() {
	
	SDL_Rect src;

	/* Initialize SDL’s video system and check for errors */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {

		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	/* Make sure SDL_Quit gets called when the program exits! */
	atexit(SDL_Quit);
	
	/* Attempt to set a 640x480 8 bit color video mode */
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 8, SDL_DOUBLEBUF );
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	Uint8 *keystate = 0;
	int quit = 0;
	SDL_Event event;

	init_invaders();
	init_player();
	init_bullets(bullets, P_BULLETS);
	init_bullets(e_bullets, E_BULLETS);
	
	/* Animate */
	while (quit == 0) {
		
		/* Update SDL's internal input state information. */
		//SDL_PumpEvents();

		/* Grab a snapshot of the keyboard. */
		keystate = SDL_GetKeyState(NULL);
		
		while (SDL_PollEvent(&event)) {

			switch(event.type) {
				
				case SDL_KEYDOWN:
					
					switch( event.key.keysym.sym ) {
					
						//exit out of game loop if escape is pressed
						case SDLK_ESCAPE:
							
							quit = 1;
						break;
						
						case SDLK_SPACE:	
							
							player_shoot();
						break;
						
						default:
						break;
					}
				break;

				default:
				break;
			}
		}
		
		//move player
		if (keystate[SDLK_LEFT]) {
			
			move_player(0);

		}

		if (keystate[SDLK_RIGHT]) {
			
			move_player(1);
		}

		//draw bg
		src.x = 0;
		src.y = 0;
		src.w = screen->w;
		src.h = screen->h;
	
		int r = SDL_FillRect(screen,&src,0);
		
		if (r !=0){
			
			printf("fill rectangle faliled");
		}
		
		draw_invaders(10, 10);
		draw_player();
		draw_bullets(bullets, P_BULLETS);
		draw_bullets(e_bullets, E_BULLETS);
		enemy_collision();
		enemy_player_collision();
		enemy_ai();
		move_invaders(invaders.speed);
		move_bullets(bullets, P_BULLETS, -30);
		move_bullets(e_bullets, E_BULLETS, 20);

		/* Ask SDL to update the entire screen. */
		SDL_Flip(screen);

		next_game_tick += 1000 / 30;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {

            		SDL_Delay(sleep);
        	}
	}
	
	return 0;
}
