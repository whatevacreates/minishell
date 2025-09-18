/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_finaltree1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprzybyl <eprzybyl@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:48:26 by bcai              #+#    #+#             */
/*   Updated: 2024/05/27 22:03:05 by eprzybyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_flags(t_qflag *flags)
{
	t_qflag	*tmp;

	while (flags != NULL)
	{
		tmp = flags;
		flags = flags->next;
		free(tmp);
	}
}

void	free_exec_content(t_execcmd *ecmd)
{
	t_list	*temp;
	t_qflag	*qflag;

	free_2darray(ecmd->cmd_args);
	qflag = ecmd->qflags;
	if (ecmd->cmdargs != NULL)
	{
		while (ecmd->cmdargs != NULL)
		{
			temp = ecmd->cmdargs;
			ecmd->cmdargs = ecmd->cmdargs->next;
			if (qflag->quote_flag == 34 || qflag->quote_flag == 0)
				free(temp->content);
			free(temp);
			qflag = qflag->next;
		}
	}
	free_flags(ecmd->qflags);
	free_flags(ecmd->cqflags);
}

void	free_redir_content(t_redircmd *rcmd, t_m *m)
{
	free(rcmd->file);
	free_tree(rcmd->cmd, m);
}

void	free_heredoc_content(t_heredoc *heredoc, t_m *m)
{
	free(heredoc->delimiter);
	heredoc->delimiter = NULL;
	free(heredoc->tmp_file);
	free_tree(heredoc->cmd, m);
}

void	free_tree(t_cmd *cmd, t_m *m)
{
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
		free_exec_content((t_execcmd *)cmd);
	else if (cmd->type == REDIR)
		free_redir_content((t_redircmd *)cmd, m);
	else if (cmd->type == HEREDOC)
		free_heredoc_content((t_heredoc *)cmd, m);
	else if (cmd->type == PIPE)
		free_lr_pipe(((t_pipecmd *)cmd)->left, ((t_pipecmd *)cmd)->right, m);
	else if (cmd->type == LIST)
		free_lr_list(((t_listcmd *)cmd)->left, ((t_listcmd *)cmd)->right, m);
	else if (cmd->type == AND)
		free_lr_and(((t_and *)cmd)->left, ((t_and *)cmd)->right, m);
	else if (cmd->type == OR)
		free_lr_or(((t_or *)cmd)->left, ((t_or *)cmd)->right, m);
	if (cmd != NULL)
	{
		free(cmd);
		cmd = NULL;
	}
}
