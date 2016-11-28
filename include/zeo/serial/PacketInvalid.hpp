#pragma once

#include "Packet.hpp"

#include <string>
#include <vector>

namespace zeo {
namespace serial {

class PacketInvalid : public Packet
{
friend class Reader;
private:
	PacketInvalid(Packet const & data, Packet::Type what, std::vector<uint8_t> const & bytes);
	PacketInvalid(PacketInvalid const & invalid, Packet const & data);
};

}
}
