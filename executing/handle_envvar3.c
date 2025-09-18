/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_envvar3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcai <bcai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:48:54 by bcai              #+#    #+#             */
/*   Updated: 2024/05/30 10:28:48 by bcai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	special_cases(char *key, char *value)
{
	if (key[0] == '!')
	{
		printf("minishell: %s=%s: event not found\n", key, value);
		return (1);
	}
	if (key[0] == '#')
		return (1);
	return (0);
}

void	print_error_msg(int export_flag, char *key, char *value)
{
	if (export_flag == 1)
		printf("minishell: export:'%s=%s': \
			not a valid identifier\n", key, value);
	else
		printf("minishell: '%s=%s': not a valid identifier\n", key, value);
}

int	check_key_validity(char *key, char *value, int export_flag)
{
	int	i;

	if (ft_isdigit(key[0]) || ft_strchr("~@%*^", key[0]))
	{
		print_error_msg(export_flag, key, value);
		return (1);
	}
	if (special_cases(key, value) == 1)
		return (1);
	i = 0;
	while (key[++i])
	{
		if (!ft_isdigit(key[i]) && !ft_isalpha(key[i]) && key[i] != '_')
		{
			print_error_msg(export_flag, key, value);
			return (1);
		}
	}
	return (0);
}

void	assign_new(char *key, char *value, int is_exported, t_envvar **new)
{
	(*new)->key = ft_strdup(key);
	(*new)->value = ft_strdup(value);
	(*new)->is_exported = is_exported;
	(*new)->next = NULL;
	free(key);
	free(value);
}

int	add_envvar(char *key, char *value, int is_exported)
{
	t_envvar	*new;
	t_envvar	*temp;
	t_gl		*gl;

	if (check_key_validity(key, value, is_exported) == 1)
		return (1);
	gl = get_gl();
	new = malloc(sizeof(t_envvar) * 1);
	if (!new)
		return (1);
	assign_new(key, value, is_exported, &new);
	if (!gl->env_vars)
	{
		gl->env_vars = new;
		return (0);
	}
	temp = gl->env_vars;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	return (0);
}
