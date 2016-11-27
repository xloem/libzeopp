#include "zeo/serial/Reader.hpp"

#include "zeo/serial/Packet.hpp"
#include "zeo/serial/ProtocolException.hpp"

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
		rdBuffer.resize(size);
		stream.read(rdBuffer.data(), size);
		rdBuffer.resize(stream.gcount());
		buffer.insert(buffer.end(), rdBuffer.begin(), rdBuffer.end());

		if (static_cast<size_t>(stream.gcount()) < size) {
			std::vector<uint8_t> badBytes(buffer.begin(), buffer.end());
			buffer.clear();
			stream.clear();
			stream.exceptions(std::istream::failbit | std::istream::badbit | std::istream::eofbit);
			throw ProtocolException("Stream unavailable", badBytes);
		}
	}
}

// Throws an error after advancing the buffer to the next packet
void Reader::protocolException(std::string descr)
{
	size_t i = 0;
	do {
		++ i;
		ensureBuffer(i + 1);
	} while (buffer[i] != 'A');

	std::vector<uint8_t> badBytes(buffer.begin(), buffer.end());

	buffer.erase(buffer.begin(), buffer.begin() + i);

	throw ProtocolException(descr, badBytes);
}

Packet Reader::read()
{
	ensureBuffer(12);
	
	char magic = buffer[0];
	char version = buffer[1];
	uint8_t checksum = buffer[2];
	uint16_t len = LE<uint16_t>(buffer, 3);
	uint16_t lenInv = LE<uint16_t>(buffer, 5);
	uint8_t sec = buffer[7];
	uint16_t subsec = LE<uint16_t>(buffer, 8);
	uint8_t seqnum = buffer[10];
	Packet::Datatype datatype = static_cast<Packet::Datatype>(buffer[11]);
	
	if (magic != 'A')
		protocolException("Incorrect magic value");

	if (len != (lenInv ^ 0xffff))
		protocolException("Corrupt data length value");

	if (version != '4')
		protocolException("Expected protocol version 4");

	ensureBuffer(11 + len);

	uint8_t cksum = 0;
	for (size_t i = 0; i < len; ++ i)
		cksum += buffer[i + 11];

	if (datatype == Packet::Datatype::ZeoTimestamp) {
		uint32_t timestamp = LE<uint32_t>(buffer, 12);
		if ((timestamp & 0xff) != sec)
			protocolException("Corrupt time packet");
		lastTimestamp = timestamp;
	}

	double timestamp;

	if ((lastTimestamp & 0xff) == sec) {
		timestamp = lastTimestamp + subsec / 65536.0;
	} else if (((lastTimestamp + 1) & 0xff) == sec) {
		timestamp = lastTimestamp + 1 + subsec / 65536.0;
	} else {
		protocolException("Packet time doesn't match");
	}

	Packet ret = Packet{
		checksum,
		cksum == checksum,
		timestamp,
		seqnum,
		datatype,
		std::vector<uint8_t>(buffer.begin() + 12, buffer.begin() + 11 + len)
	};

	buffer.erase(buffer.begin(), buffer.begin() + 11 + len);

	return ret;
}

}
}
