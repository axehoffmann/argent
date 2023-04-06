#pragma once

#include <type_traits>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include "Resource.h"

namespace ag
{
	class AssetManager 
	{
		/// TODO: have all assets use a raw binary format for fast loading
		///		see: https://vkguide.dev/docs/extra-chapter/asset_system/
	public:
		template <class T>
		static std::shared_ptr<T> Fetch(uint32_t id)
		{
			// Ensure we are fetching a resource type
			static_assert(std::is_base_of<ag::Resource, T>::value);

			std::shared_ptr<T> resource = std::dynamic_pointer_cast<T>();
		}

		template <class T>
		static uint32_t Load(std::string path)
		{
			// Ensure we are loading a resource type
			static_assert(std::is_base_of<ag::Resource, T>::value);

			uint32_t idx = resources.size();

			std::shared_ptr<ag::Resource> ptr = std::make_shared<T>(path);

			/// TODO: asynchronously load data
			ptr->Load();

			resources.push_back(ptr);
			pathToID.insert(path, idx);
		}

	private:
		static std::vector<std::shared_ptr<ag::Resource>> resources;
		static std::unordered_map<std::string, uint32_t> pathToID;
	};
}