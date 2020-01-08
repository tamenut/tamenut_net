#include "stdafx.h"
#include "SerializedPkt.h"
#include "TUtil.h"

namespace TAMENUT {
SerializedPkt::SerializedPkt(unsigned int pkt_id)
	:_pkt_id(hton_t(pkt_id))
{

}

SerializedPkt::~SerializedPkt()
{
}
unsigned int SerializedPkt::get_pkt_id()
{
	return _pkt_id;
}
}