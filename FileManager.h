#pragma once
#include <vector>
#include <string>
#include <filesystem>

class FileManager
{
public:
	FileManager();
	~FileManager();

	std::vector<std::string> Discover();
	bool InstallMods(std::vector<std::string> vModList, std::string& errors, std::vector<int>& vErrorIndices);
	std::vector<std::string> GetInstalledList();
private:
	void VerifyFileStructure();
	void ClearMods();
	bool Mount(std::string sModName, std::string& errorCode);

	std::filesystem::path cd;
};

