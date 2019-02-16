#include <ft_ls.h>

inline static char	file_type(int mode)
{
	if (S_ISREG(mode))
		return ('-');
	else if (S_ISDIR(mode))
		return ('d');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISSOCK(mode))
		return ('s');
	else if (S_ISFIFO(mode))
		return ('p');
	else
		return ('-');
}

void	load_attribute(t_file *file, char (*attr)[10])
{
	int	mode;

	mode = file->stat.st_mode;
	(*attr)[0] = file_type(mode);
	(*attr)[1] = (S_IRUSR & mode) ? 'r' : '-';
	(*attr)[2] = (S_IWUSR & mode) ? 'w' : '-';
	(*attr)[3] = (S_IXUSR & mode) ? 'x' : '-';
	(*attr)[4] = (S_IRGRP & mode) ? 'r' : '-';
	(*attr)[5] = (S_IWGRP & mode) ? 'w' : '-';
	(*attr)[6] = (S_IXGRP & mode) ? 'x' : '-';
	(*attr)[7] = (S_IROTH & mode) ? 'r' : '-';
	(*attr)[8] = (S_IWOTH & mode) ? 'w' : '-';
	(*attr)[9] = (S_IXOTH & mode) ? 'x' : '-';
	(*attr)[10] = 0;
	//check ISUID ISGID ISVTX

}

void	print_detailed_loop(t_ls *ls, int block_size)
{
	t_file		*tmp;
	char		attr[10];
	char 		*t;

	tmp = ls->file;
	while (tmp)
	{
		if (ls->numfile-- == 0)
			break;
		if (ls->flags & LSO_S)
			ft_printf("%*lld ", block_size, tmp->stat.st_blocks);
		load_attribute(tmp, &attr);
		t =  ctime(&tmp->stat.st_mtime) + 4;
		ft_printf("%s %.12s %s\n", attr, t, tmp->name);
		tmp = tmp->next;
	}
}

void	print_detailed(t_ls *ls)
{
	int	block_size;

	block_size = 0;
	if (ls->flags & (LSO_ARGC | LSO_RR) && ls->directory->zu)
		ft_printf("%s:\n", ls->directory->pv);
	block_size = max_block_size(ls);
	print_detailed_loop(ls, block_size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->file && ls->directory->next)
			ft_putchar('\n');
}

