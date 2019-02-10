# include "ft_ls.h"

inline static int		ls_available_option(char *av, int *flags)
{
	int	i;

	while (*(++av))
	{
		if (!(i = ft_strchri("lRart", av[0])))
		{
			ls_print_error(av, LSERR_USAGE);
			return (0);
		}
		*flags |= (1 << i);
	}
	return (i);
}

int						ls_parsing(int ac, char **av, int *flags)
{
	int	i;

	*flags = 0;
	i = 0;
	while (++i < ac && av[i][0] == '-' && av[i][1])
		if (!ls_available_option(av[i], flags))
			return (0);
	return (i);
}
