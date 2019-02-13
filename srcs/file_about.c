/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_about.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 15:55:31 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/13 19:42:08 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void		compute_date(t_file *file)
{
	time_t	stat;

	stat = file->stat->st_mtime;
	printf(" %s", ctime(&stat));
}

void		compute_rights(t_file *file)
{
	char c;

	c = (S_ISDIR(file->stat->st_mode) ? 'd' : '-');
	c = (S_ISCHR(file->stat->st_mode) ? 'c' : '-');
	c = (S_ISBLK(file->stat->st_mode) ? 'b' : '-');
	c = (S_ISFIFO(file->stat->st_mode) ? 'p' : '-');
	c = (S_ISLNK(file->stat->st_mode) ? 'l' : '-');
	c = (S_ISSOCK(file->stat->st_mode) ? 's' : '-');
	printf("%c", c);
	printf((S_IRUSR) ? "r" : "-");
	printf((S_IWUSR) ? "w" : "-");
	printf((S_IXUSR) ? "x" : "-");
	printf((S_IRGRP) ? "r" : "-");
	printf((S_IWGRP) ? "w" : "-");
	printf((S_IXGRP) ? "x" : "-");
	printf((S_IROTH) ? "r" : "-");
	printf((S_IWOTH) ? "w" : "-");
	printf((S_IXOTH) ? "x" : "-");
}

void		display_file(t_file *file)
{
	struct passwd	*uid;
	struct group	*gid;

	uid = getpwuid(file->stat->st_uid);
	gid = getgrgid(file->stat->st_gid);
	compute_rights(file);
	printf(" %hu", file->stat->st_nlink);
	printf("%s %s", uid->pw_name, gid->gr_name);
	printf(" %lld", file->stat->st_size);
	compute_date(file);
	printf("%s\n", file->name);
}
