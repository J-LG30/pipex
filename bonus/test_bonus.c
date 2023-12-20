#include "pipex_bonus.h"

void	write_to_pipe(int fd, char **env, char *argv)
{
	char	**arg;
	char	*cmd_path;

	arg = create_arg(argv);
	cmd_path = get_command_path(arg[0], env, fd);
	if (!cmd_path)
	{
		free_arg(arg);
		error_quit(4);
	}
	dup2(fd, 1);
	close(fd);
	execve(cmd_path, arg, env);
}

// void	pipe_loop(int fd[2], t_file files, char *argv, char **env, pid_t child_pids[])
// {
// 	if (pipe(fd) == -1)
// 		error_quit(2);
// 	pid = fork();
// 	if (pid == -1)
// 		error_quit(3);
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		close(files.fileout);
// 		close(files.filein);
// 		write_to_pipe(fd[1], env, argv);
// 	}
// 		close(fd[1]);
// 		dup2(fd[0], 0);
// 		//waitpid(pid, NULL, 0);
// }

void    handle_eof()
{
    char buf[1];
    int bytes;
    while (1)
    {
        bytes = read(STDIN_FILENO, buf, 1);
        if (bytes < 1)
            break;
    }
    if (bytes == 0)
        exit(1);
    if (bytes == -1)
    {
        perror("Read");
        exit(1);
    }
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	pid_t	child_pids[argc - 3];
	int		i;
	t_file	files;
    int     j;

	if (argc < 5)
		error_quit(1);
	i = 2;
    j = -1;
	files.fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	files.filein = open(argv[1], O_RDONLY);
	dup2(files.filein, 0);
     while (i < argc - 2)
    {
        if (pipe(fd) == -1)
		error_quit(2);
	    child_pids[i - 2] = fork();
	    if (child_pids[i - 2] == -1)
		    error_quit(3);
	    if (child_pids[i - 2] == 0)
	    {
		    close(fd[0]);
            close(files.fileout);
            close(files.filein);
            write_to_pipe(fd[1], env, argv[i]);
        }
            close(fd[1]);
            dup2(fd[0], 0);
        i++;
    }
    if ((child_pids[j] = fork()) == -1)
        error_quit(3);
    if (child_pids[j] == 0)
        write_to_pipe(files.fileout, env, argv[argc - 2]);
    while (j++ < argc - 2)
        waitpid(child_pids[j], NULL, 0);
}




//  while (i < argc - 2)
//     {
//         if (pipe(fd) == -1)
// 		error_quit(2);
// 	    child_pids[i - 2] = fork();
// 	    if (child_pids[i - 2] == -1)
// 		    error_quit(3);
// 	    if (child_pids[i - 2] == 0)
// 	    {
// 		    close(fd[0]);
//             close(files.fileout);
//             close(files.filein);
//             write_to_pipe(fd[1], env, argv[i]);
//         }
//             close(fd[1]);
//             dup2(fd[0], 0);
//         i++;
//     }
//     if ((child_pids[j] = fork()) == -1)
//         error_quit(3);
//     if (child_pids[j] == 0)
//         write_to_pipe(files.fileout, env, argv[argc - 2]);
//     while (j < argc - 2)
//         waitpid(child_pids[j], NULL, 0);




//   pid_t child_pids[argc - 3]; // Assuming the last argument is the output file

//     for (int i = 2; i < argc - 2; ++i) {
//         if (pipe(fd) == -1)
//             error_quit(2);

//         if ((child_pids[i - 2] = fork()) == -1)
//             error_quit(3);

//         if (child_pids[i - 2] == 0) {
//             // Child process executes the command
//             close(fd[0]); // Close the unused read end of the pipe
//             dup2(fd[1], STDOUT_FILENO); // Redirect command output to pipe
//             close(fd[1]); // Close the write end of the pipe
//             write_to_pipe(STDOUT_FILENO, env, argv[i]);
//             exit(0);
//         }
//         close(fd[1]); // Close the write end of the pipe in the parent process
//         fd[0] = dup(fd[0]); // Update the read end of the pipe for the next iteration
//     }

//     // Last command executed without a pipe, so handle it separately
//     if ((child_pids[argc - 3] = fork()) == -1)
//         error_quit(3);

//     if (child_pids[argc - 3] == 0) {
//         write_to_pipe(files.fileout, env, argv[argc - 2]); // Last command
//         exit(0);
//     }

//     for (int j = 0; j < argc - 2; ++j) {
//         waitpid(child_pids[j], NULL, 0); // Wait for all child processes to finish
//     }