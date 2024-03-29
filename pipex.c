/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/16 15:22:41 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	errormsg(char *msg, int exits)
{
	msg = ft_strjoin("pipex: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(errno);
}

static void	exec(int i, t_pipe *data, int fd[])
{
	char	**args;
	char	*path;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		errormsg("fork", 1);
	if (pid == 0)
	{
		args = make_args(data->argv[i]);
		path = ft_getpath(ft_strdup(args[0]), data->paths);
		close(fd[2]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(path, args, data->envp);
		exit(EXIT_FAILURE);
	}
	close(fd[i - 2]);
	data->pid[i - 2] = pid;
}

static void	fileexec(int i, t_pipe *data, int last)
{
	int		file;
	int		newfd[3];

	if (last)
	{
		file = open(data->argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 00644);
		if (file < 0)
			errormsg(data->argv[i + 1], 1);
		newfd[0] = data->fd[0];
		newfd[1] = file;
		newfd[2] = data->fd[1];
	}
	else
	{
		file = open(data->argv[i - 1], O_RDONLY);
		if (file < 0)
			return (errormsg(data->argv[i - 1], 0));
		newfd[0] = file;
		newfd[1] = data->fd[1];
		newfd[2] = data->fd[0];
	}
	exec(i, data, newfd);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;

	if (argc != 5)
		return (printf("Usage: %s file1 cmd1 cmd1 file2", argv[0]));
	data.argv = argv;
	data.argc = argc;
	data.envp = envp;
	data.status = 0;
	data.pid[0] = 0;
	while (envp && *envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (envp && *envp)
		data.paths = ft_split((*envp) + 5, ':');
	if (pipe(data.fd) == -1)
		errormsg("pipe", 1);
	fileexec(2, &data, 0);
	fileexec(3, &data, 1);
	close(data.fd[1]);
	close(data.fd[0]);
	freeall(data.paths);
	if (data.pid[0])
		waitpid(data.pid[0], &data.status, 0);
	waitpid(data.pid[1], &data.status, 0);
	return (WEXITSTATUS(data.status));
}
