#pragma once

#include "Packet.hpp"
#include "PacketInvalid.hpp"

#include <cstdint>
#include <deque>
#include <istream>
#include <vector>

namespace zeo {
namespace serial {

class Reader
{
public:
	Reader(std::istream & stream);

	Packet read();

private:
	void ensureBuffer(size_t size);
	PacketInvalid invalid(Packet const & sofar, Packet::Type what, bool advance = true);

	std::istream & stream;
	std::deque<uint8_t> buffer;
	std::vector<char> rdBuffer;

	uint32_t lastTimestamp;
};

}
}
