#pragma once

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#endif

#include "TSocket.h"
#include "SerializedPayload.h"
#include <vector>

using namespace std;


class TUdpSocket : public TSocket
{
public:

	UTIL_DLLFunction TUdpSocket();
	/*
	//Unicast UDP용 송신 생성자
	UTIL_DLLFunction TUdpSocket(char *dst_ip_str, unsigned short bind_port);
	//Unicast UDP용 수신 생성자
	UTIL_DLLFunction TUdpSocket(unsigned short bind_port);

	//Multicast UDP용 송신 생성자
	UTIL_DLLFunction TUdpSocket(char *dst_ip_str, unsigned short bind_port, vector<string>& src_ip_list);
	//Multicast UDP용 수신 생성자
	UTIL_DLLFunction TUdpSocket(vector<string> ucastAddrList, const char * mcast_addr, unsigned short bind_port);
	*/
	
	UTIL_DLLFunction virtual ~TUdpSocket(void);

	UTIL_DLLFunction void set_rcv_buf_size(unsigned int buf_size);

	UTIL_DLLFunction void set_snd_buf_size(unsigned int buf_size);

	UTIL_DLLFunction int read(char *payload, unsigned int rcv_len);
	UTIL_DLLFunction int read(SerializedPayload &serialized_payload, unsigned int rcv_len);
	UTIL_DLLFunction int post(char *payload, unsigned int length);
	UTIL_DLLFunction int post(SerializedPayload &serialized_payload);
	UTIL_DLLFunction int post(int dst_ip_addr, unsigned short bind_port, char *payload, int length);
	UTIL_DLLFunction int post(int dst_ip_addr, unsigned short bind_port, SerializedPayload &serialized_payload);
	
	UTIL_DLLFunction void set_snd_time_out(int milisec_time_out);
	UTIL_DLLFunction void set_dont_wait(bool is_dont_wait);
	UTIL_DLLFunction void set_rcv_time_out(int milisec_time_out);
	UTIL_DLLFunction unsigned short get_port();


	UTIL_DLLFunction void sender_init(int dst_ip_addr, unsigned short bind_port);
	UTIL_DLLFunction void sender_init(char *dst_ip_addr_str, unsigned short bind_port);
	UTIL_DLLFunction void receiver_init(unsigned short bind_port);

	UTIL_DLLFunction void set_src_port_for_send(unsigned short src_port);

	UTIL_DLLFunction void sender_init(int mcast_addr, unsigned short bind_port, vector<string>& src_ip_list);
	UTIL_DLLFunction void receiver_init(int mcast_addr, unsigned short bind_port, vector<string> src_ip_list);

	UTIL_DLLFunction void sender_init(char *mcast_addr_str, unsigned short bind_port, vector<string>& src_ip_list);
	UTIL_DLLFunction void receiver_init(char *mcast_addr_str, unsigned short bind_port, vector<string> src_ip_list);
private:
	

protected:
	vector<SOCKET> _snd_sock_list;
	struct sockaddr_in _snd_addr;

	unsigned short _bind_port;
	bool _is_dont_wait;

};


