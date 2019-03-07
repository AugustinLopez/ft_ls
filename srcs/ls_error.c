/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:21:00 by lubenard          #+#    #+#             */
/*   Updated: 2019/03/04 14:52:40 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int			ls_print_error(char *str, int errflag, t_ls *ls)
{
	char	**s;
	size_t	i;

	if (errflag == LSERR_USAGE)
	{
		ft_dprintf(2, "ft_ls: illegal option -- %c\n", *str);
		ft_dprintf(2, "usage: ft_ls [-1ACGRTadfgloprstu] [file ...]\n");
		return (0);
	}
	s = ft_strsplit(str, '/');
	i = 0;
	while (s[i])
		i++;
	i--;
	if (errflag == LSERR_OPENFILE || errflag == LSERR_OPENDIR
	|| errflag == LSERR_OTHER)
		ft_dprintf(2, "ft_ls: %s: %s\n", s[i], strerror(errno));
	else if (errflag == LSERR_MALLOC)
		ft_dprintf(2, "ft_ls: %s\n", strerror(errno));
	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
	ls->flags |= LSO_ERROR;
	return (0);
}

int			ls_print_error_argc(char *str, int errflag, t_ls *ls)
{
	if (errflag == LSERR_OPENFILE || errflag == LSERR_OPENDIR)
		ft_dprintf(2, "ft_ls: %s: %s\n", str, strerror(errno));
	else if (errflag == LSERR_MALLOC)
		ft_dprintf(2, "ft_ls: %s\n", strerror(errno));
	else if (errflag == LSERR_OTHER)
		ft_dprintf(2, "ft_ls: %s: %s\n", str, strerror(errno));
	ls->flags |= LSO_ERROR;
	return (0);
}
