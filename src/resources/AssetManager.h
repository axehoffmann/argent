#pragma once

#include <type_traits>
#include <unordered_map>
#include <string>
#include <memory>

#include "Resource.h"

#include "lib/vector.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>

namespace ag
{
	/// TODO: change system, we don't need integer ids, can just directly have weak_ptrs
	///		Although, we want to sort scene graph by model/material for instanced 
	///		rendering which benefits from having IDs. IDK
	class AssetManager 
	{
		/// TODO: have all assets use a raw binary format for fast loading
		///		see: https://vkguide.dev/docs/extra-chapter/asset_system/
	public:
		/**
		* Fetches a resource's data by ID.
		* @param id The ID of the resource to fetch
		* @return A pointer to the resource's data
		*/
		template <class T>
		static std::weak_ptr<T> Fetch(uint32_t id)
		{
			// Ensure we are fetching a resource type
			static_assert(std::is_base_of<ag::Resource, T>::value);

			std::weak_ptr<T> resource = std::dynamic_pointer_cast<T>(resources.at(id));

			return resource;
		}	

		/**
		* Loads a resource into memory.
		* @param path The path to the resource.
		* @tparam T The type of resource to load.
		* @return The ID of the loaded resource
		*/
		template <class T>
		static uint32_t Load(std::string path)
		{
			// Ensure we are loading a resource type
			static_assert(std::is_base_of<ag::Resource, T>::value);

			// Don't load the same file again, return already loaded asset id
			if (pathToID.find(path) != pathToID.end())
				return pathToID[path];


			std::shared_ptr<ag::Resource> ptr = std::make_shared<T>(path);
			

			/// TODO: asynchronously load data
			ptr->Load();

			uint32_t idx = static_cast<uint32_t>(resources.size());
			resources.push_back(ptr);
			pathToID[path] = idx;

			return idx;
		}

		/**
		* Unloads data and drops all Resource pointers. Clean slate.
		*/
		static void DropAll()
		{
			for (size_t i = 0; i < resources.size(); i++)
			{
				resources[i]->Unload();
			}
			vector<std::shared_ptr<ag::Resource>>().swap(resources);
			pathToID.clear();
		}

		static nlohmann::json ReadJson(std::string& path)
		{
			std::ifstream f(path);
			return nlohmann::json::parse(f);
		}

	private:
		static vector<std::shared_ptr<ag::Resource>> resources;
		static std::unordered_map<std::string, uint32_t> pathToID;
	};
}