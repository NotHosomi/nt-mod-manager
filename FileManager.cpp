#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include <fstream>
namespace fs = std::filesystem;

FileManager::FileManager()
{
    cd = fs::current_path();
    VerifyFileStructure();
}

FileManager::~FileManager()
{
}

void FileManager::VerifyFileStructure()
{
    if (!fs::exists("/mods/"))
    {
        fs::create_directory("./mods");
    }
    if (!fs::exists("./mods/.base/"))
    {
        fs::create_directory("./mods/.base/");
    }
    if (!fs::exists("./mods/.base/models/"))
    {
        fs::create_directory("./mods/.base/models/");
    }
    if (!fs::exists("./mods/.base/materials/"))
    {
        fs::create_directory("./mods/.base/materials/");
    }
    if (!fs::exists("./mods/.base/sound/"))
    {
        fs::create_directory("./mods/.base/sound/");
    }
}

// todo: use callback to update a progress bar
bool FileManager::InstallMods(std::vector<std::string> sModList, std::string& errors, std::vector<int>& vErrorIndices)
{
    ClearMods();
    // go backwards thru the list, to give higher prio based on order
    std::ofstream modListFile("./mods/mounted.dat", std::ios::trunc);
    for (int i = sModList.size() - 1; i > -1; --i)
    {
        std::string ec;
        if (Mount(sModList[i], ec))
        {
            // Add mod to installed list
            std::string line = sModList[i] + "\n";
            modListFile.write(line.c_str(), line.length());
        }
        else
        {
            // Append error message
            errors += ec + "\n";
            vErrorIndices.push_back(i);
        }
    }
    modListFile.close();
    return vErrorIndices.size() == 0;
}

std::vector<std::string> FileManager::GetInstalledList()
{
    std::vector<std::string> output;
    std::ifstream modListFile("./mods/mounted.dat", std::ios::in);
    if (!modListFile)
        return output;
    std::string line;
    while (std::getline(modListFile, line))
    {
        output.insert(output.begin(), line);
    }
    return output;
}

void FileManager::ClearMods()
{
    // Copy /.base/ into /NeotokyoSource/
    fs::copy("./mods/.base", "../NeotokyoSource", fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    // Clear the tracker file
    std::ofstream modListFile("./mods/mounted.dat", std::ios::trunc);
    modListFile.close();
}

bool FileManager::Mount(std::string sModName, std::string& sErrorMsg)
{
    // Backup defaults
    fs::path mod_dir("./mods/" + sModName);
    if (!fs::exists(fs::path("../NeotokyoSource")))
    {
        sErrorMsg = "Could not find game directory";
        return false;
    }
    if (!fs::exists(mod_dir))
    {
        fs::create_directory(mod_dir);
        sErrorMsg = sModName + ": Mod directory not found";
        return false;
    }
    mod_dir += "/NeotokyoSource/";
    if (!fs::exists(mod_dir))
    {
        sErrorMsg = sModName + ": Incompatible File Structure";
        return false;
    }
    for (const auto& iter : fs::recursive_directory_iterator(mod_dir))
    {
        fs::path slug = fs::relative(iter.path(), mod_dir);
        if (iter.is_directory())
        {
            // verify the backup dest exists
            fs::path backupDir("./mods/.base/");
            backupDir += slug;
            if (!fs::exists(backupDir))
                fs::create_directory(backupDir);
            continue;
        }

        // Backup vanilla files
        fs::path backupSrc("../NeotokyoSource/");
        backupSrc += slug;
        if (!fs::exists(backupSrc))
        {
            sErrorMsg = sModName + ": trying to replace a file that doesn't exist"; // maybe this should be a warning not an error
            return false;
        }
        fs::path backupDest("./mods/.base/");
        backupDest += slug;
        bool a = fs::exists(backupSrc);
        bool b = fs::exists(backupDest);
        try
        {
            fs::copy(backupSrc, backupDest, fs::copy_options::skip_existing | fs::copy_options::recursive);
        }
        catch (std::exception e)
        {
            std::string aaaa = e.what();
        }
    }
    // mount
    fs::copy(mod_dir, "../NeotokyoSource/", fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    return true;
}

std::vector<std::string> FileManager::Discover()
{
    std::vector<std::string> list;
    fs::path dir("./mods/");
    for (auto& p : fs::directory_iterator(dir))
    {
        if (!p.is_directory())
            continue;
        std::string sModName = fs::relative(p, dir).string();
        if (sModName[0] == '.')
            continue;
        list.push_back(sModName);
    }
    return list;
}
