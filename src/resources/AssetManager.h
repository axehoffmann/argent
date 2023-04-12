#pragma once

#include <type_traits>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include "Resource.h"

#include <iostream>

namespace ag
{
	class AssetManager 
	{
		/// TODO: have all assets use a raw binary format for fast loading
		///		see: https://vkguide.dev/docs/extra-chapter/asset_system/
	public:
		template <class T>
		static std::weak_ptr<T> Fetch(uint32_t id)
		{
			// Ensure we are fetching a resource type
			static_assert(std::is_base_of<ag::Resource, T>::value);

			std::weak_ptr<T> resource = std::dynamic_pointer_cast<T>(resources.at(id));

			return resource;
		}

		template <class T>
		static uint32_t Load(std::string path)
		{
			// Ensure we are loading a resource type
			static_assert(std::is_base_of<ag::Resource, T>::value);

			// Don't load the same file again, return already loaded asset id
			if (pathToID.find(path) != pathToID.end())
				return pathToID[path];

			uint32_t idx = resources.size();

			std::shared_ptr<ag::Resource> ptr = std::make_shared<T>(path);

			/// TODO: asynchronously load data
			ptr->Load();

			resources.push_back(ptr);
			pathToID[path] = idx;

			return idx;
		}

		static void UnloadAll()
		{
			for (size_t i = 0; i < resources.size(); i++)
			{
				resources[i]->Unload();
			}
			std::vector<std::shared_ptr<ag::Resource>>().swap(resources);
			pathToID.clear();
		}

	private:
		static std::vector<std::shared_ptr<ag::Resource>> resources;
		static std::unordered_map<std::string, uint32_t> pathToID;
	};
}