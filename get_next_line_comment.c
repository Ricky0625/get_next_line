/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wricky-t <wricky-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:15:16 by wricky-t          #+#    #+#             */
/*   Updated: 2022/07/14 10:19:37 by wricky-t         ###   ########.fr       */
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
			// if a new line is found, set nl_at to the index of new line
			// after that return 1
			*nl_at = index;
			return (1);
		}
		index++;
	}
	// -1 means the function ady iterate thru the stash but no new line is found
	// in that case, assign nl_at to -1. return 0.
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
	
	// if the stash is null, or there is not new line in the stash
	if (*stash == NULL || found_new_line(*stash, nl_at) == 0)
	{
		// read from file first, check the bytes_read
		bytes_read = read(fd, buf, BUFFER_SIZE);
		// loop if the bytes_read is greater than 0
		// if zero means there is nothing left to read
		while (bytes_read > 0)
		{
			buf[bytes_read] = '\0';
			// substr will not give you new address so can directly assign
			if (*stash == NULL)
				*stash = ft_substr(buf, 0, BUFFER_SIZE);
			else
			{
				// strjoin will give you new address, so free before assign
				temp = *stash;
				*stash = ft_strjoin(*stash, buf);
				free(temp);
			}
			// after modified stash, check if there is a new line.
			// if yes then break the loop
			// if no, read again.
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

	line = NULL;
	// if after read, stash is null and nl_at is -1, return null
	if (*stash == NULL && nl_at == -1)
		return (NULL);
	stash_len = ft_strlen(*stash);
	// three reason that this function will be invoked:
	//		1. nothing to read, stash is NULL
	//		2. detected new line in the stash
	//		3. reach EOF, need to return the last string
	if (nl_at >= 0)
	{
		// retrieve the line, nl_at start counting from 0, len start counting from 1
		// so in order to get the len, we need to nl_at + 1
		line = ft_substr(*stash, 0, (nl_at + 1));
		// clean up the line
		// we want the character after the newline after to remain, start index is
		// nl_at + 1. for the len of the substr, we use stash_len - (nl_at + 1)
		// check above to see why we need nl_at + 1
		*stash = ft_substr(*stash, (nl_at + 1), stash_len - (nl_at + 1));
	}
	else
	{
		line = ft_strdup(*stash);
		*stash = "";
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
	int		nl_at; // new line index
	
	// buf will only store BUFFER_SIZE of characters per read
	// the extra one is for the '\0'
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	// check if malloc is successful
	if (buf == NULL)
		return (NULL);
	nl_at = -1;
	// stash here should be &*stash, but &* cancel each other
	// &*stash == stash
	read_and_stash(fd, buf, stash, &nl_at);
	line = retrieve_and_clean(stash, nl_at);
	// free(line);
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
	static char	*stash; // has a default value: "(null)"
	
	// if the input could pass, then start to read from file
	if (fd < 0 || fd > FILE_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	// get the line
	line = get_the_line(fd, &stash);
	return (line);
}
