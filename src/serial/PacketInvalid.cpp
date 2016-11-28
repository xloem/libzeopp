#include "zeo/serial/PacketInvalid.hpp"

namespace zeo {
namespace serial {

PacketInvalid::PacketInvalid(Packet const & packet, Packet::Type what, std::vector<uint8_t> const & bytes)
: Packet(packet)
{
	type = what;
	data = bytes;
}

PacketInvalid::PacketInvalid(PacketInvalid const & invalid, Packet const & packet)
: Packet(packet)
{
	type = invalid.type;
	data = invalid.data;
}

}
}
