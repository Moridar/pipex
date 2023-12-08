/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/12/08 11:01:50 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"

char	*joinandfree(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}

char	**makeargs(char *argv[])
{
	char	**args;

	args = malloc(sizeof(char *) * 4);
	args[0] = "/bin/zsh";
	args[1] = "-c";
	args[2] = ft_strjoin("<", argv[1]);
	args[2] = joinandfree(args[2], " ");
	args[2] = joinandfree(args[2], argv[2]);
	args[2] = joinandfree(args[2], " | ");
	args[2] = joinandfree(args[2], argv[3]);
	args[2] = joinandfree(args[2], " >");
	args[2] = joinandfree(args[2], argv[4]);
	args[3] = NULL;

	return (args);
}


int	main(int argc, char *argv[], char *envp[])
{
	char	**args;

	if (argc != 5)
		return (ft_printf("Usage: %s file1 cmd1 cmd2 file2\n", argv[0]));
	args = makeargs(argv);
	execve(args[0], args, envp);
	free(args[2]);
	free(args);
	perror("Error executing cmd1");
	return (0);
}