#pragma once

namespace zeo {
namespace serial {

// reads a little endian integer from a container of bytes
// not optimized -- done this way to be quickly cross-platform for now
template <typename T, typename U> T LE(U const & buffer, size_t pos)
{
	T ret = 0;
	for (size_t i = 0; i < sizeof(T); ++ i) {
		ret |= buffer[i+pos] << (i * 8);
	}
	return ret;
}

}
}

