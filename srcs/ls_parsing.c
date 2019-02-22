/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 13:55:15 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/22 17:49:33 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline static int		ls_available_option(char *av, int *flags)
{
	int	i;

	while (*(++av))
	{
		if (!(i = ft_strchri("1lRartsTuofGAp", av[0])))
			return (ls_print_error(av, LSERR_USAGE));
		*flags |= (1 << (i - 1));
	}
	if (*flags & LSO_F)
	{
		*flags |= LSO_A;
		*flags &= ~LSO_AA;
	}
	if (*flags & LSO_O)
		*flags |= LSO_L;
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
