/**
 * history - Reads the history file and builds a linked list of commands
 * @info: Parameter struct
 *
 * Return: The number of history entries read
 */
int history(info_t *info)
{
	int fd, fsize, linecount = 0;
	char *buf = NULL;

	fd = open(HISTORY_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (-1);

	fsize = lseek(fd, 0, SEEK_END);
	if (fsize == -1)
	{
		close(fd);
		return (-1);
	}

	buf = malloc(fsize + 1);
	if (buf == NULL)
	{
		close(fd);
		return (-1);
	}

	if (read_history_file(fd, buf, fsize) == -1)
	{
		free(buf);
		close(fd);
		return (-1);
	}

	close(fd);

	build_history_list(info, buf, fsize, linecount);

	free(buf);
	info->histcount = linecount;

	trim_history(info);
	return (info->histcount);
}

/**
 * read_history_file - Reads the history file into a buffer
 * @fd: File descriptor of the history file
 * @buf: Buffer to store the history contents
 * @fsize: Size of the history file
 *
 * Return: 0 on success, -1 on failure
 */
int read_history_file(int fd, char *buf, int fsize)
{
	if (lseek(fd, 0, SEEK_SET) == -1)
		return (-1);

	if (read(fd, buf, fsize) == -1)
		return (-1);

	return (0);
}

/**
 * build_history_list - Adds an entry to the history linked list
 * @info: Parameter struct
 * @buf: Buffer containing the history entry
 * @fsize: Size of the buffer
 * @linecount: The history line count
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *buf, int fsize, int linecount)
{
	int last = 0;
	int i;

	for (i = 0; i < fsize; i++)
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			add_history_entry(info, buf + last, linecount++);
			last = i + 1;
		}
	}

	if (last != i)
		add_history_entry(info, buf + last, linecount++);

	return (0);
}

/**
 * add_history_entry - Adds a history entry to the linked list
 * @info: Parameter struct
 * @entry: The history entry
 * @linecount: The history line count
 */
void add_history_entry(info_t *info, char *entry, int linecount)
{
	list_t *node = create_history_node(entry, linecount);

	if (node == NULL)
		return;

	if (info->history == NULL)
		info->history = node;
	else
		add_node_end(&(info->history), node);
}

/**
 * trim_history - Trims the history list to a maximum size
 * @info: Parameter struct
 */
void trim_history(info_t *info)
{
	while (info->histcount >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);

	renumber_history(info);
}

/**
 * renumber_history - Renumbers the history linked list after changes
 * @info: Parameter struct
 */
void renumber_history(info_t *info)
{
	list_t *node = info->history

