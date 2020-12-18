#include <array>
#include <gmpxx.h>

// up to 3,317,044,064,679,887,385,961,981 (~1.37 * 2^81)
// only testing these primes is needed (see wikipedia)
static const std::array<const int, 13> primes{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};

static gmp_randclass rng(gmp_randinit_default);
static const mpz_class two = 2;

static mpz_class get_witness(const mpz_class &candidate, const int &pass) {
	if (static_cast<std::size_t>(pass) < primes.size()) {
		return primes.at(pass);
	}
	return rng.get_z_range(candidate+1-2-2)+2;
}

// primality test
// returns false if proven composite, true if maybe prime
bool miller_rabin(const mpz_class &n, const int &k) {
	// write n as 2^r*d + 1
	mpz_class d = n-1;
	mpz_class r = 0;
	while (d % 2 == 0) {
		d /= 2;
		r += 1;
	}
	
	// witness loop
	for (int j = 0; j < k; j++) {
		const mpz_class a = get_witness(n, j);
		mpz_class x;
		mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
		if (x == 1 || x == n-1)
			continue;

		bool witness = true;
		for (int i = 1; i < r; i++) {
			mpz_powm(x.get_mpz_t(), x.get_mpz_t(), two.get_mpz_t(), n.get_mpz_t());
			if (x == n-1) {
				// continue witness loop
				witness = false;
				break;
			}
		}
		if (! witness)
			continue;
		return false; // composite
	}
	return true; // maybe prime
}
