/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcai <bcai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:47:26 by bcai              #+#    #+#             */
/*   Updated: 2024/06/06 11:16:06 by bcai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	runcmd(t_cmd *cmd, t_m *m)
{
	if (cmd != NULL)
	{
		if (m->start == 0)
		{
			m->start = 1;
			set_oldpwd(m, getcwd(safe_malloc(100, CHAR, cmd), 1024));
		}
		traverse_tree(cmd, m);
		free_tree(cmd, m);
	}
}

void	traverse_lr(t_cmd *left, t_cmd *right, t_m *m)
{
	traverse_tree(left, m);
	traverse_tree(right, m);
}

void	traverse_lr_and(t_cmd *left, t_cmd *right, t_m *m)
{
	traverse_tree(left, m);
	if (m->exit_status == 0)
		traverse_tree(right, m);
}

void	traverse_lr_or(t_cmd *left, t_cmd *right, t_m *m)
{
	traverse_tree(left, m);
	if (m->exit_status != 0)
		traverse_tree(right, m);
}

void	traverse_tree(t_cmd *cmd, t_m *m)
{
	if (cmd->type == EXEC)
	{
		if (((t_execcmd *)cmd)->cmd_args == NULL)
			return ;
		builtin_exec(cmd, m);
	}
	else if (cmd->type == REDIR)
		redir_heredoc(cmd, m);
	else if (cmd->type == HEREDOC)
		redir_heredoc(cmd, m);
	else if (cmd->type == PIPE)
		execute_pipe_command((t_pipecmd *)cmd, m);
	else if (cmd->type == LIST)
		traverse_lr(((t_listcmd *)cmd)->left, ((t_listcmd *)cmd)->right, m);
	else if (cmd->type == AND)
		traverse_lr_and(((t_and *)cmd)->left, ((t_and *)cmd)->right, m);
	else if (cmd->type == OR)
		traverse_lr_or(((t_or *)cmd)->left, ((t_or *)cmd)->right, m);
	else
		return ;
}
