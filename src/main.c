#include "wolf.h"

void    init_textures(t_param *param)
{
	int     x;
	int     y;
	int     xorcolor;
	int     ycolor;
	int     xycolor;

	x = -1;
	while (++x < TEX_WIDTH)
	{
		y = - 1;
		while (++y < TEX_HEIGHT)
		{
			xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
			ycolor = y * 256 / TEX_HEIGHT;
			xycolor = y * 128 / TEX_HEIGHT + x * 128 / TEX_WIDTH;
			param->textures[0][TEX_WIDTH * y + x] = 65536 * 254 * (x != y && x != TEX_WIDTH - y); //flat red texture with black cross
			param->textures[1][TEX_WIDTH * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
			param->textures[2][TEX_WIDTH * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
			param->textures[3][TEX_WIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
			param->textures[4][TEX_WIDTH * y + x] = 256 * xorcolor; //xor green
			param->textures[5][TEX_WIDTH * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
			param->textures[6][TEX_WIDTH * y + x] = 65536 * ycolor; //red gradient
			param->textures[7][TEX_WIDTH * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
		}
	}
}

void    init_params(t_param *param)
{
	param->dir_x = 1.0;
	param->dir_y = 0.0;
	param->plane_x = 0.0;
	param->plane_y = 0.66;
	param->w = WIDTH;
	param->h = HEIGHT;
	param->time = 0.0;
	param->old_time = 0.0;
	param->move_speed = 0.09;
	param->rot_speed = 0.06;
	param->flags.minimap = 0;
	param->flags.colors = 0;
	init_textures(param);
}

void game(t_sdl *sdl, t_param *param, SDL_Event *event)
{
	int x;
	int y;

	while(1)
	{
		raycast(sdl, param, &(param->rc));
		sdl_print(sdl);
		y = -1;
		while(++y < param->h)
		{
			x = -1;
			while (++x < param->w)
				sdl_putpix(sdl, x, y, y < HEIGHT / 2 ? 0x0099ccff : 0x005c85d6);
		}
		param->old_time = param->time;
		param->time = clock();
		// printf("%f\n", 1.0 / ((param->time - param->old_time) / 1000000.0));

		if (catch_event(param, sdl, event))
			return ;
	}
}

int		main(int argc, char **argv)
{
	t_sdl		*sdl;
	SDL_Event	event;
	t_param		param;

	if (argc != 2)
		return (0);
	sdl = sdl_init(700, 200, WIDTH, HEIGHT);
	if (read_map(argv[1], &param))
	{
		init_params(&param);
		game(sdl, &param, &event);
		free(param.world_map);
	}
	sdl = sdl_destroy(sdl);

	return (0);
}