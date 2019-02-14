/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:23:37 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/14 11:15:49 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int		load_stats(t_ls *ls, char *filename)
{
	char	*tmp;

	load_file(ls);
	tmp = (char*)(ls->directory->pv);
	if (!(ls->curr_file->name = ft_strdup(filename)))
		return (ls_print_error(0, LSERR_MALLOC));
	if (lstat(ft_strcat(tmp, filename), &(ls->curr_file->stat)) < 0) 
		return (-1);
	tmp[ls->directory->zu] = 0;
	if (S_ISDIR(ls->curr_file->stat.st_mode))
	{
		if (ft_strcmp(ls->curr_file->name, ".") != 0
		&& ft_strcmp(ls->curr_file->name, "..") != 0)
			load_directory(ls);
	}
	print_ls(ls);
	if (ls->curr_file->prev)
		print_previous(ls);
	return (0);
}

int		load_info(t_ls *ls)
{
	DIR			*ddd;
	t_dirent	*dir;

	if ((ddd = opendir((char*)(ls->directory->pv))))
		while ((dir = readdir(ddd)))
			if (dir->d_name[0] != '.' || ls->flags & LSO_A)
				load_stats(ls, &dir->d_name[0]);
	if (ddd)
		closedir(ddd);
	return (0);
}

int		load_file(t_ls *ls)
{
	if (ls->directory) //every case except first call argument given
	{
		if (!(ls->file)) //we need to create a link
		{
			if (!(ls->file = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->file, sizeof(ls->file));
			ls->curr_file = ls->file;
		}
		else if (!(ls->curr_file->next))
		{
			if (!(ls->curr_file->next = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->curr_file->next, sizeof(ls->curr_file));
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
		//print_ls(&ls);
		ls.curr_file = (ls.curr_file)->next;
	}
	/*t_list *tmp;
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
	}*/
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
