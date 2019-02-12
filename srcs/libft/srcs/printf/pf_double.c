/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_double.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 19:08:07 by aulopez           #+#    #+#             */
/*   Updated: 2019/02/06 15:40:15 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static inline void	pf_load_string(t_printf *pf, char **str)
{
	double		x;
	long double	xx;
	int			dot;

	pf->precision = !(pf->flag & F_DOT) ? 6 : pf->precision;
	if (pf->flag & F_LDBL)
	{
		xx = va_arg(pf->ap, long double);
		*str = ft_ldbltoa((xx < 0 ? -xx : xx), pf->precision, pf->flag);
		pf->flag |= xx < 0 ? F_NEGATIF : 0;
	}
	else
	{
		x = va_arg(pf->ap, double);
		*str = ft_dbltoa((x < 0 ? -x : x), pf->precision, pf->flag);
		pf->flag |= x < 0 ? F_NEGATIF : 0;
	}
	dot = ft_strchri((const char *)(*str), '.');
	pf->printed = dot + ft_strlen(*str + dot);
	pf->precision = dot ? dot + pf->precision : pf->printed;
}

void				pf_putdouble(t_printf *pf)
{
	char	*str;
	int		sign;
	int		tmp;

	pf_load_string(pf, &str);
	if (str)
		str[0] == '-' ? pf->flag &= ~(F_NEGATIF | F_PLUS | F_SPACE) : 0;
	tmp = pf->length - pf->precision;
	(pf->flag & (F_NEGATIF | F_PLUS | F_SPACE)) ? --pf->length : 0;
	pf->padding = pf->length - pf->precision;
	!(pf->flag & F_ZERO) ? pf_padding(pf, 0) : 0;
	(pf->flag & (F_NEGATIF | F_PLUS | F_SPACE)) ? pf_sign(pf) : 0;
	((pf->flag & F_ZERO)) ? pf_padding(pf, 0) : 0;
	pf->padding = pf->precision - pf->printed;
	str ? pf_buffer_write(pf, str, pf->printed) : 0;
	pf->option_padding = 1;
	str ? pf_padding(pf, 0) : 0;
	pf->option_padding = 0;
	pf->padding = pf->length - pf->precision;
	pf_padding(pf, 1);
	str ? free(str) : 0;
}
