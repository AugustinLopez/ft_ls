/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:21:00 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/23 11:06:10 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int			ls_print_error(char *str, int errflag)
{
	if (errflag == LSERR_USAGE)
	{
		ft_dprintf(2, "ft_ls: invalid option -- \'%c\'\n", *str);
		ft_dprintf(2, "usage: ft_ls [-1lRartsTuofGAF] [file ...]\n");
	}
	else if (errflag == LSERR_OPENFILE || errflag == LSERR_OPENDIR)
		ft_dprintf(2, "ft_ls: %s: %s\n", str, strerror(errno));
	else if (errflag == LSERR_MALLOC)
		ft_dprintf(2, "ft_ls: %s\n", strerror(errno));
	else if (errflag == LSERR_OTHER)
		ft_dprintf(2, "ft_ls: %s: %s\n", str, strerror(errno));
	return (0);
}
