#include <zeo/serial/ProtocolException.hpp>
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

	for (;;)
	try {
		zeo::serial::Packet packet = reader.read();
		std::cout << packet.str() << std::endl;
	} catch(zeo::serial::ProtocolException pe) {
		std::cerr << "ProtocolException: " << pe.what() << std::endl;
	} catch(std::ios_base::failure ex) {
		std::cerr << "IO Error: " << ex.what() << std::endl;
		break;
	}

	return 0;
}
