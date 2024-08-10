#include "CFile.h"

#include <filesystem>
#include <fstream>

#include "CException.h"
#include <iostream>

CFile::CFile(const std::wstring& Path)
{
    if (Path.empty())
        throw Exception("Path cannot be empty!");

    const auto LastSlashPos = Path.find_last_of(L"/\\");
    const auto DoubleDotPos = Path.find(L':');

    if (LastSlashPos != std::wstring::npos && DoubleDotPos)
    {
        m_Directory = Path.substr(0, LastSlashPos);
        m_Name = Path.substr(LastSlashPos + 1);
    }
    else
    {
        m_Directory = std::filesystem::current_path().wstring();
        m_Name = Path;
    }
}

CFile::CFile(const std::string& Path) : CFile(std::wstring(Path.begin(), Path.end()))
{
}

CFile::CFile(std::wstring Directory, std::wstring Name) : 
    m_Directory(std::move(Directory)),
    m_Name(std::move(Name))
{
}

std::wstring CFile::GetNameW() const
{
    return m_Name;
}

std::string CFile::GetNameA() const
{
    return std::string(m_Name.begin(), m_Name.end());
}

std::wstring CFile::GetDirectoryW() const
{
    return m_Directory;
}

std::string CFile::GetDirectoryA() const
{
    return std::string(m_Directory.begin(), m_Directory.end());
}

std::wstring CFile::GetPathW() const
{
    return m_Directory.ends_with(L'\\') ? (m_Directory + m_Name) : (m_Directory + L'\\' + m_Name);
}

std::string CFile::GetPathA() const
{
    auto Path = this->GetPathW();
    return std::string(Path.begin(), Path.end());
}

bool CFile::Exists() const
{
    return std::filesystem::exists(this->GetPathW());
}

std::wstring CFile::Read() const
{
    std::wstring Result{};
    auto AllLines = this->ReadAllLines();

    for (const auto& Line : AllLines)
        Result += Line + L'\n';

    return Result;
}

std::vector<std::wstring> CFile::ReadAllLines() const
{
    if (!this->Exists())
        throw Exception("Tried to read from a file that doesn't exist!");

    std::wifstream IFileStream(this->GetPathW());
    if (!IFileStream || !IFileStream.is_open())
        throw Exception("Failed to open the target file!");

    std::vector<std::wstring> Result{};

    std::wstring Line;
    while (std::getline(IFileStream, Line))
        Result.emplace_back(Line);

    IFileStream.close();
    return Result;
}

std::vector<unsigned char> CFile::ReadAsBytes() const
{
    if (!this->Exists())
        throw Exception("Tried to read from a file that doesn't exist!");

    std::ifstream IFileStream(this->GetPathW(), std::ios::binary | std::ios::ate);
    if (!IFileStream || !IFileStream.is_open())
        throw Exception("Failed to open the target file!");

    auto FileSize = IFileStream.tellg();
    IFileStream.seekg(0, std::ios::beg);

    std::vector<unsigned char> Result(FileSize);

    if (!IFileStream.read(reinterpret_cast<char*>(Result.data()), FileSize))
        throw Exception("Failed to read the target file!");

    IFileStream.close();
    return Result;
}

void CFile::WriteBytes(const std::vector<unsigned char>& Bytes)
{
    if (!this->Exists())
        throw Exception("Tried to write to a file that doesn't exist!");

    std::ofstream OFileStream(this->GetPathW(), std::ios::binary | std::ios::trunc);
    if (!OFileStream || !OFileStream.is_open())
        throw Exception("Failed to open the target file!");

    OFileStream.write(reinterpret_cast<const char*>(Bytes.data()), Bytes.size());

    if (!OFileStream.good())
        throw Exception("Failed to write data to the target file!");

    OFileStream.close();
}

void CFile::Replace(const std::vector<unsigned char>& From, const std::vector<unsigned char>& To, bool bFirstMatchOnly)
{
    if (From.size() == 0)
        throw Exception("\"From\" array cannot be empty!");

    if (To.size() == 0)
        throw Exception("\"To\" array cannot be empty!");

    if (From.size() != To.size())
        throw Exception("\"From\" and \"To\" arrays must be the same size!");

    if (!this->Exists())
        throw Exception("Tried to read and write to a file that doesn't exist!");

    auto Bytes = this->ReadAsBytes();
    if (Bytes.size() == 0)
        throw Exception("Target file is empty!");

    for (size_t i = 0; i <= Bytes.size() - From.size();)
    {
        if (std::equal(From.begin(), From.end(), Bytes.begin() + i))
        {
            std::copy(To.begin(), To.end(), Bytes.begin() + i);
            i += From.size();

            if (bFirstMatchOnly)
                break;
        }
        else ++i;
    }

    this->WriteBytes(Bytes);
}
