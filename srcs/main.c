/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:23:37 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/20 10:00:27 by aulopez          ###   ########.fr       */
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

	sort_ascii(ls->file, ls->curr_file);
	while (ls->file->prev)
		ls->file = ls->file->prev;
	ls->curr_file = ls->file;
	i = 0;
	while (++i <ls->numfile)
		ls->curr_file = ls->curr_file->next;
	if (ls->flags & LSO_T)
	{
		sort_time(ls->file, ls->curr_file);
		while (ls->file->prev)
			ls->file = ls->file->prev;
		i = 0;
		ls->curr_file = ls->file;
		while (++i <ls->numfile)
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
}

/*inline static void		test(t_ls *ls)
{
	ft_printf("a: %s\n", ls->file->name);
	ft_printf("b: %s\n", ls->curr_file->name);
}*/

int						main(int ac, char **av)
{
	int		options;
	t_ls	ls;

	ft_bzero(&ls, sizeof(ls));
	if (!(options = ls_parsing(ac, av, &ls.flags)))
		return (EXIT_FAILURE);
	ac -= options;
	av += options;
	ac ? ls.flags |= LSO_ARGC : 0;
	if (!(create_first_directory(&ls, ac)))
		return (EXIT_FAILURE);
	ac ? load_info_from_argument(&ls, ac, av) : load_info_from_directory(&ls);
	if (ls.file && ls.curr_file)
		call_sorting_option(&ls);
	(ls.flags & LSO_RR || ac) ? create_directory(&ls, &ac) : 0;
	ac < 2 ? ls.flags &= ~LSO_ARGC : 0;
	ls.flags & LSO_L ? print_detailed(&ls) : print_basic(&ls);
	ls.curr_file = ls.file;
	ac = 0;
	next_dir(&ls);
	while (ls.directory)
	{
		load_info_from_directory(&ls);
		if (ls.file && ls.curr_file)
			call_sorting_option(&ls);
		create_directory(&ls, &ac);
		(ls.flags & LSO_L) ? print_detailed(&ls) : print_basic(&ls);
		ls.curr_file = ls.file;
		next_dir(&ls);
	}
	free_list(&ls);
	return (EXIT_SUCCESS);
}
