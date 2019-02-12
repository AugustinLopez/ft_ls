# include <ft_ls.h>

int		ls_create_file_link(t_ls *ls)
{
	t_stat *elem;

	ft_bzero(&elem, sizeof(elem));
	if (ls->file)
	{
		if (!(ls->curr_file = ft_lstnew(&elem, sizeof(elem))))
			return ((ls_print_error(0, LSERR_MALLOC)));
		ls->last_file->next = ls->curr_file;
		ls->last_file = ls->curr_file;
	}
	else
	{
		if (!(ls->file = ft_lstnew(&elem, sizeof(elem))))
			return ((ls_print_error(0, LSERR_MALLOC)));
		ls->curr_file = ls->file;
		ls->last_file = ls->file;
	}
	return (1);
}

int		ls_get_path(t_ls *ls, char *name, char path[PATH_MAX])
{
	int		i;
	char	*dir;

	dir = ((char*)(ls->curr_dir->pv));
	i = -1;
	while (dir[++i])
		path[i] = dir[i];
	if (i && i < PATH_MAX)
		if (!(dir[0] == '/' && dir[1] == '\0'))
			path[i++] = '/';
	while (*name && i < PATH_MAX)
		path[i++] = *name++;
	if (i < PATH_MAX)
		path[i] = '\0';
	else
	{
		errno = ENAMETOOLONG;
		return (ls_print_error(0, LSERR_LONG));
	}
	return (1);
}

int		ls_set_file(t_ls *ls, char *name, t_stat *stat)
{
	t_stat	*elem;
	char	*test;

	elem = ((t_file*)(ls->curr_file->pv))->stat;
	if (!(test = ft_strdup(name)))
		return (ls_print_error(0, LSERR_MALLOC));
	((t_file*)(ls->curr_file->pv))->stat->st_mode = stat->st_mode;
	printf("%d\n", elem->st_mode);
	elem->st_nlink = stat->st_nlink;
	elem->st_uid = stat->st_uid;
	elem->st_gid = stat->st_gid;
	elem->st_size = stat->st_size;
	elem->st_mtim.tv_sec = stat->st_mtim.tv_sec;
	elem->st_blocks = stat->st_blocks;
	ls->curr_file->zu = ls->curr_dir->zu;
	if (ls->curr_file->next)
		ls->curr_file->next->zu = 0;
	((t_file*)(ls->curr_file->pv))->name = test;
	return (1);
}

int		ls_add_file(t_ls *ls, char *name)
{
	char		path[PATH_MAX];
	t_stat	stat;

	ft_bzero(path, sizeof(char) * (PATH_MAX + 1));
	if (!ls->curr_file && !ls_create_file_link(ls))
		return (0);
	if (!(ls_get_path(ls, name, path)))
		return (ls_print_error(0, LSERR_PATH));
	if (lstat(path, &stat) == -1)
		return (ls_print_error(0, LSERR_OTHER));
	if (!ls_set_file(ls, name, &stat))
		return (0);
	return (1);

}

int		ls_read_from_directory(t_ls *ls)
{
	t_dirent		*item;
	DIR				*directory;

	ls->curr_file = ls->file;
	if (!(directory = opendir((char*)(ls->curr_dir->pv))))
		return (ls_print_error((char*)(ls->curr_dir->pv), LSERR_OTHER));
	while ((item = readdir(directory)))
	{
		if (((item->d_name)[0] != '.' || ls->flags & LSO_A))
		{
			if (!(ls_add_file(ls, item->d_name)))
			{
				closedir(directory);
				return (0);
			}
			ls->curr_file = ls->curr_file->next;
		}
	}
	if (closedir(directory) == -1)
		return (ls_print_error(0, LSERR_CLOSEDIR));
	return (1);
}
