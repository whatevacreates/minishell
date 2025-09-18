/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcai <bcai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:53:49 by eprzybyl          #+#    #+#             */
/*   Updated: 2024/06/06 14:24:41 by bcai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_sig_indicator;

typedef enum type
{
	CHAR = 0,
	EXEC = 1,
	PIPE = 2,
	REDIR = 3,
	LIST = 4,
	AND = 5,
	OR = 6,
	NODE = 7,
	STRING_ARRAY = 8,
	HEREDOC = 9,
	ON_MAIN = 10,
	ON_HEREDOC = 11,
	ON_EXEC = 12,
	QFLAG = 13
}								t_type;

typedef struct s_cmd
{
	int							type;
}								t_cmd;

typedef struct s_inout
{
	char						*file_name;
	int							mode;
	int							is_hd;
	struct s_heredoc			*hdcmd;
	struct s_inout				*next;
}								t_inout;

typedef struct s_qflag
{
	int							quote_flag;
	struct s_qflag				*next;
}								t_qflag;

typedef struct s_mini
{
	int							i;
	int							k;
	int							j;
	char						*path;
	char						**envp;
	char						**minishell_envp;
	char						*temp_path;
	char						**envp_path;
	char						*input;
	int							pfd[2];
	int							pipe_left_flag;
	int							pipe_right_flag;
	int							fdout_cpy;
	int							fdin_cpy;
	int							outcpy_emb;
	pid_t						pid;
	pid_t						pid_left;
	pid_t						pid_right;
	int							exit_status;
	int							heredoc_flag;
	int							heredoc_fd;
	int							redir_out;
	int							fd_outfile;
	int							out_cpy;
	t_cmd						*final_tree;
	t_inout						*in;
	t_inout						*out;
	int							position;
	char						*file_name;
	int							star_found;
	char						sign;
	int							star;
	char						*pattern_new;
	char						*new;
	t_list						*smlst;
	t_list						*ptr;
	t_list						*temp;
	t_list						*prev;
	t_list						*temp_w;
	t_list						*temp_list;
	t_list						*bank;
	int							last_node;
	int							wild_count;
	int							match_count;
	t_list						*check;
	char						*line;
	char						*one_ch;
	char						*history;
	char						*prev_history;
	int							initial_history_check;
	int							export_hidden;
	int							start;
	int							stop_executing;
	char						*home_cpy;
}								t_m;

typedef struct s_listcmd
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_listcmd;

typedef struct s_pipecmd
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_pipecmd;

typedef struct s_redircmd
{
	int							type;
	t_cmd						*cmd;
	char						*file;
	int							mode;
	int							fd;
}								t_redircmd;

typedef struct s_heredoc
{
	int							type;
	t_cmd						*cmd;
	char						*delimiter;
	int							fd;
	char						*tmp_file;
	int							is_quoted;
	char						**envp;
}								t_heredoc;

typedef struct s_execcmd
{
	int							type;
	t_list						*cmdargs;
	char						**cmd_args;
	t_qflag						*qflags;
	t_qflag						*cqflags;
	int							tkn_type;
	int							path_prob;
	t_m							*m;
}								t_execcmd;

typedef struct s_and
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_and;

typedef struct s_or
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_or;

typedef struct s_envvar
{
	char						*key;
	char						*value;
	int							is_exported;
	struct s_envvar				*next;
}								t_envvar;

typedef struct s_strvars
{
	char						*var_name;
	char						*first_part;
	char						*rest;
	char						*var_value;
	char						*new;
	int							i;
}								t_strvars;

typedef struct s_tkn
{
	char						*s_tkn;
	char						*e_tkn;
	int							rdr;
	int							fl;
}								t_tkn;

typedef struct s_gl
{
	char						*spaces;
	char						*signs;
	t_envvar					*env_vars;
	struct termios				orig_termios;
	int							quoted;
	int							consec_quotes;
	t_list						*oldpwd_list;
}								t_gl;

// executing main functions
void							partial_reinit_m(t_m *m);
void							runcmd(t_cmd *cmd, t_m *m);
void							traverse_tree(t_cmd *cmd, t_m *m);
void							find_path(t_m *m);
void							builtin_exec(t_cmd *cmd, t_m *m);
void							execute_simple_command(t_execcmd *ecmd, t_m *m);
void							redir_heredoc(t_cmd *cmd, t_m *m);
void							execute_pipe_command(t_pipecmd *pcmd, t_m *m);
void							execute_heredoc_command(t_heredoc *heredoc,
									t_m *m);
void							run_builtin(t_cmd *cmd, t_m *m);
void							pipe_left_exec(t_m *m, t_pipecmd *pcmd);
void							pipe_right_exec(t_m *m, t_pipecmd *pcmd);
void							lastfree_restore(void);

// utils for execution part
void							free_2darray(char **arr);
int								fork_check(t_m *m);
void							exit_error(int error_code, char *str, t_m *m,
									t_cmd *cmd);
void							report_error(int error_code, char *str, t_m *m,
									t_cmd *cmd);
void							check_exit_status(int status, t_m *m);
void							close_fds(int fdin_cpy, int fd);

// free final tree after execution
void							free_exec_content(t_execcmd *ecmd);
void							free_redir_content(t_redircmd *rcmd, t_m *m);
void							free_heredoc_content(t_heredoc *heredoc,
									t_m *m);
void							free_lr_list(t_cmd *left, t_cmd *right, t_m *m);
void							free_lr_pipe(t_cmd *left, t_cmd *right, t_m *m);
void							free_lr_and(t_cmd *left, t_cmd *right, t_m *m);
void							free_lr_or(t_cmd *left, t_cmd *right, t_m *m);
void							free_tree(t_cmd *cmd, t_m *m);

// signals
void							signal_tracking(struct sigaction *sa, t_m *m);
void							handle_sigint_heredoc(int sig,
									siginfo_t *siginfo, void *context);
void							handle_sigint(int sig, siginfo_t *siginfo,
									void *context);
void							handle_sigint_exec(int sig, siginfo_t *siginfo,
									void *context);
void							handle_sigquit_exec(int sig, siginfo_t *siginfo,
									void *context);

// multiple in_outfiles utiles
void							build_redir_list(t_cmd *cmd, t_m *m,
									t_inout **list);
void							add_node(t_inout **list, t_inout *new_node);
void							parselist_execute(t_cmd *cmd, t_m *m);
void							parselist_execute2(t_cmd *cmd, t_m *m);
void							inlist_execution_loop(t_m *m, t_execcmd *ecmd);
void							inlist_execution_loop2(t_m *m, t_execcmd *ecmd);
void							inlist_execution(t_execcmd *ecmd, t_m *m);
int								inlist_execution_util(t_execcmd *ecmd, t_m *m,
									t_inout **in_temp);
void							restore_inout(int fd_cpy, int in_out, t_m *m);
void							free_list(t_inout **list);
void							traverse_tree2(t_cmd *cmd, t_m *m);
void							unlink_heredoc_file(t_m *m);

// cmd struction initiation
t_execcmd						*execcmd_init(t_m *m);
t_redircmd						*redircmd_init(t_cmd *subcmd, char *s_token,
									size_t size);
void							init_mode_fd(t_redircmd *rcmd, int mode,
									int fd);

t_heredoc						*hdinit(t_cmd *subcmd, char *s_token,
									size_t size, int fd);
t_pipecmd						*pipecmd_init(t_cmd *left_cmd,
									t_cmd *right_cmd);
t_listcmd						*listcmd_init(t_cmd *left_cmd,
									t_cmd *right_cmd);
t_and							*andcmd_init(t_cmd *left_cmd, t_cmd *right_cmd);
t_or							*orcmd_init(t_cmd *left_cmd, t_cmd *right_cmd);

// parsing utils
int								get_type(char **start, char *end);
int								gettoken(char **start, char *end,
									char **s_token, char **e_token);
bool							skipspace_peek(char **start, char *end,
									char *check);
t_cmd							*quoted_delimiter(t_cmd *cmd, char **start,
									char *s_token, char *e_token);
void							add_qflag(t_qflag **lst, t_qflag *new);
size_t							ft_strlcpy_special(char *dst, const char *src,
									size_t dstsize);

// initial and last set of cmd before entering exeution
void							initial_setup(t_m *m, char **envp);
void							set_exec(t_cmd *cmd, t_m *m);
void							parse_left_right(t_cmd *left, t_cmd *right,
									t_m *m);
void							last_set(t_cmd *cmd, t_m *m);
char							*replace_dollar(char *original, t_m *m);
char							*get_newstr(char *temp, char *e_cpy, t_m *m,
									char *first_part);
void							get_strlen(char *temp, int *i);
void							update_temp(char **temp);

// parsing main functions
t_cmd							*parsecmd(char *input, t_m *m);
t_cmd							*parselist(char **start, char *end, t_m *m);
t_cmd							*parse_and_or(char **start, char *end, t_m *m);
t_cmd							*parsepipe(char **start, char *end, t_m *m);
t_cmd							*parseredirs(t_cmd *cmd, char **start,
									char *end);
t_cmd							*get_redircmd(t_cmd *cmd, char *s_token,
									char *e_token, int redir_type);
t_cmd							*parseexec(char **start, char *end, t_m *m);
t_cmd							*parseblock(char **start, char *end, t_m *m);

// cmd and args handling
void							get_cmd_args(t_execcmd *execcmd, t_cmd *cmd);
void							populate_cmdargs(t_execcmd *ecmd, char *s_token,
									char *e_token, t_cmd *cmd);
void							cmdargs_quote(t_execcmd *ecmd, char *s_token,
									char *e_token, char **start);
char							*getvalue_freename(char *var_name);
char							*get_current_envvar(char *key);
void							check_exec_flags(t_execcmd *ecmd);
void							add_flagnode(t_qflag **flags);
void							build_envvar_list(t_envvar *envvars,
									t_list **envcpy);

// safe_malloc modules
void							*safe_malloc(size_t size, t_type type,
									t_cmd *cmd);
char							*safe_malloc2(size_t size, t_type type,
									t_cmd *left_cmd, t_cmd *right_cmd);

// free memory in case of errors during parsing
void							free_memory(t_cmd *cmd);
void							free_heredoc(t_cmd *cmd);
void							free_redir(t_cmd *cmd);
void							free_exec(t_cmd *cmd);
void							free_flags(t_qflag *flags);
void							free_lrlist(t_cmd *cmd);
void							free_lrpipe(t_cmd *cmd);
void							free_lrand(t_cmd *cmd);
void							free_lror(t_cmd *cmd);

// other utils used during parsing
void							handle_error(char *msg, t_cmd *cmd);
char							*join_free_all(char *storage, char *buffer,
									t_cmd *cmd);
char							*minishell_strjoin(char const *s1,
									char const *s2, t_cmd *cmd);
char							*m_sub(char const *s, unsigned int start,
									size_t len, t_cmd *cmd);
int								get_quotetype(char **start, char *end);
int								get_atype(char **start, char *end);
int								get_andtype(char **start);
int								get_ortype(char **start);
int								get_outtype(char **start);
int								get_intype(char **start);
int								get_squotetype(char **start, char *end);
int								get_blocktype(char **start);

// environment variables handling
void							init_envvars(char **envp, int i, t_m *m);
void							fill_basic_envvars(void);
void							resize(char *buffer, size_t *size);
int								add_envvar(char *key, char *value,
									int is_exported);
int								check_key_validity(char *key, char *value,
									int export_flag);
int								update_envvars(char *key, char *value,
									int is_exported);
void							update_target(t_envvar *target, char *value,
									char *key, int is_exported);
void							remove_envvar(char *key);
t_envvar						*getter(void);
t_envvar						*get_envvar(char *key);
void							print_envvars(void);
void							free_envvars(void);
int								cmplen(char *lst_key, char *key);
void							minishell_envp(t_m *m);

// builtins and builtin utils
void							builtin_cd(t_cmd *cmd, t_m *m);
void							set_oldpwd(t_m *m, char *add_pwd);
void							resize_or_free(char *buffer, t_m *m,
									size_t *size);
void							builtin_echo(t_cmd *cmd);
void							builtin_export(t_cmd *cmd, t_m *m);
void							export_all(t_m *m);
void							set_envvar(t_cmd *cmd, t_m *m);
void							builtin_unset(t_cmd *cmd, t_m *m);
void							builtin_pwd(t_cmd *cmd, t_m *m);
void							builtin_env(t_cmd *cmd, t_m *m);
void							builtin_exit(t_cmd *cmd, t_m *m);
void							builtin_error(t_cmd *cmd, t_m *m, char *msg);
int								is_builtin(char *cmd);
char							*expand_value(char *start, char *target);
char							*handle_expansion(char *start, t_cmd *cmd,
									t_m *m);
char							*get_value(char *arg, char *quote_letter,
									t_cmd *cmd, t_m *m);
void							print_helper(char **cmd_args, int *i,
									t_qflag *cqflags);
void							update_eles(int *n_flag, t_qflag **cqflags,
									int *i, char *cmd_args);
void							no_value_case(char *arg, t_m *m);
int								no_value_after_equal1(int *i, char **cmd_args,
									char *equal, t_qflag **qflags);
int								value_after_equal(int i, t_cmd *cmd, t_m *m,
									char *equal);

// heredoc functions
void							no_line_expansion(t_heredoc *heredoc,
									int heredoc_fd, t_m *m);
void							expand_line(t_heredoc *heredoc, int heredoc_fd,
									t_m *m);
void							expand_env_cmd(char *start, char *target,
									int heredoc_fd, t_heredoc *heredoc);
void							embedded_cmdline(char *cmdline, int heredoc_fd,
									t_heredoc *heredoc);

// custom readline function
char							*ft_readline(char *prompt, t_m *m);
char							*delete_char(char *line);
void							clean_exit(char *one_char, t_m *m);
void							set_raw_mode(void);

// wildcards utils
int								pattern_match(char *file_name, char *pattern);
t_list							*open_cd(char *str, t_list *list, t_m *m);
t_list							*expand_list(t_list *list, char *file_name);
char							*open_cd_in_out(char *str, t_m *m);
char							*expand_input_check(char *input, t_m *m);
char							*find_pattern(char *input, t_m *m);
char							*merge_files_input(char *t, char *in, t_m *m);

int								check_if_pattern(char *input, int i);
void							set_up_signs(char *input, t_m *m);
char							*f(char *input, t_m *m, char *pattern);
void							build_files_list(t_list **list, char *file);
t_list							*check_wildcard(t_list *list);
t_list							*find_smallest_and_remove(t_list **array_files,
									t_m *m);
char							*build_array_files(t_list *array_list, t_m *m);
t_list							*building_sorted(t_list *smallest, t_list *new);
void							merge_files_loop(t_m *m, char *input, int i,
									char *temp);
void							sort_array(t_list **array_files, t_m *m);
void							find_smallest(t_list **array_files, t_m *m);
t_list							*check_list(t_list **list);
t_list							*add_check_node(t_list *list, t_list *node);
t_list							*merge_list(t_list *list, t_list *temp);
t_list							*remove_wildcards(t_list *list, t_list *bank);
void							free_t_list(t_list **list);
void							find_executable_path(t_m *m, t_execcmd *ecmd);
void							initialize_var_wild(t_m *m);
t_list							*wildcards_to_remove_check(t_list *list,
									t_list **bank);

// gl
t_gl							*init_gl_var(void);
t_gl							*get_gl(void);

// history
void							update_working_history(t_m *m);
int								does_file_history_exist(void);
void							update_history_file(t_m *m);
void							load_history(t_m *m);
char							*remove_line_break(char *line);

#endif
