//================================={ include library }=======================================//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#include <iostream>

using namespace std;

//============================================================================================//

//===================================={ preparation }============================================//

#define NAMA_APLIKASI "Game by Kelompok 10"
#define IMAGE_BACKGROUND "image/background.jpg"
#define SCREEN_HEIGHT 750
#define SCREEN_WIDTH 1375

#define JUMLAH_HANTU 10
#define UKURAN_HANTU 250
#define KECEPETAN_HANTU 30
#define NYAWA_HANTU 1
#define IMAGE_HANTU "image/ghost.png"


#define KECEPATAN_TARGET 40
#define UKURAN_TARGET 100
#define DAMAGE_TEMBAKAN 1
#define IMAGE_TARGET1 "image/target.png"
#define IMAGE_TARGET2 "image/target2.png"
#define IMAGE_TARGET3 "image/target3.png"
int kursor_pilih_target = 0;

//===================================================================================================//


//====================================={ block hantu }==================================================//
//setting hantu
typedef struct setting_hantu 
{
  int x;
  int y;
  int arahx;
  int arahy;
  int nyawa;
  
 
} KOMPONEN_HANTU;


void gerak_hantu(KOMPONEN_HANTU *hnt) {
	int gerakan_x = rand() % (2*KECEPETAN_HANTU) - KECEPETAN_HANTU;
	int gerakan_y = rand() % (2*KECEPETAN_HANTU) - KECEPETAN_HANTU;
	

	hnt->x += gerakan_x;
	hnt->y += gerakan_y;

	if (hnt->y < 0) 
	{
		hnt->y = 0;
	}
	if (hnt->x < 0) 
	{
		hnt->x = 0;
	}
	if (hnt->x >SCREEN_WIDTH-1) 
	{
		hnt->x = SCREEN_WIDTH-1;
	}
	if (hnt->y >SCREEN_HEIGHT-1) 
	{
		hnt->y = SCREEN_HEIGHT;
	}
}

void gambar_hantu(SDL_Renderer* r, SDL_Texture* tex, KOMPONEN_HANTU *hnt) 
{
	if (hnt->nyawa <= 0) 
	{
		return;
	}
	SDL_Rect rect_hantu;
	rect_hantu.x = 0;
	rect_hantu.y = 0;
    rect_hantu.h = UKURAN_HANTU;
	rect_hantu.w = UKURAN_HANTU;

	rect_hantu.x = hnt->x;
	rect_hantu.y = hnt->y;
	SDL_RenderCopy(r, tex, NULL, &rect_hantu);

}

//=========================================================================================//

//===================================={ kena tembakan }===================================//

int definisi_ketembak(KOMPONEN_HANTU *hantuu, int x,int y) 
{
	int x0 = hantuu->x;
	int x1 = x0+UKURAN_HANTU;
	int y0 = hantuu->y;
	int y1 = y0+UKURAN_HANTU;
	int resul = 0;
    cout << "x hantu " << x0 << ", y hantu " << y0 << ", x target " << x << ", y target " << y <<endl;
	

	if (x0 <= x && x1 >=x && y0 <= y && y1 >= y) 
	{
        resul = 1;;
	}
	return resul;
}

void cek_ketembak(KOMPONEN_HANTU* daftar_sasaran,int x, int y) 
{
	for(int i = 0; i < JUMLAH_HANTU; i++) 
	{
		if (daftar_sasaran[i].nyawa == 0) continue;

		if (definisi_ketembak(&daftar_sasaran[i],x,y) == 1)
		{
			daftar_sasaran[i].nyawa = NYAWA_HANTU-DAMAGE_TEMBAKAN;
		}
	}
} 
//===========================================================================================//


int main(int argc, char* argv[])
{
	//=========================={ setting screen }===========================================//
	


	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
        cout << "error initializing SDL: " << SDL_GetError() <<endl;
	}
	SDL_Window* screen = SDL_CreateWindow(NAMA_APLIKASI,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, 0);



	// your graphics hardware and sets flags
	Uint64 render_flags = SDL_RENDERER_ACCELERATED;

	// creates a renderer to render our images
	SDL_Renderer* render = SDL_CreateRenderer(screen, -1, render_flags);

	
	
    // SDL_Renderer = dictionary from SDL Library to create a surface to load an image from our computer memory into this game main memory
	SDL_Surface* img_screen_background;

    // IMG_Load = function from SDL Library to take image from our computer memory
    img_screen_background = IMG_Load(IMAGE_BACKGROUND);

    // SDL_Texture = dictionary from SDL Library that contain setting loads image from our game main meory to our graphics hardware memory
    SDL_Texture* screen_texture = SDL_CreateTextureFromSurface(render, img_screen_background);

    // SDL_FreeSurface = Function from SDL Library to clears image file from game main memory
    SDL_FreeSurface(img_screen_background);

    // SDL_Rect = dictionary from SDL Library that contain framewqork to create frame for our texture image 
    SDL_Rect kanvas_background;
    //we fill the framework information here
    kanvas_background.x = 0;
	kanvas_background.y = 0;
    kanvas_background.h = SCREEN_HEIGHT;
	kanvas_background.w = SCREEN_WIDTH;

    //==========================================================================================//

    //========================={ setting gambar hantu dan target }===========================//

    IMG_Init( IMG_INIT_PNG ); //to make png image with transparant

    //------------------------------[ setting gambar hantu ]----------------------------//

    SDL_Surface* img_hantu;
	img_hantu = IMG_Load(IMAGE_HANTU);
    SDL_Texture* texture_hantu = SDL_CreateTextureFromSurface(render, img_hantu);
    SDL_FreeSurface(img_hantu);
    

    srand(time(NULL));   // Initialization, should only be called once.
	KOMPONEN_HANTU hantus[JUMLAH_HANTU];
	for (int i=0; i< JUMLAH_HANTU; i++) 
	{
		
		hantus[i].x = 500;
		hantus[i].y = 500;
		hantus[i].nyawa = NYAWA_HANTU;

	}



    //----------------------------------------------------------------------------------//

    //----------------------------[ setting gambar target ]------------------------------//
    SDL_Surface* img_target1;
	SDL_Surface* img_target2;
	SDL_Surface* img_target3;

    
	img_target1 = IMG_Load(IMAGE_TARGET1); 
	img_target2 = IMG_Load(IMAGE_TARGET2); 
	img_target3 = IMG_Load(IMAGE_TARGET3); 
	
	SDL_Texture* targets[3];

	SDL_Texture* texture_target;
    texture_target = SDL_CreateTextureFromSurface(render, img_target1);
	targets[0] = texture_target;
	texture_target = SDL_CreateTextureFromSurface(render, img_target2);
	targets[1] = texture_target;
	texture_target = SDL_CreateTextureFromSurface(render, img_target3);
	targets[2] = texture_target;

	SDL_FreeSurface(img_target1);
	SDL_FreeSurface(img_target2);
	SDL_FreeSurface(img_target3);

    
    SDL_Rect kanvas_target;
	
	// SDL_Query = function from SDL Library SDL_Quis used to retrieve the basic settings of a texture, including the format, access, width, and height.
    // and we need access to connects our texture with dest to control position
	SDL_QueryTexture(texture_target, NULL, NULL, &kanvas_target.w, &kanvas_target.h);

    

    kanvas_target.w = UKURAN_TARGET;
	kanvas_target.h = UKURAN_TARGET;    
	kanvas_target.x = (SCREEN_WIDTH - kanvas_target.w) / 2;
	kanvas_target.y = (SCREEN_HEIGHT - kanvas_target.h) / 2;

    //==============================================================================================================//


	//==================================={ game loop }=============================================//
    

    int close = 0;

	while (!close) 
	{
		SDL_Event event;

		// Events management
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_QUIT:
				// handling of close button
				close = 1;
				break;

			case SDL_KEYDOWN:
				// keyboard API for key pressed
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_X:
					{
					int tengahx = kanvas_target.x+(kanvas_target.w/2);
					int tengahy = kanvas_target.y+(kanvas_target.y/2);
					cek_ketembak(hantus,tengahx,tengahy);
					}
					break;
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					kanvas_target.y -= KECEPATAN_TARGET;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					kanvas_target.x -= KECEPATAN_TARGET;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					kanvas_target.y += KECEPATAN_TARGET;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					kanvas_target.x += KECEPATAN_TARGET;
					break;
				case SDL_SCANCODE_M:
					kursor_pilih_target++;				
					if (kursor_pilih_target > 2)
					{
						kursor_pilih_target = 0;
					}
				default:
					break;
				}
			}
		}

		//------------pembatasan gerak target------------------//
		if (kanvas_target.x + kanvas_target.w > SCREEN_WIDTH){
            kanvas_target.x = SCREEN_WIDTH - kanvas_target.w;
        }
		if (kanvas_target.x < 0){
            kanvas_target.x = 0;
        }
		if (kanvas_target.y + kanvas_target.h > SCREEN_HEIGHT){
            kanvas_target.y = SCREEN_HEIGHT - kanvas_target.h;
        }
			
		if (kanvas_target.y < 0){
            kanvas_target.y = 0;
        }
		//---------------------------------------------------//



		// clears the screen
		SDL_RenderClear(render);

        //gambar layar//
		SDL_RenderCopy(render, screen_texture, NULL, &kanvas_background);


        //gambar hantu//
		for (int i = 0; i < JUMLAH_HANTU; i++) {
			gerak_hantu(&hantus[i]);
			gambar_hantu(render,texture_hantu,&hantus[i]);

		}

        //gambar target//
		SDL_RenderCopy(render, targets[kursor_pilih_target], NULL, &kanvas_target);

		
		// SDL function for multiple rendering
		SDL_RenderPresent(render);

		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}

    //================================================================================================//

	// destroy texture
	SDL_DestroyTexture(texture_target);
	SDL_DestroyTexture(texture_hantu);
	SDL_DestroyTexture(screen_texture);

	// destroy renderer
	SDL_DestroyRenderer(render);

	// destroy window
	SDL_DestroyWindow(screen);
	
	// close SDL
	SDL_Quit();

	return 0;
}
