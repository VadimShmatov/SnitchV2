#pragma once
#include <cstdint>
#include <limits>
#include <cstring>
#include <random>

class isaac32_engine {
public:
	typedef std::uint32_t result_type;

	static constexpr result_type default_seed = 0;
	static constexpr result_type min(void) { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max(void) { return std::numeric_limits<result_type>::max(); }
	static constexpr std::size_t randsizl(void) { return 8; }
	static constexpr std::size_t randsiz(void) { return 1 << isaac32_engine::randsizl(); }

	explicit isaac32_engine(result_type value = default_seed);

	explicit isaac32_engine(result_type s[256]);

	/*template<typename sseq>
	explicit isaac32_engine(sseq& s);*/

	void seed(result_type value = default_seed);

	void seed(result_type s[256]);

	/*template<typename Sseq>
	void seed(Sseq& s);*/

	result_type operator()(void);

	void discard(unsigned long long z);

private:
	std::size_t m_randcnt;
	result_type m_randrsl[256];
	result_type m_mm[256];
	std::uint32_t m_aa;
	std::uint32_t m_bb;
	std::uint32_t m_cc;

	void init(void);

	static void mix(std::uint32_t& a, std::uint32_t& b, std::uint32_t& c, std::uint32_t& d, std::uint32_t& e, std::uint32_t& f, std::uint32_t& g, std::uint32_t& h);

	void isaac32(void);

	isaac32_engine(const isaac32_engine&) = delete;
	isaac32_engine& operator=(const isaac32_engine&) = delete;
};
