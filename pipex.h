/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:59:48 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/16 13:31:21 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

typedef struct s_pipe
{
	int		status;
	int		fd[2];
	pid_t	pid[2];
	int		argc;
	char	**envp;
	char	**paths;
	char	**argv;
}	t_pipe;

void	freeall(char **strarray);
char	**make_args(char *arg);
char	*ft_getpath(char *cmd, char **paths);
#endif
