/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drina <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/07 13:03:23 by drina             #+#    #+#             */
/*   Updated: 2020/10/07 13:03:26 by drina            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct 	s_zone
{
	int 	width;
	int		height;
	char	background;
}				t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	color;
}				t_shape;

int 	ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

int 	str_error(const char *str, int ret)
{
	write(1, str, ft_strlen(str));
	return (ret);
}

int 	clear_all(FILE *file, char *str)
{
	fclose(file);
	if (str)
		free(str);
	return (1);
}

int		get_zone(FILE *file, t_zone *zone)
{
	int check;

	if ((check = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (zone->width > 300 || zone->width < 0 || zone->height > 300 || zone->height < 0)
		return (0);
	if (check == -1)
		return (0);
	return (1);
}

char	*map(t_zone *zone)
{
	char	*map;
	int 	i;

	i = 0;
	map = malloc(sizeof(char) * (zone->width * zone->height));
	if (map == NULL)
		return (NULL);

	while (i < zone->height * zone->width)
		map[i++] = zone->background;

	return (map);
}

int 	check_shape(t_shape *tmp)
{
	return (tmp->h > 0.00000000 && tmp->w > 0.00000000 && (tmp->type == 'r' || tmp->type == 'R'));
}

int 	in_rectangle(float x, float y, t_shape *shape)
{
	if ((x < shape->x || (shape->x + shape->w < x)) || ((y < shape->y) || (shape->y + shape->h < y)))
		return (0);
	if ((x - shape->x < 1.00000000) || ((shape->x + shape->w) - x < 1.00000000)
			|| (y - shape->y < 1.00000000) || ((shape->y + shape->h) - y < 1.00000000))
		return (2);
	return (1);
}

void 	draw_shape(char **map, t_shape shape, t_zone *zone)
{
	int		i;
	int 	j;
	int		ret;

	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
		{
			ret = in_rectangle(j, i, &shape);
			if ((shape.type == 'r' && ret == 2) || (shape.type == 'R' && ret))
			{
				(*map)[(i * zone->width) + j] = shape.color;
			}
			j++;
		}
		i++;
	}

}

int 	draw_shapes(FILE *file, char	**map, t_zone *zone)
{
	t_shape tmp;
	int		ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.w, &tmp.h, &tmp.color)) == 6)
	{
		if (!(check_shape(&tmp)))
			return (0);
		draw_shape(map, tmp, zone);
	}
	if (ret != -1)
		return (0);
	return (1);
}

void 	draw_map(char *map, t_zone *zone)
{
	int i;

	i = 0;
	while (i < zone->height)
	{
		write(1, map + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}

}

int		main(int argc, char **argv)
{
	FILE	*file;
	t_zone 	zone;
	char 	*drawing;

	zone.height = 0;
	zone.width = 0;
	zone.background = 0;
	if (argc != 2)
	{
		return (str_error("Error: argument\n", 1));
	}
	if (!(file = fopen(argv[1], "r")))
	{
		return (str_error("Error: Operation file corrupted\n", 1));
	}
	if (!get_zone(file, &zone))
	{
		return  (clear_all(file, NULL) && str_error("Error: Operation file corrupted\n", 1));
	}
	if (!(drawing = map(&zone)))
	{
		return (clear_all(file, NULL) && str_error("Error: Malloc Failed\n", 1));
	}
	if (!draw_shapes(file, &drawing, &zone))
	{
		return (clear_all(file, drawing) && str_error("Error: Operation file corrupted\n", 1));
	}
	draw_map(drawing, &zone);
	clear_all(file, drawing);
	return (1);
}