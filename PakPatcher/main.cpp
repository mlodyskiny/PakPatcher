#include <iostream>

#include "CException.h"
#include "CFile.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 4)
		{
			std::cout << "Usage: PakPatcher.exe [.pak file path] [old asset path] [new asset path]\n";
			return 1;
		}

		auto PakFile = CFile(argv[1]);

		auto OriginalAsset = CFile(argv[2]);
		auto NewAsset = CFile(argv[3]);

		PakFile.Replace(OriginalAsset.ReadAsBytes(), NewAsset.ReadAsBytes());
		
		std::cout << std::format(
			"Replaced {} with {} in {} successfully!\n",
			OriginalAsset.GetNameA(),
			NewAsset.GetNameA(),
			PakFile.GetNameA()
		);
	}
	catch (const CException& Exception)
	{
		std::cout << std::format(
			"An internal exception has occured:\n{}\n",
			Exception.what()
		);

		return -1;
	}
	catch (const std::exception& Exception)
	{
		std::cout << std::format(
			"A standard exception has occured:\n{}\n",
			Exception.what()
		);

		return -2;
	}
	catch (...)
	{
		std::cout << "An unknown exception has occured!\n";
		
		return -3;
	}

	return 0;
}