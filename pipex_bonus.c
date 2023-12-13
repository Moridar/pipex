/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/12/13 16:40:24 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	exec(int i, t_pipe *data, int fd[])
{
	char	**args;
	char	*path;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		errorexit("fork");
	if (pid == 0)
	{
		args = make_args(data->argv[i]);
		path = ft_getpath(ft_strdup(args[0]), data->paths);
		if (i == 2 || data->argc - i == 2)
			close(fd[2]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(path, args, data->envp);
		exit(EXIT_FAILURE);
	}
	data->pid[i - 2] = pid;
	if (i == 2 || data->argc - i == 2)
		close(fd[3]);
}

static void	fileexec(int i, t_pipe *data, int last)
{
	int		file;
	int		newfd[4];

	if (last)
	{
		file = open(data->argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 00644);
		if (file < 0)
			errorexit("outfile error");
		newfd[0] = data->fd[(i + 1) % 2][0];
		newfd[1] = file;
		newfd[2] = data->fd[(i + 1) % 2][1];
		newfd[3] = file;
	}
	else
	{
		file = open(data->argv[i - 1], O_RDONLY);
		if (file < 0)
			return (perror("infile error"));
		newfd[0] = file;
		newfd[1] = data->fd[0][1];
		newfd[2] = data->fd[0][0];
		newfd[3] = file;
	}
	exec(i, data, newfd);
}

static void	execute(int i, t_pipe *data)
{
	if (i == 2)
	{
		if (pipe(data->fd[0]) == -1)
			errorexit("pipe");
		fileexec(i, data, 0);
	}
	else if (i == data->argc - 2)
	{
		fileexec(i, data, 1);
		close(data->fd[(i + 1) % 2][1]);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 3], &data->status, 0);
		waitpid(data->pid[i - 2], &data->status, 0);
	}
	else
	{
		if (pipe(data->fd[i % 2]) == -1)
			errorexit("pipe");
		close(data->fd[(i + 1) % 2][1]);
		data->fd[(i + 1) % 2][1] = data->fd[i % 2][1];
		exec(i, data, data->fd[(i + 1) % 2]);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 3], &data->status, 0);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;
	int		i;

	if (argc < 5)
		return (printf("Usage: %s infile cmd1 cmd2 (cmd3) outfile", argv[0]));
	data.argv = argv;
	data.argc = argc;
	data.envp = envp;
	data.status = 0;
	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	data.paths = ft_split((*envp) + 5, ':');
	data.pid = ft_calloc(sizeof(pid_t), (argc - 3));
	i = 1;
	while (++i < argc - 1)
		execute(i, &data);
	free(data.pid);
	freeall(data.paths);
	return (WEXITSTATUS(data.status));
}
