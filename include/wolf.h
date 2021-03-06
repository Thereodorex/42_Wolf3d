#ifndef WOLF_H
# define WOLF_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <SDL.h>
# include <SDL_image.h>
# include <time.h>

# define STOP exit(0);

# define WIDTH 1920
# define HEIGHT 1080
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
// # define mapWidth 24
// # define mapHeight 24

typedef struct	s_img
{
	int				w;
	int				h;
	unsigned int	*buffer;
}				t_img;

typedef struct	s_sdl
{
	SDL_Window		*win;
	SDL_Renderer	*ren;
	SDL_Texture		*tex;
	SDL_Surface		*surf;
	t_img			img;
}				t_sdl;

typedef struct	s_flags
{
	int			minimap;
	int			fps;
	int			colors;
}				t_flags;

typedef struct	s_floor
{
	float ray_dir_x0;
	float ray_dir_y0;
	float ray_dir_x1;
	float ray_dir_y1;
	int p;
	float pos_z;
	float row_distance;
	float step_x;
	float step_y;
	float x;
	float y;
	int cell_x;
	int cell_y;
	int tx;
	int ty;
}				t_floor;

typedef struct	s_raycast
{
	double		camera_x;
	double		ray_dir_x;
	double		ray_dir_y;
	int			map_x;
	int			map_y;
	double		side_dist_x;
	double		side_dist_y;
	double		perp_wall_dist;
	double		delta_dist_x;
	double		delta_dist_y;
	int			step_x;
	int			step_y;
	int			hit;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			tex_num;
	double		wall_x;
	int			tex_x;
	int			tex_y;
	double		tex_pos;
	double		step;
	int			hit_side;
	t_floor		floor;
}				t_raycast;

typedef struct	s_param
{
	t_raycast	rc;
	t_flags		flags;
	int			*world_map;
	int			map_width;
	int			map_height;
	int			w;
    int			h;
    double		pos_x;
    double		pos_y;
    double		dir_y;
    double		dir_x;
    double		plane_x;
    double		plane_y;
    double		time;
    double		old_time;
    double		move_speed;
    double		rot_speed;
	int			textures[8][TEX_HEIGHT * TEX_WIDTH];
}				t_param;

t_sdl			*sdl_init(int x, int y, int w, int h);
void			*sdl_destroy(t_sdl *sdl);
void			sdl_print(t_sdl *sdl);
void			sdl_putpix(t_sdl *sdl, int x, int y, unsigned int color);
void			load_image_to_buffer(t_sdl *sdl, char *filename, int *buffer);

int				catch_event(t_param *p, t_sdl *sdl, SDL_Event *event);

void			raycast(t_sdl *sdl, t_param *param, t_raycast *rc);
void			floor_cast(t_sdl *sdl, t_param *param, t_raycast *rc);

void			draw_minimap(t_sdl *sdl, t_param *param);

void			*read_map(char *filename, t_param *param);

#endif