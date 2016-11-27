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
	enum class Datatype : uint8_t 
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
		SleepStage    = 0x9d
	};

	static std::string datatypeStr(Datatype type);

	uint8_t checksum;
	bool checksumCorrect;
	double timestamp;
	uint8_t sequenceNumber;
	Datatype datatype;
	
	std::vector<uint8_t> rawData;

	std::string str() const;
};



}
}
