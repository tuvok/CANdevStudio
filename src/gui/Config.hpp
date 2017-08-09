#ifndef SRC_COMMON_CONFIG_HPP_
#define SRC_COMMON_CONFIG_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

#include <mutex>
#include <string>

/**
 * This class handles CANdevStudio configuration.
 * Default configuration file is located at ~/.config/CANdevStudio/studio.conf
 */
class Config
{
    Config() = delete;

public:

    /**
     * Read configuration file
     * @return true if reading file succeeded, false otherwise
     */
    static bool init();

    /**
     * Get value for configuration parameter stored at key
     * @param key Key to stored value in format: "category.valueName"
     * @return boost::optional object storing configuration value (if available)
     */
    template<typename T>
    static boost::optional<T> get(const std::string& key);

    /**
     * Get value for configuration parameter stored at key
     * @param key Key to stored value in format: "category.valueName"
     * @param defVal Default value to be returned if there is no value at key
     * @return configuration value
     */
    template<typename T>
    static T get(const std::string& key, T defVal);

    /**
     * Store a value in the application configuration. Method automatically writes configuration file to disk.
     * @param key Key at which new value will be stored. Key format: "category.valueName"
     * @param val Value to be stored in configuration
     */
    template<typename T>
    static void put(const std::string& key, T val);

private:
    static bool initialized;
    static std::mutex treeMutex;
    static boost::property_tree::ptree tree;

    /**
     * Save configuration file to disk
     */
    static void save();

    /**
     * Create configuration directory and file
     */
    static void createConfig();
};

template<typename T>
inline boost::optional<T> Config::get(const std::string& key)
{
    if (!initialized)
        init();
    return tree.get_optional<T>(key);
}

template<typename T>
inline T Config::get(const std::string& key, T defVal)
{
    if (!initialized)
        init();
    return tree.get(key, defVal);
}

template<typename T>
inline void Config::put(const std::string& key, T val)
{
    if (!initialized)
        init();
    tree.put<T>(key, val);

    save();
}

#endif /* SRC_COMMON_CONFIG_HPP_ */
