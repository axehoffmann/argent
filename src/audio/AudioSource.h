#pragma once

#include <nlohmann/json.hpp>
#include "../ecs/Component.h"
#include "../resources/AssetManager.h"

namespace ag
{
	struct AudioSource
	{
		float volume;
		float pitch;
		float distance;

		uint32_t audioClipID;

		AudioSource(float v, float p, float d) : volume(v), pitch(p), distance(d)
		{

		}

		static nlohmann::json ToJSON(AudioSource a)
		{
			using json = nlohmann::json;
			json ob = {
				{ "volume", a.volume },
				{ "pitch", a.pitch },
				{ "distance", a.distance }
				/// TODO: this doesn't re-serialise the paths of the resources
			};
		}

		static AudioSource FromJSON(nlohmann::json& ob)
		{
			AudioSource a(ob["volume"], ob["pitch"], ob["distance"]);
			// a.audioClipID = ag::AssetManager::Load
			return a;
		}

	private:
		static ag::Component::Serialiser<AudioSource> serialiser;
	};
}