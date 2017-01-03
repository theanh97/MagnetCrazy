#pragma once
#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;

class MyStringUtils
{
public:
	static string replace(const string& subject, const string& search, const string& replace)
	{
		string str = subject;
		size_t pos = 0;
		while ((pos = str.find(search, pos)) != string::npos)
		{
			str.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return str;
	};
	//----------------------------------------------------------------------
	static int string_to_int(const string &s)
	{
		int i;
		stringstream ss;
		ss << s;
		ss >> i;
		return i;
	};
	//----------------------------------------------------------------------
	static string int_to_string(int i)
	{
		stringstream ss;
		ss << i;
		return ss.str();
	};
	//----------------------------------------------------------------------
	static bool is_numeric_string(const string &str)
	{
		if (!str.empty())
		{
			for (int i = 0, ix = str.length(); i<ix; i++)
			{
				if (str[i] < '0' || '9' < str[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	};
	//----------------------------------------------------------------------
	static string uppercase(const std::string& str)
	{
		string s = str;
		for (size_t i = 0, ix = s.length() - 1; i <= ix; i++)
		{
			if ('a' <= s[i] && s[i] <= 'z')
			{
				s[i] -= 32;
			}
		}
		return s;
	};
	//----------------------------------------------------------------------
	static string lowercase(const std::string& str)
	{
		string s = str;
		for (size_t i = 0, ix = s.length() - 1; i <= ix; i++)
		{
			if ('A' <= s[i] && s[i] <= 'Z')
			{
				s[i] += 32;
			}
		}
		return s;
	};
	//----------------------------------------------------------------------
	static string ltrim(const string& str)
	{
		size_t startpos = str.find_first_not_of(" \t\n\r");
		return string::npos != startpos ? str.substr(startpos) : str;
	};
	//----------------------------------------------------------------------
	static string rtrim(const string& str)
	{
		size_t endpos = str.find_last_not_of(" \t\n\r");
		return string::npos != endpos ? str.substr(0, endpos + 1) : str;
	};
	//----------------------------------------------------------------------
	static string trim(const string& str)
	{
		string s = str;
		s.erase(s.find_last_not_of(" \t\n\r") + 1);
		s.erase(0, s.find_first_not_of(" \t\n\r"));
		return s;
	};
	//----------------------------------------------------------------------
	static bool beginswith(const string& subject, const string& search)
	{
		return subject.substr(0, search.length()) == search;
	};
	//----------------------------------------------------------------------
	static bool endswith(const string& subject, const string& search)
	{
		return subject.substr(subject.length() - search.length(), search.length()) == search;
	};
	//----------------------------------------------------------------------
	static bool contains(const string& subject, const string& search)
	{
		return subject.find(search) != string::npos;
	};
	//----------------------------------------------------------------------
private:
	MyStringUtils() {};//static class

};