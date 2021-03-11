#include "utils/general.hpp"
#include "utils/logger.hpp"

#define ERROR(_EXPR) mnt::Logger::Error(_EXPR);
#define WARN(_EXPR) mnt::Logger::Warn(_EXPR);
#define INFO(_EXPR) mnt::Logger::Info(_EXPR);
#define DEBUG(_EXPR) mnt::Logger::Debug(_EXPR);

