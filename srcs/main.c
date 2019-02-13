/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:23:37 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/13 15:07:33 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int		load_stats(t_ls *ls, char *filename)
{
	struct stat filestat;
	char	*tmp;

	tmp = (char*)(ls->directory->pv);
	load_file(ls);
	if (!(ls->curr_file->name = ft_strdup(filename)))
		return (ls_print_error(0, LSERR_MALLOC));
	printf("Getting info about %s%s\n", ls->directory->pv, filename);
	if (lstat(ft_strcat(ls->directory->pv, filename), &filestat) < 0) //filename should be the whole path. Use ls->directory->pv. Current strlen of ls->directory->pv is contain in ls->directory->zu
		return (-1);
	printf("tmp = %s\n", tmp);
	tmp[ls->directory->zu] = 0;
	ls->curr_file->stat = &filestat;
	printf("mode %ho\n", filestat.st_mode);
	if (S_ISDIR(filestat.st_mode))
	{
		printf("Directory called %s\n\n", ls->curr_file->name);
		if (ft_strcmp(ls->curr_file->name, ".") != 0
		&& ft_strcmp(ls->curr_file->name, "..") != 0)
			load_directory(ls);
	}
	else
		printf("File called %s\n", ls->curr_file->name);
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
				//if (!(ls->curr_file->name = ft_strdup(&(dir->d_name[0]))))
				//	return (ls_print_error(0, LSERR_MALLOC));
				load_stats(ls, &dir->d_name[0]);
			}
		}
	}
	if (ddd)
		closedir(ddd);
	return (0);
}

int		load_file(t_ls *ls)
{
	if (ls->directory) //every case except first call argument given
	{
		if (!ls->file) //we need to create a link
		{
			if (!(ls->file = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->file, sizeof(ls->file));
			ls->curr_file = ls->file;
		}
		else if (!ls->curr_file->next)
		{
			if (!(ls->curr_file->next = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->curr_file->next, sizeof(ls->file));
			ls->curr_file->next->prev = ls->curr_file;
			ls->curr_file = ls->curr_file->next;
			ls->curr_file->next = 0;
		}
		else
			ls->curr_file = ls->curr_file->next;
	}
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
		if (!(load_directory(&ls)))
			return (EXIT_FAILURE);
		load_info(&ls);
	}
	ls.curr_file = ls.file;
	while (ls.curr_file)
	{
	//	print_ls(&ls); Disabled print ls to avoid message spamming 
		ls.curr_file = (ls.curr_file)->next;
	}
	t_list *tmp;
	tmp = ls.directory;
	ls.directory = ls.directory->next;
	ls.curr_file = ls.file;
	load_info(&ls);
	ls.curr_file = ls.file;
	printf("\n\n\n\n\n\n\n\n");
	while (ls.curr_file)
	{
		print_ls(&ls);
		ls.curr_file = (ls.curr_file)->next;
	}
	while (tmp)
	{
		printf("%s\n", (char*)(tmp->pv));
		tmp = (tmp)->next;
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
