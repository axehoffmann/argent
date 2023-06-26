#pragma once

#include "ECSTypes.h"
#include <atomic>
#include <unordered_map>

#include <nlohmann/json.hpp>
namespace ag
{

	class ComponentInfo
	{
	public:
		/**
		* Allocates a unique ID for each type of component registered with this function.
		* @tparam The type of the component
		* @return The ID of the component type
		*/
		template <typename T>
		static ComponentTypeID GetID()
		{
			// Generates a unique ID for each type
			// trick from @nice_byte
			static ComponentTypeID id = ++nextComponentID;
			componentSize[id] = sizeof(T);
			return id;
		}
		static int GetSize(ComponentTypeID id);

	private:
		inline static std::atomic<ComponentTypeID> nextComponentID{0};
		/// TODO: Evaluate if this map is necessary.
		inline static std::unordered_map<ComponentTypeID, int> componentSize{};
	};

	/**
	* Can store any type of data to be used as a component
	*/
	class Component
	{
	public:
		/**
		* Casts the component's data back to its type.
		* Note that this expects the template parameter to match its true type.
		*/
		template <typename T>
		T GetData()
		{
			if (ComponentInfo::GetID<T>() != id)
			{
				/// TODO: Throw error
			}
			return *static_cast<T*>(data);
		}

		/**
		* Gets the type ID of a component instance
		*/
		ComponentTypeID GetID()
		{
			return id;
		}

		static Component FromJSON(nlohmann::json data)
		{
			return Serialisers()[data["type"]]->FromJSON(data);
		}

		int Size()
		{
			return ComponentInfo::GetSize(id);
		}

		class ISerialiser
		{
		public:
			virtual nlohmann::json ToJSON(Component& c) = 0;
			virtual Component FromJSON(nlohmann::json ob) = 0;
		};

		template <typename T>
		class Serialiser : public ISerialiser
		{
		public:
			Serialiser(std::string componentName)
			{
				Serialisers()[componentName] = this;
			}

			nlohmann::json ToJSON(Component& c) override
			{
				return T::ToJSON(c.GetData<T>());
			}
			Component FromJSON(nlohmann::json ob) override
			{
				return Create<T>(T::FromJSON(ob));
			}
		};

	private:
		static std::map<std::string, ISerialiser*>& Serialisers()
		{
			static std::map<std::string, ISerialiser*> s;
			return s;
		}

		template <typename T>
		static Component Create(T data)
		{
			/// TODO: Maybe memory leak? clean up raw data? Consider lifetime of component object
			Component c;
			T* raw = new T(data);
			c.data = (void*)raw;
			c.id = ComponentInfo::GetID<T>();
			return c;
		}

		ComponentTypeID id;
		void* data;
	};
}

