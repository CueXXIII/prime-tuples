#include <chrono>
#include <iostream>

#include "mr.hh"

// how many miller rabin passes to calculate
const int passes = 31;

inline void print_timestamp(const std::chrono::high_resolution_clock::time_point &starttime) {
	std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - starttime).count() / 1000.0 << "s] ";
}

void print_sixtuples(const mpz_class &start) {
	auto starttime = std::chrono::high_resolution_clock::now();

	mpz_class first;

	if (start <= 5) {
		print_timestamp(starttime);
		std::cout << "(5, 7, 11, 13, 17, 19)\n";
	}
	if (start <= 7) {
		print_timestamp(starttime);
		std::cout << "(7, 11, 13, 17, 19, 23)\n";
	}
	if (start <= 97) {
		first = 0;
	} else {
		first = (start-97+209)/210;
	}

	// all other sextuplets have starting with n=0 the form
	// (210n + 97, 210n + 101, 210n + 103, 210n + 107, 210n + 109, 210n + 113)
	for (mpz_class n = first;; n++) {
		const mpz_class n210 = n*210;
		if (miller_rabin(n210+97, passes) &&
		    miller_rabin(n210+101, passes) &&
		    miller_rabin(n210+103, passes) &&
		    miller_rabin(n210+107, passes) &&
		    miller_rabin(n210+109, passes) &&
		    miller_rabin(n210+113, passes)) {
			print_timestamp(starttime);
			std::cout << "(" << n210+97
				<< ", " << n210+101
				<< ", " << n210+103
				<< ", " << n210+107
				<< ", " << n210+109
				<< ", " << n210+113 << ")\n";
		}

	}
}

int main(void) {
	print_sixtuples(18446744073709551615_mpz);
}
