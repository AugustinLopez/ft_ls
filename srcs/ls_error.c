/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 17:21:00 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/12 17:22:27 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int			ls_print_error(char *str, int errflag)
{
	if (errflag == LSERR_USAGE)
	{
		dprintf(2, "ft_ls: invalid option -- \'%c\'\n", *str);
		dprintf(2, "usage: ft_ls [-lRart] [file ...]\n");
	}
	else if (errflag == LSERR_MALLOC)
		dprintf(2, "ft_ls: %s\n", strerror(errno));
	else if (errflag == LSERR_OTHER)
		dprintf(2, "ft_ls: %s: %s\n", str, strerror(errno));
	return (0);
}
