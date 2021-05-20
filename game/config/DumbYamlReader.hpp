#pragma once


#include <map>

namespace awd::game {

    enum class DumbYamlParseState {
        EXPECTING_KEY,
        EXPECTING_VALUE
    };

    class DumbYamlReader {
    private:
        std::map<std::string, std::string> nodes;
        DumbYamlParseState state = DumbYamlParseState::EXPECTING_KEY;
        std::string currentKey;

        void processToken(const std::string& token);

    public:
        explicit DumbYamlReader(const std::string& file);

        std::string getString       (const std::string& key) const;
        int32_t     getSignedInt32  (const std::string& key) const;
        uint32_t    getUnsignedInt32(const std::string& key) const;
        int64_t     getSignedInt64  (const std::string& key) const;
        uint64_t    getUnsignedInt64(const std::string& key) const;
        float       getFloat        (const std::string& key) const;
        double      getDouble       (const std::string& key) const;
        bool        getBoolean      (const std::string& key) const;
    };

}

