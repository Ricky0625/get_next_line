/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:20:21 by wricky-t          #+#    #+#             */
/*   Updated: 2022/07/13 18:54:31 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

// libraries
# include <unistd.h>
# include <stdlib.h>

// for testing only (must remove after everything is good)
# include <stdio.h>
# include <sys/stat.h>
# include <fcntl.h>

// macros
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif
# define FILE_MAX 1024

// main function
char	*get_next_line(int fd);

// utils
size_t	ft_strlen(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);

#endif