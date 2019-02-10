# include <ft_ls.h>

/*
** If no argument besides options are given, we initialize a directory list.
** If such arguments are given, we initialize a file list.
*/

int		ls_initialize_struct(t_ls *ls, int ac, char **av)
{
	t_file	elem;
	char	nothing[PATH_MAX];

	if (!ac)
	{
		bzero(nothing, sizeof(PATH_MAX + 1));
		nothing[0] = '.';
		nothing[1] = 0;
		if(!(ls->directory = ft_lstnew(nothing, sizeof(char) * (PATH_MAX + 1))))
			return (1);
		ls->directory->zu = ls->indexdir;
		ls->curr_dir = ls->directory;
		ls->last_dir = ls->directory;
		return (0);
	}
	bzero(&elem, sizeof(elem));
	if (!(ls->file = ft_lstnew(&elem, sizeof(elem))))
		return (1);
	ls->curr_file = ls->file;
	ls->file->zu = 1;
	((t_file*)(ls->file->pv))->name = *(av++);
	while (ac--)
	{
		if (!(ls->curr_file = ft_lstnew(&elem, sizeof(elem))))
			return (1);
		ls->curr_file->zu = 1;
		((t_file*)(ls->curr_file->pv))->name = *(av++);
	}
	ls->last_file = ls->curr_file;
	return (0);
}
