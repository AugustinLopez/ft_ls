#include <ft_ls.h>

int			ls_print_error(char *str, int errflag)
{
	if (errflag == LSERR_USAGE)
		dprintf(2, "ft_ls: invalid option -- \'%c\'\nusage: ft_ls [-lRart] [file ...]\n", *str);
	else if (errflag == LSERR_MALLOC)
		dprintf(2, "ft_ls: %s\n", strerror(errno));
	else if (errflag == LSERR_OTHER)
		dprintf(2, "ft_ls: %s: %s\n", str, strerror(errno));
	return (0);
}
