#include "wolf.h"

static void		draw_pos(t_sdl *sdl, t_param *param)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (++i < 3)
		while (++j < 1)
			sdl_putpix(sdl, (int)(param->pos_x * 10) + i,
				(int)(param->pos_y * 10) + j, 0x00ff0000);
}

void			draw_minimap(t_sdl *sdl, t_param *param)
{
	int		i;
	int		j;
	int		k;
	int		m;

	i = -1;
	while (++i < param->map_height)
	{
		j = -1;
		while (++j < param->map_width)
		{
			k = -1;
			while (++k < 10)
			{
				m = -1;
				while (++m < 10)
					if (param->world_map[i * param->map_width + j] == 0)
						sdl_putpix(sdl, j * 10 + k, i * 10 + m, 0x00ffffff);
					else
						sdl_putpix(sdl, j * 10 + k, i * 10 + m, 0x00);
			}
		}
	}
	draw_pos(sdl, param);
}
