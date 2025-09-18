/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcai <bcai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:45:38 by bcai              #+#    #+#             */
/*   Updated: 2024/06/06 13:41:32 by bcai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_builtin(t_cmd *cmd, t_m *m)
{
	char	**cmd_args;

	cmd_args = ((t_execcmd *)cmd)->cmd_args;
	if (ft_strncmp(cmd_args[0], "cd", 2) == 0)
		builtin_cd(cmd, m);
	else if (ft_strncmp(cmd_args[0], "echo", 4) == 0)
		builtin_echo(cmd);
	else if (ft_strncmp(cmd_args[0], "pwd", 3) == 0)
		builtin_pwd(cmd, m);
	else if (ft_strncmp(cmd_args[0], "export", 6) == 0)
		builtin_export(cmd, m);
	else if (ft_strchr(cmd_args[0], '='))
		set_envvar(cmd, m);
	else if (ft_strncmp(cmd_args[0], "unset", 5) == 0)
		builtin_unset(cmd, m);
	else if (ft_strncmp(cmd_args[0], "env", 3) == 0)
		builtin_env(cmd, m);
	else if (ft_strncmp(cmd_args[0], "exit", 4) == 0)
		builtin_exit(cmd, m);
}

void	builtin_echo(t_cmd *cmd)
{
	char	**cmd_args;
	int		n_flag;
	int		i;
	t_qflag	*cqflags;

	cmd_args = ((t_execcmd *)cmd)->cmd_args;
	cqflags = ((t_execcmd *)cmd)->cqflags->next;
	if (cmd_args[1] == NULL)
	{
		printf("\n");
		return ;
	}
	i = 1;
	n_flag = 0;
	if (cmd_args[1] && cmd_args[1][0] == '-' && cmd_args[1][1] == 'n')
		update_eles(&n_flag, &cqflags, &i, cmd_args[1]);
	while (cmd_args[i] != NULL)
	{
		print_helper(cmd_args, &i, cqflags);
		cqflags = cqflags->next;
	}
	((t_execcmd *)cmd)->m->exit_status = 0;
	if (n_flag == 0)
		printf("\n");
}
