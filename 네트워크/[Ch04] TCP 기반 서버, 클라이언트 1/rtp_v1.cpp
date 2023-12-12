#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// RTP Header
struct RTPHeader // 32 bit
{
	uint8_t version : 2; //
	uint8_t padding : 1;
	uint8_t extension : 1;
	uint8_t csrcCount : 4;
	uint8_t marker : 1;
	uint8_t payload : 7;
	uint16_t sequenceNumber;
	uint32_t timestamp;
	uint32_t ssrc;

};

int main()
{
	struct RTPHeader rtpHeader;
	memset(&rtpHeader, 0, sizeof(struct RTPHeader));

	rtpHeader.version = 2;
	rtpHeader.padding = 0;
	rtpHeader.extension = 0;
	rtpHeader.csrcCount = 0;
	rtpHeader.marker = 0;
	rtpHeader.payload = 0; // RTP payload type (e.g., audio or video)
	rtpHeader.sequenceNumber = htons(12345);
	rtpHeader.timestamp = htonl(987654321);
	rtpHeader.ssrc = htonl(123456789);

	int header_size = sizeof(rtpHeader);
	uint8_t buffer[header_size]; // 1byte

	printf("%d  %d  %d\n", header_size, sizeof(buffer), sizeof(uint8_t));

	memcpy(buffer, &rtpHeader, header_size);

	int sock = socker(AF_INET, SOCK_DGRAM, IPPROTO_UDP);



	return 0;
}
