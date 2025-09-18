/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcai <bcai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:48:54 by bcai              #+#    #+#             */
/*   Updated: 2024/05/30 09:59:36 by bcai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_working_history(t_m *m)
{
	t_execcmd	*ecmd;

	if (does_file_history_exist() == 0)
	{
		ecmd = execcmd_init(m);
		ecmd->cmd_args = malloc(sizeof(char *) * 3);
		if (!ecmd->cmd_args)
			return ;
		ecmd->cmd_args[0] = ft_strdup("touch");
		ecmd->cmd_args[1] = ft_strdup("history.txt");
		ecmd->cmd_args[2] = NULL;
		execute_simple_command(ecmd, m);
		free_2darray(ecmd->cmd_args);
		free(ecmd);
		ecmd = NULL;
	}
	else
		load_history(m);
}

void	load_history(t_m *m)
{
	int		fd;
	char	*line;
	char	*new_line;

	(void)m;
	fd = open("history.txt", O_RDONLY);
	if (fd < 0 || fd > 4095)
	{
		printf("Error: failed to load the history file\n");
		return ;
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		new_line = remove_line_break(line);
		add_history(new_line);
		free(new_line);
		new_line = NULL;
	}
	close(fd);
}

char	*remove_line_break(char *line)
{
	char	*new;
	int		i;

	i = 0;
	new = (char *)malloc(sizeof(char) * ft_strlen(line));
	if (!new)
		return (NULL);
	while (line[i] != '\n')
	{
		new[i] = line[i];
		i++;
	}
	new[i] = '\0';
	free(line);
	return (new);
}

int	does_file_history_exist(void)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (dir == NULL)
		return (2);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (pattern_match(entry->d_name, "history.txt"))
		{
			closedir(dir);
			return (1);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (0);
}

void	update_history_file(t_m *m)
{
	int	fd;

	fd = open("history.txt", O_RDWR | O_APPEND, 0666);
	write(fd, m->input, ft_strlen(m->input));
	write(fd, "\n", 1);
	close(fd);
}
