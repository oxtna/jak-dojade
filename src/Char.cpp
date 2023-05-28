#include "Char.h"

char Char::Lowercase(char c)
{
	return c | 0x20;
}

char Char::Uppercase(char c)
{
	return c & (~0x20);
}

bool Char::IsDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool Char::IsLetter(char c)
{
	return Uppercase(c) >= 'A' && Uppercase(c) <= 'Z';
}

bool Char::IsAlnum(char c)
{
	return IsLetter(c) || IsDigit(c);
}

bool Char::IsWhitespace(char c)
{
	return (
		c == ' ' ||
		c == '\t' ||
		c == '\n' ||
		c == '\r' ||
		c == '\v'
	);
}
