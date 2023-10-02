#pragma once

#include <string>
#include <exception>

namespace json
{
	class ParserError : public std::exception
	{
	private:
		std::string _error;

	public:
		ParserError(const std::string& message, unsigned line, unsigned column):
			_error("stdin:" + std::to_string(line) + ':' + std::to_string(column) + ": error: " + message) {}
	
		const char* what() const noexcept override
		{
			return _error.c_str();
		}
	};
}
