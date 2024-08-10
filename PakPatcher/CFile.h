#pragma once

#include <string>
#include <vector>

// using wstring as many people tend to use unicode chars in their folder names xd

class CFile
{
private:
	std::wstring m_Name;
	std::wstring m_Directory;

public:
	CFile(const std::wstring& Path);
	CFile(const std::string& Path);
	CFile(std::wstring Directory, std::wstring Name);

	~CFile() = default;

public:
	std::wstring GetNameW() const;
	std::string GetNameA() const;
	std::wstring GetDirectoryW() const;
	std::string GetDirectoryA() const;
	std::wstring GetPathW() const;
	std::string GetPathA() const;

	bool Exists() const;

public:
	std::wstring Read() const;
	std::vector<std::wstring> ReadAllLines() const;
	std::vector<unsigned char> ReadAsBytes() const;

	void WriteBytes(const std::vector<unsigned char>& Bytes);

	void Replace(const std::vector<unsigned char>& From, const std::vector<unsigned char>& To, bool bFirstMatchOnly = true);
};