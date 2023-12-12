/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_ulti.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:33:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/12/12 15:38:16 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	freeall(char **strarray)
{
	int	i;

	i = -1;
	while (strarray[++i])
		free(strarray[i]);
	free(strarray);
}

void	errorexit(char *errormsg)
{
	perror(errormsg);
	exit(errno);
}

char	**make_args(char *arg)
{
	char	**args;
	int		i;
	char	*strend;
	char	c;

	args = ft_split(arg, ' ');
	if (!args)
		exit(1);
	c = 0;
	if (args[1] && (*args[1] == '\'' || *args[1] == '"'))
		c = *args[1];
	if (c)
	{
		free(args[1]);
		args[1] = ft_strchr(arg, c) + 1;
		strend = ft_strchr(args[1], c);
		*strend = 0;
		i = 2;
		while (args[i])
			free(args[i++]);
		args[2] = 0;
	}
	return (args);
}

char	*ft_getpath(char *cmd, char **paths)
{
	int		i;
	char	*cmdpath;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	cmdpath = cmd;
	cmd = ft_strjoin("/", cmd);
	free(cmdpath);
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
	free(paths);
	if (!cmdpath)
		exit(1);
	return (cmdpath);
}
