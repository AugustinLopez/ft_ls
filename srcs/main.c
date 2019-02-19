/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:23:37 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/18 19:29:48 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

/*static inline void	readder(t_ls *ls)
{
	ls->curr_file = ls->file;
	while (ls->curr_file)
	{
		if (ls->numfile-- == 0)
			break;
		print_basic(ls);
		ls->curr_file = (ls->curr_file)->next;
	}
}*/

static inline void		free_list(t_ls *ls)
{
	while (ls->file)
	{
		ls->curr_file = ls->file->next;
		free(ls->file->name);
		free(ls->file);
		ls->file = ls->curr_file;
	}
	if (ls->directory)
		ft_lstdel(&ls->directory, *ft_lstfree);
}

int						main(int ac, char **av)
{
	int		options;
	t_ls	ls;

	ft_bzero(&ls, sizeof(ls));
	if (!(options = ls_parsing(ac, av, &ls.flags)))
		return (EXIT_FAILURE);
	ac -= options;
	av += options;
	if (ac)
		ls.flags |= LSO_ARGC;
	if (!(create_directory(&ls, &ac)))
		return (EXIT_FAILURE);
	if (ac)
		load_info_from_argument(&ls, ac, av);
	else
		load_info_from_directory(&ls);
	sort_ascii(ls.file, ls.curr_file);
	while (ls.file->prev)
		ls.file = ls.file->prev;
	if (ls.flags & LSO_T)
	{
		sort_time(ls.file, ls.curr_file);
		while (ls.file->prev)
			ls.file = ls.file->prev;
	}
	ls.curr_file = ls.file;
	if (ls.flags & LSO_RR || ac)
		create_directory(&ls, &ac);
	if (ac < 2)
		ls.flags &= ~LSO_ARGC;
	if (ls.flags & LSO_L)
		print_detailed(&ls);
	else
		print_basic(&ls);
	ac = 0;
	next_dir(&ls);
	while (ls.directory)
	{
		load_info_from_directory(&ls);
		sort_ascii(ls.file, ls.curr_file);
		while (ls.file->prev)
			ls.file = ls.file->prev;
		if (ls.flags & LSO_T)
		{
			sort_time(ls.file, ls.curr_file);
			while (ls.file->prev)
				ls.file = ls.file->prev;
		}
		ls.curr_file = ls.file;
		create_directory(&ls, &ac);
		if (ls.flags & LSO_L)
			print_detailed(&ls);
		else
			print_basic(&ls);
		next_dir(&ls);
	}
	free_list(&ls);
	return (EXIT_SUCCESS);
}
