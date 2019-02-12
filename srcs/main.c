/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:23:37 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/12 21:23:01 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int		load_stats(t_ls *ls)
{
	struct stat filestat;

	if (lstat(ls->curr_file->name, &filestat) < 0)
		return (-1);
	ls->curr_file->stat = &filestat;
	if (S_ISDIR(ls->curr_file->stat->st_mode))
	{
		printf("fichier called %s\n", ls->curr_file->name);
		if (ft_strcmp(ls->curr_file->name, ".") != 0
		&& ft_strcmp(ls->curr_file->name, "..") != 0)
		{
			printf("EUUUUH\n");
			load_directory(ls);
		}
	}
	return (0);
}

int		load_info(t_ls *ls)
{
	DIR			*ddd;
	t_dirent	*dir;

	if ((ddd = opendir((char*)(ls->directory->pv))))
	{
		while ((dir = readdir(ddd)))
		{
			if (dir->d_name[0] != '.' || ls->flags & LSO_A)
			{
				if (!(ls->curr_file->name = ft_strdup(&(dir->d_name[0]))))
					return (ls_print_error(0, LSERR_MALLOC));
				load_stats(ls);
			}
		}
	}
	if (ddd)
		closedir(ddd);
	return (0);
}

int		load_file(t_ls *ls)
{
	t_file elem;

	if (ls->directory) //every case except first call argument given
	{
		if (!ls->curr_file) //we need to create a link
		{
			if (!ls->file) //we need to create the list
			{
				ft_bzero(&elem, sizeof(elem));
				if (!(ls->file = malloc(sizeof(elem))))
					return (ls_print_error(0, LSERR_MALLOC));
				ls->curr_file = ls->file;
			}
			else
			{
				ft_bzero(&elem, sizeof(elem));
				if (!(ls->curr_file = malloc(sizeof(elem))))
					return (ls_print_error(0, LSERR_MALLOC));
			}
		}
	}
	load_info(ls);
	return (1);
}

int		main(int ac, char **av)
{
	int		options;
	t_ls	ls;

	ft_bzero(&ls, sizeof(ls));
	if (!(options = ls_parsing(ac, av, &ls.flags)))
		return (EXIT_FAILURE);
	ac -= options;
	av += options;
	if (!ac)
	{
		if (!(load_directory(&ls)) || !(load_file(&ls)))
			return (EXIT_FAILURE);
	}
	/*if (ls_initialize_struct(&ls, ac, av))
		return (EXIT_FAILURE); //free_ls
	if (ls.directory)
		ls_read_from_directory(&ls);
	else if (ls.file)
	{
		//ls_ascii_order(&ls);
		//ls_find_directory(&ls);
		ls_read_from_directory(&ls);
	}*/
	return (EXIT_SUCCESS);
}
