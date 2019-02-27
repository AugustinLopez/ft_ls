/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 13:55:15 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/27 14:17:48 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline static int		ls_available_option(char *av, int *flags)
{
	int	i;

	while (*(++av))
	{
		if (!(i = ft_strchri("1lRartsTuofGAp", av[0])))
			return (ls_print_error(av, LSERR_USAGE, 0));
		*flags |= (1 << (i - 1));
	}
	if (*flags & LSO_F)
	{
		*flags |= LSO_A;
		*flags &= ~(LSO_AA | LSO_R);
	}
	if (*flags & LSO_O)
		*flags |= LSO_L;
	if (*flags & LSO_A)
		*flags &= ~LSO_AA;
	return (i);
}

int						ls_parsing(int ac, char **av, int *flags)
{
	int	i;

	*flags = 0;
	i = 0;
	while (++i < ac && av[i][0] == '-' && av[i][1])
	{
		if (av[i][1] == '-' && !av[i][2])
			return (i + 1);
		if (!ls_available_option(av[i], flags))
			return (0);
	}
	return (i);
}
