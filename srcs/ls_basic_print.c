/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_basic_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 20:06:08 by lubenard          #+#    #+#             */
/*   Updated: 2019/03/07 10:53:21 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void					set_colors(t_file *file, t_ls *ls)
{
	if (ls->flags & LSO_GG)
	{
		if (S_ISDIR(file->stat.st_mode))
			ft_printf("%s%s%s", FT_BLUE, file->name, FT_EOC);
		else if (S_ISLNK(file->stat.st_mode))
			ft_printf("%s%s%s", FT_PURPLE, file->name, FT_EOC);
		else if (S_ISBLK(file->stat.st_mode))
			ft_printf("%s%s%s", FT_CYAN, file->name, FT_EOC);
		else if (S_ISCHR(file->stat.st_mode))
			ft_printf("%s%s%s", FT_YELLOW, file->name, FT_EOC);
		else if (S_ISSOCK(file->stat.st_mode))
			ft_printf("%s%s%s", FT_CYAN, file->name, FT_EOC);
		else if (S_ISFIFO(file->stat.st_mode))
			ft_printf("%s%s%s", FT_UNDER, file->name, FT_EOC);
		else if (S_ISREG(file->stat.st_mode) && (S_IXUSR & file->stat.st_mode))
			ft_printf("%s%s%s", FT_RED, file->name, FT_EOC);
		else
			ft_printf("%s", file->name);
	}
	else
		ft_printf("%s", file->name);
	if (ls->flags & LSO_P && S_ISDIR(file->stat.st_mode))
		ft_putchar('/');
}

inline static void		print_basic_loop(t_ls *ls, int block_size)
{
	t_file		*tmp;

	tmp = (ls->flags & LSO_R) ? ls->curr_file : ls->file;
	while (tmp)
	{
		if ((ls->flags & LSO_AA)
			&& (!ft_strcmp(tmp->name, ".") || !ft_strcmp(tmp->name, "..")))
		{
			tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
			continue ;
		}
		if (ls->numfile-- == 0)
			break ;
		else if (ls->flags & LSO_S)
		{
			ft_printf("%*lld ", block_size, tmp->stat.st_blocks);
			set_colors(tmp, ls);
		}
		else
			set_colors(tmp, ls);
		ft_putchar('\n');
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
	}
}

inline static int		max_size(t_ls *ls, long long *total, int *block_size)
{
	t_file		*tmp;
	long long	highest;
	int			longest;
	int			length;
	size_t		numfile;

	highest = 0;
	longest = 0;
	tmp = ls->file;
	numfile = ls->numfile;
	while (tmp)
	{
		if (numfile-- == 0)
			break ;
		*total += tmp->stat.st_blocks;
		highest = highest < tmp->stat.st_blocks ? tmp->stat.st_blocks : highest;
		if ((ls->flags & LSO_CC) && (length = ft_strlen(tmp->name)
		+ (ls->flags & LSO_P && S_ISDIR(tmp->stat.st_mode)))
		&& longest < length)
			longest = length;
		tmp = tmp->next;
	}
	*block_size = ft_nprintf("%lld", highest);
	return (longest);
}

void					print_basic(t_ls *ls, int first)
{
	int			block_size;
	long long	total_size;
	int			str_size;

	block_size = 0;
	total_size = 0;
	str_size = 0;
	if ((ls->flags & (LSO_S | LSO_CC)) && ls->numfile)
		str_size = max_size(ls, &total_size, &block_size);
	if (ls->directory->zu)
		((char *)ls->directory->pv)[ls->directory->zu - 1] = 0;
	if (first && (ls->flags & (LSO_RR | LSO_ARGC))
	&& ls->directory->pv && (!(ls->flags & LSO_ERROPEN)))
		ft_printf("%s:\n", ls->directory->pv);
	if (ls->directory->zu)
		((char *)ls->directory->pv)[ls->directory->zu - 1] = '/';
	if (ls->flags & LSO_S && ls->file && ls->directory->zu && ls->numfile)
		ft_printf("total %d\n", total_size);
	ls->flags & LSO_CC ? print_column_loop(ls, block_size, str_size) :
	print_basic_loop(ls, block_size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->directory->next && !(!ls->directory->zu && !ls->file))
			ft_putchar('\n');
}
