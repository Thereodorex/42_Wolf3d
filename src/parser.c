#include "wolf.h"

int			validate(t_param *param, int fd)
{
	char	c;
	int		i;
	char	*line;
	int		start;

	start = 0;
	c = 1;
	param->map_width = -1;
	while (c != '\n')
	{
		if ((read(fd, &c, 1)) != 1)
			return (0);
		if (c == 'X')
		{
			if (start)
				return (0);
			start = 1;
		}
		else if ((c > '9' || c < '0') && c != '\n')
			return (0);
		param->map_width++;
	}
	param->map_height = 1;
	line = (char*)malloc(sizeof(char) * (param->map_width + 1));
	while ((read(fd, line, param->map_width + 1) == param->map_width + 1))
	{
		if (line[param->map_width] != '\n')
			return (0);
		line[param->map_width] = '\0';
		i = -1;
		while (++i < param->map_width)
			if (line[i] == 'X')
			{
				if (start)
					return (0);
				start = 1;
			}
			else if (line[i] > '9' || line[i] < '0')
				return (0);
		param->map_height++;
	}
	free(line);
	if (!start)
		return(0);
	return (1);
}

void		*read_map(char *filename, t_param *param)
{
	int		fd;
	char	*line;
	int		j;
	int		i;

	param->pos_x = 0;
	param->pos_y = 0;
	if ((fd = open(filename, O_RDONLY)) == -1)
		STOP;
	if (!validate(param, fd))
	{
		printf("here\n");
		return NULL;
	}
	close(fd);
	param->world_map = (int*)malloc(sizeof(int) * (param->map_height+2) * (param->map_width+2));
	line = (char*)malloc(sizeof(char) * (param->map_width + 1));
	if ((fd = open(filename, O_RDONLY)) == -1)
		STOP;
	i = -1;
	while(++i < param->map_height + 2)
	{
		if (i != 0 && i != param->map_height + 1)
			read(fd, line, param->map_width + 1);
		j = -1;
		while (++j < param->map_width + 2)
		{
			if (i == 0 || j == 0 ||
				i == param->map_height + 1 ||
				j == param->map_width + 1)
				param->world_map[i * (param->map_width+2) + j] = 1;
			else if (line[j-1] == 'X')
			{
				param->pos_x = (double)j + 0.5;
				param->pos_y = (double)i + 0.5;
				param->world_map[i * (param->map_width+2) + j] = 0;
			}
			else
				param->world_map[i * (param->map_width+2) + j] = (int)(line[j-1] - '0');
		}
	}
	param->map_width += 2;
	param->map_height += 2;
	close(fd);
	free(line);
	printf("%d  %d", param->map_width, param->map_height);
	return ((void*)param);
}
