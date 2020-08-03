#include "wolf.h"

static void		move(t_param *param, int back)
{
	int x;
	int y;
	double move_speed;

	move_speed = back ? -param->move_speed : param->move_speed;
	x = (int)(param->pos_x + param->dir_x * move_speed);
	y = (int)(param->pos_y);
	if(*(param->world_map + y * param->map_width + x) == 0)
		param->pos_x += param->dir_x * move_speed;
	x = (int)(param->pos_x);
	y = (int)(param->pos_y + param->dir_y * move_speed);
	if(*(param->world_map + y * param->map_width + x) == 0)
		param->pos_y += param->dir_y * move_speed;
}

static void		rotate(t_param *param, int left)
{
	double old_dir_x;
	double old_plane_x;
	double rot_speed;

	rot_speed = left ? param->rot_speed : -param->rot_speed;
	old_dir_x = param->dir_x;
	param->dir_x = param->dir_x * cos(rot_speed) - param->dir_y * sin(rot_speed);
	param->dir_y = old_dir_x * sin(rot_speed) + param->dir_y * cos(rot_speed);
	old_plane_x = param->plane_x;
	param->plane_x = param->plane_x * cos(rot_speed) - param->plane_y * sin(rot_speed);
	param->plane_y = old_plane_x * sin(rot_speed) + param->plane_y * cos(rot_speed);
}

int			catch_event(t_param *param, t_sdl *sdl, SDL_Event *event)
{
	if (SDL_PollEvent(event))
        {
            if (event->type == SDL_QUIT)
                return (1);
            else if (event->type == SDL_WINDOWEVENT)
            {
                if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    SDL_DestroyRenderer(sdl->ren);
                    sdl->ren = SDL_CreateRenderer(sdl->win, -1,
                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                    sdl_print(sdl);
                }
            }
            else if (event->type == SDL_KEYDOWN)
            {
                if (event->key.keysym.sym == SDLK_ESCAPE)
                    return (1);
                if (event->key.keysym.sym == SDLK_UP)
					move(param, 0);
                if (event->key.keysym.sym == SDLK_DOWN)
					move(param, 1);
                if (event->key.keysym.sym == SDLK_RIGHT)
					rotate(param, 0);
                if (event->key.keysym.sym == SDLK_LEFT)
					rotate(param, 1);
            }
        }
	return (0);
}