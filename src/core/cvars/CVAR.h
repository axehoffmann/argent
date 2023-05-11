#pragma once

#include <atomic>
#include <string>

namespace ag
{
	namespace cvars 
	{
		struct ICVAR
		{
			size_t type;
			std::string name;
			std::string desc;


			/**
			* Allocates a unique ID for each type of CVAR registered with this function.
			* @tparam The type of CVAR
			* @return The ID of the CVAR type
			*/
			template <typename T>
			static size_t GetID()
			{
				// Generates a unique ID for each type
				// trick from @nice_byte
				static size_t id = ++nextCVARTypeID;
				return id;
			}

			virtual void Reset() = 0;

		private:
			static std::atomic<size_t> nextCVARTypeID;
		};

		template <typename T>
		struct CVAR : public ICVAR
		{
			T data;
			T defaultVal;

			CVAR(std::string n, std::string d, T def)
			{
				name = n;
				desc = d;
				defaultVal = def;
				data = def;

				type = GetID<T>();
			}

			void Reset() override
			{
				data = defaultVal;
			}
		};
	}
}