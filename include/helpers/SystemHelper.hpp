#pragma once
#include "helpers/Logger.hpp"
#include <stdexcept>
#include <switch.h>
namespace Helper {
namespace System {

u64 GetActiveApplicationProcessID();
u64 GetActiveTitleID();

} // namespace System

} // namespace Helper