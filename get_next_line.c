/* ************************************************************************** */n
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathmart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 14:37:32 by mathmart          #+#    #+#             */
/*   Updated: 2021/11/11 18:35:25 by mathmart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static int	ft_strichr(const char *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (FALSE);
}

//si s1 != NULL => free(backup)
static char	*ft_spe_join(char *s1, char *s2)
{
	char	*buff;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1 && s1[i++] && s2 && s2[j++])
		;
	buff = malloc(sizeof(char) * i + j + 1);
	if (!buff)
		return (NULL);
	i = -1;
	while (s1 && s1[++i])
		buff[i] = s1[i];
	j = -1;
	while (s2 && s2[++j])
		buff[i + j] = s2[j];
	buff[i + j] = '\0';
	printf("%d  %s\n", i + j, buff);
	if (s1)
		free(s1);
	return (buff);
}

static int	ft_get_read(int fd, char **backup, char **line)
{
	char	buff[BUFFER_SIZE + 1];
	int		eof;

	*line = ft_spe_join(*line, *backup);
	if (ft_strichr(*line, '\n'))
	{
		eof = read(fd, buff, BUFFER_SIZE);
		if (eof == FALSE)
		{
			if (*backup)
				free(*backup);
			*backup = NULL;
			return (*line && (*line)[eof] != '\0');
		}
		buff[eof] = '\0';
		*line = ft_spe_join(*line, buff);
	}
	eof = ft_strichr(*line, '\n');
	if (eof >= 0)
	{
		//free(*backup);
		*backup = ft_spe_join(NULL, &(*line)[eof]);
		(*line)[eof + 1] = '\0';
	}
	return (TRUE);
}

char	*get_next_line(int fd)
{
	static char	*backup[OPEN_MAX];
	char		*line;

	line = NULL;
	if (fd > OPEN_MAX || fd <= -1 || BUFFER_SIZE < 1)
		return (0);
	if (ft_get_read(fd, &(backup[fd]), &line) != FALSE)
	{
		if (line)
			free(line);
		line = NULL;
	}
	return (line);
}

/*
0123456789

char *buf;

int nb_readed_chars = read(fd, buf, BUFFER_SIZE);

0123456789
buf[nb_readed_chars] = '\0';
0123456789\0
 */					

int	main(int ac, char **av)
{
	int	fd;
	(void)ac;

	if (ac < 1)
		exit(EXIT_FAILURE);
	fd = open(av[1], O_RDONLY);
	char	*dest;
		dest = get_next_line(fd);
		printf("%s", dest);
		free(dest);
		dest = get_next_line(fd);
		printf("%s", dest);
		free(dest);
		/*dest = get_next_line(fd);
		printf("%s", dest);
		free(dest);
		dest = get_next_line(fd);
		printf("%s", dest);
		free(dest);*/
	close(fd);
}
