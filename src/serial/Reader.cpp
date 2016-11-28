#include "zeo/serial/Reader.hpp"

#include "zeo/serial/Packet.hpp"
#include "zeo/serial/PacketInvalid.hpp"

#include "Util.hpp"

#include <cassert>

namespace zeo {
namespace serial {

Reader::Reader(std::istream & stream)
: stream(stream), lastTimestamp(0)
{ }

// Reads until the buffer has the given size
void Reader::ensureBuffer(size_t size)
{
	if (buffer.size() < size) {
		size_t toRead = size - buffer.size();
		rdBuffer.resize(toRead);
		stream.read(rdBuffer.data(), toRead);
		rdBuffer.resize(stream.gcount());
		buffer.insert(buffer.end(), rdBuffer.begin(), rdBuffer.end());

		if (static_cast<size_t>(stream.gcount()) < toRead) {
			stream.clear();
			stream.exceptions(std::istream::failbit | std::istream::badbit | std::istream::eofbit);
			throw invalid({}, Packet::Type::StreamClosed, false);
		}
	}
}

// Returns error packet after advancing the buffer to the next packet if necessary
PacketInvalid Reader::invalid(Packet const & sofar, Packet::Type what, bool advance)
{
	size_t i;
	if (advance) {
		i = 0;
		do {
			++ i;
			ensureBuffer(i + 1);
		} while (buffer[i] != 'A');
	} else {
		i = buffer.size();
	}

	std::vector<uint8_t> badBytes(buffer.begin(), buffer.end());

	buffer.erase(buffer.begin(), buffer.begin() + i);

	return PacketInvalid(sofar, what, badBytes);
}

Packet Reader::read()
{
	Packet ret = {};
	try {
		ensureBuffer(12);
		
		char magic = buffer[0];
		char version = buffer[1];
		ret.checksum = buffer[2];
		uint16_t len = LE<uint16_t>(buffer, 3);
		uint16_t lenInv = LE<uint16_t>(buffer, 5);
		uint8_t sec = buffer[7];
		uint16_t subsec = LE<uint16_t>(buffer, 8);
		ret.sequenceNumber = buffer[10];
		ret.type = static_cast<Packet::Type>(buffer[11]);
		
		if (magic != 'A')
			return invalid({}, Packet::Type::BadMagic);
	
		if (len != (lenInv ^ 0xffff) || len < 1)
			return invalid(ret, Packet::Type::CorruptLength);
	
		if (version != '4')
			return invalid(ret, Packet::Type::BadVersion);
	
		if ((lastTimestamp & 0xff) == sec) {
			ret.timestamp = lastTimestamp + subsec / 65536.0;
		} else if (((lastTimestamp + 1) & 0xff) == sec) {
			++ lastTimestamp;
			ret.timestamp = lastTimestamp + subsec / 65536.0;
		}
	
		ensureBuffer(11 + len);
	
		ret.data = std::vector<uint8_t>(buffer.begin() + 12, buffer.begin() + 11 + len);
	
		uint8_t cksum = static_cast<uint8_t>(ret.type);
		for (size_t i = 1; i < len; ++ i)
			cksum += ret.data[i - 1];

		if (cksum != ret.checksum)
			return invalid(ret, Packet::Type::CorruptData);
	
		if (ret.type == Packet::Type::ZeoTimestamp) {
			uint32_t timestamp = LE<uint32_t>(ret.data, 0);
			if ((timestamp & 0xff) != sec)
				return invalid(ret, Packet::Type::IncorrectTime);
			lastTimestamp = timestamp;
			ret.timestamp = timestamp + subsec / 65536.0;
		} else if (! ret.timestamp) {
			return invalid(ret, Packet::Type::SkippedTime);
		}
	
		buffer.erase(buffer.begin(), buffer.begin() + 11 + len);
	
		return ret;

	} catch(PacketInvalid pi) {
		return PacketInvalid(pi, ret);
	}
}

}
}
