#include <ft_ls.h>

int		main(int ac, char **av)
{
	int		options;
	t_ls	ls;

	ft_bzero(&ls, sizeof(ls));
	if (!(options = ls_parsing(ac, av, &ls.flags)))
		return (EXIT_FAILURE);
	ac -= options;
	av += options;
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
