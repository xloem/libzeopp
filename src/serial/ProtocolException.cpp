#include "zeo/serial/ProtocolException.hpp"

namespace zeo {
namespace serial {

ProtocolException::ProtocolException(std::string what, std::vector<uint8_t> data)
: std::runtime_error(what), data(data)
{}

const std::vector<uint8_t> & ProtocolException::whatData()
{
	return data;
}

}
}
