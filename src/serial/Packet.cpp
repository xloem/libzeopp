#include "zeo/serial/Packet.hpp"

#include <iomanip>
#include <sstream>

namespace zeo {
namespace serial {

std::string Packet::datatypeStr(Datatype type)
{
	switch (type) {
	case Datatype::Event:
		return "Event";
	case Datatype::SliceEnd:
		return "SliceEnd";
	case Datatype::Version:
		return "Version";
	case Datatype::Waveform:
		return "Waveform";
	case Datatype::FrequencyBins:
		return "FrequencyBins";
	case Datatype::SQI:
		return "SQI";
	case Datatype::ZeoTimestamp:
		return "ZeoTimestamp";
	case Datatype::Impedance:
		return "Impedance";
	case Datatype::BadSignal:
		return "BadSignal";
	case Datatype::SleepStage:
		return "SleepStage";
	default:
		std::stringstream ss;
		ss << "Unknown(" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint8_t>(type) << ")";
		return ss.str();
	}
}

std::string Packet::str() const
{
	std::stringstream ss;
	ss << "ZeoSerialPacket #" << +sequenceNumber << " @" << std::setprecision(17) << timestamp;
	if (checksumCorrect)
		ss << " intact ";
	else
		ss << " corrupt ";
	ss << datatypeStr(datatype);
	ss << std::hex;
	for (uint8_t byte : rawData)
		ss << " " << std::setfill('0') << std::setw(2) << +byte;
	return ss.str();
}

}
}
