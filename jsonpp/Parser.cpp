#include <string>
#include <iostream>
#include <istream>
#include <sstream>

#include "Parser.h"
#include "Error.h"

namespace json
{
	void Parser::skip_whitespace()
	{
		while (*_pos != '\0' && is_space(*_pos))
		{
			if (*_pos == Whitespace::LineFeed)
			{
				++_current_line;
				_current_column = 0;
			}
			++_pos;
			++_current_column;
		}
	}

	template<typename Node>
	void Parser::parse_string(Node& node)
	{
		const char* copy = _pos + 1;

		if (*copy == '\0')
		{
			throw ParserError("Expected string, received EOF instead", _current_line, _current_column);
		}

		std::string result;

		while (1)
		{
			++_current_column;

			if (*copy == '"')
			{
				break;
			}

			if (*copy != '\\')
			{
				result.push_back(*copy++);
				continue;
			}

			++copy;

			switch (*copy)
			{
			case '/':
				result.push_back('/');
				break;
			case '\\':
				result.push_back('\\');
				break;
			case '"':
				result.push_back('\"');
				break;
			case 'f':
				result.push_back('\f');
				break;
			case 'n':
				result.push_back('\n');
				break;
			case 'r':
				result.push_back('\r');
				break;
			case 't':
				result.push_back('\t');
				break;
			case 'b':
				result.push_back('\b');
				break;
			default:
				throw ParserError("Invalid escape character found", _current_line, _current_column);
			}
		}

		node = result;
		++copy;
		_pos = copy;
	}

	void Parser::parse_object(Node& node)
	{
		++_pos;
		Node::Object obj;
		std::string key;
		Node value;

		skip_whitespace();

		if (*_pos == '\0')
		{
			throw ParserError("Expected closing bracket, received EOF instead", _current_line, _current_column);
		}

		while (*_pos)
		{
			if (*_pos == '}')
			{
				break;
			}

			if (*_pos != '"')
			{
				throw ParserError("Expected string as a key for the object not received", _current_line, _current_column);
			}

			parse_string(key);

			skip_whitespace();

			if (*_pos != ':')
			{
				throw ParserError("Missing colon", _current_line, _current_column);
			}
			++_pos;

			skip_whitespace();

			parse_node(value);

			if (*_pos != ',' && *_pos != '}')
			{
				throw ParserError("Missing comma", _current_line, _current_column);
			}
			else if (*_pos == ',')
			{
				++_pos;
			}

			obj.emplace(key, value);
		}

		node = obj;
		if (*_pos == '}')
		{
			++_pos;
		}
	}

	void Parser::parse_node(Node& node)
	{
		skip_whitespace();

		if (*_pos == '\0')
		{
			throw ParserError("Unexpected EOF found", _current_line, _current_column);
			return;
		}

		switch (*_pos)
		{
		case '{':
			parse_object(node);
			break;
		case '"':
			parse_string(node);
			break;
		default:
			throw ParserError("Unexpected token found", _current_line, _current_column);
		}

		skip_whitespace();
	}

	void Parser::parse(Node& node)
	{
		parse_node(node);

		if (*_pos != '\0')
		{
			std::cout << "Unexpected token found at line "
				<< _current_line
				<< " in column "
				<< _current_column
				<< "." << std::endl;
		}
	}
}
