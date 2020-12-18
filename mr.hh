#pragma once

#include <gmpxx.h>

bool miller_rabin(const mpz_class &candidate, const int &passes);
bool miller_rabin(const mpz_class &candidate);
