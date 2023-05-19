#include "shell.h"

/**
 * _myexit - exits the shell
 * @info: Structure containing potential arguments.
 *
 * Return: Always 0
 */
int _myexit(info_t *info)
{
	if (info->argv[1]) /* If there is an exit argument */
	{
		int exit_code = _erratoi(info->argv[1]);

		if (exit_code == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}

		info->err_num = exit_code;
	}

	return (0);
}

/**
 * _mycd - changes the current directory of the process
 * @info: Structure containing potential arguments.
 *
 * Return: Always 0
 */
int _mycd(info_t *info)
{
	char *dir = NULL;
	char buffer[1024];
	char *cwd = getcwd(buffer, sizeof(buffer));

	if (!cwd)
		_puts("TODO: >>getcwd failure emsg here<<\n");

	if (!info->argv[1])
		dir = _getenv(info, "HOME=");
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		dir = _getenv(info, "OLDPWD=");

		if (!dir)
		{
			_puts(cwd);
			_putchar('\n');
			return (1);
		}

		_puts(dir);
	}
	else
		dir = info->argv[1];

	if (chdir(dir) == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(dir);
		_eputchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD"));
		_setenv(info, "PWD", getcwd(buffer, sizeof(buffer)));
	}

	return (0);
}

/**
 * _myhelp - provides help information
 * @info: Structure containing potential arguments.
 *
 * Return: Always 0
 */
int _myhelp(info_t *info)
{
	(void)info;
	_puts("help call works. Function not yet implemented \n");
	return (0);
}

