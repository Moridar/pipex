/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/12/11 11:43:28 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*ft_getpath(char *cmd, char *envp[])
{
	char	**paths;
	int		i;
	char	*cmdpath;

	cmdpath = cmd;
	cmd = ft_strjoin("/", cmd);
	free(cmdpath);
	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	paths = ft_split((*envp) + 5, ':');
	i = -1;
	while (paths[++i])
	{
		cmdpath = ft_strjoin(paths[i], cmd);
		if (access(cmdpath, F_OK) == 0)
			break ;
		free(cmdpath);
		cmdpath = NULL;
	}
	free(cmd);
	freeall(paths);
	return (cmdpath);
}

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
		args = ft_split(data->argv[i], ' ');
		if (args)
			path = ft_getpath(args[0], data->envp);
		if (i == 2 || i == 3)
			close(fd[2]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(path, args, data->envp);
		dprintf(2, "error\n");
		exit(EXIT_FAILURE);
	}
	data->pid[i - 2] = pid;
	if (i == 2 || i == 3)
		close(fd[i - 2]);
}

static void	fileexec(int i, t_pipe *data, int last)
{
	int		file;
	int		newfd[3];

	if (last)
	{
		file = open(data->argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 00777);
		if (file < 0)
			errorexit("outfile error");
		newfd[0] = data->fd[0];
		newfd[1] = file;
		newfd[2] = data->fd[1];
	}
	else
	{
		file = open(data->argv[i - 1], O_RDONLY);
		if (file < 0)
			errorexit("infile error");
		newfd[0] = file;
		newfd[1] = data->fd[1];
		newfd[2] = data->fd[0];
	}
	exec(i, data, newfd);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;

	data.argv = argv;
	data.argc = argc;
	data.envp = envp;
	if (argc != 5)
		return (printf("Usage: %s file1 cmd1 cmd1 file2", argv[0]));
	if (pipe(data.fd) == -1)
		errorexit("pipe");
	fileexec(2, &data, 0);
	fileexec(3, &data, 1);
	close(data.fd[1]);
	close(data.fd[0]);
	waitpid(data.pid[0], NULL, 0);
	waitpid(data.pid[1], NULL, 0);
	return (0);
}
