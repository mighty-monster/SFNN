#include "utils/general.hpp"
#include "utils/logger.hpp"


#define ERROR(_EXPR) nne::Logger::Error(_EXPR);
#define WARN(_EXPR) nne::Logger::Warn(_EXPR);
#define INFO(_EXPR) nne::Logger::Info(_EXPR);
#define DEBUG(_EXPR) nne::Logger::Debug(_EXPR);

