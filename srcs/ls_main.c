/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:23:37 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/20 18:25:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static void		free_list(t_ls *ls)
{
	while (ls->file)
	{
		ls->curr_file = ls->file->next;
		free(ls->file);
		ls->file = ls->curr_file;
	}
	if (ls->directory)
		ft_lstdel(&ls->directory, *ft_lstfree);
}

inline static void		call_sorting_option(t_ls *ls)
{
	unsigned int i;

	if (ls->flags & !LSO_F) //need approval
		sort_ascii(ls->file, ls->curr_file);
	while (ls->file->prev)
		ls->file = ls->file->prev;
	ls->curr_file = ls->file;
	i = 0;
	while (++i < ls->numfile)
		ls->curr_file = ls->curr_file->next;
	if (ls->flags & LSO_T)
	{
		sort_time(ls->file, ls->curr_file);
		while (ls->file->prev)
			ls->file = ls->file->prev;
		i = 0;
		ls->curr_file = ls->file;
		while (++i < ls->numfile)
			ls->curr_file = ls->curr_file->next;
	}
}

inline static void		next_dir(t_ls *ls)
{
	t_list		*tmp;

	tmp = ls->directory;
	ls->directory = ls->directory->next;
	ls->curr_dir = ls->directory;
	ft_lstdelone(&tmp, *ft_lstfree);
	ls->curr_file = ls->file;
}

inline static int		initialisation(t_ls *ls, int *ac, char ***av)
{
	int option;

	ft_bzero(ls, sizeof(*ls));
	if (!(option = ls_parsing(*ac, *av, &ls->flags)))
		return (0);
	*ac -= option;
	*av += option;
	option = 1;
	*ac ? ls->flags |= LSO_ARGC : 0;
	if (!(create_first_directory(ls, *ac)))
		return (0);
	return (1);
}

int						main(int ac, char **av)
{
	t_ls	ls;

	if (!initialisation(&ls, &ac, &av))
		return (EXIT_FAILURE);
	(ac) ? load_info_from_argument(&ls, ac, av) : load_info_from_directory(&ls);
	(ls.file && ls.curr_file) ? call_sorting_option(&ls) : 0;
	(ls.flags & LSO_RR || ac) ? create_directory(&ls, ac) : 0;
	(!ac && !ls.directory->next) ? ls.flags &= ~LSO_RR : 0;
	ls.flags & LSO_L ? print_detailed(&ls, 0) : print_basic(&ls, 0);
	next_dir(&ls);
	while (ls.directory)
	{
		load_info_from_directory(&ls);
		(ls.file && ls.curr_file) ? call_sorting_option(&ls) : 0;
		create_directory(&ls, 0);
		if (ac && !ls.directory->next && !(ac = 0))
			(ls.flags & LSO_L) ? print_detailed(&ls, 0) : print_basic(&ls, 0);
		else
			(ls.flags & LSO_L) ? print_detailed(&ls, 1) : print_basic(&ls, 1);
		next_dir(&ls);
	}
	free_list(&ls);
	return (EXIT_SUCCESS);
}
