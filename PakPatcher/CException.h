#pragma once

#include <exception>
#include <string>
#include <format>

// used to sort out exceptions between our shit and std shit
class CException : public std::exception
{
public:
	CException(const std::string& Message) : std::exception(Message.c_str()) {}
};

#define Exception(Message) CException(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " - " + Message)
