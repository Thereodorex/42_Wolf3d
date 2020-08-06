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
	IMG_Quit();
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

void		load_image_to_buffer(t_sdl *sdl, char *filename, int *buffer)
{
	unsigned		flags;
	SDL_Surface		*img;
	int				y;
	int				x;
	unsigned int	*ptr;

	flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags))
		return ;
	if (!(img = IMG_Load(filename)))
		return ;
	if (!(img = SDL_ConvertSurface(img, sdl->surf->format, 0)))
		return ;
	if (img->w != TEX_WIDTH || img->h != TEX_HEIGHT)
		return ;
	ptr = (unsigned int *)img->pixels;
	y = -1;
	while (++y < TEX_HEIGHT)
	{
		x = -1;
		while (++x < TEX_WIDTH)
			buffer[TEX_WIDTH * y + x] = *(ptr + y * img->w + x);
	}
}

void		sdl_putpix(t_sdl *sdl, int x, int y, unsigned int color)
{
	unsigned int	*ptr;

	ptr = (unsigned int *)sdl->surf->pixels;
	if (x < sdl->surf->w && y < sdl->surf->h)
		*(ptr + y * sdl->surf->w + x) = color;
}
