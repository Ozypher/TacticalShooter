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
    int mx,my;
    float mf = 0;
	float pf = 0;
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
	Vector3D vecRightAng;
	float enemydead = 0;
	float levelNumber = 0;
    
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
    sprite = gf2d_sprite_load_image("images/backgrounds/preview16.jpg");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	player = gf2d_sprite_load_all("images/walktrue.png",128,128,2);
	enemy = gf2d_sprite_load_all("images/enemy.png", 128, 128, 1);
	tracer = gf2d_sprite_load_all("images/tracer.png", 24, 128, 1);
	barrel = gf2d_sprite_load_all("images/barrel.png", 128, 128, 1);
	lightbulb = gf2d_sprite_load_all("images/light.png", 128, 128, 1); 
	wall = gf2d_sprite_load_all("images/breachable.png", 128, 128, 1);
	wall2 = gf2d_sprite_load_all("images/reinforced.png", 128, 128, 1);
    /*main game loop*/
	playerEnt->speed = 1.0;
	enemyEnt->position = vector2d(150, 150);
	barrelEnt->position = vector2d(780, 500);
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
		}
		else{
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
		// SHOOTING CODE ENDS HERE
		// WALL COLLISION START
		while (collide_circle(playerEnt->position, 32, barrelEnt->position, 32)){
			playerEnt->speed*=-1;
			break;
		}
		while (collide_rect(playerBox,wall1Box)){
			playerEnt->speed *= -1;
			break;
		}
		while (collide_rect(playerBox, wall2Box)){
			playerEnt->speed *= -1;
			break;
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




			gf2d_sprite_draw(wall2, vector2d(wallEnt2->position.x - 64, wallEnt2->position.y - 64), &scaleWall1, NULL, &vecRightAng, NULL, NULL, (int)mf);
			gf2d_sprite_draw(barrel, vector2d(barrelEnt->position.x - 64, barrelEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)mf);
			break;
		}
			gf2d_sprite_draw(player, vector2d(playerEnt->position.x-64,playerEnt->position.y-64) ,NULL,NULL,vecturn,NULL,NULL,(int)pf);
			gf2d_sprite_draw(wall, vector2d(wallEnt1->position.x-64, wallEnt1->position.y-64), &scaleWall1, NULL, NULL, NULL, NULL, (int)mf);
			if (enemydead < 3){
				gf2d_sprite_draw(enemy, vector2d(enemyEnt->position.x - 64, enemyEnt->position.y - 64), NULL, NULL, NULL, NULL, NULL, (int)mf);
			}
			gf2d_sprite_draw(tracer, vector2d(mx+32, my+32), scale, originpoint, vecturn, NULL, colorshiftturn, 0);
            
            //UI elements last
            //gf2d_sprite_draw(
              //  mouse,
                //vector2d(mx,my),
                //NULL,
                //NULL,
                //NULL,
                //NULL,
                //&mouseColor,
                //(int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
