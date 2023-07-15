#include "Parser.cpp"

int main(void) {
	Parser parse;

	parse.takeInput("join");
	parse.findCmd();
	parse.printCmd();
	return 0;
}