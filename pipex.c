/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/12/11 08:05:20 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	errorexit(char *errormsg)
{
	perror(errormsg);
	exit(EXIT_FAILURE);
}

int	exec(int i, char *argv[], int fd[], char *envp[])
{
	pid_t	pid;
	char	*args[] = {"cat", NULL};
	
	pid = fork();
	if (pid == -1)
		errorexit("fork");
	if (pid == 0)
	{
		if (i == 2 || i == 3)
			close(fd[2]);
		printf("i:%d executing\n", i);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		printf("i:%d dupped stdin\n", i);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		dprintf(2, "i:%d dupped stdout\n", i);
		execve("/bin/cat", args, envp);
		printf("error");
		exit(EXIT_SUCCESS);
	}
	if (i == 2 || i == 3)
		close(fd[i - 2]);
	return (pid);
}

static int	lastexec(char *argv[], int fd[2], char *envp[])
{
	int		outfile;
	int		newfd[3];

	outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 00777);
	if (outfile < 0)
		errorexit("outfile error");
	newfd[2] = fd[1];
	newfd[0] = fd[0];
	newfd[1] = outfile;
	printf("last exec\n");
	return (exec(3, argv, newfd, envp));
}

static int	firstexec(char *argv[], int fd[2], char *envp[])
{
	int		infile;
	int		newfd[3];

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		errorexit("infile error");
	newfd[2] = fd[0];
	newfd[0] = infile;
	newfd[1] = fd[1];
	printf("first exec\n");
	return (exec(2, argv, newfd, envp));
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd[2];
	int		pid[2];

	if (argc != 5)
		return (printf("Usage: %s file1 cmd1 cmd1 file2", argv[0]));
	if (pipe(fd) == -1)
		errorexit("pipe");
	pid[0] = firstexec(argv, fd, envp);
	pid[1] = lastexec(argv, fd, envp);
	close(fd[1]);
	close(fd[0]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	while (1) {}
}
