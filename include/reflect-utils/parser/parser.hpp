#pragma once

#include "error.hpp"
#include "rfl/json.hpp"
#include "rfl.hpp"
#include "rfl/Processors.hpp"

namespace Parser{

std::vector<std::string> splitErrors(const std::string& errorMessage) {
    std::vector<std::string> errors;
    std::istringstream stream(errorMessage);
    std::string line;

    while (std::getline(stream, line, '\n')) {
        if (!line.empty()) {
            errors.push_back(line);
        }
    }

    if (errors.size() > 1){
        return std::vector<std::string>(errors.begin() + 1, errors.end());
    }
    return errors;
}

template <typename T>
std::pair<T*, std::vector<std::string>> Read(const std::string& jsonStr) {
    try {
        auto parsedResult = rfl::json::read<T, rfl::NoExtraFields>(jsonStr);
        return {new T(parsedResult.value()), {}};  // Return a new object on success
    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        auto errStrs = splitErrors(errMsg);
        std::vector<std::string> errs;
        for (auto errStr : errStrs){
            auto errSt = mapErrors(errStr);
            errs.push_back(errSt);
        }

        return {nullptr, errs};
    }
}

}