#include <unistd.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	pid_t	pid1;

	pipe(fd);
	dup2(fd[1], STDOUT_FILENO);
	pid1 = fork();
	if (pid1 == -1)
		exit();
	if (pid1 == 0)
		execvp(file, argv);

	return (fd)
}
