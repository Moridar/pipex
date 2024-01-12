/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:33:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/12 14:37:36 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	freeall(char **strarray)
{
	int	i;

	if (strarray)
	{
		i = -1;
		while (strarray[++i])
			free(strarray[i]);
		free(strarray);
	}
}

void	errormsg(char *msg, int exits)
{
	msg = ft_strjoin("pipex: ", msg);
	perror(msg);
	free(msg);
	if (exits)
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

static void	cmdnfound_exit(char *cmd)
{
	char	*errmsg;

	errmsg = ft_strjoin("pipex: ", cmd);
	errmsg = ft_strjoin(errmsg, ": command not found\n");
	write(2, errmsg, ft_strlen(errmsg));
	exit(127);
}

char	*ft_getpath(char *cmd, char **paths)
{
	int		i;
	char	*cmdpath;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	if (!paths)
		paths = ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':');
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
	free(paths);
	if (!cmdpath)
		cmdnfound_exit(cmd + 1);
	free(cmd);
	return (cmdpath);
}
