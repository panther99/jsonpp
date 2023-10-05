#include <iostream>

#include "Parser.h"
#include "Error.h"

int main()
{
	json::Node node;

	try
	{
		json::parse("{ \"name\" : \"Nikola\", \"available\": true }\0", node);
		json::parse("{", node); // will output error
	}
	catch (json::ParserError& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
