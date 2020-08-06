#include "wolf.h"

static void		rc_init(t_param *param, t_raycast *rc, int x)
{
	rc->camera_x = 2 * x / (double)param->w - 1;
	rc->ray_dir_x = param->dir_x + param->plane_x * rc->camera_x;
	rc->ray_dir_y = param->dir_y + param->plane_y * rc->camera_x;
	rc->map_x = (int)param->pos_x;
	rc->map_y = (int)param->pos_y;
	rc->delta_dist_x = fabs(1 / rc->ray_dir_x);
	rc->delta_dist_y = fabs(1 / rc->ray_dir_y);
	rc->hit = 0;
	rc->step_x = -1;
	rc->step_y = -1;
	if (rc->ray_dir_x < 0)
		rc->side_dist_x = (param->pos_x - rc->map_x) * rc->delta_dist_x;
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0 - param->pos_x) * rc->delta_dist_x;
	}
	if (rc->ray_dir_y < 0)
		rc->side_dist_y = (param->pos_y - rc->map_y) * rc->delta_dist_y;
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0 - param->pos_y) * rc->delta_dist_y;
	}
}

static void		rc_dda(t_param *param, t_raycast *rc)
{
	while (rc->hit == 0)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += rc->step_x;
			rc->side = 0;
			rc->tex_num = 0;
			if (rc->map_x > (int)param->pos_x)
				rc->tex_num = 2;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += rc->step_y;
			rc->side = 1;
			rc->tex_num = 1;
			if (rc->map_y > (int)param->pos_y)
				rc->tex_num = 3;
		}
		if (param->world_map[rc->map_y * param->map_width + rc->map_x] > 0)
			rc->hit = 1;
	}
}

static void		rc_line_height(t_param *param, t_raycast *rc)
{
	if (rc->side == 0)
		rc->perp_wall_dist = (rc->map_x - param->pos_x +
							(1 - rc->step_x) / 2) / rc->ray_dir_x;
	else
		rc->perp_wall_dist = (rc->map_y - param->pos_y +
								(1 - rc->step_y) / 2) / rc->ray_dir_y;
}

static void		rc_wall_cast(t_param *param, t_raycast *rc)
{
	rc_line_height(param, rc);
	rc->line_height = (int)(param->h / rc->perp_wall_dist);
	rc->draw_start = -rc->line_height / 2 + param->h / 2;
	if (rc->draw_start < 0)
		rc->draw_start = 0;
	rc->draw_end = rc->line_height / 2 + param->h / 2;
	if (rc->draw_end >= param->h)
		rc->draw_end = param->h - 1;
	if (param->flags.colors)
		rc->tex_num = param->world_map[rc->map_y *
						param->map_width + rc->map_x] - 1;
	if (rc->side == 0)
		rc->wall_x = param->pos_y + rc->perp_wall_dist * rc->ray_dir_y;
	else
		rc->wall_x = param->pos_x + rc->perp_wall_dist * rc->ray_dir_x;
	rc->wall_x -= floor((rc->wall_x));
	rc->tex_x = (int)(rc->wall_x * (double)TEX_WIDTH);
	if (rc->side == 0 && rc->ray_dir_x > 0)
		rc->tex_x = TEX_WIDTH - rc->tex_x - 1;
	if (rc->side == 1 && rc->ray_dir_y < 0)
		rc->tex_x = TEX_WIDTH - rc->tex_x - 1;
	rc->step = 1.0 * TEX_HEIGHT / rc->line_height;
	rc->tex_pos = (rc->draw_start - param->h /
					2 + rc->line_height / 2) * rc->step;
}

void			raycast(t_sdl *sdl, t_param *param, t_raycast *rc)
{
	int		x;
	int		y;
	int		color;

	x = -1;
	while (++x < param->w)
	{
		rc_init(param, rc, x);
		rc_dda(param, rc);
		rc_wall_cast(param, rc);
		y = rc->draw_start - 1;
		while (++y < rc->draw_end)
		{
			rc->tex_y = (int)rc->tex_pos & (TEX_HEIGHT - 1);
			rc->tex_pos += rc->step;
			color = param->textures[rc->tex_num][TEX_HEIGHT *
											rc->tex_y + rc->tex_x];
			if (rc->side == 1)
				color = (color >> 1) & 8355711;
			sdl_putpix(sdl, x, y, color);
		}
	}
}
