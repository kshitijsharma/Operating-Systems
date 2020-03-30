#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{

	int dirfd;

	DIR *dir;
	struct dirent *ent;
	struct stat fstat;
	if (argc < 3)
	{
		if(argc == 2)
			dir = opendir(argv[1]);
		else 
			dir = opendir(".");

		if (!dir)
		{
			fprintf(stderr, "Invalid Directory\n");
			return 1;
		}

		ent = readdir(dir);

		while (ent)
		{
			if (ent->d_name[0] != '.')
				printf("%s\n", ent->d_name);
			ent = readdir(dir);
		}
		return 0;
	}

	if (argc == 3)
	{
		dir = opendir(argv[2]);

		if (!dir)
		{
			fprintf(stderr, "Invalid Directory\n");
			return 1;
		}

		ent = readdir(dir);

		if (strcmp(argv[1], "-l") == 0)
		{
			while (ent)
			{
				dirfd = stat(ent->d_name, &fstat);
				if (dirfd < 0)
				{
					fprintf(stderr, "Fatal Error!\n");
					return 1;
				}
				printf((S_ISDIR(fstat.st_mode)) ? "d" : "-");
				printf((fstat.st_mode & S_IRUSR) ? "r" : "-");
				printf((fstat.st_mode & S_IWUSR) ? "w" : "-");
				printf((fstat.st_mode & S_IXUSR) ? "x" : "-");
				printf((fstat.st_mode & S_IRGRP) ? "r" : "-");
				printf((fstat.st_mode & S_IWGRP) ? "w" : "-");
				printf((fstat.st_mode & S_IXGRP) ? "x" : "-");
				printf((fstat.st_mode & S_IROTH) ? "r" : "-");
				printf((fstat.st_mode & S_IWOTH) ? "w" : "-");
				printf((fstat.st_mode & S_IXOTH) ? "x" : "-");
				printf(" %-ld ", fstat.st_nlink);
				printf(" %-6ld ", fstat.st_size);
				printf(" %-15s\n", ent->d_name);
				ent = readdir(dir);
			}
		}
		else if (strcmp(argv[1], "-i") == 0)
		{
			while (ent)
			{
				dirfd = stat(ent->d_name, &fstat);
				if (dirfd < 0)
				{
					fprintf(stderr, "Fatal Error!\n");
					return 1;
				}
				printf("%-10ld %-20s\n", fstat.st_ino, ent->d_name);
				ent = readdir(dir);
			}
		}
		else if (strcmp(argv[1], "-a") == 0)
		{

			while (ent)
			{
				printf("%s\n", ent->d_name);
				ent = readdir(dir);
			}
		}
		else if (strcmp(argv[1], "-r") == 0)
		{

			char arrays[100][100];
			int counter = -1;

			while (ent)
			{
				if (ent->d_name[0] != '.')
					strcpy(arrays[++counter], ent->d_name);
				ent = readdir(dir);
			}

			for (; counter > -1; counter--)
				printf("%s\n", arrays[counter]);
		}
		else
		{
			fprintf(stderr, "Invalid Argument!\n");
			return 1;
		}
	}

	closedir(dir);
}
