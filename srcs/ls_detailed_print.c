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

void	load_attribute(t_file *file, char (*attr)[12])
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
	(*attr)[10] = ' ';
	(*attr)[11] = 0;
	//check ISUID ISGID ISVTX

}

void	print_detailed_loop(t_ls *ls, long long (*s)[10])
{
	t_file		*tmp;
	char		attr[12];
	char 		*t;
	char		buf[PATH_MAX + 1];

	tmp = ls->file;
	while (tmp)
	{
		if (ls->numfile-- == 0)
			break;
		if (ls->flags & LSO_S)
			ft_printf("%*lld ", (*s)[0], tmp->stat.st_blocks);
		load_attribute(tmp, &attr);
		t =  ctime(&tmp->stat.st_mtime) + 4;
		if (time(NULL) > 60 * 60 * 24 * 30 * 6 
		&& tmp->stat.st_mtime > time(NULL) - 60 * 60 * 24 * 30 * 6)
		{
			if (getpwuid(tmp->stat.st_uid)) //need to make getgrgid as well as length.
				ft_printf("%s %*lld %-*s %-*s  %*lld %.12s %s",
					attr, (*s)[1], tmp->stat.st_nlink, (*s)[2],
					getpwuid(tmp->stat.st_uid)->pw_name,(*s)[3],
					getgrgid(tmp->stat.st_gid)->gr_name, (*s)[4],
					tmp->stat.st_size, t, tmp->name);
			else
				ft_printf("%s %*lld %-*lld %-*s  %*lld %.12s %s",
					attr, (*s)[1], tmp->stat.st_nlink, (*s)[2],
					tmp->stat.st_uid,(*s)[3],
					getgrgid(tmp->stat.st_gid)->gr_name, (*s)[4],
					tmp->stat.st_size, t, tmp->name);
		}
		else
		{
			if (getpwuid(tmp->stat.st_uid))
				ft_printf("%s %*lld %-*s %-*s  %*lld %.6s  %.4s %s",
					attr, (*s)[1], tmp->stat.st_nlink, (*s)[2],
					getpwuid(tmp->stat.st_uid)->pw_name,(*s)[3],
					getgrgid(tmp->stat.st_gid)->gr_name, (*s)[4],
					tmp->stat.st_size, t, t + 16, tmp->name);
			else
				ft_printf("%s %*lld %-*lld %-*s  %*lld %.6s  %.4s %s",
					attr, (*s)[1], tmp->stat.st_nlink, (*s)[2],
					tmp->stat.st_uid,(*s)[3],
					getgrgid(tmp->stat.st_gid)->gr_name, (*s)[4],
					tmp->stat.st_size, t, t + 16,  tmp->name);

		}
		if (attr[0] == 'l')
		{
			ft_bzero(buf, PATH_MAX + 1);
			((char*)(ls->directory->pv))[ls->directory->zu] = 0;
			ft_strcat(ls->directory->pv, tmp->name);
			readlink((char*)ls->directory->pv, buf, PATH_MAX);
			((char*)(ls->directory->pv))[ls->directory->zu] = 0;
			ft_printf(" -> %s\n", buf);
		}
		else
			ft_putchar('\n');
		tmp = tmp->next;
	}
}

void		set_detailed_list_length(t_ls *ls, long long (*s)[10])
{
	t_file *tmp;
	t_stat stat;
	size_t	numfile;
	long long len;

	tmp = ls->file;
	numfile = ls->numfile;
	len = 0;
	while (tmp)
	{
		if (numfile-- == 0)
			break;
		stat = tmp->stat;
		(*s)[7] += stat.st_blocks;
		(*s)[0] = (*s)[0] < stat.st_blocks ? stat.st_blocks : (*s)[0];
		(*s)[1] = (*s)[1] < stat.st_nlink ? stat.st_nlink : (*s)[1];
		if (getpwuid(stat.st_uid))
			len = ft_strlen(getpwuid(stat.st_uid)->pw_name);
		(*s)[2] = (*s)[2] < len ? len : (*s)[2];
		if (getgrgid(stat.st_gid))
			len = ft_strlen(getgrgid(stat.st_gid)->gr_name);
		(*s)[3] = (*s)[3] < len ? len : (*s)[3];
		if (!S_ISCHR(stat.st_mode))
				(*s)[4] = (*s)[4] < stat.st_size ? stat.st_size : (*s)[4];
		else
			(*s)[4] = 0;
		tmp = tmp->next;
	}
	(*s)[0] = ft_nprintf("%lld", (*s)[0]);
	(*s)[1] = ft_nprintf("%lld", (*s)[1]);
	(*s)[4] = ft_nprintf("%lld", (*s)[4]);
}

void	print_detailed(t_ls *ls)
{
	int			block_size;
	long long	size[10];

	ft_bzero(size, 10 * (sizeof(long long)));
	block_size = 0;
	if (ls->flags & (LSO_ARGC | LSO_RR) && ls->directory->zu)
		ft_printf("%s:\n", ls->directory->pv);
	//block_size = max_block_size(ls);
	set_detailed_list_length(ls, &size);
	if (ls->numfile)
		ft_printf("total %d\n", (int)size[7]);
	print_detailed_loop(ls, &size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->file && ls->directory->next)
			ft_putchar('\n');
}

