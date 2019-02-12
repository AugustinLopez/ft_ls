#include <ft_ls.h>

int		load_directory(t_ls *ls)
{
	char		*tmp;
	size_t		i;
	t_list		*tmpdir;

	if (!ls->directory && !ls->file) //first call no argument given
	{
		if (!(ls->directory = ft_lstnew("./", sizeof(char) * 2)))
			return (ls_print_error(0, LSERR_MALLOC));
		if (!(tmp = ft_strnew(2 + 256)))
			return (ls_print_error(0, LSERR_MALLOC));
		tmp[0] = '.';
		tmp[1] = '/';
		tmp[2] = 0;
		free(ls->directory->pv);
		ft->directory->pv = tmp;
		ft->directory->zu= 2;
		return (1);
	}
	if (!(tmpdir = ft_lstnew("./", sizeof(char) * 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	i = ft_strlen(ls->curr_file->name);
	if (!(tmp = ft_strnew(i + 1 + 256)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, ls->curr_file->name);
	tmp[i] = '/';
	tmp[i + 1] = 0;
	tmpdir->pv = tmp;
	tmpdir->zu = i;
	if (!ls->directory)
		ls->directory = tmpdir;
	else if (!ls->directory->next)
		ls->directory->next = tmpdir;
	else
		ft_lstadd(&(ls->directory->next), tmpdir);
	return (1);
}

int		load_info(t_ls *ls)
{
	DIR 		*ddd;
	t_dirent	*dir;

	if ((ddd = opendir((char*)(ls->directory->pv))))
	{
		while ((dir = readdir(ddd)))
		{
			if (dir->d_name[0] != '.' || ls->flags & LSO_A)
			{
				if (!(ls->curr_file->name = ft_strdup(&(dir->d_name))))
					return (ls_print_error(0, LSERR_MALLOC));
				ft_printf("%s ", ls->curr_file->name);
			}
		}
	}
	printf("\n");
	if (ddd)
		closedir(ddd);
	return (0);
}

int		load_file(t_ls *ls)
{
	if (!ls->curr_file) //we need to create a link
	{
		if (!ls->file) //we need to create the list
		{
			if (!(ls->file = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->file, sizeof(&(ls->file)));
			ls->curr_file = ls->file;
		}
		else
		{
			if (!(ls->curr_file = malloc(sizeof(t_file))))
				return (ls_print_error(0, LSERR_MALLOC));
			ft_bzero(ls->curr_file, sizeof(&(ls->file)));
			ls->curr_file
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
