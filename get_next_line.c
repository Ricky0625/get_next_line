/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:15:16 by wricky-t          #+#    #+#             */
/*   Updated: 2022/07/14 13:28:35 by wricky-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * iterate through stash and check if there is a new line
 * if yes, return 1 (true)
 * if no, return 0 (false)
**/
static int	found_new_line(char *stash, int *nl_at)
{
	int	index;

	index = 0;
	while (*(stash + index) != '\0')
	{
		if (*(stash + index) == '\n')
		{
			*nl_at = index;
			return (1);
		}
		index++;
	}
	*nl_at = -1;
	return (0);
}

/**
 * fd	: file descriptor
 * buf	: to store the nbytes of data after read
 * 
 * This function will read nbytes from the file that pointed by fd and store
 * those nbytes into stash
 * 
 * Flow
 * 1. read from file, store to buf
 * 2. check if there is a new line
 * 		a. yes. store buf to stash first, break loop
 * 		b. no. store buf to stash, continue loop
 * after this function end, the stash should contain a whole line with '\n'
 * that is ready for return
**/
static void	read_and_stash(int fd, char *buf, char **stash, int *nl_at)
{
	ssize_t	bytes_read;
	char	*temp;

	if (*stash == NULL || found_new_line(*stash, nl_at) == 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		while (bytes_read > 0)
		{
			buf[bytes_read] = '\0';
			if (*stash == NULL)
				*stash = ft_substr(buf, 0, BUFFER_SIZE);
			else
			{
				temp = ft_strjoin(*stash, buf);
				free(*stash);
				*stash = temp;
			}
			if (found_new_line(*stash, nl_at) == 1)
				break ;
			bytes_read = read(fd, buf, BUFFER_SIZE);
		}
	}
}

/**
 * line		: the line that the function is going to return at the end
 * stash_len: the length of the stash
 * 
 * retrieve the line from the stash once a new line is detected.
 * if the nl_at is greater than and equal to 0, meaning there is a new line
 * to retrieve the line, create a substr using stash, start from index 0 to
 * nl_at (index of the new line).
 * to clean up the stash, use substr as well. start from nl_at + 1 to
 * stash_len - (nl_at + 1). nl_at start counting from 0 so need plus 1
 * if nl_at is less than 0, meaning no new line is found also means that
 * we reach EOF ady. so line will equal the stash.
**/
char	*retrieve_and_clean(char **stash, int nl_at)
{
	char	*line;
	size_t	stash_len;
	char	*reset;

	line = NULL;
	reset = NULL;
	if ((*stash == NULL && nl_at == -1) || *stash[0] == '\0')
		return (NULL);
	stash_len = ft_strlen(*stash);
	if (nl_at >= 0)
	{
		line = ft_substr(*stash, 0, (nl_at + 1));
		*stash = ft_substr(*stash, (nl_at + 1), stash_len - (nl_at + 1));
	}
	else
	{
		line = ft_strdup(*stash);
		*stash = reset;
		// free(reset);
	}
	return (line);
}

/**
 * buf	: buffer
 * line	: the return line
 * nl_at: index of the new line in stash
 * 
 * The process to get the line
 * 1. read and stash
 * 2. retrieve and clean
 * 3. return line
**/
static char	*get_the_line(int fd, char **stash)
{
	char	*buf;
	char	*line;
	int		nl_at;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (NULL);
	nl_at = -1;
	read_and_stash(fd, buf, stash, &nl_at);
	line = retrieve_and_clean(stash, nl_at);
	free(buf);
	return (line);
}

/**
 * GET_NEXT_LINE
 * buf  : to store the nbytes of data after read
 * stash: a temporary storage to store the data read from file b4 a newline is
 *        detected
 * 
 * Flow
 * 1. check input validity (fd, BUFFER_SIZE)
 *      fd will only be possitive value, and will not exceed FILE_MAX (1024)
 *      BUFFER_SIZE <= 0 will be rejected
 * 2. detect new line (nl)
 * 		a. if no nl, read and stash. check nl again after this
 * 		b. if nl found, retrieve the line and clean the stash
**/
char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || fd > FILE_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	line = get_the_line(fd, &stash);
	return (line);
}
