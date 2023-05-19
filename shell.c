#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define ARGUMENTS_LIMIT 10

/**
 * tokenize_command - Tokenizes the command into individual arguments.
 * @command: The command line string to tokenize.
 * @args: An array to store the tokenized arguments.
 *
 * Return: The number of arguments.
 */
int tokenize_command(char *command, char **args)
{
    int arg_count = 0;
    char *token;

    token = strtok(command, " ");
    while (token != NULL && arg_count < ARGUMENTS_LIMIT)
    {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    return arg_count;
}

/**
 * execute_command - Executes the command with the given arguments.
 * @args: The array of command arguments.
 */
void execute_command(char **args)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(NULL);
    }
}

int main(void)
{
    char *command;
    size_t bufsize = BUFFER_SIZE;
    ssize_t characters_read;

    command = (char *)malloc(bufsize * sizeof(char));
    if (command == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *args[ARGUMENTS_LIMIT];
    int arg_count;

    while (1)
    {
        printf("#cisfun$ ");

        characters_read = getline(&command, &bufsize, stdin);
        if (characters_read == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            else
            {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        if (command[characters_read - 1] == '\n')
            command[characters_read - 1] = '\0';

        arg_count = tokenize_command(command, args);

        execute_command(args);
    }

    free(command);
    return 0;
}

