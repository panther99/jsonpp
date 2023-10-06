#pragma once

#include <map>
#include <vector>
#include <string>
#include <variant>
#include <memory>

namespace json
{
	class Node
	{
	public:
		using Object = std::map<std::string, Node>;
		using Array = std::vector<Node>;

	private:
		enum class StorageType
		{
			Null,
			Object,
			Array,
			String,
			Bool,
			Double
		};

		std::variant<
			std::nullptr_t,
			std::shared_ptr<Object>,
			std::shared_ptr<Array>,
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

		Node(const Array& array) : _storage_type(StorageType::Array)
		{
			_storage = std::make_shared<Array>(array);
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
