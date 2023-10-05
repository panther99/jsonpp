#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <cctype>
#include <fmt/core.h>

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
			throw ParserError("Unexpected EOF received, expected string", _current_line, _current_column);
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
				throw ParserError(fmt::format("Unexpected '{}' escape character received", *copy), _current_line, _current_column);
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
			throw ParserError("Unexpected EOF received, expected closing bracket", _current_line, _current_column);
		}

		while (*_pos)
		{
			skip_whitespace();

			if (*_pos == '}')
			{
				break;
			}

			if (*_pos != '"')
			{
				throw ParserError(fmt::format("Unexpected '{}', expected string for the object key", *_pos), _current_line, _current_column);
			}

			parse_string(key);

			skip_whitespace();

			if (*_pos != ':')
			{
				throw ParserError(fmt::format("Unexpected '{}', expected colon", *_pos), _current_line, _current_column);
			}
			++_pos;

			skip_whitespace();

			parse_node(value);

			if (*_pos != ',' && *_pos != '}')
			{
				throw ParserError("Missing comma after the key-value declaration", _current_line, _current_column);
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

	void Parser::parse_bool(Node& node)
	{
		const char* true_kw = "true";
		const char* false_kw = "false";

		std::string result;

		switch (*_pos)
		{
		case 'f':
			while (isalpha(*_pos) && isalpha(*false_kw) && *_pos == *false_kw)
			{
				result.push_back(*false_kw++);
				++_pos;
				continue;
			}
			break;
		case 't':
			while (isalpha(*_pos) && isalpha(*true_kw) && *_pos == *true_kw)
			{
				result.push_back(*true_kw++);
				++_pos;
				continue;
			}
			break;
		}

		++_pos;

		if (result == "true")
		{
			node = true;
			return;
		}
		else if (result == "false")
		{
			node = false;
			return;
		}

		throw ParserError(fmt::format("Unexpected identifier '{}', expected 'true' or 'false'", result), _current_line, _current_column);
	}

	void Parser::parse_node(Node& node)
	{
		skip_whitespace();

		if (*_pos == '\0')
		{
			throw ParserError("Unexpected EOF received", _current_line, _current_column);
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
		case 't':
		case 'f':
			parse_bool(node);
			break;
		default:
			throw ParserError("Unexpected token received", _current_line, _current_column);
		}

		skip_whitespace();
	}

	void Parser::parse(Node& node)
	{
		parse_node(node);

		if (*_pos != '\0')
		{
			throw ParserError("Unexpected token received, expected EOF", _current_line, _current_column);
		}
	}
}
