#pragma once
namespace TAMENUT {
class SerializedPkt
{
private:
	unsigned int _pkt_id;
public:
	SerializedPkt(unsigned int pkt_id);
	~SerializedPkt();
};
}

