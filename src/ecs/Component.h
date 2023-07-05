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
		static std::atomic<ComponentTypeID> nextComponentID;
		/// TODO: Evaluate if this map is necessary.
		static std::unordered_map<ComponentTypeID, int> componentSize;
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
			return static_cast<RawData<T>*>(data.get())->value;
		}

		void* GetRawData()
		{
			return dataRaw;
		}

		/**
		* Gets the type ID of a component instance
		*/
		ComponentTypeID GetID()
		{
			return id;
		}

		static Component FromJSON(nlohmann::json ob)
		{
			return Serialisers()[ob["type"]]->FromJSON(ob);
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
			RawData<T>* wrapper = new RawData<T>(data);
			std::unique_ptr<IRawData> genericWrapper = std::unique_ptr<IRawData>(wrapper);
			return ag::Component(ComponentInfo::GetID<T>(), std::move(genericWrapper), (void*)&wrapper->value);
		}

		/**
		* Type-erased component data handler
		*/
		struct IRawData{};
		template <typename T>
		struct RawData : public IRawData
		{
			RawData(T v) : value(v) { }
			T value;
		};

		ComponentTypeID id;
		std::unique_ptr<IRawData> data;
		void* dataRaw;

		Component(ComponentTypeID i, std::unique_ptr<IRawData> d, void* raw) : id(i), data(std::move(d)), dataRaw(raw) {}
	};
}

