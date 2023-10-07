#pragma once

#include <string>
#include <sstream>

#include "Node.h"

namespace json
{
	enum Whitespace
	{
		Tab = 0x09,
		Space = 0x20,
		LineFeed = 0x0A,
		CarriageReturn = 0x0D
	};

	inline bool is_space(char ch)
	{
		switch (ch)
		{
		case Whitespace::Tab:
		case Whitespace::Space:
		case Whitespace::LineFeed:
		case Whitespace::CarriageReturn:
			return true;
		default:
			return false;
		}
	}

	struct Parser
	{
	private:
		unsigned _current_line = 1;
		unsigned _current_column = 1;
		const char* _pos;

		void skip_whitespace();
		void skip_comment();
		void parse_object(Node& node);
		void parse_array(Node& node);
		template<typename Node>
		void parse_string(Node& node);
		void parse_bool(Node& node);
		void parse_null(Node& node);
		void parse_number(Node& node);
		void parse_node(Node& node);

	public:
		Parser(const char* str) : _pos(str) {}

		void parse(Node& node);
	};

	inline void parse(const std::string& str, Node& node)
	{
		Parser p(str.c_str());
		p.parse(node);
	}

	inline void parse(std::istream& in, Node& node)
	{
		if (in)
		{
			std::ostringstream oss;
			oss << in.rdbuf();
			parse(oss.str(), node);
		}
	}
}
