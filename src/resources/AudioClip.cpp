#include "AudioClip.h"

bool ag::AudioClip::IsReady() const
{
	return data.size() > 0;
}

void ag::AudioClip::Load()
{
	/// TODO: load audio files
}

void ag::AudioClip::Unload()
{
	std::vector<uint16_t>().swap(data);
}
