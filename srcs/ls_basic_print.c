/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_basic_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 20:06:08 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/25 17:27:56 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void					set_colors(t_file *file, t_ls *ls)
{
	if (ls->flags & LSO_GG)
	{
		if (S_ISDIR(file->stat.st_mode))
			ft_printf("%s%s%s", PF_BLUE, file->name, PF_EOC);
		else if (S_ISLNK(file->stat.st_mode))
			ft_printf("%s%s%s", PF_PURPLE, file->name, PF_EOC);
		else if (S_ISBLK(file->stat.st_mode))
			ft_printf("%s%s%s", PF_CYAN, file->name, PF_EOC);
		else if (S_ISCHR(file->stat.st_mode))
			ft_printf("%s%s%s", PF_YELLOW, file->name, PF_EOC);
		else if (S_ISSOCK(file->stat.st_mode))
			ft_printf("%s%s%s", PF_CYAN, file->name, PF_EOC);
		else if (S_ISFIFO(file->stat.st_mode))
			ft_printf("%s%s%s", PF_UNDER, file->name, PF_EOC);
		else if (S_ISREG(file->stat.st_mode) && (S_IXUSR & file->stat.st_mode))
			ft_printf("%s%s%s", PF_RED, file->name, PF_EOC);
		else
			ft_printf("%s", file->name);
	}
	else
		ft_printf("%s", file->name);
	if (ls->flags & LSO_P && S_ISDIR(file->stat.st_mode))
		ft_putchar('/');
}

void					print_basic_loop(t_ls *ls, int block_size)
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

inline static int		max_block_size(t_ls *ls, long long *total)
{
	t_file		*tmp;
	long long	highest;
	size_t		numfile;

	highest = 0;
	tmp = ls->file;
	numfile = ls->numfile;
	while (tmp)
	{
		if (numfile-- == 0)
			break ;
		*total += tmp->stat.st_blocks;
		if (highest < tmp->stat.st_blocks)
			highest = tmp->stat.st_blocks;
		tmp = tmp->next;
	}
	return (ft_nprintf("%lld", highest));
}

void					print_basic(t_ls *ls, int first)
{
	int			block_size;
	long long	total_size;

	block_size = 0;
	total_size = 0;
	if ((ls->flags & LSO_S) && ls->numfile)
		block_size = max_block_size(ls, &total_size);
	if (ls->directory->zu)
		((char *)ls->directory->pv)[ls->directory->zu - 1] = 0;
	if (first && (ls->flags & (LSO_RR | LSO_ARGC))
	&& ls->directory->zu && (!(ls->flags & LSO_ERROPEN)))
		ft_printf("%s:\n", ls->directory->pv);
	if (ls->directory->zu)
		((char *)ls->directory->pv)[ls->directory->zu - 1] = '/';
	if (ls->flags & LSO_S && ls->file && ls->directory->zu && ls->numfile)
		ft_printf("total %d\n", total_size);
	print_basic_loop(ls, block_size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->directory->next && !(!ls->directory->zu && !ls->file))
			ft_putchar('\n');
}
