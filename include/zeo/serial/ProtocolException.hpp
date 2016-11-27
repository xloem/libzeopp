#pragma once

#include <stdexcept>
#include <vector>

namespace zeo {
namespace serial {

class ProtocolException : public std::runtime_error
{
public:
	ProtocolException(std::string what, std::vector<uint8_t> data);
	const std::vector<uint8_t> & whatData();

private:
	const std::vector<uint8_t> data;
};

}
}
