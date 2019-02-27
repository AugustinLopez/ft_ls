/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_basic_print_column.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 17:03:59 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/27 20:28:13 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int		create_string(t_ls *ls, t_file *file, char *dst,
						char *src)
{
	ft_strcpy(dst, src);
	if (ls->flags & LSO_P && S_ISDIR(file->stat.st_mode))
	{
		ft_strcat(dst, PF_EOC);
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
			ft_printf("%s%-*s%s", PF_BLUE, str, cpy, PF_EOC);
		else if (S_ISLNK(file->stat.st_mode))
			ft_printf("%s%-*s%s", PF_PURPLE, str, cpy, PF_EOC);
		else if (S_ISBLK(file->stat.st_mode))
			ft_printf("%s%-*s%s", PF_CYAN, str, cpy, PF_EOC);
		else if (S_ISCHR(file->stat.st_mode))
			ft_printf("%s%-*s%s", PF_YELLOW, str, cpy, PF_EOC);
		else if (S_ISSOCK(file->stat.st_mode))
			ft_printf("%s%-*s%s", PF_CYAN, str, cpy, PF_EOC);
		else if (S_ISFIFO(file->stat.st_mode))
			ft_printf("%s%-*s%s", PF_UNDER, file->name, PF_EOC);
		else if (S_ISREG(file->stat.st_mode) && (S_IXUSR & file->stat.st_mode))
			ft_printf("%s%-*s%s", PF_RED, str, cpy, PF_EOC);
		else
			ft_printf("%-*s", str, cpy);
	}
	else
		ft_printf("%-*s", str, cpy);
}

inline static void		actual_print(t_ls *ls, size_t size[6], t_file *tmp)
{
	t_file	*tmp2;
	int		i;

	i = ls->numfile;
	while (size[5]--)
	{
		if (!tmp)
			break ;
		size[4] = size[3];
		if ((ls->flags & LSO_AA)
				&& (!ft_strcmp(tmp->name, ".") || !ft_strcmp(tmp->name, "..")))
		{
			while (size[4]-- && tmp)
				tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
			i -= size[3];
			continue ;
		}
		else if (ls->flags & LSO_S)
			ft_printf("%*lld ", size[0], tmp->stat.st_blocks);
		tmp2 = tmp;
		while (size[4]-- && tmp)
			tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
		i--;
		if (i >= (int)size[3])
			(size[5] && tmp) ? set_colors_space(tmp2, ls, size[1]) :
			set_colors_space(tmp2, ls, 0);
	}
}

void					print_column_loop(t_ls *ls, int block_size,
						int str_size)
{
	t_file			*tmp;
	t_file			*tmp2;
	struct ttysize	ts;
	size_t			size[6];

	size[0] = block_size;
	size[1] = str_size + 1 + (size[0] != 0);
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
	ft_printf("%zu %zu %zu\n", ls->numfile, size[2], size[3]);
	while (ls->numfile)
	{
		tmp2 = tmp;
		size[5] = size[2];
		actual_print(ls, size, tmp2);
		ft_putchar('\n');
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
		ls->numfile -= ls->numfile < size[2] ? ls->numfile : size[2];
	}
}
