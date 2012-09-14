
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"

#define WIDTH 800 
#define HEIGHT 600
#define E_WIDTH 30
#define E_HEIGHT 30
#define P_WIDTH 30
#define P_HEIGHT 7
#define B_WIDTH 5
#define B_HEIGHT 15
#define P_BULLETS 1
#define E_BULLETS 5
#define BASE 4
#define BASE_WIDTH 60
#define BASE_HEIGHT 40
#define DAMAGE 100

/*TODO 
* Comment the hell out of this. Especially the per pixel hit detection for the bases
* Add in flying saucer
* Add in scoreing system
* Update enemy AI
*/

enum colour_t {red, green, purple};
enum direction_t {left, right};

struct score_t {

	int shots;
	int points;
};

struct saucer_t {
	
	SDL_Rect hitbox;
	int alive;
	int points;
	enum direction_t direction;
};

struct enemy_t {

	SDL_Rect hitbox;
	enum colour_t colour;
	int alive;
	int points;
};

struct invaders_t {

	struct enemy_t enemy[5][10];
	enum direction_t direction;
	int speed;
};

struct damage_t {
	
	SDL_Rect area;
	int x;
	int y;
	int set;
};

struct base_t {

	SDL_Rect hitbox;
	struct damage_t damage[DAMAGE];
};

struct player_t {

	SDL_Rect hitbox;
	int lives;
};

struct bullet_t {
	
	SDL_Rect hitbox;
	int alive;
};

//global variables, for convenience.
static SDL_Surface *screen;
struct score_t score;
struct invaders_t invaders;
struct saucer_t saucer;
struct base_t base[BASE];
struct player_t player;
struct bullet_t bullets[P_BULLETS];
struct bullet_t e_bullets[E_BULLETS];

//Initialize the score structure
void init_score() {

	score.shots = 0;
	score.points = 0;
}

//Initialize the enemies starting positions, direction, speed and colour
void init_invaders() {
	
	invaders.direction = right;
	invaders.speed = 1;

	int i,j;
	int x = 100;
	int y = 30;
	
	for (i = 0; i < 5; i++) {
	
		for (j = 0; j < 10; j++) {
		
			invaders.enemy[i][j].alive = 1;
			invaders.enemy[i][j].hitbox.x = x;
			invaders.enemy[i][j].hitbox.y = y;
			invaders.enemy[i][j].hitbox.w = E_WIDTH;
			invaders.enemy[i][j].hitbox.h = E_HEIGHT;
			
			x += E_WIDTH + 15; // gap size
			
			if (i == 0) {
				
				invaders.enemy[i][j].colour = purple;
				invaders.enemy[i][j].points = 30;
	
			} else if (i >= 1 && i < 3) {
			
				invaders.enemy[i][j].colour = green;
				invaders.enemy[i][j].points = 20;
		
			} else {
		
				invaders.enemy[i][j].colour = red;
				invaders.enemy[i][j].points = 10;
			}
		}
		
		x = 100; //reset line
		y += E_HEIGHT + 15;
	}

}

//Initialize the player starting position and dimentions
void init_player() {

	player.hitbox.x = (WIDTH / 2) - (P_WIDTH / 2);
	player.hitbox.y = HEIGHT - (P_HEIGHT + 10);
	player.hitbox.w = P_WIDTH;
	player.hitbox.h = P_HEIGHT;
	player.lives = 3;
}

//Initialize the bases starting position and dimentions
void init_bases() {

	int i,j;
	int x = 100;
	int y = 500;

	for (i = 0; i < BASE; i++) {
		
		base[i].hitbox.x = x;
		base[i].hitbox.y = y;
		base[i].hitbox.w = BASE_WIDTH;
		base[i].hitbox.h = BASE_HEIGHT;
		
		for (j = 0; j < DAMAGE; j++) {
			
			base[i].damage[j].x = 0;
			base[i].damage[j].y = 0;
			base[i].damage[j].set = 0;
		}

		x += 180; //distance apart
	}
}

//Initialize the player bullets dimentions
void init_bullets(struct bullet_t b[], int max) {

	int i;

	for (i = 0; i < max; i++) {
	
		b[i].alive = 0;
		b[i].hitbox.x = 0;
		b[i].hitbox.y = 0;
		b[i].hitbox.w = B_WIDTH;
		b[i].hitbox.h = B_HEIGHT;
	}
}

//Initialize the saucer position and dimentions
void init_saucer() {

	saucer.hitbox.x = 0;	
	saucer.hitbox.y	= 0;
	saucer.hitbox.w	= 30;
	saucer.hitbox.h = 20;
	saucer.alive = 0;
	saucer.points = 0;
	saucer.direction = right;
}

//Draw the background
void draw_background () {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	SDL_FillRect(screen,&src,0);
}

//Draw the saucer if its alive
void draw_saucer() {

	if (saucer.alive == 1) {
		
		Uint8 c = SDL_MapRGB(screen->format, 255, 0, 0);
		SDL_FillRect(screen, &saucer.hitbox, c);
	}
}

//Draw the invaders if there alive
void draw_invaders() {

	SDL_Rect src;
	Uint8 c;
	int i,j;
	
	src.w = E_WIDTH;
	src.h = E_HEIGHT;

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

			src.x = invaders.enemy[i][j].hitbox.x;
			src.y = invaders.enemy[i][j].hitbox.y;
			
			if (invaders.enemy[i][j].alive == 0) {
				
				continue;
			}
			
			SDL_FillRect(screen, &src, c);
		}
	}
}

//Draw the bases
void draw_bases() {

	//Uint8 c = SDL_MapRGB(screen->format, 255, 150, 0);
	int i;

	for(i = 0; i < BASE; i++) {
		
		SDL_FillRect(screen, &base[i].hitbox, 3);
	}
}

//Draw the player
void draw_player() {

	Uint8 c = SDL_MapRGB(screen->format, 255, 255, 0);

	SDL_FillRect(screen, &player.hitbox, c);
}

//Draw both the enemy and the players bullets if there alive
void draw_bullets(struct bullet_t b[], int max) {

	//Uint8 c = SDL_MapRGB(screen->format, 255, 255, 255);
	int i;


	for (i = 0; i < max; i++) {
	
		if (b[i].alive == 1) {
		
			SDL_FillRect(screen, &b[i].hitbox, 255);
		}
	}
}

//Draw the damage on top of the base's
void draw_damage() {

	int i,j;

	for (i = 0; i < BASE; i++) {
	
		for (j = 0; j < DAMAGE; j++) {
			
			if (base[i].damage[j].set == 1) {
			
				SDL_FillRect(screen, &base[i].damage[j].area, 0);
			}
		}
	}
}

//Populate the base's dmage array with a rectange pin-pointing where damages is to be used by draw_damage function
int set_base_damage(struct base_t *base, int x, int y, int w, int h) {

	int i;

	for ( i = 0; i < DAMAGE; i++) {
	
		//found a free damage record
		if (base->damage[i].set == 0) {
			
			base->damage[i].set = 1;
			base->damage[i].area.x = x;
			base->damage[i].area.y = y;
			base->damage[i].area.w = w;
			base->damage[i].area.h = h;
			return 0;
		}
	}
	
	return 1;
}

//Print currnet score to stdout
void print_score() {

	printf("shot %d. score = %d\n", score.shots, score.points);
}

//Update positions of both enemy and player bullets on screen
int move_bullets(struct bullet_t b[], int max, int speed) {
	
	int i;

	for(i = 0; i < max; i++) {
	
		if (b[i].alive == 1) {
			
			b[i].hitbox.y += speed;
			
			if (b[i].hitbox.y <= 0) {
		
				b[i].alive = 0;	
			}
			
			if (b[i].hitbox.y + b[i].hitbox.h >= HEIGHT) {
		
				b[i].alive = 0;	
			}
		}
	}

	return 0;
}

//Move invaders down one space once the reach the edge
void move_invaders_down() {

	int i,j;

	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
		
			invaders.enemy[i][j].hitbox.y += E_HEIGHT;
		}
	}
			
	invaders.speed++;
}

//Move invaders based on there current direction
int move_invaders(int speed) {
	
	int i,j;

	if (invaders.direction == left) {
	
		for (i = 0; i < 10; i++) {
		
			for (j = 0; j < 5; j++) {
			
				if (invaders.enemy[j][i].alive == 1) {
	
					if (invaders.enemy[j][i].hitbox.x <= 0) {
					
						invaders.direction = right;
						move_invaders_down();
						return 0;
					}

					invaders.enemy[j][i].hitbox.x -= invaders.speed;
				}
			}
		}

	} else if (invaders.direction == right) {
	
		for (i = 9; i >= 0; i--) {
		
			for (j = 0; j < 5; j++) {
			
				if (invaders.enemy[j][i].alive == 1) {
				
					if (invaders.enemy[j][i].hitbox.x + E_WIDTH >= WIDTH) {
				
						invaders.direction = left;
						move_invaders_down();
						return 0;
					}
	
					invaders.enemy[j][i].hitbox.x += invaders.speed;
				}
			}
		}
	}

	return 0;
}

//Move player left or right
void move_player(int direction) {

	if (direction == 0) {
			
		if (player.hitbox.x > 0) {
			
			player.hitbox.x -= 10;
		}

	} else if (direction == 1) {

		if (player.hitbox.x + player.hitbox.w < WIDTH){

			player.hitbox.x += 10;
		}
	}
}

//Move saucer based on there current direction
void move_saucer() {

	if (saucer.alive == 1) {

		if (saucer.direction == left) {
		
			saucer.hitbox.x -= 5;

			if (saucer.hitbox.x < 0) {
				
				saucer.alive = 0;
				saucer.hitbox.x = 0;
				saucer.direction = right;
			}
		}

		if (saucer.direction == right) {
		
			saucer.hitbox.x += 5;

			if (saucer.hitbox.x + saucer.hitbox.w > WIDTH) {
			
				saucer.alive = 0;
				saucer.hitbox.x = WIDTH - saucer.hitbox.w;
				saucer.direction = left;
			}
		}
	}
}

//Detect any collision between any two non rotated rectangles
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

//Populate base's damages array from player or enemy bullet collisions
void bullet_base_damage(struct base_t *base, struct bullet_t *bullet, int l) {
	
	draw_bases();
	draw_damage();
	
	int i;
	int x,y;
	SDL_LockSurface(screen);
	Uint8 *raw_pixels;

	raw_pixels = (Uint8 *) screen->pixels;
	
	int pix_offset;

	//bottom
	if (l == 0) {
		
		x = bullet->hitbox.x;
		y = base->hitbox.y + base->hitbox.h;

		for(i = 0; i < base->hitbox.h; i++) {
			
			pix_offset = y * screen->pitch  + x;	
		
			//found part part of the base
			if (raw_pixels[pix_offset] == 3) {
					
				bullet->alive = 0;
			
				set_base_damage(base, x, y - (B_HEIGHT -1), bullet->hitbox.w, bullet->hitbox.h);

				break;
			}
			
			y--;
		}
	}
	
	//top
	if (l == 1) {

		x = bullet->hitbox.x;
		y = base->hitbox.y;
		
		for(i = 0; i < base->hitbox.h; i++) {
			
			pix_offset = y * screen->pitch  + x;	
		
			//found part part of the base
			if (raw_pixels[pix_offset] == 3) {
					
				bullet->alive = 0;
			
				set_base_damage(base, x, y -1, bullet->hitbox.w, bullet->hitbox.h);
				
				break;
			}
			
			y++;
		}
	}

	SDL_UnlockSurface(screen);
}

//Populate base's damages array from invader collisions
void enemy_base_damage(struct enemy_t *enemy, struct base_t *base) {
	
	draw_bases();
	draw_damage();
	
	SDL_LockSurface(screen);
	Uint8 *raw_pixels;

	raw_pixels = (Uint8 *) screen->pixels;
	
	int pix_offset;
	int i,x,y;
	int dist;

	if (invaders.direction == right) {
		
		x = enemy->hitbox.x;
		y = enemy->hitbox.y + enemy->hitbox.h;
		y--;//1 pix up
		
		for (i = 0; i < enemy->hitbox.h; i++) {
			
			dist = enemy->hitbox.x - base->hitbox.x;
			
			if (dist > 0) {
				
				pix_offset = y * screen->pitch  + x;
			
				if (raw_pixels[pix_offset] == 3) {
				
					set_base_damage(base, x - dist, enemy->hitbox.y, enemy->hitbox.w + dist, enemy->hitbox.h);			
				}

				y--;
			}
		}
	}

	if (invaders.direction == left) {
		
		x = enemy->hitbox.x + enemy->hitbox.w;
		y = enemy->hitbox.y + enemy->hitbox.h;
		y--; //exact bottom corner of enemy hitbox;
		x--; //1 pix to the left
		
		for (i = 0; i < enemy->hitbox.h; i++) {
			
			dist = (base->hitbox.x + base->hitbox.w) -(enemy->hitbox.x + enemy->hitbox.w);
			
			if (dist > 0) {
				
				pix_offset = y * screen->pitch  + x;
			
				if (raw_pixels[pix_offset] == 3) {
				
					set_base_damage(base, enemy->hitbox.x, enemy->hitbox.y, enemy->hitbox.w + dist, enemy->hitbox.h);			
				}

				y--;
			}
		}
	}

	SDL_UnlockSurface(screen);
	draw_invaders();
}

//Look for collisions based on enemy and base rectangles
void enemy_base_collision() {

	int i,j,k,c;

	for (i = 0; i < 5; i++) {

		for (j = 0;  j < 10; j++) {
		
			for (k = 0;  k < BASE; k++) {
			
				if (invaders.enemy[i][j].alive == 1) {
				
					c = collision(invaders.enemy[i][j].hitbox,base[k].hitbox);		
					
					if (c == 1) {
						
						//puts("enemy hit base !");
						enemy_base_damage(&invaders.enemy[i][j], &base[k]);
					}
				}
			}
		}
	}
}

//Look for collisions based on player bullet and invader rectangles
void enemy_hit_collision() {

	int i,j,k,c;
	
	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
			
			if (invaders.enemy[i][j].alive == 1) {
			
				for (k = 0; k < P_BULLETS; k++) {
			
					if (bullets[k].alive == 1) {
						
						c = collision(bullets[k].hitbox, invaders.enemy[i][j].hitbox);
				
						if (c == 1) {
				
							invaders.enemy[i][j].alive = 0;
							bullets[k].alive = 0;
							bullets[k].hitbox.x = 0;
							bullets[k].hitbox.y = 0;
							score.points += invaders.enemy[i][j].points;
							print_score();
						}
					}
				}
			}
		}
	}
}

//Look for collisions based on enemy bullet and player rectangles
void player_hit_collision() {

	int i,c;

	for(i = 0; i < E_BULLETS; i++) {
	
		if (e_bullets[i].alive == 1) {

			c = collision(e_bullets[i].hitbox, player.hitbox);

			if (c == 1) {
		
				//puts("Player Hit !");
			}
		}
	}
}

//Look for collisions based on player bullet and saucer rectangles
void saucer_hit_collision() {

	int i,c;

	if (saucer.alive == 1) {
	
		for(i = 0; i < P_BULLETS; i++) {
	
			c = collision(bullets[i].hitbox, saucer.hitbox);
	
			if (c == 1) {
				
				int r = rand() % 3;
				

				switch (r) {
			
					case 0:
						score.points += 50;
						puts("Saucer Hit 50 points");
						break;

					case 1:
						score.points += 150;
						puts("Saucer Hit 150 points");
						break;

					case 2:
						score.points += 300;
						puts("Saucer Hit 300 points");
						break;

					default:
						puts("Saucer Hit no points");
						break;
				}
				
				print_score();
			}
		}
	}
}

//Look for collisions based on invader and player rectangles
void enemy_player_collision() {

	int i,j,c;

	for(i = 0; i < 5; i++) {

		for(j = 0; j < 10; j++) {
		
			if (invaders.enemy[i][j].alive == 1) {
					
				c = collision(player.hitbox, invaders.enemy[i][j].hitbox);

				if (c == 1) {
				
					invaders.speed = 0;
				}
			}
		}
	}
}

//Look for collisions based on bullet and base rectangles
void bullet_base_collision(struct bullet_t b[], int max, int l) {

	int i,j,c;

	for (i = 0; i < max; i++) {
		
		for (j = 0; j < BASE; j++) {
	
			if (b[i].alive == 1) {
			
				c = collision(b[i].hitbox, base[j].hitbox);

				if (c == 1) {
					
					//printf("bullet hit base !, %d\n",l);
					bullet_base_damage(&base[j], &b[i],l);
				}
			}
		}
	}
}

//Shoot bullet/s from player
void player_shoot() {

	int i;
	
	for (i = 0; i < P_BULLETS; i++) {
				
		if (bullets[i].alive == 0) {
			
			//count number of shots fired
			score.shots++;

			bullets[i].hitbox.x = player.hitbox.x + (P_WIDTH / 2);
			//-5 at the end so the bullets ends closer to the top of the screen due to 30px speed
			bullets[i].hitbox.y = player.hitbox.y - (bullets[i].hitbox.h - 5);
			bullets[i].alive = 1;
			break;
		}
	}
}

//Determin when saucer should apper
void saucer_ai() {

	//every 20 shots
	if (score.shots != 0 && score.shots % 2 == 0) {
	
		saucer.alive = 1;
	}
}

//Determin when invaders should shoot
void enemy_ai() {

	int i,j,k;

	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
			
		       if (invaders.enemy[i][j].alive == 1 && player.hitbox.x == invaders.enemy[i][j].hitbox.x) {
				 
				for (k = 0; k < E_BULLETS; k++) {
		
					if (e_bullets[i].alive == 0) {
			
						e_bullets[k].hitbox.x = invaders.enemy[i][j].hitbox.x;
						e_bullets[k].hitbox.y = invaders.enemy[i][j].hitbox.y;
						e_bullets[k].alive = 1;
						break;
					}
				}		
			}
		}
	}
}

//Main program
int main() {
	
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
	init_bases();
	init_player();
	init_saucer();
	init_bullets(bullets, P_BULLETS);
	init_bullets(e_bullets, E_BULLETS);
		
	/* Animate */
	while (quit == 0) {
		
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
							saucer_ai();
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

		draw_background();
		draw_player();
		draw_bases();
		draw_damage();
		draw_invaders();
		draw_saucer();
		draw_bullets(bullets, P_BULLETS);
		draw_bullets(e_bullets, E_BULLETS);
		enemy_hit_collision();
		player_hit_collision();
		enemy_base_collision();
		saucer_hit_collision();
		bullet_base_collision(e_bullets, E_BULLETS, 1);
		bullet_base_collision(bullets, P_BULLETS, 0);
		enemy_player_collision();
		enemy_ai();
		move_invaders(invaders.speed);
		move_saucer();
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
