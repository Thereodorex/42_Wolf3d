#include "wolf.h"

static void	calc_colors(t_param *param, int x, int y)
{
	int		xorcolor;
	int		ycolor;
	int		xycolor;

	xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
	ycolor = y * 256 / TEX_HEIGHT;
	xycolor = y * 128 / TEX_HEIGHT + x * 128 / TEX_WIDTH;
	param->textures[0][TEX_WIDTH * y + x] =
		65536 * 254 * (x != y && x != TEX_WIDTH - y);
	param->textures[1][TEX_WIDTH * y + x] =
		xycolor + 256 * xycolor + 65536 * xycolor;
	param->textures[2][TEX_WIDTH * y + x] = 256 * xycolor + 65536 * xycolor;
	param->textures[3][TEX_WIDTH * y + x] =
		xorcolor + 256 * xorcolor + 65536 * xorcolor;
	param->textures[4][TEX_WIDTH * y + x] = 256 * xorcolor;
	param->textures[5][TEX_WIDTH * y + x] = 65536 * 192 * (x % 16 && y % 16);
	param->textures[6][TEX_WIDTH * y + x] = 65536 * ycolor;
	param->textures[7][TEX_WIDTH * y + x] = 128 + 256 * 128 + 65536 * 128;
}

static void	init_textures(t_sdl *sdl, t_param *param)
{
	int		x;
	int		y;

	x = -1;
	while (++x < TEX_WIDTH)
	{
		y = -1;
		while (++y < TEX_HEIGHT)
			calc_colors(param, x, y);
	}
	load_image_to_buffer(sdl, "./pics/greystone.png", param->textures[0]);
	load_image_to_buffer(sdl, "./pics/bluestone.png", param->textures[1]);
	load_image_to_buffer(sdl, "./pics/eagle.png", param->textures[2]);
	load_image_to_buffer(sdl, "./pics/redbrick.png", param->textures[3]);
	load_image_to_buffer(sdl, "./pics/purplestone.png", param->textures[4]);
	load_image_to_buffer(sdl, "./pics/colorstone.png", param->textures[5]);
	load_image_to_buffer(sdl, "./pics/mossy.png", param->textures[6]);
	load_image_to_buffer(sdl, "./pics/wood.png", param->textures[7]);
}

static void	init_params(t_sdl *sdl, t_param *param)
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
	init_textures(sdl, param);
}

static void	run_game(t_sdl *sdl, t_param *param, SDL_Event *event)
{
	while (1)
	{
		floor_cast(sdl, param, &(param->rc));
		raycast(sdl, param, &(param->rc));
		if (param->flags.minimap)
			draw_minimap(sdl, param);
		sdl_print(sdl);
		param->old_time = param->time;
		param->time = clock();
		if (catch_event(param, sdl, event))
			return ;
	}
}

int			main(int argc, char **argv)
{
	t_sdl		*sdl;
	SDL_Event	event;
	t_param		param;

	if (argc != 2)
		return (0);
	sdl = sdl_init(700, 200, WIDTH, HEIGHT);
	if (read_map(argv[1], &param))
	{
		init_params(sdl, &param);
		run_game(sdl, &param, &event);
		free(param.world_map);
	}
	sdl = sdl_destroy(sdl);
	return (0);
}
