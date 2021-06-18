#include <fstream>
#include "DumbYamlReader.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void DumbYamlReader::processToken(const std::string& token) {
        if (token.empty())
            throw std::runtime_error("DumbYamlReader does not support empty tokens");

        if (token.find('#') != std::string::npos)
            throw std::runtime_error("DumbYamlReader does not support comments");

        if (token[token.length() - 1] == ':') {
            // This token is a KEY.
            if (state == DumbYamlParseState::EXPECTING_KEY) {
                currentKey = token.substr(0, token.length() - 1);
                state = DumbYamlParseState::EXPECTING_VALUE;
            } else
                throw std::runtime_error("unexpected token: VALUE expected, KEY received");
        } else {
            // This token is a VALUE.
            if (state == DumbYamlParseState::EXPECTING_VALUE) {
                nodes[currentKey] = token;
                state = DumbYamlParseState::EXPECTING_KEY;
            } else
                throw std::runtime_error("unexpected token: KEY expected, VALUE received");
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    DumbYamlReader::DumbYamlReader(const std::string& file) {
        try {
            std::ifstream fileStream(file);
            std::string token;

            while (fileStream >> token)
                processToken(token);

            if (state == DumbYamlParseState::EXPECTING_VALUE)
                throw std::runtime_error("unexpected end of file: VALUE expected for KEY: " + currentKey);
        } catch (const std::ifstream::failure& ex) {
            throw std::runtime_error("failed to read file (" + std::string(ex.what()) + "): " + file);
        }
    }

    std::string DumbYamlReader::getString(const std::string& key) const {
        auto cursor = nodes.find(key);

        if (cursor == nodes.cend())
            throw std::runtime_error("no such node with KEY: " + key);

        return cursor->second;
    }

    int32_t DumbYamlReader::getSignedInt32(const std::string& key) const {
        std::string str = getString(key);

        try {
            return std::stoi(str);
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a signed int32: " + key);
        } catch (const std::out_of_range& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a signed int32: " + key);
        }
    }

    uint32_t DumbYamlReader::getUnsignedInt32(const std::string& key) const {
        int32_t sint32 = getSignedInt32(key);

        if (sint32 >= 0)
            return (uint32_t) sint32;
        else
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not an unsigned int32: " + key);
    }

    int64_t DumbYamlReader::getSignedInt64(const std::string& key) const {
        std::string str = getString(key);

        try {
            return std::stoll(str);
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a signed int64: " + key);
        } catch (const std::out_of_range& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a signed int64: " + key);
        }
    }

    uint64_t DumbYamlReader::getUnsignedInt64(const std::string& key) const {
        int64_t sint64 = getSignedInt64(key);

        if (sint64 >= 0)
            return (uint64_t) sint64;
        else
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not an unsigned int64: " + key);
    }

    float DumbYamlReader::getFloat(const std::string& key) const {
        std::string str = getString(key);

        try {
            return std::stof(str);
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a float: " + key);
        } catch (const std::out_of_range& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a float: " + key);
        }
    }

    double DumbYamlReader::getDouble(const std::string& key) const {
        std::string str = getString(key);

        try {
            return std::stod(str);
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a double: " + key);
        } catch (const std::out_of_range& ex) {
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a double: " + key);
        }
    }

    bool DumbYamlReader::getBoolean(const std::string& key) const {
        std::string str = getString(key);

        if (str == "true")
            return true;
        else if (str == "false")
            return false;
        else
            throw std::runtime_error(
                    "node VALUE corresponding to the following KEY is not a boolean: " + key);
    }

}
