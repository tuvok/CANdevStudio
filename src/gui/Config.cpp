#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <Config.hpp>
#include <log.hpp>

#include <iostream>

#include <cstdlib>

bool Config::initialized = false;
std::mutex Config::treeMutex;
boost::property_tree::ptree Config::tree;

static const boost::filesystem::path filename = std::string(std::getenv("HOME")) + "/.config/CANdevStudio/studio.conf";

bool Config::init()
{
    if (initialized)
        return initialized;

    {
        std::lock_guard<std::mutex> lock(treeMutex);

        try
        {
            createConfig();
            boost::property_tree::read_ini(filename.c_str(), tree);
        }
        catch (std::exception& e)
        {
            cds_error("Failed to initialize Config: {0}", e.what());
            return initialized;
        }
    }

    return initialized = true;
}

void Config::save()
{
    std::lock_guard<std::mutex> lock(treeMutex);
    try
    {
        boost::property_tree::write_ini(filename.c_str(), tree);
    }
    catch (std::exception& e)
    {
        cds_error("Failed to save configuration");
    }
}

void Config::createConfig()
{
    if (boost::filesystem::exists(filename))
    {
        if (boost::filesystem::is_regular_file(filename))
            return;

        throw std::runtime_error(std::string("Configuration path does not point to regular file: ") + filename.c_str());
    }

    if (!boost::filesystem::create_directories(filename.parent_path()))
        throw std::runtime_error(std::string("Failed to create directory: ") + filename.parent_path().c_str());

    std::fstream fs;
    fs.open(filename.c_str(), std::ios::out);
    fs.close();
}
