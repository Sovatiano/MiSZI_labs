#pragma once
#include "Include.h"
#include "Utility.h"


std::vector<std::string> Encrypt(std::vector<big_int> message, big_int exp, big_int mod);

std::vector<big_int> Decrypt(std::vector<std::string> encrypted_message, big_int d, big_int mod);
