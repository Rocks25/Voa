#pragma once

#include <vector>

struct ErrorMessage
{
	char *message;
	int life;
};

class Errors
{
private:
	std::vector<ErrorMessage> messages;
public:
	Errors(void);
	~Errors(void);
	void NewError(char *text);
	void PrintErrors();
};

extern Errors *Error;