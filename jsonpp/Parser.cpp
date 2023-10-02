#include <string>
#include <iostream>
#include <istream>
#include <sstream>

#include "Parser.h"

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

	void Parser::parse_object(Node& node)
	{
		++_pos;
		Node::Object obj;
		std::string key;
		Node value;

		skip_whitespace();

		if (*_pos == '\0')
		{
			std::cout << "Expected closing bracket, received EOF instead in line "
				<< _current_line
				<< " at column "
				<< _current_column
				<< "." << std::endl;
			return;
		}

		while (*_pos)
		{
			if (*_pos == '}')
			{
				break;
			}
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
			std::cout << "Unexpected EOF found" << std::endl;
			return;
		}

		switch (*_pos)
		{
		case '{':
			parse_object(node);
			break;
		default:
			std::cout << "Unexpected token found at line "
				<< _current_line
				<< " in column "
				<< _current_column
				<< "." << std::endl;
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
