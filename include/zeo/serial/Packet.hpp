#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace zeo {
namespace serial {

class Packet
{
friend class Reader;
public:
	enum class Type
	{
		Event         = 0x00,
		SliceEnd      = 0x02,
		Version       = 0x03,
		Waveform      = 0x80,
		FrequencyBins = 0x83,
		SQI           = 0x84,
		ZeoTimestamp  = 0x8a,
		Impedance     = 0x97,
		BadSignal     = 0x9c,
		SleepStage    = 0x9d,

		StreamClosed  = -0x01,
		BadMagic      = -0x02,
		CorruptLength = -0x03,
		BadVersion    = -0x04,
		CorruptData   = -0x05,
		IncorrectTime = -0x06,
		SkippedTime   = -0x07
	};

	uint8_t checksum;
	double timestamp;
	uint8_t sequenceNumber;
	Type type;
	
	std::vector<uint8_t> data;

	bool isValid();

	std::string str() const;

	static std::string typeStr(Type type);
};



}
}
