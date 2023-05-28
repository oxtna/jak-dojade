#pragma once

struct Char
{
	static char Lowercase(char c);
	static char Uppercase(char c);
	static bool IsDigit(char c);
	static bool IsLetter(char c);
	static bool IsAlnum(char c);
	static bool IsWhitespace(char c);
};
