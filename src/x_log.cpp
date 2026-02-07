//
// Created by rui ding on 2025/9/12.
//

#include "x_log.h"

#include <memory>

std::shared_ptr<spdlog::logger> XLog::logger;

void XLog::Init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    logger = spdlog::stdout_color_mt("X_LOG");
    logger->set_level(spdlog::level::trace);
}
