/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_buffer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 19:11:26 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/22 11:34:47 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static inline void	pf_stdout(t_printf *pf, void *str, size_t size)
{
	size_t		i;
	int			diff;

	i = 0;
	while (PF_BUFF_SIZE - pf->index < size)
	{
		diff = PF_BUFF_SIZE - pf->index;
		ft_memcpy((void *)(pf->buff + pf->index),
		(void *)((char *)(str + i)), diff);
		size -= diff;
		i += diff;
		pf->len += diff;
		write(pf->fd, pf->buff, pf->index + diff);
		pf->index = 0;
	}
	ft_memcpy((void *)(pf->buff + pf->index),
	(void *)((char *)(str + i)), size);
	pf->index += size;
	pf->len += size;
}

void				pf_char_write(t_printf *pf)
{
	char	*tmp;

	tmp = pf->pc;
	(pf->buff)[pf->index] = 0;
	pf->pc = ft_strjoin(tmp, pf->buff);
	if (tmp)
		free(tmp);
}

static inline void	pf_not_stdout(t_printf *pf, void *str, size_t size)
{
	size_t		i;
	int			diff;

	i = 0;
	while (PF_BUFF_SIZE - pf->index < size)
	{
		diff = PF_BUFF_SIZE - pf->index;
		ft_memcpy((void *)(pf->buff + pf->index),
		(void *)((char *)(str + i)), diff);
		size -= diff;
		i += diff;
		pf->len += diff;
		if (pf->pc)
			pf_char_write(pf);
		pf->index = 0;
	}
	ft_memcpy((void *)(pf->buff + pf->index),
	(void *)((char *)(str + i)), size);
	pf->index += size;
	pf->len += size;
}

void				pf_padding(t_printf *pf, int n)
{
	char		c;
	uint32_t	tmp;

	c = (pf->flag & F_ZERO || pf->option_padding) ? '0' : ' ';
	if (pf->padding > 0)
		if ((!n && !(pf->flag & F_MINUS)) || (n && (pf->flag & F_MINUS))
			|| pf->option_padding)
			while (pf->padding--)
				pf_buffer_write(pf, &c, 1);
}

void				pf_buffer_write(t_printf *pf, void *str, size_t size)
{
	if (!pf->noprint)
		pf_stdout(pf, str, size);
	else
		pf_not_stdout(pf, str, size);
}