#pragma once

#include <map>
#include <string>
#include <variant>

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
			Object,
			String,
			Bool,
			Double
		};

		std::variant<
			std::nullptr_t,
			std::shared_ptr<Object>,
			std::shared_ptr<std::string>,
			bool,
			double
		> _storage;
		StorageType _storage_type;

	public:
		Node() : _storage { nullptr }, _storage_type(StorageType::Null) {}

		Node(const Object& object) : _storage_type(StorageType::Object)
		{
			_storage = std::make_shared<Object>(object);
		}

		Node(const std::string& str) : _storage_type(StorageType::String)
		{
			_storage = std::make_shared<std::string>(str);
		}

		Node(bool boolean) : _storage_type(StorageType::Bool)
		{
			_storage = boolean;
		}

		Node(double number) : _storage_type(StorageType::Double)
		{
			_storage = number;
		}

	};
}
