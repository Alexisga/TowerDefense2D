#pragma once
#include "GameTypes.h"
#include <filesystem>
class CsvLogger {
public:
    explicit CsvLogger(std::filesystem::path path="data/eventos.csv");
    bool Write(const LogEvent& event);
    const std::string& Error() const { return error_; }
    static std::string Escape(const std::string& value);
    static std::string NewMatchId();
    static std::string Timestamp();
    static const char* Header();
private:
    std::filesystem::path path_;
    std::string error_;
};
