/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 17:02:08 by wricky-t          #+#    #+#             */
/*   Updated: 2022/07/14 11:40:45 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (*str++)
		count++;
	return (count);
}

static size_t	ft_get_size(char const *s, unsigned int start, size_t len)
{
	size_t	length;

	length = ft_strlen(s);
	if (start >= length)
		return (0);
	if ((start + len) > length)
		return (length - start);
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	index;
	char	*str;

	if (!s)
		return (NULL);
	len = ft_get_size(s, start, len);
	str = malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	index = 0;
	while (len-- && *(s + start))
	{
		*(str + index) = *(s + start);
		index++;
		start++;
	}
	*(str + index) = '\0';
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	size;
	size_t	index;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (size + 1));
	if (str == NULL)
		return (NULL);
	index = 0;
	while (*s1)
		*(str + index++) = *s1++;
	while (*s2)
		*(str + index++) = *s2++;
	*(str + index) = '\0';
	return (str);
}

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*str;
	size_t	index;

	len = ft_strlen(s1);
	str = malloc(sizeof(char) * (len + 1));
	index = 0;
	if (str == NULL)
		return (NULL);
	while (*s1)
	{
		*(str + index) = *(s1++);
		index++;
	}
	*(str + index) = '\0';
	return (str);
}
