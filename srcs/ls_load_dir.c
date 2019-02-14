/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 20:43:00 by lubenard          #+#    #+#             */
/*   Updated: 2019/02/14 12:04:32 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

inline static int		first_call_no_argument_given(t_ls *ls)
{
	char		*tmp;
	t_list		*tmpdir;

	if (!(tmpdir = ft_lstnew(".", sizeof(char) * 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	free(tmpdir->pv);
	if (!(tmp = ft_strnew(2 + 256)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, "./");
	tmp[2] = 0;
	tmpdir->pv = tmp;
	tmpdir->zu= 2;
	ls->directory = tmpdir;
	return (1);
}

inline static int		call_no_argument_given(t_ls *ls)
{
	char		*tmp;
	size_t		i;
	t_list		*tmpdir;
	
	if (!(tmpdir = ft_lstnew(".", sizeof(char) * 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	free(tmpdir->pv);
	i = ft_strlen(ls->curr_file->name);
	if (!(tmp = ft_strnew(i + 1 + 256 + ls->directory->zu)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, ls->directory->pv);
	ft_strcat(tmp + ls->directory->zu, ls->curr_file->name);
	ft_strcat(tmp + ls->directory->zu + i, "/");
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

inline static int first_call_argument_given(t_ls *ls)
{
	//need to handle : adding '/' at the end if necessary
	//absolute path : /etc/, ~/goinfre ...
	//it may be simpler to check here which argument is a directory or not
	//Considering adding a "dummy" directory with an empty string
	//maybe a variable in ls?
	(void)ls;
	return (1);
}

inline static int call_argument_given(t_ls *ls)
{
	(void)ls;
	return (1);
}

int		load_directory(t_ls *ls)
{
	if (!ls->directory && !ls->file) 
		return (first_call_no_argument_given(ls));
	else if (!ls->directory)
		return (first_call_argument_given(ls));
	else if (!strncmp(ls->directory->pv, "./", 2))
		return (call_no_argument_given(ls));
	else
		return (call_argument_given(ls));
}
