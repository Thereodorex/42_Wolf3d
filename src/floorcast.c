#include "wolf.h"

static void		floor_init(t_param *param, t_raycast *rc, int y)
{
	rc->floor.ray_dir_x0 = param->dir_x - param->plane_x;
	rc->floor.ray_dir_y0 = param->dir_y - param->plane_y;
	rc->floor.ray_dir_x1 = param->dir_x + param->plane_x;
	rc->floor.ray_dir_y1 = param->dir_y + param->plane_y;
	rc->floor.p = y - param->h / 2;
	rc->floor.pos_z = 0.5 * param->h;
	rc->floor.row_distance = rc->floor.pos_z / rc->floor.p;
	rc->floor.step_x = rc->floor.row_distance *
		(rc->floor.ray_dir_x1 - rc->floor.ray_dir_x0) / param->w;
	rc->floor.step_y = rc->floor.row_distance *
		(rc->floor.ray_dir_y1 - rc->floor.ray_dir_y0) / param->w;
	rc->floor.x = param->pos_x + rc->floor.row_distance * rc->floor.ray_dir_x0;
	rc->floor.y = param->pos_y + rc->floor.row_distance * rc->floor.ray_dir_y0;
}

static void		step(t_raycast *rc)
{
	rc->floor.cell_x = (int)(rc->floor.x);
	rc->floor.cell_y = (int)(rc->floor.y);
	rc->floor.tx = (int)((double)TEX_WIDTH *
		(rc->floor.x - rc->floor.cell_x)) & (TEX_WIDTH - 1);
	rc->floor.ty = (int)((double)TEX_HEIGHT *
		(rc->floor.y - rc->floor.cell_y)) & (TEX_HEIGHT - 1);
	rc->floor.x += rc->floor.step_x;
	rc->floor.y += rc->floor.step_y;
}

void			floor_cast(t_sdl *sdl, t_param *param, t_raycast *rc)
{
	int		x;
	int		y;
	int		color;

	y = -1;
	while (++y < param->h)
	{
		floor_init(param, rc, y);
		x = -1;
		while (++x < param->w)
		{
			step(rc);
			color = param->textures[6][TEX_WIDTH * rc->floor.ty + rc->floor.tx];
			color = (color >> 1) & 8355711;
			sdl_putpix(sdl, x, y, color);
			color = param->textures[7][TEX_WIDTH * rc->floor.ty + rc->floor.tx];
			color = (color >> 1) & 8355711;
			sdl_putpix(sdl, x, param->h - y - 1, color);
		}
	}
}
