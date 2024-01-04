#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char* argv[])
{
	if (argc !=2)
	{
		fputs("Enter hostname\n", stderr);
		exit(1);
	}

	struct hostent* host = gethostbyname(argv[1]);

	// 1. h_name
	printf("Official name : %s\n", host->h_name);

	// 2. h_aliases
	for (int i=0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s\n", (i+1), host->h_aliases[i]);

	// 3. h_addrtype
	printf("Address type: %s\n", host->h_addrtype==AF_INET?"AF_INET":"AF_INET6");

	// 4. h_length
	printf("Address Length : %d\n", host->h_length);

	// 5. h_addr_list
	for (int i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s\n", (i+1), inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	return 0;
}
