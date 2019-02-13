/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 20:43:00 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/13 15:03:47 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

int		load_directory(t_ls *ls)
{
	char		*tmp;
	size_t		i;
	t_list		*tmpdir;

	if (!ls->directory && !ls->file) //first call no argument given
	{
		if (!(ls->directory = ft_lstnew("./", sizeof(char) * 2)))
			return (ls_print_error(0, LSERR_MALLOC));
		if (!(tmp = ft_strnew(2 + 256)))
			return (ls_print_error(0, LSERR_MALLOC));
		tmp[0] = '.';
		tmp[1] = '/';
		tmp[2] = 0;
		free(ls->directory->pv);
		ls->directory->pv = tmp;
		ls->directory->zu= 2;
		return (1);
	}
	if (!(tmpdir = ft_lstnew("./", sizeof(char) * 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	i = ft_strlen(ls->curr_file->name);
	if (!(tmp = ft_strnew(i + 1 + 256 + ls->directory->zu)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, ls->directory->pv);
	ft_strcat(tmp, ls->curr_file->name);
	ft_strcat(tmp, "/");
	tmp[i + 2 + ls->directory->zu] = 0;
	tmpdir->pv = tmp;
	tmpdir->zu = i + 1 + ls->directory->zu;
	if (!ls->directory)
		ls->directory = tmpdir;
	else if (!ls->directory->next)
		ls->directory->next = tmpdir;
	else
		ft_lstadd(&(ls->directory->next), tmpdir);
	return (1);
}
