/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_basic_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 20:06:08 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/20 11:55:13 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void			print_basic_loop(t_ls *ls, int block_size)
{
	t_file		*tmp;

	tmp = (ls->flags & LSO_R) ? ls->curr_file : ls->file ;
	while (tmp)
	{
		if (ls->numfile-- == 0)
			break ;
		else if (ls->flags & LSO_S)
			ft_printf("%*lld %s\n", block_size, tmp->stat.st_blocks, tmp->name);
		else
			ft_printf("%s\n", tmp->name);
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
	}
}

int				max_block_size(t_ls *ls, long long *total, int *onedir)
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
		if (S_ISDIR(tmp->stat.st_mode))
			*onedir = 1;
		tmp = tmp->next;
	}
	return (ft_nprintf("%lld", highest));
}

void			print_basic(t_ls *ls)
{
	int			block_size;
	long long	total_size;
	int			onedir;

	onedir = 0;
	block_size = 0;
	total_size = 0;
	if ((ls->flags & LSO_S) && ls->numfile)
		block_size = max_block_size(ls, &total_size, &onedir);
	if (ls->flags & (LSO_RR | LSO_ARGC)
	&& ls->directory->zu && !(ls->flags & LSO_ERROPEN) && onedir)
		ft_printf("%s:\n", ls->directory->pv);
	if (ls->flags & (LSO_L | LSO_S))
		ft_printf("total %d\n", total_size);
	print_basic_loop(ls, block_size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->file && ls->directory->next)
			ft_putchar('\n');
}
