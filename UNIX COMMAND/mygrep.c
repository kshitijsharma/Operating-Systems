#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{

	int source_d;

	char line[100];

	char *pattern;

	char buffer;
	int counter = 0;

	if (argv[1][0] != '-')
	{
		pattern = argv[1];
		for (int i = 0; i < argc - 2; i++)
		{
			source_d = open(argv[i+2], O_RDONLY);

			if (source_d == -1)
			{
				fprintf(stderr, "No such file\n");
				return 1;
			}

			while (read(source_d, &buffer, 1))
			{
				if (buffer == '\n')
				{
					line[counter] = '\0';
					if (strstr(line, pattern)){
						if(argc > 3)
							printf("%s: ",argv[i+2]);
						printf("%s\n", line);
					}
					counter = 0;
					for (int i = 0; i < 100; i++)
						line[i] = 0;
				}
				else
					line[counter++] = buffer;
			}
			close(source_d);
		}

		return 0;
	}

	if (argc >= 4)
	{
		pattern = argv[2];

		for (int i = 0; i < (argc - 3); i++)
		{
			source_d = open(argv[3 + i], O_RDONLY);

			if (source_d == -1)
			{
				fprintf(stderr, "No such file %s\n!", argv[i + 3]);
				return 1;
			}

			if (strcmp(argv[1], "-c") == 0)
			{
				int matches = 0;
				while (read(source_d, &buffer, 1))
				{
					if (buffer == '\n')
					{
						line[counter] = '\0';
						if (strstr(line, pattern))
							matches++;
						counter = 0;
						for (int i = 0; i < 100; i++)
							line[i] = 0;
					}
					else
						line[counter++] = buffer;
				}
				if (argc > 4)
					printf("%s: ", argv[i + 3]);
				printf("%d\n", matches);
			}
			else if (strcmp(argv[1], "-h") == 0)
			{

				while (read(source_d, &buffer, 1))
				{
					if (buffer == '\n')
					{
						line[counter] = '\0';
						if (strstr(line, pattern))
							printf("%s\n", line);
						counter = 0;
						for (int i = 0; i < 100; i++)
							line[i] = 0;
					}
					else
						line[counter++] = buffer;
				}
			}
			else if (strcmp(argv[1], "-v") == 0)
			{

				while (read(source_d, &buffer, 1))
				{
					if (buffer == '\n')
					{
						line[counter] = '\0';
						if (!strstr(line, pattern))
						{
							if (argc > 4)
								printf("%s: ", argv[i + 3]);
							printf("%s\n", line);
						}
						counter = 0;
						for (int i = 0; i < 100; i++)
							line[i] = 0;
					}
					else
						line[counter++] = buffer;
				}
			}
			else
			{
				fprintf(stderr, "No Such argument!\n");
				return 1;
			}
			close(source_d);
		}
	}
	close(source_d);
}
