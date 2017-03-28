#include "SDLfc.h"
#include <mmsystem.h>

using namespace std;

SDL_Surface *numbers;

SDL_Surface *intro;
SDL_Rect intro_r;

SDL_Surface *head_image;
SDL_Rect head_info;

SDL_Surface *go_image;
SDL_Rect go_info;

SDL_Surface *head1_image;

SDL_Surface *back_image;

SDL_Surface *score_image;
SDL_Rect score_info;

SDL_Surface *body_image;
SDL_Rect body_info[200];


SDL_Surface *mouse_image[3];
SDL_Rect mouse_info;
SDL_Rect mouse_infoa;

SDL_Surface *mouse1_image;

Uint32 color;
int mousephase = 0;
int keystroke;
int r,g,b;
int quit=0;
int dx = 16;
int dy = 0;
int speed = 16;
int nlenght = 2;
int time = 250;
bool ok = 0;
int scor = 0;
int hscor = 0;
int cheat = 0;
int timecount = 0;

void check_keyboard(void);
/*
void InitSnake(int lenght)
{

while(lenght >0)
 {
 body_info[lenght].y = 240;
 body_info[lenght].x = 320 - 10*lenght;
 lenght--;
 }

} */
void introfn()
{
SDL_FillRect(screen, NULL, color);
SDL_BlitSurface(intro,NULL,screen,&intro_r);
SDL_Flip(screen);
SDL_Delay(3000);
}

void ShowSnake(int lenght)
{
body_info[1].x =head_info.x;
body_info[1].y =head_info.y;

while (lenght>0)
 {
 body_info[lenght] = body_info[lenght - 1];

 if (!(lenght == 1))
 {
 SDL_BlitSurface(body_image,NULL,screen,&body_info[lenght]);
// ShowInt(lenght,numbers,body_info[lenght].x,body_info[lenght].y,screen);

 if(collisioncheck(head_info,body_info[lenght],-5)&ok&!(lenght == 2))
 {
 head_info.x=0;
 head_info.y=0;
 quit=0;
 dx = 16;
 dy = 0;
 speed = 16;
 nlenght = 2;
 time = 250;
 ok = 0;
 scor = 0;
 SDL_BlitSurface(go_image,NULL,screen,&go_info);
 SDL_Flip(screen);
 SDL_Delay(3000);
 introfn();
 }
 }
 if (!ok) ok = 1;

 lenght --;
 }

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

 if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0) exit(1);
 {
    SDL_WM_SetIcon(SDL_LoadBMP("snake.bmp"), NULL);
	screen=SDL_SetVideoMode(320,240,16,SDL_HWSURFACE|SDL_DOUBLEBUF);
 }
	if (screen==NULL) { exit(1); }
 SDL_WM_SetCaption("Snake",NULL );

 intro=SDL_LoadBMP("snakegfx/intro.bsb");
 intro_r.x=70;
 intro_r.y=50;
 SDL_SetColorKey(intro, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 head_image=SDL_LoadBMP("snakegfx/part.bsb");
 head_info.x=5;
 head_info.y=5;
 SDL_SetColorKey(head_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 go_image=SDL_LoadBMP("snakegfx/partd.bsb");
 go_info.x=120;
 go_info.y=75;
 SDL_SetColorKey(go_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));


 head1_image=SDL_LoadBMP("snakegfx/part1.bsb");
 SDL_SetColorKey(head1_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 mouse1_image=SDL_LoadBMP("snakegfx/mouse2.bsb");
 SDL_SetColorKey(mouse1_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 body_image=SDL_LoadBMP("snakegfx/part2.bsb");
 SDL_SetColorKey(body_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 back_image=SDL_LoadBMP("snakegfx/back.bsb");
 SDL_SetColorKey(back_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 score_image=SDL_LoadBMP("snakegfx/score.bsb");
 score_info.x = 240;
 score_info.y = 10;
 SDL_SetColorKey(score_image, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 numbers=SDL_LoadBMP("snakegfx/Numbers.bsb");
 SDL_SetColorKey(numbers, SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 mouse_image[0]=SDL_LoadBMP("snakegfx/mouse1.bsb");
 mouse_info.x = rnd(300);
 mouse_info.y = rnd(220);
 mouse_infoa.x = rnd(300);
 mouse_infoa.y = rnd(220);
 SDL_SetColorKey(mouse_image[0], SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 mouse_image[1]=SDL_LoadBMP("snakegfx/mouse2.bsb");
 SDL_SetColorKey(mouse_image[1], SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

 mouse_image[2]=SDL_LoadBMP("snakegfx/mouse3.bsb");
 SDL_SetColorKey(mouse_image[2], SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

// mouse_image[3]=SDL_LoadBMP("snakegfx/mouse3.bsb");
// SDL_SetColorKey(mouse_image[3], SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0, 0,0));

// InitSnake(lenght);
 ifstream InFile;//se incarca
 InFile.open("snakegfx/apart.bsb");

 InFile >> hscor;

 InFile.close();// se incarca



 while ( ! quit)
     {

     color = SDL_MapRGB(screen->format, r,g,b);
     SDL_FillRect(screen, NULL, color);
     check_keyboard();
     SDL_BlitSurface(back_image,NULL,screen,NULL);

     ShowSnake(nlenght);

     head_info.x += dx;
     head_info.y += dy;

     if (head_info.x >300) head_info.x = 4;
     if (head_info.y >220) head_info.y = 4;
     if (head_info.x <4) head_info.x = 300;
     if (head_info.y <4) head_info.y = 220;

     SDL_BlitSurface(head_image,NULL,screen,&head_info);
     SDL_BlitSurface(mouse_image[mousephase],NULL,screen,&mouse_info);
     mousephase ++;
     if (mousephase == 3) mousephase = 0;

     if (collisioncheck(head_info,mouse_info,0))
     {
     nlenght ++;
     scor += 10;

     //PlaySound("C:/WINDOWS/MEDIA/Microsoft Office 2000/Laser.wav",
    // NULL, SND_FILENAME | SND_ASYNC );

     if (scor>hscor) hscor = scor;

     mouse_info.x = 5 + rnd(300);
     mouse_info.y = 5 + rnd(220);
     mouse_infoa.x = 5 + rnd(300);
     mouse_infoa.y = 5 + rnd(220);

     if (time>200) time -= 10;
     if (time>150) time -= 5;
     if (time>100) time -=2;
     }
     SDL_BlitSurface(score_image,NULL,screen,&score_info);
     ShowInt(scor,numbers,280,10,screen);
     ShowInt(hscor,numbers,280,20,screen);

     if (AddTo(mouse_info.x,3,mouse_infoa.x))
       {
       mouse_infoa.x = 5 + rnd (300);
       }
     if (AddTo(mouse_info.y,3,mouse_infoa.y))
       {
       mouse_infoa.y = 5 + rnd (220);
       }

     SDL_Flip(screen);
     SDL_Delay(time);
     }
SDL_Quit();

ofstream OutFile;      // se salveaza
OutFile.open("snakegfx/apart.bsb");

OutFile << hscor;

OutFile.close();



return 1;
}

void check_keyboard(void)
{ 

SDL_Event event;
      while ( SDL_PollEvent(&event) )
       {

        if ( event.type == SDL_QUIT ) quit = 1;
   
        keystroke=event.key.keysym.sym;
        if (keystroke==SDLK_ESCAPE) quit=1;

        if(keystroke==SDLK_RIGHT)
         {
         dx = speed;
         dy = 0;
         }
        if(keystroke==SDLK_LEFT)
         {
         dx = -speed;
         dy = 0;
         }
        if(keystroke==SDLK_UP)
         {
         dy = -speed;
         dx = 0;
         }
        if(keystroke==SDLK_DOWN)
         {
         dy = speed;
         dx = 0;
         }
         if((keystroke == SDLK_s)&(cheat == 0)) cheat = 1;
         if((keystroke == SDLK_n)&(cheat == 1)) cheat = 2;
         if((keystroke == SDLK_a)&(cheat == 2)) cheat = 3;
         if((keystroke == SDLK_k)&(cheat == 3)) cheat = 4;
         if((keystroke == SDLK_e)&(cheat == 4)) cheat = 5;
         if((keystroke == SDLK_6)&(cheat == 5)) cheat = 6;
         if((keystroke == SDLK_9)&(cheat == 6))
         {
         head_image = head1_image;
         mouse_image[1] = mouse1_image;
         }
      }

}
