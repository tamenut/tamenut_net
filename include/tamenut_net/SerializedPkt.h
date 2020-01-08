#pragma once
#include "define_dll.h"

#pragma pack(push, 1)
namespace TAMENUT {
	
class SerializedPkt
{
private:
	unsigned int _pkt_id;
public:
	DLL_FUNC SerializedPkt(unsigned int pkt_id);
	DLL_FUNC virtual ~SerializedPkt();

	DLL_FUNC unsigned int get_pkt_id();
};

}
#pragma pack(pop)

