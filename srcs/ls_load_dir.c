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
	ls->curr_dir = tmpdir;
	return (1);
}

inline static int		normal_call(t_ls *ls)
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
	if (!ls->curr_dir)
		ls->curr_dir = tmpdir;
	else if (!ls->curr_dir->next)
		ls->curr_dir->next = tmpdir;
	else
		ft_lstadd(&(ls->curr_dir->next), tmpdir);
	ls->curr_dir = ls->curr_dir->next;
	return (1);
}

inline static int first_call_argument_given(t_ls *ls)
{
	char		*tmp;
	t_list		*tmpdir;

	if (!(tmpdir = ft_lstnew(".", sizeof(char) * 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	free(tmpdir->pv);
	if (!(tmp = ft_strnew(2 + 256)))
		return (ls_print_error(0, LSERR_MALLOC));
	tmp[0] = 0;
	tmpdir->pv = tmp;
	tmpdir->zu= 0;
	ls->directory = tmpdir;
	ls->curr_dir = tmpdir;
	return (1);
}

int		load_directory(t_ls *ls)
{
	if (!ls->directory && !ls->file) 
	{
		if (ls->flags & LSO_ARGC)
			return (first_call_argument_given(ls));
		return (first_call_no_argument_given(ls));
	}
	else
		return (normal_call(ls));
}

int		create_directory_from_argument(t_ls *ls, char *argv)
{
	char		*tmp;
	size_t		i;
	t_list		*tmpdir;
	
	if (!(tmpdir = ft_lstnew(".", sizeof(char) * 2)))
		return (ls_print_error(0, LSERR_MALLOC));
	free(tmpdir->pv);
	i = ft_strlen(argv);
	if (!(tmp = ft_strnew(i + 1 + 256 + ls->directory->zu)))
		return (ls_print_error(0, LSERR_MALLOC));
	ft_strcat(tmp, ls->directory->pv);
	ft_strcat(tmp + ls->directory->zu, argv);
	if (argv[i  - 1] != '/')
	{
		ft_strcat(tmp + ls->directory->zu + i, "/");
		i++;
	}
	tmp[i + 1 + ls->directory->zu] = 0;
	tmpdir->pv = tmp;
	tmpdir->zu = i + ls->directory->zu;
	if (!ls->curr_dir)
		ls->curr_dir = tmpdir;
	else if (!ls->curr_dir->next)
		ls->curr_dir->next = tmpdir;
	else
		ft_lstadd(&(ls->curr_dir->next), tmpdir);
	ls->curr_dir = ls->curr_dir->next;
	return (1);
}
