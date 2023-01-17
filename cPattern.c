#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

void change_pattern(int8_t *referer)
{
	static int8_t i = 1, j = 0, k = 0, l = 0;

	if (i == 10) {
		i = 0;
		++j;
		if (j == 10) {
			j = 0;
			++k;
			if (k == 26) {
				k = 0;
				++l;
				if (l == 26) {
					l = 0;
				}
				referer[0] = (65 + l) % 91;
			}
			referer[1] = (65 + k) % 91;
		}
		referer[2] = (48 + j) % 58;
	}
	referer[3] = (48 + i) % 58;
	++i;
}

void gen_pattern(int8_t *referer, int32_t size)
{
	int32_t i = 0, t = 0;

	printf("[-] Generated String:\n\n");

	while (1) {
		if (i == 4) {
			change_pattern(referer);
			i = 0;
		}

		printf("%c", referer[i]);

		if (t == size - 1)
			break;

		++i;
		++t;
	}

	printf("\n");
}

void find_offset(int8_t *referer, int32_t num)
{
	int8_t max[] = {'Z','Z','9','9'};
	int8_t *ch = (int8_t *) &num;
	int32_t i = 0, t = 0, flag = 1;

	while (1) {
		flag = 1;

		for (i = 3; i > -1; --i)
			if (ch[i] != referer[i]) {
				flag = 0;
				break;
			}

		if (flag == 1) {
			printf("[-] OFFSET FOUND: \033[1;32m%d\033[0m\n",
					t * 4);	
			break;
		}

		for (i = 0; i < 4; ++i)
			if (referer[i] != max[i]) {
				flag = 1;
				break;
			}

		if (flag == 0) {
			printf("\033[1;31m[-] OFFSET NOT FOUND\033[0m\n");
			return;
		}

		change_pattern(referer);
		++t;
	}
}

void message(void)
{
	srand(time(NULL));

	int8_t m_number = 1 + rand() % 8;

	printf("\033[%d;3%dm", 1 + m_number % 2, m_number);
	printf("                                                 \n"
			" #######################################################\n" 
			"#         ____ ____       _   _                    	#\n"
			"#        / ___|  _ \\ __ _| |_| |_ ___ _ __ _ __    	#\n"
			"#       | |   | |_) / _` | __| __/ _ \\ '__| '_  \\  	#\n"
			"#       | |___|  __/ (_| | |_| ||  __/ |  | | | |  	#\n"
			"#        \\____|_|   \\__,_|\\__|\\__\\___|_|  |_| |_|	#\n"
			"#                                                  	#\n"
			"#		     Pedro Ferreira                  	#\n"
			" #######################################################\n");

	printf("\n\n\033[0m"); 

}

void error(void)
{
	printf("Compile:\tgcc -o cPattern cPattern.c\n\n\n");
	printf("Usage:\t\t--offset to find the EIP Value\n");
	printf("\t\t--string to generate a string composed of a unique pattern\n");

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int8_t referer[4] = {'A', 'A', '0', '0'};

	message();

	if (argc != 3) { error(); }

	if (!strcmp("--offset", argv[1])) {
		if (argv[2] == NULL) error();

		if (strlen(argv[2]) < 8) {
			printf("[!] OFFSET MUST BE 8 BYTES LENGHT\n");
			return EXIT_FAILURE;
		}

		int32_t address = strtol(argv[2], NULL, 16);

		if (address < 0) { error(); }

		find_offset(referer, address);

	} else if (!strcmp("--string", argv[1])) {
		if (argv[2] == NULL) { error(); }

		int32_t size = atoi(argv[2]);

		if (size < 0) { error(); }

		gen_pattern(referer, size);
	} else {
		error();
	}

	return 0;
}
