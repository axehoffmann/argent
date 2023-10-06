#pragma once

#include <string>
#include <atomic>

namespace ag
{
	class Resource
	{

	};

	template <class R>
	concept Resource_T = std::is_base_of_v<Resource, R>;

	/**
	* Describes a resource's location
	*/
	template <Resource_T T>
	struct resrc_specifier
	{
		resrc_specifier(std::string fpath)
		{
			path = fpath;
		}

	private:
		std::string path;
	};

	template <Resource_T T>
	struct resrc_handle
	{
		bool is_loaded() const
		{
			return (*status) > 0;
		}

	private:
		/**
		* = 0 if unloaded
		* > 0 otherwise
		*/
		std::atomic<uint8_t>* status;

		T* data;
	};
}