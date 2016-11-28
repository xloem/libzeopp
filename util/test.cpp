#include <zeo/serial/Reader.hpp>

#include <iostream>
#include <fstream>

int main(int argc, const char ** argv)
{
	std::istream *stream = &std::cin;
	std::ifstream fstream;
	if (argc > 1) {
		fstream.open(argv[1]);
		stream = &fstream;
	}

	zeo::serial::Reader reader(*stream);

	zeo::serial::Packet packet;
	do {
		packet = reader.read();
		if (packet.isValid())
			std::cout << packet.str() << std::endl;
		else
			std::cerr << packet.str() << std::endl;
	} while (packet.type != zeo::serial::Packet::Type::StreamClosed);

	return 0;
}
