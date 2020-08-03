#include "wolf.h"

void		*sdl_destroy(t_sdl *sdl)
{
	if (!sdl)
		return (NULL);
	if (sdl->surf)
		SDL_FreeSurface(sdl->surf);
	if (sdl->tex)
		SDL_DestroyTexture(sdl->tex);
	if (sdl->ren)
		SDL_DestroyRenderer(sdl->ren);
	if (sdl->win)
		SDL_DestroyWindow(sdl->win);
	free(sdl);
	SDL_Quit();
	return (NULL);
}

t_sdl		*sdl_init(int x, int y, int w, int h)
{
	t_sdl	*sdl;

	if ((sdl = (t_sdl *)malloc(sizeof(t_sdl))) == NULL)
		return (NULL);
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return (sdl_destroy(sdl));
	if (!(sdl->win = SDL_CreateWindow("RTv1", x, y, w, h, SDL_WINDOW_SHOWN |
					SDL_WINDOW_RESIZABLE)))
		return (sdl_destroy(sdl));
	sdl->ren = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_ACCELERATED |
											SDL_RENDERER_PRESENTVSYNC);
	if (sdl->ren == NULL)
		return (sdl_destroy(sdl));
	sdl->tex = SDL_CreateTexture(sdl->ren, SDL_PIXELFORMAT_ARGB8888,
									SDL_TEXTUREACCESS_TARGET, w, h);
	if (sdl->tex == NULL)
		return (sdl_destroy(sdl));
	if (!(sdl->surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0)))
		return (sdl_destroy(sdl));
	return (sdl);
}

// void		sdl_set_img(t_sdl *sdl, t_img *img, int x_start, int y_start)
// {
// 	int x_current;
// 	int y_current;
// 	if (img->w + x_start >= sdl->surf->w)
// 		return ;
// 	if (img->h + y_start >= sdl->surf->h)
// 		return ;
// 	y_current = 0;
// 	while (y_current < img->h)
// 	{
// 		x_current = 0;
// 		while (x_current < img->w)
// 			sdl_putpix(sdl, x_start + x_current, y_start + y_current, *(img->buffer + y_current * img->w + x_current));
// 	}
// }

void		sdl_print(t_sdl *sdl)
{
	if (sdl->tex)
		SDL_DestroyTexture(sdl->tex);
	sdl->tex = SDL_CreateTextureFromSurface(sdl->ren, sdl->surf);
	if (!sdl->tex)
		STOP;
	if (SDL_RenderCopy(sdl->ren, sdl->tex, NULL, NULL))
		STOP;
	SDL_RenderPresent(sdl->ren);
}

void		sdl_putpix(t_sdl *sdl, int x, int y, unsigned int color)
{
	unsigned int	*ptr;

	ptr = (unsigned int *)sdl->surf->pixels;
	if (x < sdl->surf->w && y < sdl->surf->h)
		*(ptr + y * sdl->surf->w + x) = color;
}