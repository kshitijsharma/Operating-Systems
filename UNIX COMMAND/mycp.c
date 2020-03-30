#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	char choice;
	int source_d;
	int dest_d;
	char buffer[4096];

	if (argc < 3)
	{
		printf("Insufficient Arguments!\n");
		return 1;
	}

	if (argc == 4)
	{
		source_d = open(argv[2], O_RDONLY);

		if (strcmp(argv[1], "-i") == 0)
		{
			printf("Are you sure you want to copy? Y/N ");
			scanf("%c", &choice);

			if (!(choice == 'y' || choice == 'Y'))
				return 0;
		}
		else if (strcmp(argv[1], "-f") == 0)
		{

			;
		}
		else if (strcmp(argv[1], "-n") == 0)
		{

			dest_d = open(argv[3], O_RDONLY);

			if (dest_d != -1)
			{ //File Exists
				printf("File exists!Quitting!\n");
				close(dest_d);
				return 0;
			}
		}
		else if (strcmp(argv[1], "-v") == 0)
		{

			printf("\'%s\' -> \'%s\'\n", argv[2], argv[3]);
		}
		else if (strcmp(argv[1], "-r") == 0)
		{
			DIR *source;
			DIR *dest;
			source = opendir(argv[2]);
			struct dirent *ent;
			int fd = -1;
			char dest_path[1000];
			char source_path[1000];
			int len;

			if (!source)
			{
				fprintf(stderr, "No Such Directory!\n");
				return 0;
			}
			dest = opendir(argv[3]);
			if (!dest)
				mkdir(argv[3], 0770);
			else
				closedir(dest);

			while ((ent = readdir(source)) != NULL)
			{
				if (ent->d_name[0] == '.')
					continue;
				sprintf(dest_path, "%s/%s", argv[3],ent -> d_name);
				sprintf(source_path,"%s/%s",argv[2],ent -> d_name);
				creat(dest_path, S_IWUSR | S_IRUSR);
				dest_d = open(dest_path, O_WRONLY);
				if (!fd)
				{
					fprintf(stderr, "Fatal Error!\n");
					return 1;
				}

				source_d = open(source_path, O_RDONLY);
				if (!(source_d || dest_d))
				{
					fprintf(stderr, "Fatal Error! Cannot open file!\n");
					return 1;
				}
				len = read(source_d, buffer, 4096);
				while (len > 0)
				{
					write(dest_d, buffer, len);
					len = read(source_d, buffer, 4096);
				}
				close(dest_d);
				close(source_d);
			}
			closedir(source);
			return 0;
		}
		else
		{
			fprintf(stderr, "Invalid Arguments [ -r , -n , -i ,-f ,-v supported]!\n");
			return 1;
		}
		creat(argv[3], S_IRWXU);
		dest_d = open(argv[3], O_RDWR);
	}

	if (argc == 3)
	{
		source_d = open(argv[1], O_RDONLY);
		creat(argv[2], S_IRWXU);
		dest_d = open(argv[2], O_RDWR);
	}

	if (source_d == -1 || dest_d == -1)
	{
		fprintf(stderr, "Critical Error!\n");
		return 1;
	}

	int len;

	while ((len = read(source_d, buffer, 4096)))
		write(dest_d, buffer, len);

	close(source_d);
	close(dest_d);
}
