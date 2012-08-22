
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
#define P_BULLETS 3
#define E_BULLETS 5

enum colour_t {red, green, purple};
enum direction_t {left, right};

struct enemy {

	enum colour_t colour;
	int alive;
	int x;
	int y;
};

struct player_t {

	int x;
	int y;
};

struct bullet_t {
	
	int alive;
	int x;
	int y;
};

//global variables, for convenience.
static SDL_Surface *screen;
struct enemy invaders[5][10];
struct player_t player;
struct bullet_t bullets[P_BULLETS];
struct bullet_t e_bullets[E_BULLETS];
enum direction_t direction;
int speed = 1;

//initilize the enemys starting positions, direction and colour
void init_invaders() {
	
	direction = left;

	int i,j;
	int x = 100;
	int y = 10;

	for (i = 0; i < 5; i++) {
	
		for (j = 0; j < 10; j++) {
		
			invaders[i][j].alive = 1;
			invaders[i][j].x = x;
			invaders[i][j].y = y;
			
			x += E_WIDTH + 5; // gap size
			
			if (i == 0) {
				
				invaders[i][j].colour = purple;
	
			} else if (i >= 1 && i < 3) {
			
				invaders[i][j].colour = green;
		
			} else {
		
				invaders[i][j].colour = red;
			}
		}
		
		x = 100;
		y += E_HEIGHT + 5;
	}
}

void init_player() {

	player.x = (WIDTH / 2) - (P_WIDTH / 2);
	player.y = HEIGHT - (P_HEIGHT + 10);
}

void init_bullets(struct bullet_t b[], int max) {

	int i;

	for (i = 0; i < max; i++) {
	
		b[i].alive = 0;
		b[i].x = 0;
		b[i].y = 0;
	}
}

void draw_invaders(int x, int y) {

	SDL_Rect src;
	int i,j;

	src.w = E_WIDTH;
	src.h = E_HEIGHT;
	
	Uint8 c;
	
	for (i = 0; i < 5; i++) {
		
		switch (invaders[i][0].colour) {
		
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
			
			src.x = invaders[i][j].x;
			src.y = invaders[i][j].y;

			if (invaders[i][j].alive == 0) {
				
				continue;
			}
			
			SDL_FillRect(screen, &src, c);
		}
	}
}

void draw_player(int x) {

	SDL_Rect src;
	Uint8 c = SDL_MapRGB(screen->format, 255, 255, 0);

	src.x = player.x;
	src.y = player.y;
	src.w = P_WIDTH;
	src.h = P_HEIGHT;
	
	SDL_FillRect(screen, &src, c);
}

void draw_bullets() {

	SDL_Rect src;
	Uint8 c = SDL_MapRGB(screen->format, 255, 255, 255);
	int i;

	src.w = B_WIDTH;
	src.h = B_HEIGHT;

	for (i = 0; i < P_BULLETS; i++) {
	
		if (bullets[i].alive == 1) {
		
			src.x = bullets[i].x;
			src.y = bullets[i].y;
			SDL_FillRect(screen, &src, c);
		}
	}
	
	for (i = 0; i < E_BULLETS; i++) {
	
		if (e_bullets[i].alive == 1) {
		
			src.x = e_bullets[i].x;
			src.y = e_bullets[i].y;
			SDL_FillRect(screen, &src, c);
		}
	}
}

int move_bullets() {
	
	int i;

	for(i = 0; i < P_BULLETS; i++) {
	
		if (bullets[i].alive == 1) {
			
			bullets[i].y -= 15;
			
			if (bullets[i].y <= 0) {
		
				bullets[i].alive = 0;	
			}
		}
	}

	for(i = 0; i < E_BULLETS; i++) {
	
		if (e_bullets[i].alive == 1) {
			
			e_bullets[i].y += 15;
			
			if (e_bullets[i].y <= 0) {
		
				e_bullets[i].alive = 0;	
			}
		}
	}

	return 0;
}

void move_invaders_down() {

	int i,j;

	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
		
			invaders[i][j].y += E_HEIGHT;
		}
	}
			
	speed++;
}

int move_invaders(int speed) {
	
	int i,j;

	if (direction == left) {
	
		for (i = 0; i < 10; i++) {
		
			for (j = 0; j < 5; j++) {
			
				if (invaders[j][i].alive == 1) {
	
					if (invaders[j][i].x <= 0) {
					
						direction = right;
						move_invaders_down();
						return 0;
					}

					invaders[j][i].x -= speed;
				}
			}
		}

	} else if (direction == right) {
	
		for (i = 9; i >= 0; i--) {
		
			for (j = 0; j < 5; j++) {
			
				if (invaders[j][i].alive == 1) {
				
					if (invaders[j][i].x + E_WIDTH >= WIDTH) {
				
						direction = left;
						move_invaders_down();
						return 0;
					}
	
					invaders[j][i].x += speed;
				}
			}
		}
	}

	return 0;
}

void move_player(int direction) {

	if (direction == 0) {
			
		if (player.x > 0) {
			
			player.x -= 10;
		}

	} else if (direction == 1) {

		if (player.x + P_WIDTH < WIDTH){

			player.x += 10;
		}
	}
}

int collision(int i, int j, int k) {

	if (bullets[k].y + B_HEIGHT < invaders[i][j].y) {
					
		return 0;
	}
				
	if (bullets[k].y > invaders[i][j].y + E_HEIGHT) {
					
		return 0;
	}
				
	if (bullets[k].x > invaders[i][j].x + E_WIDTH) {
					
		return 0;
	}
			
	if (bullets[k].x + B_WIDTH < invaders[i][j].x) {
					
		return 0;
	}

	return 1;
}

void enemy_collision() {

	int i,j,k,c;

	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
			
			if (invaders[i][j].alive == 1) {
			
				for (k = 0; k < P_BULLETS; k++) {
			
					if (bullets[k].alive == 1) {
					
						c = collision(i,j,k);
				
						if (c == 1) {
				
							invaders[i][j].alive = 0;
							bullets[k].alive = 0;
							bullets[k].x = 0;
							bullets[k].y = 0;
						}
					}
				}
			}
		}
	}
}

void player_shoot() {

	int i;

	for (i = 0; i < P_BULLETS; i++) {
				
		if (bullets[i].alive == 0) {
			
			bullets[i].x = player.x + (P_WIDTH / 2);
			bullets[i].y = player.y;
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
			
			if (invaders[i][j].alive == 1 && player.x == invaders[i][j].x) {
				 
				for (k = 0; k < E_BULLETS; k++) {
		
					if (e_bullets[i].alive == 0) {
			
						e_bullets[k].x = invaders[i][j].x;
						e_bullets[k].y = invaders[i][j].y;
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
			
			printf("fill rectangle faliled in func drawball()");
		}
		
		draw_invaders(10, 10);
		draw_player(player.x);
		draw_bullets();
		enemy_collision();	
		enemy_ai();
		move_invaders(speed);
		move_bullets();

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
