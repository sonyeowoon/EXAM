int	ft_popen(const char *file, char *const argv[], char type)
{
	int	fd[2];

	pipe(fd);
	pid_t pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	else if (pid == 0)
	{
		if (type == 'r')
			dup2(fd[1], STDOUT_FILENO);
		else
			dup2(fd[0], STDIN_FILENO);
		execvp(file, argv);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else
	{
		close(fd[0]);
		return (fd[1]);
	}
	return -1;
}
