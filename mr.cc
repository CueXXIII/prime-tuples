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

// auto-choose number of passes
bool miller_rabin(const mpz_class &n) {
	// wikipedia
	if (n < 2'047)
		return miller_rabin(n, 1);
	if (n < 1'373'653)
		return miller_rabin(n, 2);
	if (n < 25'326'001)
		return miller_rabin(n, 3);
	if (n < 3'215'031'751)
		return miller_rabin(n, 4);
	if (n < 2'152'302'898'747)
		return miller_rabin(n, 5);
	if (n < 3'474'749'660'383)
		return miller_rabin(n, 6);
	if (n < 341'550'071'728'321)
		return miller_rabin(n, 7);
	if (n < 3'825'123'056'546'413'051)
		return miller_rabin(n, 9);
	//if (n < 18'446'744'073'709'551'616)
	//	return miller_rabin(n, 12);
	if (n < 318665857834031151167461_mpz)
		return miller_rabin(n, 12);
	if (n < 3317044064679887385961981_mpz)
		return miller_rabin(n, 13);
	// https://security.stackexchange.com/questions/4544/how-many-iterations-of-rabin-miller-should-be-used-to-generate-cryptographic-saf
	return miller_rabin(n, 40);

}
