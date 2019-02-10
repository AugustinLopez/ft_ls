# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>

typedef struct dirent t_dirent;

int	main(void)
{
	DIR			*directory;
	t_dirent	*file;

	errno = ENOENT;
	if(!(directory = opendir("test")))
	{
		perror("opendir");
		return (-1);
	}
	while ((file = readdir(directory)))
		printf("%-10s %-20lu %hhu\n", file->d_name, file->d_ino, file->d_type);
	if (closedir(directory) == -1)
	{
		perror("closedir");
		return (-1);
	}
	return (0);
	/*
	printf("DT_BLK=%hhu\n", DT_BLK);
	printf("DT_CHR=%hhu\n", DT_CHR);
	printf("DT_DIR=%hhu\n", DT_DIR);
	printf("DT_FIFO=%hhu\n", DT_FIFO);
	printf("DT_LNK=%hhu\n", DT_LNK);
	printf("DT_REG=%hhu\n", DT_REG);
	printf("DT_SOCK=%hhu\n", DT_SOCK);
	printf("DT_UNKNOWN=%hhu\n", DT_UNKNOWN);
	*/
	return (0);
}
