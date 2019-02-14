#ifndef FT_LS_H
# define FT_LS_H

# include <libft.h>
# include <errno.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <sys/types.h>
# include <sys/xattr.h>
# include <sys/ioctl.h>
# include <stdio.h> //To be removed

enum {LSERR_USAGE, LSERR_MALLOC, LSERR_OTHER, LSERR_CLOSEDIR, LSERR_OPENDIR, LSERR_PATH, LSERR_LONG};
/*# define LSERR_USAGE 1
# define LSERR_MALLOC 2
# define LSERR_OTHER 3
# define LSERR_CLOSEDIR 4

# define LSERR_PATH 5
# define LSERR_LONG 6*/
# define LSO_L 2
# define LSO_RR 4
# define LSO_A 8
# define LSO_R 16
# define LSO_T 32

typedef struct		stat 	t_stat;
typedef struct		dirent	t_dirent;

typedef struct		s_file
{
	/*mode_t			mode;
	nlink_t			link;
	uid_t			user;
	gid_t			group;
	off_t			size;
	time_t			lastmod;
	blkcnt_t		block;*/
	t_stat			stat;
	char			*name;
	struct s_file	*prev;
	struct s_file	*next;
}					t_file;

typedef struct		s_ls
{
	t_file			*file;
	t_file			*curr_file;
	t_list			*directory;
	int				flags;
}					t_ls;


int			ls_parsing(int ac, char **av, int *flags);
int			ls_print_error(char *str, int errflag);
int			load_info_from_directory(t_ls *ls);
int			load_directory(t_ls *ls);
void		sort2(t_ls *lkd_list);
void		display_file(t_file *file);

void		print_ls(t_ls *liste1);

#endif
