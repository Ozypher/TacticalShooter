#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_entity.h"
#include "math.h"
#include "collision.h"
#include "SDL_mixer.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	Sprite *player;
	Sprite *enemy;
	Sprite *tracer;
	Sprite *barrel;
	Sprite *lightbulb;
	Sprite *wall;
	Sprite *wall2;
	Sprite *electric;
	Sprite *hostageZone;
	Sprite *killZone;
	Sprite *hostage;
	Sprite *door;
	Sprite *breached;
	Sprite *mission;
	Sprite *check;
	Sprite *menu;
	Sprite *screen;
	Sprite *enemyOne;
	Sprite *enemyTwo;
	Sprite *enemyThree;
	Sprite *flutter;
	Sprite *moBO;
    int mx,my;
    float mf = 0;
	float pf = 0;
	float ef = 0;
	float gdistance = 0;
	float distance = 0;
	float gangle = 0;
	float angle = 0;
	float truDistance = 0;
	Vector2D origin;
	Vector2D *originpoint;
	Vector3D vecangle;
	Vector2D scalevec;
	Vector3D *vecturn;
	Vector2D *scale;
	Vector2D scaleWall1;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
	Entity *playerEnt;
	Vector4D colorShiftTracer;
	Vector4D colorShiftBossOne;
	Vector4D *colorshiftturn;
	Entity *enemyEnt;
	Entity *enemyEnt1;
	Entity *enemyEnt2;
	Entity *enemyEnt3;
	Entity *tracerEnt;
	Entity *barrelEnt;
	Entity *lightbulbEnt;
	Entity *wallEnt1;
	Entity *wallEnt2;
	Entity *wallEnt3;
	Entity *wallEnt4;
	Entity *wallEnt5;
	Entity *wallEnt6;
	Entity *wallEnt7;
	Entity *wallEnt8;
	Entity *wallEnt9;
	Entity *wallEnt10;
	Entity *electricBox;
	Entity *killCheck;
	Entity *hostageCheck;
	Entity *hostageEnt;
	Entity *doorEnt;
	Entity *bossOne;
	Entity *bossTwo;
	Vector3D vecRightAng;
	float enemydead = 0;
	float levelNumber = 0;
	float wallBreached = 0;
	float electricboxalive = 1;
	float hostageSaved = 0;
	float menuOn = 0;
	float lightBulbAlive = 1;
	float timeEnemy = 0;
	float MenuState = 0;
	float etf = 0;
	float ethf = 0;
	float eof = 0;
	float fbf = 0;
	float mbf = 0;
	float flutterHP = 20;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1270,
        720,
        1270,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
	colorShiftBossOne.w = 255;
	gf2d_entity_manager_init(30);
	bossOne = gf2d_entity_new();
	bossTwo = gf2d_entity_new();
	tracerEnt = gf2d_entity_new();
	playerEnt = gf2d_entity_new();
	enemyEnt = gf2d_entity_new();
	barrelEnt = gf2d_entity_new();
	wallEnt1 = gf2d_entity_new();
	wallEnt2 = gf2d_entity_new();
	lightbulbEnt = gf2d_entity_new();
	electricBox = gf2d_entity_new();
	hostageEnt = gf2d_entity_new();
	hostageCheck = gf2d_entity_new();
	doorEnt = gf2d_entity_new();
	killCheck = gf2d_entity_new();
	enemyEnt1 = gf2d_entity_new();
	enemyEnt2 = gf2d_entity_new();
	enemyEnt3 = gf2d_entity_new();

	//Music Time
	Mix_Music *gMusic = NULL;
	Mix_Chunk *gknife = NULL;
	Mix_Chunk *gshoot = NULL;
	Mix_Chunk *gpush = NULL;
	Mix_Chunk *gdeath = NULL;
	
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gMusic = Mix_LoadMUS("maintheme.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayMusic(gMusic, -1);
	//
	// Sound Effects Time
	//Load sound effects
	gknife = Mix_LoadWAV("knife.ogg");
	if (gknife == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gshoot = Mix_LoadWAV("bang.ogg");
	if (gshoot == NULL)
	{
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gpush = Mix_LoadWAV("push.ogg");
	if (gpush == NULL)
	{
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gdeath = Mix_LoadWAV("dead.ogg");
	if (gdeath == NULL)
	{
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	//

    sprite = gf2d_sprite_load_image("images/backgrounds/preview16.jpg");
	screen = gf2d_sprite_load_image("images/menumain.png");
	breached = gf2d_sprite_load_image("images/breached.png");
	//crawler
	enemyOne = gf2d_sprite_load_all("images/enemyone.png", 128, 128, 6);
	//spikeyboi
	enemyTwo = gf2d_sprite_load_all("images/enemytwo.png", 128, 128, 12);
	//drone
	enemyThree = gf2d_sprite_load_all("images/enemythree.png", 128, 128, 4);
	//FlutterBoss
	flutter = gf2d_sprite_load_all("images/miniboss.png", 128, 128, 2);
	//MotherBoard
	moBO = gf2d_sprite_load_all("images/finalboss.png", 128, 128, 17);

    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	player = gf2d_sprite_load_all("images/walktrue.png",128,128,2);
	enemy = gf2d_sprite_load_all("images/enemy.png", 128, 128, 1);
	tracer = gf2d_sprite_load_all("images/tracer.png", 24, 128, 1);
	barrel = gf2d_sprite_load_all("images/barrel.png", 128, 128, 1);
	lightbulb = gf2d_sprite_load_all("images/light.png", 128, 128, 1);
	electric = gf2d_sprite_load_all("images/electric box.png", 128, 128, 3);
	wall = gf2d_sprite_load_all("images/breachable.png", 128, 128, 1);
	wall2 = gf2d_sprite_load_all("images/reinforced.png", 128, 128, 1);
	hostage = gf2d_sprite_load_all("images/hostage.png", 128, 128, 1);
	hostageZone = gf2d_sprite_load_all("images/hostagedrop.png", 128, 128, 1);
	door = gf2d_sprite_load_image("images/door.png");
	killZone = gf2d_sprite_load_image("images/end.png");
	mission = gf2d_sprite_load_image("images/missions.png");
	check = gf2d_sprite_load_image("images/checked.png");
	menu = gf2d_sprite_load_image("images/menu.png");
    /*main game loop*/
	hostageEnt->position = vector2d(900, 500);
	playerEnt->speed = 1.0;
	enemyEnt->position = vector2d(150, 150);
	barrelEnt->position = vector2d(780, 500);
	hostageCheck->position = vector2d(300, 300);
	doorEnt->position = vector2d(572, 200);
	lightbulbEnt->position = vector2d(850, 150);
	playerEnt->position = vector2d(300, 300);
	enemyEnt3->speed = 1;
	
	while (!done)
	{
		SDL_Event event;
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		tracerEnt->position = vector2d(mx, my);

		mf += 0.1;
		eof += 0.1;
		etf += 0.1;
		ethf += 0.1;
		fbf += 0.1;
		mbf += 0.1;
		if (fbf >= 17)fbf = 0;
		if (mbf >= 2)mbf = 0;
		if (eof >= 6.0)eof = 0;
		if (etf >= 12.0)etf = 0;
		if (ethf >= 4.0)ethf = 0;
		if (mf >= 1.0)mf = 0;
		if (wallBreached == 0){
			wallEnt1->sprite = wall;
		}
		if (levelNumber == 1){
			sprite = gf2d_sprite_load_image("images/concrete.png");
		}
		gdistance = ((playerEnt->position.x - mx)*(playerEnt->position.x - mx) + (playerEnt->position.y - my) * (playerEnt->position.y - my));
		distance = sqrt(gdistance);
		truDistance = (mx - playerEnt->position.x) + (my - playerEnt->position.y);
		gangle = (playerEnt->position.y - my) / (playerEnt->position.x - mx);
		angle = atan(gangle);
		vecangle = vector3d(64, 64, (angle*(57.296) + 90));
		scalevec = vector2d(0.25, distance / 100);
		scale = &scalevec;
		origin = vector2d(0, 64);
		originpoint = &origin;
		//slog("truDistance is: %f", truDistance);
		// check if player is 'ahead' of cursor, if they are, flip the rotation, also check if its on top, before doing so.
		if (playerEnt->position.x >= mx){
			if (!(playerEnt->position.y == my && playerEnt->position.x == mx)){
				vecangle.z += 180;
			}
		}
		if (distance > -2 && distance < 2){
			vecangle.z = 0;
		}

		vecturn = &vecangle;
		//slog("The angle is updated to : %f,%f,%f", vecangle.x,vecangle.y,vecangle.z);
		//slog("the mouse is in : %i, %i", mx, my);
		//slog("distance between is : %f", distance);
		// sprint and rifle mode
		if (distance > 250){
			playerEnt->speed = 3.0;
			colorShiftTracer = vector4d(0, 255, 0, 255);
		}
		// run and shotgun
		if (distance <= 250 && distance >= 60){
			playerEnt->speed = 2.0;
			colorShiftTracer = vector4d(255, 255, 0, 255);
		}
		// walk and knife
		if (distance < 60){
			playerEnt->speed = 1.0;
			colorShiftTracer = vector4d(255, 0, 0, 255);
		}
		colorshiftturn = &colorShiftTracer;
		// GRABBING CODE STARTS HERE
		if (collide_circle(tracerEnt->position, 5, barrelEnt->position, 32)){
			if (keys[SDL_SCANCODE_SPACE]){
				barrelEnt->position = tracerEnt->position;
			}
		}
		// GRABBING CODE ENDS HERE
		if (keys[SDL_SCANCODE_R]){
			playerEnt->position = vector2d(400, 400);
		}
		// Hit Box Code
		SDL_Rect playerBox = {
			playerEnt->position.x - 45,
			playerEnt->position.y - 45,
			90,
			90
		};
		SDL_Rect wall1Box = {
			wallEnt1->position.x -15,
			wallEnt1->position.y,
			256,
			18 };
		SDL_Rect wall2Box = {
			wallEnt2->position.x+64,
			wallEnt2->position.y-110,
			18,
			230
		};
		SDL_Rect hostageBox = {
			hostageEnt->position.x - 64,
			hostageEnt->position.y - 64,
			128,
			128
		};
		SDL_Rect hostageArea = {
			hostageCheck->position.x,
			hostageCheck->position.y,
			128,
			128
		};
		SDL_Rect enemyBoxOne = {
			enemyEnt->position.x - 64,
			enemyEnt->position.y - 64,
			128,
			128
		};
		SDL_Rect doorBox = {
			doorEnt->position.x + 64,
			doorEnt->position.y,
			18,
			128
		};
		SDL_Rect KillBox = {
			killCheck->position.x,
			killCheck->position.y,
			128,
			128
		};
		SDL_Rect Mouse = {
			mx,
			my,
			16,
			16
		};
		SDL_Rect Yes = {
			344,
			237,
			408,
			136 };
		SDL_Rect electricBOXBOX = {
			electricBox->position.x - 64,
			electricBox->position.y - 64,
			64,
			64
		};
		SDL_Rect No = {
			359, 403,
			392, 115
		};
		SDL_Rect enemyBoxOna = {
			enemyEnt1->position.x - 64,
			enemyEnt1->position.y - 64,
			128,
			128
		};
		SDL_Rect enemyBoxTwo = {
			enemyEnt2->position.x - 64,
			enemyEnt2->position.y - 64,
			128,
			128
		};
		SDL_Rect enemyBoxThree = {
			enemyEnt3->position.x -45,
			enemyEnt3->position.y -45,
			90,
			90
		};
		SDL_Rect BossOneBox = {
			bossOne->position.x -45,
			bossOne->position.y -45,
			90,
			90
		};
		// collision testing
		if (collide_rect(Mouse, enemyBoxOne))slog("ping");
		if (collide_rect(Mouse, enemyBoxOna))slog("ping");
		if (collide_rect(Mouse, enemyBoxTwo))slog("ping");
		if (collide_rect(Mouse, enemyBoxThree))slog("ping");
		if (collide_rect(Mouse, BossOneBox))slog("ping"); 
		if (collide_rect(Mouse, playerBox))slog("ping");
		if (collide_rect(Mouse, wall1Box))slog("ping");
		if (collide_rect(Mouse, wall2Box))slog("ping");
		if (collide_rect(Mouse, doorBox))slog("ping");
		// SHOOTING CODE STARTS HERE
		if (distance > 250){
			while (SDL_PollEvent(&event)) // check to see if an event has happened
			{
				switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					Mix_PlayChannel(-1, gshoot, 0);
					if (collide_rect(Mouse, enemyBoxOne)){
						enemyEnt->health += 1;
						slog("Bang!");
						break;
					}
					if (collide_rect(Mouse, BossOneBox)){
						flutterHP -= 1;
						colorShiftBossOne.z += 12.75;
						slog("Bang!");
						break;
					}
					if (collide_rect(Mouse, enemyBoxOna)){
						enemyEnt1->health += 1;
						slog("Bang!");
						break;
					}
					if (collide_rect(Mouse, enemyBoxTwo)){
						enemyEnt2->health += 1;
						slog("Bang!");
						break;
					}
					if (collide_rect(Mouse, enemyBoxThree)){
						enemyEnt3->health += 1;
						slog("Bang!");
						break;
					}
				}
			}
		}
		if (distance < 250 && distance > 60){
			if (collide_rect(enemyBoxTwo, Mouse)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						slog("Whoosh!");
						if (playerEnt->position.x >= enemyEnt2->position.x &&  playerEnt->position.y > enemyEnt2->position.y){
							enemyEnt2->position.x -= 80.0;
							enemyEnt2->position.y -= 80.0;
						}
						if (playerEnt->position.x < enemyEnt2->position.x &&  playerEnt->position.y > enemyEnt2->position.y){
							enemyEnt2->position.x += 80.0;
							enemyEnt2->position.y -= 80.0;
						}
						if (playerEnt->position.x < enemyEnt2->position.x &&  playerEnt->position.y < enemyEnt2->position.y){
							enemyEnt2->position.x += 80.0;
							enemyEnt2->position.y += 80.0;
						}
						if (playerEnt->position.x > enemyEnt2->position.x &&  playerEnt->position.y <= enemyEnt2->position.y){
							enemyEnt2->position.x -= 80.0;
							enemyEnt2->position.y += 80.0;
						}
						Mix_PlayChannel(-1, gpush, 0);
						break;
					}
				}
			}
			if (collide_rect(Mouse, BossOneBox)){
				while (SDL_PollEvent(&event)){
					switch (event.type){
					case SDL_MOUSEBUTTONDOWN:
						if (playerEnt->position.x >= bossOne->position.x &&  playerEnt->position.y > bossOne->position.y){
							bossOne->position.x -= 80.0;
							bossOne->position.y -= 80.0;
						}
						if (playerEnt->position.x < bossOne->position.x &&  playerEnt->position.y > bossOne->position.y){
							bossOne->position.x += 80.0;
							bossOne->position.y -= 80.0;
						}
						if (playerEnt->position.x < bossOne->position.x &&  playerEnt->position.y < bossOne->position.y){
							bossOne->position.x += 80.0;
							bossOne->position.y += 80.0;
						}
						if (playerEnt->position.x > bossOne->position.x &&  playerEnt->position.y <= bossOne->position.y){
							bossOne->position.x -= 80.0;
							bossOne->position.y += 80.0;
						}
						Mix_PlayChannel(-1, gpush, 0);
						break;
					}
				}
			}
			if (collide_rect(Mouse, wall1Box)){
				slog("yes.");
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						slog("BREACH!");
						wallEnt1->sprite = breached;
						wallBreached = 1;
						Mix_PlayChannel(-1, gknife, 0);
						break;
					}
				}
			}
		}
		else{
			if (collide_rect(Mouse, enemyBoxOne)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						enemyEnt->health += 3;
						break;
					}
				}
			}
			if (collide_rect(Mouse, enemyBoxOna)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						enemyEnt1->health += 3;
						break;
					}
				}
			}
			if (collide_rect(Mouse, enemyBoxTwo)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						enemyEnt2->health += 3;
						break;
					}
				}
			}
			if (collide_rect(Mouse, enemyBoxThree)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						enemyEnt3->health += 3;
						break;
					}
				}
			}
		}
		if (collide_circle(tracerEnt->position, 5, vector2d(lightbulbEnt->position.x + 64, lightbulbEnt->position.y + 64), 32)){
			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					lightBulbAlive = 0;
					lightbulbEnt->position = vector2d(13000, 130000);
					break;
				}
			}
		}
		if (levelNumber == 0 || levelNumber == 1|| levelNumber == 1.5){
			if (collide_circle(enemyEnt->position, 32, lightbulbEnt->position, 1200)){
				if (collide_rect(playerBox, enemyBoxOne)){
					playerEnt->position = vector2d(30000000000000, 300000000000);
					Mix_PlayChannel(-1, gdeath, 0);
				}
			}
			if (collide_circle(enemyEnt1->position, 32, lightbulbEnt->position, 1200)){
				if (collide_rect(playerBox, enemyBoxOna)){
					playerEnt->position = vector2d(30000000000000, 300000000000);
					Mix_PlayChannel(-1, gdeath, 0);
				}
			}
			if (collide_circle(enemyEnt2->position, 32, lightbulbEnt->position, 1200)){
				if (collide_rect(playerBox, enemyBoxTwo)){
					playerEnt->position = vector2d(30000000000000, 300000000000);
					Mix_PlayChannel(-1, gdeath, 0);
				}
			}
			if (collide_circle(enemyEnt3->position, 32, lightbulbEnt->position, 1200)){
				if (collide_rect(playerBox, enemyBoxThree)){
					playerEnt->position = vector2d(30000000000000, 300000000000);
					Mix_PlayChannel(-1, gdeath, 0);
				}
			}
			if (collide_rect(BossOneBox, playerBox)){
				playerEnt->position = vector2d(3000000000000, 300000000000000);
				Mix_PlayChannel(-1, gdeath, 0);
			}
		}

		// SHOOTING CODE ENDS HERE
		// WALL COLLISION START
		while (collide_circle(playerEnt->position, 32, barrelEnt->position, 32)){
			playerEnt->speed*=-1;
			break;
		}
		while (collide_rect(playerBox,wall1Box)){
			if (wallBreached == 0){
				playerEnt->speed *= -1;
			}
			break;
		}
		while (collide_rect(playerBox, wall2Box)){
			playerEnt->speed *= -1;
			break;
		}
		while (collide_rect(playerBox, doorBox)){
			playerEnt->speed *= -1;
			if (keys[SDL_SCANCODE_E]){
				doorEnt->position = vector2d(3000, 3000);
			}
			break;
		}
		while (collide_rect(playerBox, hostageBox)){
			hostageEnt->position = playerEnt->position;
			break;
		}
		if (collide_rect(hostageBox, hostageArea)){
			levelNumber = 1.5;
			wallBreached = 0;
			hostageSaved = 1;
			lightBulbAlive = 1;
			MenuState = 3;
			enemyEnt1->position = vector2d(10, 10);
			enemyEnt2->position = vector2d(600, 600);
			enemyEnt3->position = vector2d(1000, 500);
		}
		if (collide_rect(enemyBoxOne, playerBox)){
			slog("ping!");
		}
		if (collide_rect(electricBOXBOX, playerBox)){
			electricboxalive = 0;
		}
		// WALL COLLISION STOP
		if (keys[SDL_SCANCODE_W]){
			if (mx >= playerEnt->position.x){
				playerEnt->position.x += playerEnt->speed;
			}
			if (mx < playerEnt->position.x){
				playerEnt->position.x -= playerEnt->speed;
			}
			if (my >= playerEnt->position.y){
				playerEnt->position.y += playerEnt->speed;
			}
			if (my < playerEnt->position.y){
				playerEnt->position.y -= playerEnt->speed;
			}
			//slog("The position has been updated to %f", playerEnt->position);
		}
		if (keys[SDL_SCANCODE_S]){
			playerEnt->speed *= -1;
			if (mx >= playerEnt->position.x){
				playerEnt->position.x += playerEnt->speed;
			}
			if (mx < playerEnt->position.x){
				playerEnt->position.x -= playerEnt->speed;
			}
			if (my >= playerEnt->position.y){
				playerEnt->position.y += playerEnt->speed;
			}
			if (my < playerEnt->position.y){
				playerEnt->position.y -= playerEnt->speed;
			}
			//slog("The position has been updated to %f", playerEnt->position);
		}
		if (lightBulbAlive == 1 && levelNumber == 1){
			if (enemyEnt2->position.x >= playerEnt->position.x){
				enemyEnt2->position.x -= .5;
			}
			if (enemyEnt2->position.x < playerEnt->position.x){
				enemyEnt2->position.x += .5;
			}
			if (enemyEnt2->position.y >= playerEnt->position.y){
				enemyEnt2->position.y -= .5;
			}
			if (enemyEnt2->position.y < playerEnt->position.y){
				enemyEnt2->position.y += .5;
			}
		}
		if (levelNumber == 1.5 && MenuState == 3.5 && !(flutterHP<=0)){
			if (bossOne->position.x >= playerEnt->position.x){
				bossOne->position.x -= 1.5;
			}
			if (bossOne->position.x < playerEnt->position.x){
				bossOne->position.x += 1.5;
			}
			if (bossOne->position.y >= playerEnt->position.y){
				bossOne->position.y -= 1.5;
			}
			if (bossOne->position.y < playerEnt->position.y){
				bossOne->position.y += 1.5;
			}
		}
		if (flutterHP <= 0){
			bossOne->position = vector2d(5000000000, 5000000000);
			levelNumber = 1;
			MenuState = 4;
		}
		if (lightBulbAlive == 1){
			timeEnemy += 0.1;
			if (timeEnemy >= 20){
				timeEnemy = 0;
				enemyEnt3->speed *= -1;
			}
			enemyEnt3->position.y += enemyEnt3->speed;
		}
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        
		//backgrounds drawn first
		if (pf >= 2.0)pf = 0;
		gf2d_sprite_draw_image(sprite, vector2d(0, 0));
		gf2d_sprite_draw_image(sprite, vector2d(780, 585));
		gf2d_sprite_draw_image(sprite, vector2d(780, 0));
		gf2d_sprite_draw_image(sprite, vector2d(0, 585));
		//level design here
		while (levelNumber == 0 && MenuState == 0){
			gf2d_sprite_draw_image(screen, vector2d(0, 0));
			break;
		}
		if (lightBulbAlive == 0){
			etf = 0;
			ethf = 0;
			eof = 0;
		}
		// debug purposes
		if (keys[SDL_SCANCODE_0]){
			MenuState = 1;
		}
		while (levelNumber == 0 && MenuState == 1){
			gf2d_sprite_draw_image(screen, vector2d(0, 0));
			screen = gf2d_sprite_load_image("images/menone.png");
			if (keys[SDL_SCANCODE_RETURN]){
				MenuState = 2;
			}
			break;
		}
		while (levelNumber == 0 && MenuState == 2){
			enemyEnt1->position = vector2d(3000000000, 30000000);
			enemyEnt2->position = vector2d(3000000000, 30000000);
			enemyEnt3->position = vector2d(3000000000, 30000000);
			wallEnt1->position = vector2d(700, 585);
			scaleWall1 = vector2d(2, 1);
			wallEnt2->position = vector2d(572, 457);
			vecRightAng = vector3d(64, 64, 90);
			if (enemyEnt->health < 3){
				gf2d_sprite_draw(enemyTwo, vector2d(enemyEnt->position.x - 64, enemyEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)etf);
				enemyEnt->position = vector2d(750, 300);
			}
			else{
				enemyEnt->position = vector2d(2000, 2000);
			}
			gf2d_sprite_draw(door, doorEnt->position,NULL,NULL,&vecRightAng,NULL,NULL,NULL);
			gf2d_sprite_draw(lightbulb, lightbulbEnt->position, NULL, NULL, NULL, NULL, NULL, NULL);
			gf2d_sprite_draw(hostageZone, vector2d(hostageCheck->position.x, hostageCheck->position.y), NULL, NULL, NULL, NULL, NULL, NULL);
			gf2d_sprite_draw(hostage, vector2d(hostageEnt->position.x - 64, hostageEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)mf);
			gf2d_sprite_draw(wall2, vector2d(wallEnt2->position.x - 64, wallEnt2->position.y - 64), &scaleWall1, NULL, &vecRightAng, NULL, NULL, (int)mf);
			gf2d_sprite_draw(wall2, vector2d(wallEnt2->position.x - 64, wallEnt2->position.y - 448), &scaleWall1, NULL, &vecRightAng, NULL, NULL, (int)mf);
			gf2d_sprite_draw(barrel, vector2d(barrelEnt->position.x - 64, barrelEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)mf);
			break;
		}
		while (levelNumber == 1.5 && MenuState == 3){
			gf2d_sprite_draw_image(screen, vector2d(0, 0));
			screen = gf2d_sprite_load_image("images/mentwo.png");
			if (keys[SDL_SCANCODE_RETURN]){
				MenuState = 3.5;
			}
			break;
		}
		while (levelNumber == 1.5 && MenuState == 3.5){
			wallEnt1->position = vector2d(50000000, 50000000);
			wallEnt2->position = vector2d(500000000, 500000000);
			barrelEnt->position = vector2d(3000, 3000);
			doorEnt->position = vector2d(5400000000, 5000000000);
			if (lightBulbAlive == 1){
				lightbulbEnt->position = vector2d(500, 500);
			}
			hostageCheck->position = vector2d(30000, 3000);
			break;

			
		}
		while (levelNumber == 1 && MenuState == 4){
			wallEnt2->position = vector2d(625, 500);
			barrelEnt->position = vector2d(3000, 3000);
			doorEnt->position = vector2d(5400000000, 5000000000);
			if (lightBulbAlive == 1){
				lightbulbEnt->position = vector2d(500, 500);
			}
			hostageCheck->position = vector2d(30000, 3000);
			if (enemyEnt->health < 3){
				gf2d_sprite_draw(enemyTwo, vector2d(enemyEnt->position.x -64, enemyEnt->position.y -64), NULL, NULL, NULL, NULL, NULL, (int)etf);
				enemyEnt->position = vector2d(100, 100);
			}
			else{
				enemyEnt->position = vector2d(2000, 2000);
			}
			if (enemyEnt1->health < 3){
				gf2d_sprite_draw(enemyTwo, vector2d(enemyEnt1->position.x-64, enemyEnt1->position.y-64), NULL, NULL, NULL, NULL, NULL, (int)etf);
			}
			else{
				enemyEnt1->position = vector2d(2000000, 20000);
			}
			if (enemyEnt2->health < 3){
				gf2d_sprite_draw(enemyThree, vector2d(enemyEnt2->position.x-64, enemyEnt2->position.y-64), NULL, NULL, NULL, NULL, NULL, (int)ethf);
			}
			else{
				enemyEnt2->position = vector2d(2000555555, 2000555555);
			}
			if (enemyEnt3->health < 3){
				gf2d_sprite_draw(enemyOne, vector2d(enemyEnt3->position.x-64, enemyEnt3->position.y-64), NULL, NULL, NULL, NULL, NULL, (int)eof);
			}
			else{
				enemyEnt3->position = vector2d(2003333333330, 203333333300);
			}
			electricBox->position = vector2d(1000, 300);
			if (electricboxalive == 1){
				gf2d_sprite_draw(electric, electricBox->position, NULL, NULL, NULL, NULL, NULL, (int)ef);
				if (ef >= 1.0)ef = 0;
				ef += 0.1;
			}
			else{
				ef = 1.0;
				lightbulbEnt->position = vector2d(13000000, 130000000);
				killCheck->position = vector2d(300, 300);
				gf2d_sprite_draw(killZone, killCheck->position, NULL, NULL, NULL, NULL, NULL, NULL);

			}
			gf2d_sprite_draw(lightbulb, lightbulbEnt->position, NULL, NULL, NULL, NULL, NULL, NULL);
			gf2d_sprite_draw(wall2, vector2d(wallEnt2->position.x - 64, wallEnt2->position.y - 64), &scaleWall1, NULL, &vecRightAng, NULL, NULL, (int)mf);
			break;
		}
		if (MenuState == 2 || MenuState == 4 || MenuState == 3.5){
			gf2d_sprite_draw(player, vector2d(playerEnt->position.x - 64, playerEnt->position.y - 64), NULL, NULL, vecturn, NULL, NULL, (int)pf);
			gf2d_sprite_draw(tracer, vector2d(mx + 32, my + 32), scale, originpoint, vecturn, NULL, colorshiftturn, 0);
		}	
		if (MenuState == 5){
			gf2d_sprite_draw(moBO, vector2d(bossTwo->position.x - 64, bossTwo->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)fbf);
		}
		if (MenuState == 3.5){
			gf2d_sprite_draw(flutter, vector2d(bossOne->position.x - 64, bossOne->position.y - 64), NULL, NULL, NULL, NULL, &colorShiftBossOne, (int)mbf);
		}
			gf2d_sprite_draw(wallEnt1->sprite, vector2d(wallEnt1->position.x-64, wallEnt1->position.y-64), &scaleWall1, NULL, NULL, NULL, NULL, (int)mf);
			if (keys[SDL_SCANCODE_TAB]){
				gf2d_sprite_draw_image(mission, vector2d(0, 0));
				if (hostageSaved == 1){
					gf2d_sprite_draw_image(check, vector2d(50, 150));
				}
				if (electricboxalive == 0){
					gf2d_sprite_draw_image(check, vector2d(40, 330));
				}
			}
			if (keys[SDL_SCANCODE_P]){
				//slog("menu");
				menuOn = 1;
			}
			if (menuOn == 1){
				gf2d_sprite_draw_image(menu, vector2d(0, 0));
				if (collide_rect(Mouse, Yes)){
					while (SDL_PollEvent(&event))
					{
						switch (event.type) {
						case SDL_MOUSEBUTTONDOWN:
							done = 1;
							break;
						case SDL_MOUSEBUTTONUP:
							break;
						}
					}
				}
				if (collide_rect(Mouse, No)){
					while (SDL_PollEvent(&event)){
						switch (event.type){
						case SDL_MOUSEBUTTONDOWN:
							menuOn = 0;
							break;
						case SDL_MOUSEBUTTONUP:
							break;
						}
					}
				}
			}
		
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
