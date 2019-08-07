#pragma once
#include "helpers/Logger.hpp"
#include <stdexcept>
#include <switch.h>
#include <vector>

namespace Helper
{
namespace System
{

u64 GetActiveApplicationProcessID();
u64 GetActiveTitleID();
std::vector<HidControllerID> GetConnectedControllers();

} // namespace System

} // namespace Helper