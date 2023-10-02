#include <iostream>

#include "Parser.h"

int main()
{
	json::Node node;
	json::parse("{}\0", node);
	json::parse("{", node); // will output error
}
