#include "zeo/serial/Packet.hpp"

#include <iomanip>
#include <sstream>

namespace zeo {
namespace serial {

bool Packet::isValid()
{
	return static_cast<int>(type) >= 0;
}

std::string Packet::typeStr(Type type)
{
	switch (type) {
	case Type::Event:
		return "Event";
	case Type::SliceEnd:
		return "SliceEnd";
	case Type::Version:
		return "Version";
	case Type::Waveform:
		return "Waveform";
	case Type::FrequencyBins:
		return "FrequencyBins";
	case Type::SQI:
		return "SQI";
	case Type::ZeoTimestamp:
		return "ZeoTimestamp";
	case Type::Impedance:
		return "Impedance";
	case Type::BadSignal:
		return "BadSignal";
	case Type::SleepStage:
		return "SleepStage";

	case Type::StreamClosed:
		return "StreamClosed";
	case Type::BadMagic:
		return "BadMagic";
	case Type::CorruptLength:
		return "CorruptLength";
	case Type::BadVersion:
		return "BadVersion";
	case Type::CorruptData:
		return "CorruptData";
	case Type::IncorrectTime:
		return "IncorrectTime";
	case Type::SkippedTime:
		return "SkippedTime";

	default:
		std::stringstream ss;
		ss << "Unknown(" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint8_t>(type) << ")";
		return ss.str();
	}
}

std::string Packet::str() const
{
	std::stringstream ss;
	ss << "ZeoSerialPacket #" << +sequenceNumber << " @" << std::setprecision(17) << timestamp << " " << typeStr(type);
	ss << std::hex;
	for (uint8_t byte : data)
		ss << " " << std::setfill('0') << std::setw(2) << +byte;
	return ss.str();
}

}
}
