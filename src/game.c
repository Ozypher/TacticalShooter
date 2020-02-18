#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_entity.h"
#include "math.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	Sprite *player;
    int mx,my;
    float mf = 0;
	float pf = 0;
	float gdistance = 0;
	float distance = 0;
	float gangle = 0;
	float angle = 0;
	float truDistance = 0;
	Vector3D vecangle;
	Vector3D *vecturn;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
	Entity *playerEnt;
    
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
	playerEnt = gf2d_entity_new();
    sprite = gf2d_sprite_load_image("images/backgrounds/preview16.jpg");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	player = gf2d_sprite_load_all("images/walktrue.png",128,128,2);
    /*main game loop*/
	playerEnt->speed = 1.0;
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		gdistance = ((playerEnt->position.x - mx)*(playerEnt->position.x - mx) + (playerEnt->position.y - my) * (playerEnt->position.y - my));
		distance = sqrt(gdistance);
		truDistance = (mx - playerEnt->position.x) + (my- playerEnt->position.y);
		gangle = (playerEnt->position.y - my) / (playerEnt->position.x - mx);
		angle = atan(gangle);
		vecangle = vector3d(64, 64, (angle*(57.296)+90));
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
		slog("The angle is updated to : %f,%f,%f", vecangle.x,vecangle.y,vecangle.z);
		
		//slog("distance between is : %f", distance);
		// sprint
		if (distance >= 400){
			playerEnt->speed = 3.0;
		}
		// run
		if (distance >= 200){
			playerEnt->speed = 2.0;
		}
		// walk
		else{
			playerEnt->speed = 1.0;
		}
		if (keys[SDL_SCANCODE_W]){
			pf += 0.05 * playerEnt->speed;
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
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
			gf2d_sprite_draw_image(sprite, vector2d(780, 585));
			gf2d_sprite_draw_image(sprite, vector2d(780, 0));
			gf2d_sprite_draw_image(sprite, vector2d(0, 585));
			gf2d_sprite_draw(player, vector2d(playerEnt->position.x-64,playerEnt->position.y-64) ,NULL,NULL,vecturn,NULL,NULL,(int)pf);
            
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
