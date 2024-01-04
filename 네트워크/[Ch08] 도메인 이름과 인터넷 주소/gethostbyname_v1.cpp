#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <arpa/inet.h>

void err_handle(const char* err_msg)
{
	fputs(err_msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	if (argc != 2 )
	{
		fputs("Enter hostname\n", stderr);
		exit(1);
	}

	struct hostent *host = gethostbyname(argv[1]);
	if (host == NULL)
	{
		fputs("wrong hostname\n", stderr);
		exit(1);
	}

	// (1). h_name
	printf("Official name : %s\n", host->h_name);

	// (2). char** h_aliases
	for (int i=0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", (i+1), host->h_aliases[i]);

	// (3). h_addrtype
	int addrType = host->h_addrtype;
	printf("h_addrType : %s\n", host->h_addrtype == AF_INET?"IPV4":"IPV6");

	// (4). h_length
	printf("h_length : %d\n", host->h_length);

	// (5). h_addr_list
	for (int i=0; host->h_addr_list[i]; i++)
	{
		printf("ip %d : %s\n", (i+1), inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	return 0;
}