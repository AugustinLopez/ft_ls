# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <errno.h>
# include <time.h>
# include <sys/sysmacros.h>

typedef struct stat	t_stat;
typedef struct tm	t_tm;

char * format_time(time_t cal_time)
{
  t_tm			*time_struct;
  static char	string[30];

  time_struct=localtime(&cal_time);
  strftime(string, sizeof string, "%h %e %H:%M\n", time_struct);
  return(string);
}
int	main(void)
{
	t_stat pstat;
	errno = ENOENT;

	if((stat("infofile.c", &pstat)) == -1)
	{
		perror("stat");
		return (-1);
	}
	printf("%s\n", "infofile.c");
	printf("ID of containing device:  [%lx,%lx]\n",
	(long) major(pstat.st_dev), (long) minor(pstat.st_dev));

	printf("File type:                ");
	switch (pstat.st_mode & S_IFMT)
	{
	case S_IFBLK:  printf("block device\n");            break;
	case S_IFCHR:  printf("character device\n");        break;
	case S_IFDIR:  printf("directory\n");               break;
	case S_IFIFO:  printf("FIFO/pipe\n");               break;
	case S_IFLNK:  printf("symlink\n");                 break;
	case S_IFREG:  printf("regular file\n");            break;
	case S_IFSOCK: printf("socket\n");                  break;
	default:       printf("unknown?\n");                break;
	}
	printf("I-node number:            %ld\n", (long) pstat.st_ino);
	printf("Mode:                     %lo (octal)\n",                  (unsigned long) pstat.st_mode);
	printf("Link count:               %ld\n", (long) pstat.st_nlink);
	printf("Ownership:                UID=%ld   GID=%ld\n",                  (long) pstat.st_uid, (long) pstat.st_gid);
	printf("Preferred I/O block size: %ld bytes\n",                   (long) pstat.st_blksize);
	printf("File size:                %lld bytes\n",(long long) pstat.st_size);
	printf("Blocks allocated:         %lld\n",(long long) pstat.st_blocks);
	printf("Last status change:       %s", format_time(pstat.st_ctime));
	printf("Last file access:         %s", format_time(pstat.st_mtime));
	printf("Last file modification:   %s", format_time(pstat.st_atime));
	return (0);
}
