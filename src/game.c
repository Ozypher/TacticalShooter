#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_entity.h"
#include "math.h"
#include "collision.h"

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
	Vector4D *colorshiftturn;
	Entity *enemyEnt;
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
	Vector3D vecRightAng;
	float enemydead = 0;
	float levelNumber = 0;
	float wallBreached = 0;
	float electricboxalive = 1;
	float hostageSaved = 0;
	float menuOn = 0;
    
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
	gf2d_entity_manager_init(20);
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
    sprite = gf2d_sprite_load_image("images/backgrounds/preview16.jpg");
	breached = gf2d_sprite_load_image("images/breached.png");
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
    while(!done)
    {
		SDL_Event event;
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		tracerEnt->position = vector2d(mx, my);

        mf+=0.1;
        if (mf >= 1.0)mf = 0;
		if (wallBreached == 0){
			wallEnt1->sprite = wall;
		}
		if (levelNumber == 1){
			sprite = gf2d_sprite_load_image("images/concrete.png");
		}
		gdistance = ((playerEnt->position.x - mx)*(playerEnt->position.x - mx) + (playerEnt->position.y - my) * (playerEnt->position.y - my));
		distance = sqrt(gdistance);
		truDistance = (mx - playerEnt->position.x) + (my- playerEnt->position.y);
		gangle = (playerEnt->position.y - my) / (playerEnt->position.x - mx);
		angle = atan(gangle);
		vecangle = vector3d(64, 64, (angle*(57.296)+90));
		scalevec = vector2d(0.25, distance/100);
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
		slog("the mouse is in : %i, %i", mx, my);
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
		// Hit Box Code
		SDL_Rect playerBox = {
			playerEnt->position.x+64,
			playerEnt->position.y+64,
			64,
			64
		};
		SDL_Rect wall1Box = {
			wallEnt1->position.x+18,
			wallEnt1->position.y+46,
			256,
			18 };
		SDL_Rect wall2Box = {
			wallEnt2->position.x+153,
			wallEnt2->position.y,
			18,
			230
		};
		SDL_Rect hostageBox = {
			hostageEnt->position.x + 64,
			hostageEnt->position.y + 64,
			64,
			64
		};
		SDL_Rect hostageArea = {
			hostageCheck->position.x,
			hostageCheck->position.y,
			128,
			128
		};
		SDL_Rect enemyBoxOne = {
			enemyEnt->position.x+64,
			enemyEnt->position.y+64,
			64,
			64
		};
		SDL_Rect doorBox = {
			doorEnt->position.x+165,
			doorEnt->position.y+95,
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
			1,
			1
		};
		SDL_Rect Yes = {
		344,
		237,
		408,
		136};
		SDL_Rect No = {
			359, 403,
			392, 115
		};
		// SHOOTING CODE STARTS HERE
		if (distance > 250){
			if (collide_circle(tracerEnt->position, 5, enemyEnt->position, 32)){
				while (SDL_PollEvent(&event)) // check to see if an event has happened
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						enemydead += 1;
						slog("Bang!");
						break;
					}
				}
			}
		}
		if (distance < 250 && distance > 60){
			if (collide_circle(tracerEnt->position, 5, enemyEnt->position, 32)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						slog("Whoosh!");
						if (playerEnt->position.x >= enemyEnt->position.x &&  playerEnt->position.y > enemyEnt->position.y){
							enemyEnt->position.x -= 80.0;
							enemyEnt->position.y -= 80.0;
						}
						if (playerEnt->position.x < enemyEnt->position.x &&  playerEnt->position.y > enemyEnt->position.y){
							enemyEnt->position.x += 80.0;
							enemyEnt->position.y -= 80.0;
						}
						if (playerEnt->position.x < enemyEnt->position.x &&  playerEnt->position.y < enemyEnt->position.y){
							enemyEnt->position.x += 80.0;
							enemyEnt->position.y += 80.0;
						}
						if (playerEnt->position.x > enemyEnt->position.x &&  playerEnt->position.y <= enemyEnt->position.y){
							enemyEnt->position.x -= 80.0;
							enemyEnt->position.y += 80.0;
						}
						break;
					}
				}
			}
			if (collide_circle(tracerEnt->position, 5, wallEnt1->position, 32)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						slog("BREACH!");
						wallEnt1->sprite = breached;
						wallBreached = 1;
					}
				}
			}
		}
		else{
			if (collide_circle(tracerEnt->position, 5, electricBox->position, 32)){
				while (SDL_PollEvent(&event)){
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						electricboxalive = 0;
					}
				}
			}
			if (collide_circle(tracerEnt->position, 5, enemyEnt->position, 32)){
				while (SDL_PollEvent(&event))
				{
					switch (event.type) {
					case SDL_MOUSEBUTTONDOWN:
						enemydead += 3;
					}
				}
			}
		}
		if (collide_circle(tracerEnt->position, 5, vector2d(lightbulbEnt->position.x+64,lightbulbEnt->position.y+64), 32)){
			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					lightbulbEnt->position = vector2d(13000, 130000);
				}
			}
		}
		if (collide_circle(enemyEnt->position, 32, lightbulbEnt->position, 1200)){
			if (collide_rect(playerBox, enemyBoxOne)){
				playerEnt->position = vector2d(30000000000000, 300000000000);
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
			levelNumber = 1;
			wallBreached = 0;
			hostageSaved = 1;
		}
		if (collide_rect(enemyBoxOne, playerBox)){
			slog("ping!");
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
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        
		//backgrounds drawn first
		if (pf >= 2.0)pf = 0;
		gf2d_sprite_draw_image(sprite, vector2d(0, 0));
		gf2d_sprite_draw_image(sprite, vector2d(780, 585));
		gf2d_sprite_draw_image(sprite, vector2d(780, 0));
		gf2d_sprite_draw_image(sprite, vector2d(0, 585));
		//level design here
		while (levelNumber == 0){
			wallEnt1->position = vector2d(700, 585);
			scaleWall1 = vector2d(2, 1);
			wallEnt2->position = vector2d(572, 457);
			vecRightAng = vector3d(64, 64, 90);
			if (enemydead < 3){
				gf2d_sprite_draw(enemy, vector2d(enemyEnt->position.x - 64, enemyEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)mf);
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
		while (levelNumber == 1){
			wallEnt1->position = vector2d(500, 500);
			wallEnt2->position = vector2d(625, 500);
			barrelEnt->position = vector2d(3000, 3000);
			hostageCheck->position = vector2d(30000, 3000);
			if (enemydead < 3){
				gf2d_sprite_draw(enemy, vector2d(enemyEnt->position.x - 64, enemyEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)mf);
				enemyEnt->position = vector2d(100, 100);
			}
			else{
				enemyEnt->position = vector2d(2000, 2000);
			}
			electricBox->position = vector2d(500, 500);
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
			gf2d_sprite_draw(wall2, vector2d(wallEnt2->position.x - 64, wallEnt2->position.y - 64), &scaleWall1, NULL, &vecRightAng, NULL, NULL, (int)mf);
			break;
		}
			gf2d_sprite_draw(player, vector2d(playerEnt->position.x-64,playerEnt->position.y-64) ,NULL,NULL,vecturn,NULL,NULL,(int)pf);
			gf2d_sprite_draw(wallEnt1->sprite, vector2d(wallEnt1->position.x-64, wallEnt1->position.y-64), &scaleWall1, NULL, NULL, NULL, NULL, (int)mf);
			gf2d_sprite_draw(tracer, vector2d(mx+32, my+32), scale, originpoint, vecturn, NULL, colorshiftturn, 0);
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
						}
					}
				}
				if (collide_rect(Mouse, No)){
					while (SDL_PollEvent(&event)){
						switch (event.type){
						case SDL_MOUSEBUTTONDOWN:
							menuOn = 0;
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
