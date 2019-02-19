/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_detailed_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:19:19 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/18 21:52:46 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static char			file_type(int mode)
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

/*inline static char			load_acl(t_ls *ls)
{
	acl_t	tmp;

	if (listxattr((char*)ls->directory->pv, NULL, 0, XATTR_NOFOLLOW) > 0)
		return ('@');
	if ((tmp = acl_get_link_np((char*)ls->directory->pv, ACL_TYPE_EXTENDED)))
	{
		acl_free(tmp);
		return ('+');
	}
	return (' ');
}*/

void						load_attribute(t_file *file, t_ls *ls, char (*attr)[12])
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
	if (S_ISUID & mode)
		(*attr)[3] = (*attr)[3] == 'x' ? 's' : 'S';
	if (S_ISGID & mode)
		(*attr)[6] = (*attr)[6] == 'x' ? 's' : 'S';
	if (S_ISVTX & mode)
		(*attr)[9] = (*attr)[9] == 'x' ? 't' : 'T';
	//(*attr)[10] = load_acl(ls);
	(*attr)[10] = ' ';
	(void)ls;
	(*attr)[11] = 0;
}

void						print_detailed_loop(t_ls *ls, long long (*s)[10])
{
	t_file		*tmp;
	char		attr[12];
	char		*t;
	char		buf[PATH_MAX + 1];

	tmp = (ls->flags & LSO_R) ? ls->curr_file : ls->file;
	while (tmp)
	{
		if (ls->numfile-- == 0)
			break ;
		((char*)(ls->directory->pv))[ls->directory->zu] = 0;
		ft_strcat(ls->directory->pv, tmp->name);
		if (ls->flags & LSO_S)
			ft_printf("%*lld ", (*s)[0], tmp->stat.st_blocks);
		load_attribute(tmp, ls, &attr);
		ft_printf("%s %*lld ", attr, (*s)[1], tmp->stat.st_nlink);
		if (getpwuid(tmp->stat.st_uid))
			ft_printf("%-*s  ", (*s)[2], getpwuid(tmp->stat.st_uid)->pw_name);
		else
			ft_printf("%-*lld  ", (*s)[2], tmp->stat.st_uid);
		if (getgrgid(tmp->stat.st_gid))
			ft_printf("%-*s  ", (*s)[3], getgrgid(tmp->stat.st_gid)->gr_name);
		else
			ft_printf("%-*lld  ", (*s)[3], tmp->stat.st_gid);
		if ((*s)[8] && (attr[0] == 'b' || attr[0] == 'c'))
			ft_printf("%*lld, %*lld ", (*s)[5], major(tmp->stat.st_rdev), (*s)[6], minor(tmp->stat.st_rdev));
		else
			ft_printf("%*lld ", (*s)[4], tmp->stat.st_size);
		t = ctime(&tmp->stat.st_mtime) + 4;
		if (time(NULL) > 60 * 60 * 24 * 30 * 6
		&& tmp->stat.st_mtime > time(NULL) - 60 * 60 * 24 * 30 * 6)
		{
			if (ls->flags & LSO_TT)
				ft_printf("%.15s %s", t, tmp->name);
			else
				ft_printf("%.12s %s", t, tmp->name);
		}
		else
			ft_printf("%.6s  %.4s %s", t, t + 16, tmp->name);
		if (attr[0] == 'l')
		{
			ft_bzero(buf, PATH_MAX + 1);
			readlink((char*)ls->directory->pv, buf, PATH_MAX);
			ft_printf(" -> %s\n", buf);
		}
		else
			ft_putchar('\n');
		((char*)(ls->directory->pv))[ls->directory->zu] = 0;
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
	}
}

void						set_detailed_list_length(t_ls *ls, long long (*s)[10])
{
	t_file		*tmp;
	t_stat		stat;
	size_t		numfile;
	long long	len;

	tmp = ls->file;
	numfile = ls->numfile;
	len = 0;
	while (tmp)
	{
		if (numfile-- == 0)
			break ;
		stat = tmp->stat;
		(*s)[7] += stat.st_blocks;
		(*s)[0] = (*s)[0] < stat.st_blocks ? stat.st_blocks : (*s)[0];
		(*s)[1] = (*s)[1] < (signed long long)stat.st_nlink ? (signed long long)stat.st_nlink : (*s)[1];
		if (getpwuid(stat.st_uid))
			len = ft_strlen(getpwuid(stat.st_uid)->pw_name);
		(*s)[2] = (*s)[2] < len ? len : (*s)[2];
		if (getgrgid(stat.st_gid))
			len = ft_strlen(getgrgid(stat.st_gid)->gr_name);
		(*s)[3] = (*s)[3] < len ? len : (*s)[3];
		if (!S_ISCHR(stat.st_mode))
			(*s)[4] = (*s)[4] < stat.st_size ? stat.st_size : (*s)[4];
		else
		{
			(*s)[5] = (*s)[5] < major(stat.st_rdev) ? major(stat.st_rdev) : (*s)[5];
			(*s)[6] = (*s)[6] < minor(stat.st_rdev) ? minor(stat.st_rdev) : (*s)[6];
			(*s)[8] = 1;
		}
		tmp = tmp->next;
	}
	(*s)[0] = ft_nprintf("%lld", (*s)[0]);
	(*s)[1] = ft_nprintf("%lld", (*s)[1]);
	(*s)[4] = ft_nprintf("%lld", (*s)[4]);
	if ((*s)[8])
	{
		(*s)[5] = ft_nprintf("%lld", (*s)[5]);
		(*s)[6] = ft_nprintf("%lld", (*s)[6]);
		(*s)[5] < (*s)[4] - (*s)[6] - 1 ? (*s)[5] = (*s)[4] - (*s)[6] - 1 : 0;
		(*s)[4] < (*s)[5] + (*s)[6] + 2 ? (*s)[4] = (*s)[5] + (*s)[6] + 2 : 0;
	}
}

void						print_detailed(t_ls *ls)
{
	long long	size[10];

	ft_bzero(size, 10 * (sizeof(long long)));
	if(ls->flags & (LSO_RR | LSO_ARGC) && ls->file && ls->directory->zu && !(ls->flags & LSO_ERROPEN))
		ft_printf("%s:\n", ls->directory->pv);
	set_detailed_list_length(ls, &size);
	if (ls->numfile && ls->file)
		ft_printf("total %d\n", (int)size[7]);
	print_detailed_loop(ls, &size);
	if (ls->flags & (LSO_ARGC | LSO_RR))
		if (ls->file && ls->directory->next)
			ft_putchar('\n');
}
