#include "Utils.h"

void FatalError(wstring msg)
{
	MessageBox(NULL, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
	exit(0);
}

int DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories)
{
	bool            bSubdirectory = false;       // Flag, indicating whether
												 // subdirectories have been found
	HANDLE          hFile;                       // Handle to directory
	std::string     strFilePath;                 // Filepath
	std::string     strPattern;                  // Pattern
	WIN32_FIND_DATAA FileInformation;             // File information


	strPattern = refcstrRootDirectory + "\\*.*";
	hFile = FindFirstFileA(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FileInformation.cFileName[0] != '.')
			{
				strFilePath.erase();
				strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (bDeleteSubdirectories)
					{
						// Delete subdirectory
						int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
						if (iRC)
							return iRC;
					}
					else
						bSubdirectory = true;
				}
				else
				{
					// Set file attributes
					if (::SetFileAttributesA(strFilePath.c_str(),
						FILE_ATTRIBUTE_NORMAL) == FALSE)
						return ::GetLastError();

					// Delete file
					if (::DeleteFileA(strFilePath.c_str()) == FALSE)
						return ::GetLastError();
				}
			}
		} while (::FindNextFileA(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);

		DWORD dwError = ::GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
			return dwError;
		else
		{
			if (!bSubdirectory)
			{
				// Set directory attributes
				if (::SetFileAttributesA(refcstrRootDirectory.c_str(),
					FILE_ATTRIBUTE_NORMAL) == FALSE)
					return ::GetLastError();

				// Delete directory
				if (::RemoveDirectoryA(refcstrRootDirectory.c_str()) == FALSE)
					return ::GetLastError();
			}
		}
	}

	return 0;
}

char Hex(unsigned int n)
{
	if (n < 10)
		return '0' + n;
	else
		return 'A' + n - 10;
}

unsigned int Dec(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else
		return c - 'A' + 10;
}

string BinToHex(string str)
{
	string result;
	for (int i = 0; i < str.length(); i++)
	{
		result.push_back(Hex((unsigned char)str[i] >> 4));
		result.push_back(Hex((unsigned char)str[i] & 15));
	}
	return result;
}

string HexToBin(string str)
{
	string result;
	for (int i = 0; i < str.length(); i += 2)
	{
		result.push_back((Dec(str[i])) << 4 | Dec(str[i + 1]));
	}
	return result;
}

string Hash(string str)
{
	string hash(picosha2::k_digest_size, 0);
	picosha2::hash256(str.begin(), str.end(), hash.begin(), hash.end());
	return hash;
}

bool IsUrlChar(char c)
{
	return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9')) || (c == '-') || (c == '.') || (c == '_') || (c == '~');
}

string UrlEncode(string str)
{
	string result;
	for (int i = 0; i < str.length(); i++)
	{
		if (IsUrlChar(str[i]))
		{
			result.push_back(str[i]);
		}
		else
		{
			result.push_back('%');
			result.push_back(Hex((unsigned char)str[i] >> 4));
			result.push_back(Hex((unsigned char)str[i] & 15));
		}
	}
	return result;
}

string UrlDecode(string str)
{
	string result;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] != '%')
		{
			result.push_back(str[i]);
		}
		else
		{
			result.push_back((Dec(str[i + 1])) << 4 | Dec(str[i + 2]));
			i += 2;
		}
	}
	return result;
}