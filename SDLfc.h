#include <windows.h>
#include <SDL.h>
#include <fstream>
#include <math.h>

#define PI    3.14159
#define rndINCREMENT          13849
#define rndFLOATING_MODULUS   65536.0
#define rndMODULUS            65536
#define rndMULTIPLIER         25273

SDL_Surface *screen;
int rndseed =  17;

/////////////////////////////////////////////////////////////////
int rnd(int var) {
	rndseed = (rndMULTIPLIER * rndseed + rndINCREMENT + (int)GetTickCount())
	          % rndMODULUS;
	return (int) ((rndseed / rndFLOATING_MODULUS)*var);

}

/////////////////////////////////////////////////////////////////
void display_bmp(char *file_name) {
	SDL_Surface *image;

	/* Load the BMP file into a surface */
	image = SDL_LoadBMP(file_name);
	if (image == NULL) {
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return;
	}

	/*
	 * Palettized screen modes will have a default palette (a standard
	 * 8*8*4 colour cube), but if the image is palettized as well we can
	 * use that palette for a nicer colour matching
	 */
	if (image->format->palette && screen->format->palette) {
		SDL_SetColors(screen, image->format->palette->colors, 0,
		              image->format->palette->ncolors);
	}

	/* Blit onto the screen surface */
	if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

	SDL_UpdateRect(screen, 0, 0, image->w, image->h);

	/* Free the allocated BMP surface */
	SDL_FreeSurface(image);
}
///////////////////////////////////////////////////////////////////

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}
////////////////////////////////////////////////////////////////////

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}
//////////////////////////////////////////////////////////////////////
void makesemitransparent(SDL_Surface *surface,SDL_Rect info,Uint32 color) {
	bool b = true;
	int x = 0;
	int y = 0;

	SDL_LockSurface( surface );
	while (x<info.w) {
		if (b) {
			y = 1;
			b = false;
		} else {
			y = 0;
			b = true;
		}

		while (y<info.h) {
			putpixel(surface,x,y,color);
			y+=2;
		}
		x+=2;
	}
	SDL_UnlockSurface( surface );
}

////////////////////////////////////////////////////////////////////

bool collisioncheck (SDL_Rect recta1,SDL_Rect recta2,int margin) {
	SDL_Rect rect1;
	SDL_Rect rect2;

	rect1.x = recta1.x + margin;
	rect1.y = recta1.y + margin;
	rect1.h = recta1.h + margin;
	rect1.w = recta1.w + margin;

	rect2.x = recta2.x + margin;
	rect2.y = recta2.y + margin;
	rect2.h = recta2.h + margin;
	rect2.w = recta2.w + margin;

	if((((rect1.x+rect1.w>=rect2.x) &&
	        (rect2.x+rect2.w>=rect1.x) &&
	        (rect1.y+rect1.h>=rect2.y) &&
	        (rect2.y+rect2.h>=rect1.y)))) {
		return true;
	} else {
		return false;
	}
}

/////////////////////////////////////////////////////////////////
// input data:
//int number - an int ranging from 0 to 9
//SDL_Surface *numbers - a surface with a loaded bitmap with ten numbers from 0 to 9
//int x - the x coordinate where the number will show on the screen
//int y - the y cordinate
//SDL_Surface *swnumbers - the surface where the bitmap numbers will be drown

// output:
//am image with the corespondent bitmap number will be drown to the
// output surface
void ShowNumber(int number,SDL_Surface *numbers,int x,int y,
                SDL_Surface *swnumbers) {

	SDL_Rect nrect;// the rect of SDL_Surface *numbers
	SDL_Rect swrect;// output rect

	swrect.x = x;
	swrect.y = y;

	if ((number<10)&(number>-2)) { // only if number is from 0 to 9
		SDL_GetClipRect(numbers,&nrect); // get info about the bitmap

		nrect.w = nrect.w/11;  // a number's width is a tenth of the width of the
		// bitmap width

		switch ( number) {
			case 0: {
				nrect.x = 0;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 1: {
				nrect.x = nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 2: {
				nrect.x = 2 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 3: {
				nrect.x = 3 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 4: {
				nrect.x = 4 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 5: {
				nrect.x = 5 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 6: {
				nrect.x = 6 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 7: {
				nrect.x = 7 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 8: {
				nrect.x = 8 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case 9: {
				nrect.x = 9 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
			case -1: {
				nrect.x = 10 * nrect.w;
				SDL_BlitSurface(numbers,&nrect,swnumbers,&swrect);
			}
			break;
		}

	}
}
/////////////////////////////////////////////////////////////////////////
int Power(int base, int exponent) {
	int result = 1;
	while (exponent > 0) {
		result *= base;
		exponent -= 1;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////
int IntLenght(int number) {
	int lenght = 0;
	if (number == 0) number = 1;
	while (number>0) {
		number /= 10;
		lenght ++;
	}
	return lenght;
}
/////////////////////////////////////////////////////////////////////////
void ShowInt(int number,SDL_Surface *numbers,int x,int y,
             SDL_Surface *swnumbers) {



	SDL_Rect nrect;
	SDL_GetClipRect(numbers,&nrect);

	if (number<0) {
		ShowNumber(-1,numbers,x,y,swnumbers);
		x += nrect.w/11;
		number = -number;
	}

	int lenght = IntLenght(number) - 1;
	int unit = 0;
	int val = 0;
	unit = number /Power(10,lenght);


	while (lenght>-1) {


		ShowNumber(unit,numbers,x,y,swnumbers);

		val = (val + unit)*10;

		lenght -= 1;

		unit = number/Power(10,lenght) - val;
		x += nrect.w/11;
	}


}


///////////////////////////////////////////////////////////////////////

void ShowChar(char mychar,SDL_Surface *chars,int x,int y,
              SDL_Surface *swchars) {

	SDL_Rect nrect;// the rect of SDL_Surface *chars
	SDL_Rect swrect;// output rect

	swrect.x = x;
	swrect.y = y;

	SDL_GetClipRect(chars,&nrect); // get info about the bitmap

	nrect.w = nrect.w/27;  // a number's width is a tenth of the width of the
	// bitmap width
	nrect.h = (nrect.h)/4;

	if ((y< 480 + nrect.h)&(y>-nrect.h)&(x<640+nrect.w)&(x>-nrect.w))
// making sure that it's on the screen
	{

		switch ( mychar) {
			case 'a': {
				nrect.x = 0;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'b': {
				nrect.x = nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'c': {
				nrect.x = 2*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'd': {
				nrect.x = 3*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'e': {
				nrect.x = 4*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'f': {
				nrect.x = 5*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'g': {
				nrect.x = 6*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'h': {
				nrect.x = 7*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'i': {
				nrect.x = 8*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'j': {
				nrect.x = 9*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'k': {
				nrect.x = 10*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'l': {
				nrect.x = 11*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'm': {
				nrect.x = 12*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'n': {
				nrect.x = 13*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'o': {
				nrect.x = 14*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'p': {
				nrect.x = 15*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'q': {
				nrect.x = 16*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'r': {
				nrect.x = 17*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 's': {
				nrect.x = 18*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 't': {
				nrect.x = 19*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'u': {
				nrect.x = 20*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'v': {
				nrect.x = 21*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'w': {
				nrect.x = 22*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'x': {
				nrect.x = 23*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'y': {
				nrect.x = 24*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'z': {
				nrect.x = 25*nrect.w;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			// majuscule
			case 'A': {
				nrect.x = 0;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'B': {
				nrect.x = nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'C': {
				nrect.x = 2*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'D': {
				nrect.x = 3*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'E': {
				nrect.x = 4*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'F': {
				nrect.x = 5*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'G': {
				nrect.x = 6*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'H': {
				nrect.x = 7*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'I': {
				nrect.x = 8*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'J': {
				nrect.x = 9*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'K': {
				nrect.x = 10*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'L': {
				nrect.x = 11*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'M': {
				nrect.x = 12*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'N': {
				nrect.x = 13*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'O': {
				nrect.x = 14*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'P': {
				nrect.x = 15*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'Q': {
				nrect.x = 16*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'R': {
				nrect.x = 17*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'S': {
				nrect.x = 18*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'T': {
				nrect.x = 19*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'U': {
				nrect.x = 20*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'V': {
				nrect.x = 21*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'W': {
				nrect.x = 22*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'X': {
				nrect.x = 23*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'Y': {
				nrect.x = 24*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case 'Z': {
				nrect.x = 25*nrect.w;
				nrect.y = nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '.': {
				nrect.x = 0;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case ',': {
				nrect.x = nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '!': {
				nrect.x = 2*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '?': {
				nrect.x = 3*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case ':': {
				nrect.x = 4*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '"': {
				nrect.x = 5*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '%': {
				nrect.x = 6*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '\'': {
				nrect.x = 7*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '-': {
				nrect.x = 8*nrect.w;
				nrect.y = 2*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '0': {
				nrect.x = 0;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '1': {
				nrect.x = nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '2': {
				nrect.x = 2*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '3': {
				nrect.x = 3*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '4': {
				nrect.x = 4*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '5': {
				nrect.x = 5*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '6': {
				nrect.x = 6*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '7': {
				nrect.x = 7*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '8': {
				nrect.x = 8*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
			break;
			case '9': {
				nrect.x = 9*nrect.w;
				nrect.y = 3*nrect.h;
				SDL_BlitSurface(chars,&nrect,swchars,&swrect);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////
void ShowString(char string[],SDL_Surface *chars,int x,int y,
                SDL_Surface *swchars) {

	int oldx = x;
	int n = 0;

	SDL_Rect nrect;
	SDL_GetClipRect(chars,&nrect);

	while (!(string[n] == '\0')) {
		if (string[n] == '\n') {
			y += (nrect.h - 2)/3+3;
			x = oldx;
		} else {
			ShowChar(string[n],chars, x, y,swchars);
			x += nrect.w/27;
		}



		if (string[n+1] == ' ') { // daca textul contine ' ' spatiu
			x += nrect.w/27;// se lasa spatiu
			n++; // se sare peste spatiu
		}



		n++;
	}
}
/////////////////////////////////////////////////////////////////
void efect(SDL_Rect rect,SDL_Surface *surface1,SDL_Surface *surface2) {
	int x = 0;
	int y = 0;
	Uint32 color;
	Uint32 color1;
	Uint32 color2;

	while (y < rect.h) {
		while (x<rect.w) {
			SDL_LockSurface( surface1 );
			SDL_LockSurface( surface2 );

			color1 = getpixel(surface1,x,y);
			color2 = getpixel(surface2,x + rect.x,y + rect.y);
			color = (color1+color2)/2 + rnd(256);

			putpixel(surface2,x + rect.x,y + rect.y,color);

			SDL_UnlockSurface( surface1 );
			SDL_UnlockSurface( surface2 );
			x +=1;
		}
		x = 0;
		y +=1;
	}

}
////////////////////////////////////////////////////////////////////////////////
/*
void Rotate(SDL_Surface *image,SDL_Surface *blank,int rad)
{
SDL_Rect rect;
SDL_Rect brect;
int raza = 0;
int r = 0;
Uint32 color;

SDL_GetClipRect(image,&rect);
brect = rect;
brect.x = 0;
brect.y = 0;
raza = rect.h/2;

SDL_LockSurface( image );
SDL_LockSurface( blank );
while(raza>0)
    {
    while (r<360)
         {

         color = getpixel(image,(rect.w/2+ raza* sin(PI*r/180)),
         (rect.w/2 +raza* cos(PI*r/180)));
         putpixel(blank,(rect.w/2 +raza* sin(PI*(r+rad)/180)),
         (rect.w/2 +raza* cos(PI*(r+rad)/180)),color);
         r++;
         color = SDL_MapRGB(screen->format, 0,0,0);
         }
    r = 0;
    raza --;
    }
SDL_UnlockSurface( blank );
SDL_UnlockSurface( image );
SDL_FillRect(image,&rect,color);
SDL_BlitSurface(blank,NULL,image,&brect);

}
/////////////////////////////////////////////////////////////

void Line(float x, float y, float px,float py,Uint32 color,SDL_Surface *image)
{
int var;
float dx;
float dy;
SDL_LockSurface(screen);
if (y == dy)
{
dy = dy/0;
}
else
{
dy = (float) ((py - y)/(px - x+.00001));
}
if (x == dx)
{
dx= 0;
}
else
{
dx = (float) ((px - x)/(py - y+.00001));
}
if(dx<dy)
{
dx = 1;
while (x<px)
    {
    putpixel(image,x,y,color);
    y += dy;
    x += dx;
    }
}
else
{
dy = 1;
while (y<py)
    {
    putpixel(image,x,y,color);
    y += dy;
    x += dx;
    }
}
 putpixel(image,x,y,color*3);
 putpixel(image,px,py,color*2);
 SDL_UnlockSurface(screen);

}
////////////////////////////////////////////////////////////////////////
*/
void ShowProgress(int value,int max,int x,int y,SDL_Surface *source
                  ,SDL_Surface *dest) {

	if ((value>-1)&(max>-1)) {
		if (value < max+1) {
			SDL_Rect source_info;
			SDL_Rect dest_info;

			SDL_GetClipRect(source,&source_info);
			SDL_GetClipRect(dest,&dest_info);
			source_info.w = int (value*source_info.w/max);
			dest_info.x = x;
			dest_info.y = y;

			SDL_BlitSurface(source,&source_info,dest,&dest_info);
		} else {
			SDL_Rect source_info;
			SDL_Rect dest_info;

			SDL_GetClipRect(source,&source_info);
			SDL_GetClipRect(dest,&dest_info);
			dest_info.x = x;
			dest_info.y = y;

			SDL_BlitSurface(source,&source_info,dest,&dest_info);
		}
	}
}
/////////////////////////////////////////////////////////////////////////

bool AddTo(Sint16 &val,int increment,Sint16 limit) {
	bool ok = false;

	if (val < limit) {
		val += increment;
		if (val>increment) {
			val = limit;
			ok = true;
		}
	}
	if (val > limit) {
		val -= increment;
		if (val<increment) {
			val = limit;
			ok = true;
		}
	}
	return ok;
}
