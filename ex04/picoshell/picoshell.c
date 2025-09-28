int	picoshell(char **cmds[])
{
	int	fd[2];
	pid_t	pid;

	pipe(fd);
	while (cmds[i])
	{
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
	}
}
