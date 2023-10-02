#pragma once

#include <map>
#include <string>

namespace json
{
	class Node
	{
	public:
		using Object = std::map<std::string, Node>;

	private:
		enum class StorageType
		{
			Null,
			Object
		};

		union storage_t {
			Object* object;
		} _storage;
		StorageType _storage_type;

	public:
		Node() : _storage { nullptr }, _storage_type(StorageType::Null) {}

		Node(const Object& object) : _storage_type(StorageType::Object)
		{
			_storage.object = new Object(object);
		}

	};
}
