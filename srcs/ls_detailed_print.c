/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_detailed_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:19:19 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/22 18:30:03 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

inline static char	load_acl(t_ls *ls)
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
}

void				load_attribute(t_file *file, t_ls *ls, char (*attr)[12])
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
	(*attr)[10] = load_acl(ls);
	(*attr)[11] = 0;
}

inline static void	print_detailed_loop_2(t_ls *ls, t_file *tmp,
		long long (*s)[12], char (*attr)[12])
{
	char		*t;

	ls->flags & LSO_S ? ft_printf("%*lld ", (*s)[0], tmp->stat.st_blocks) : 0;
	ft_printf("%s %*lld ", *attr, (*s)[1], tmp->stat.st_nlink);
	!(*s)[10] || getpwuid(tmp->stat.st_uid) ?
		ft_printf("%-*s  ", (*s)[2], getpwuid(tmp->stat.st_uid)->pw_name) :
		ft_printf("%-*lld  ", (*s)[2], tmp->stat.st_uid);
	if (!(ls->flags & LSO_O))
		!(*s)[11] || getgrgid(tmp->stat.st_gid) ?
			ft_printf("%-*s  ", (*s)[3], getgrgid(tmp->stat.st_gid)->gr_name) :
			ft_printf("%-*lld  ", (*s)[3], tmp->stat.st_gid);
	((*s)[8] && ((*attr)[0] == 'b' || (*attr)[0] == 'c')) ?
		ft_printf("%*lld, %*lld ", (*s)[5], major(tmp->stat.st_rdev),
		(*s)[6], minor(tmp->stat.st_rdev)) :
		ft_printf("%*lld ", (*s)[4], tmp->stat.st_size);
	t = ctime(&tmp->stat.st_mtime) + 4;
	if (ls->flags & LSO_TT)
		ft_printf("%.20s ", t);
	else if (time(NULL) > LS_SIX_MONTH
	&& tmp->stat.st_mtime > time(NULL) - LS_SIX_MONTH)
		ft_printf("%.12s ", t);
	else
		ft_printf("%.6s  %.4s ", t, t + 16);
	set_colors(tmp, ls);
}

inline static void	print_link(char *path)
{
	char	buf[PATH_MAX + 1];

	ft_bzero(buf, PATH_MAX + 1);
	readlink(path, buf, PATH_MAX);
	ft_printf(" -> %s", buf);
}

void				print_detailed_loop(t_ls *ls, long long (*s)[12])
{
	t_file		*tmp;
	char		attr[12];

	tmp = (ls->flags & LSO_R) ? ls->curr_file : ls->file;
	while (tmp)
	{
		if ((ls->flags & LSO_AA)
			&& (!ft_strcmp(tmp->name, ".") || !ft_strcmp(tmp->name, "..")))
		{
			tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
			continue ;
		}
		if (ls->numfile-- == 0)
			break ;
		((char*)(ls->directory->pv))[ls->directory->zu] = 0;
		ft_strcat(ls->directory->pv, tmp->name);
		load_attribute(tmp, ls, &attr);
		print_detailed_loop_2(ls, tmp, s, &attr);
		attr[0] == 'l' ? print_link((char*)ls->directory->pv) : 0;
		ft_putchar('\n');
		((char*)(ls->directory->pv))[ls->directory->zu] = 0;
		tmp = (ls->flags & LSO_R) ? tmp->prev : tmp->next;
	}
}

inline static void	length_loop(long long (*s)[12], t_file *tmp)
{
	t_stat	stat;

	stat = tmp->stat;
	(*s)[7] += stat.st_blocks;
	(*s)[0] = (*s)[0] < stat.st_blocks ? stat.st_blocks : (*s)[0];
	if ((*s)[1] < (signed long long)stat.st_nlink)
		(*s)[1] = (signed long long)stat.st_nlink;
	(*s)[9] = (getpwuid(stat.st_uid)) ?
		ft_strlen(getpwuid(stat.st_uid)->pw_name) :
		ft_nprintf("%lld%n", stat.st_uid, &((*s)[10]));
	(*s)[2] = (*s)[2] < (*s)[9] ? (*s)[9] : (*s)[2];
	(*s)[9] = (getgrgid(stat.st_gid)) ?
		ft_strlen(getgrgid(stat.st_gid)->gr_name) :
		ft_nprintf("%lld%n", stat.st_gid, &((*s)[11]));
	(*s)[3] = (*s)[3] < (*s)[9] ? (*s)[9] : (*s)[3];
	if (!S_ISCHR(stat.st_mode))
		(*s)[4] = (*s)[4] < stat.st_size ? stat.st_size : (*s)[4];
	else
	{
		(*s)[9] = major(stat.st_rdev);
		(*s)[5] = (*s)[5] < (*s)[9] ? (*s)[9] : (*s)[5];
		(*s)[9] = minor(stat.st_rdev);
		(*s)[6] = (*s)[6] < (*s)[9] ? (*s)[9] : (*s)[6];
		(*s)[8] = 1;
	}
}

void				set_detailed_list_length(t_ls *ls, long long (*s)[12])
{
	t_file		*tmp;
	size_t		numfile;

	tmp = ls->file;
	numfile = ls->numfile;
	while (tmp)
	{
		if (numfile-- == 0)
			break ;
		length_loop(s, tmp);
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

/*
** 0-4 are block/link/user/group/size length
** 5-6 are major/minor
** 7 is total block
** 8 is used to know if there are major/minor
** 9 is a temp variable
** if 10/11,no name associated to a uid/gid : we use the ID instead
*/

void				print_detailed(t_ls *ls, int non_first)
{
	long long	size[12];

	ft_bzero(size, 12 * (sizeof(long long)));
	set_detailed_list_length(ls, &size);
	if (ls->directory->zu)
		((char*)ls->directory->pv)[ls->directory->zu - 1] = 0;
	if (non_first && !(ls->flags & LSO_ERROPEN))
		ft_printf("%s:\n", ls->directory->pv);
	if (ls->directory->zu)
		((char*)ls->directory->pv)[ls->directory->zu - 1] = '/';
	if (ls->numfile && ls->file)
		ft_printf("total %d\n", (int)size[7]);
	print_detailed_loop(ls, &size);
	if (ls->flags & (LSO_ARGC | LSO_RR) && (ls->file && ls->directory->next))
		ft_putchar('\n');
}
