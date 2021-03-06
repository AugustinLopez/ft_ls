/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_basic_print_column.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 17:03:59 by aulopez           #+#    #+#             */
/*   Updated: 2019/03/07 10:53:38 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int		create_string(t_ls *ls, t_file *file, char *dst,
						char *src)
{
	ft_strcpy(dst, src);
	if (ls->flags & LSO_P && S_ISDIR(file->stat.st_mode))
	{
		ft_strcat(dst, FT_EOC);
		ft_strcat(dst, "/");
		return (4);
	}
	return (0);
}

inline static void		set_colors_space(t_file *file, t_ls *ls, int str)
{
	char	cpy[265];

	str += create_string(ls, file, cpy, file->name);
	if (ls->flags & LSO_GG)
	{
		if (S_ISDIR(file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_BLUE, str, cpy, FT_EOC);
		else if (S_ISLNK(file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_PURPLE, str, cpy, FT_EOC);
		else if (S_ISBLK(file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_CYAN, str, cpy, FT_EOC);
		else if (S_ISCHR(file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_YELLOW, str, cpy, FT_EOC);
		else if (S_ISSOCK(file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_CYAN, str, cpy, FT_EOC);
		else if (S_ISFIFO(file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_UNDER, file->name, FT_EOC);
		else if (S_ISREG(file->stat.st_mode) && (S_IXUSR & file->stat.st_mode))
			ft_printf("%s%-*s%s", FT_RED, str, cpy, FT_EOC);
		else
			ft_printf("%-*s", str, cpy);
	}
	else
		ft_printf("%-*s", str, cpy);
}

inline static void		actual_print(t_ls *ls, size_t size[7], t_file *file)
{
	t_file	*tmp2;
	t_file	*tmp;

	tmp = file;
	while (size[5]--)
	{
		if (!tmp || size[6] || tmp->prev == ls->curr_file)
			break ;
		size[4] = size[3];
		if (ls->flags & LSO_S)
			ft_printf("%*lld ", size[0], tmp->stat.st_blocks);
		tmp2 = tmp;
		while (size[4]-- && tmp)
		{
			if (tmp->prev == ls->curr_file)
				size[6] = 1;
			tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
		}
		(size[5] && tmp) ? set_colors_space(tmp2, ls, size[1]) :
			set_colors_space(tmp2, ls, 0);
	}
}

void					print_column_loop(t_ls *ls, int block_size,
						int str_size)
{
	t_file			*tmp;
	struct ttysize	ts;
	size_t			size[7];

	size[0] = block_size;
	size[1] = str_size + 2;
	ioctl(0, TIOCGWINSZ, &ts);
	size[2] = (ls->flags & LSO_S) ?
	(int)(ts.ts_cols / (size[1] + size[0] + 1)) :
	(int)(ts.ts_cols / (size[1]));
	size[2] > ls->numfile ? size[2] = ls->numfile : 0;
	!size[2] ? size[2] = 1 : 0;
	size[3] = (ls->numfile / size[2]);
	(size[3] * size[2]) > ls->numfile ? size[2]-- : 0;
	(size[3] * size[2]) < ls->numfile ? size[3]++ : 0;
	tmp = (ls->flags & LSO_R) ? ls->curr_file : ls->file;
	while (ls->numfile)
	{
		size[5] = size[2];
		size[6] = 0;
		actual_print(ls, size, tmp);
		ft_putchar('\n');
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
		ls->numfile -= ls->numfile < size[2] ? ls->numfile : size[2];
	}
}
