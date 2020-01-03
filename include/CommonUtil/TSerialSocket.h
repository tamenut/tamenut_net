#pragma once

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#endif
#include "TThread.h"
#include "TMsgQueue.h"
#include "TSocket.h"
#include <string.h>

enum SERIAL_BAUD_RATE
{
	SERIAL_BAUD_RATE_110 = 110,
	SERIAL_BAUD_RATE_300 = 300,
	SERIAL_BAUD_RATE_600 = 600,
	SERIAL_BAUD_RATE_1200 = 1200,
	SERIAL_BAUD_RATE_2400 = 2400,
	SERIAL_BAUD_RATE_4800 = 4800,
	SERIAL_BAUD_RATE_9600 = 9600,
	SERIAL_BAUD_RATE_14400 = 14400,
	SERIAL_BAUD_RATE_19200 = 19200,
	SERIAL_BAUD_RATE_38400 = 38400,
	SERIAL_BAUD_RATE_56000 = 56000,
	SERIAL_BAUD_RATE_57600 = 57600,
	SERIAL_BAUD_RATE_115200 = 115200,
	SERIAL_BAUD_RATE_128000 = 128000,
	SERIAL_BAUD_RATE_256000 = 256000,
};

enum SERIAL_PARITY
{
	SERIAL_PARITY_NO = 0,
	SERIAL_PARITY_ODD = 1,
	SERIAL_PARITY_EVEN = 2,
	SERIAL_PARITY_MARK = 3,
	SERIAL_PARITY_SPACE = 4,
};

enum SERIAL_DATA_BIT
{
	SERIAL_DATA_BIT_7 = 7,
	SERIAL_DATA_BIT_8 = 8,
};

enum SERIAL_STOP_BIT
{
	SERIAL_STOP_BIT_ONE = 0,
	SERIAL_STOP_BIT_ONE5 = 1,
	SERIAL_STOP_BIT_TWO = 2,
};


class TSerialSocket : public TThread
{
public:
	UTIL_DLLFunction TSerialSocket(string _com_port, SERIAL_BAUD_RATE _baud_rate, SERIAL_PARITY _parity, SERIAL_DATA_BIT _data_bit, SERIAL_STOP_BIT _stop_bit);
	UTIL_DLLFunction virtual ~TSerialSocket(void);

	UTIL_DLLFunction int post(char *payload, int payload_len);
	UTIL_DLLFunction int read(char *payload);

protected:
	void ResetSerial();
	void run();
	

private:
	bool is_connected;
	TMsgQueue<QueueNode> user_data_queue;

	string com_port;
	SERIAL_BAUD_RATE baud_rate;
	SERIAL_PARITY parity;
	SERIAL_DATA_BIT data_bit;
	SERIAL_STOP_BIT stop_bit;
	unsigned short flow_check;
	bool is_openned;

	HANDLE m_hComDev;
};


