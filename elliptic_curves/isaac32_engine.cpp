#include "isaac32_engine.h"

//constexpr isaac32_engine::result_type isaac32_engine::min(void) { return std::numeric_limits<result_type>::min(); }
//
//constexpr isaac32_engine::result_type isaac32_engine::max(void) { return std::numeric_limits<result_type>::max(); }
//
//constexpr std::size_t isaac32_engine::randsizl(void) { return 8; }
//
//constexpr std::size_t isaac32_engine::randsiz(void) { return 1 << isaac32_engine::randsizl(); }

isaac32_engine::isaac32_engine(result_type value)
	: m_randcnt(0), m_randrsl(), m_mm(), m_aa(0), m_bb(0), m_cc(0)
{
	this->seed(value);
}

isaac32_engine::isaac32_engine(result_type s[256])
{
	this->seed(s);
}

void isaac32_engine::seed(result_type value)
{
	for (std::size_t i = 0; i<randsiz(); ++i) {
		this->m_randrsl[i] = value;
	}

	this->init();
}

void isaac32_engine::seed(result_type s[256])
{
	std::memcpy(this->m_randrsl, s, sizeof(this->m_randrsl)); // sizeof(this->m_randrsl) = 1024, sizeof(s) = 8
	this->init();
}

isaac32_engine::result_type isaac32_engine::operator()(void)
{
	if (!this->m_randcnt) {
		this->isaac32();
		this->m_randcnt = randsiz() - 1;
		return this->m_randrsl[this->m_randcnt];
	}

	--this->m_randcnt;
	return this->m_randrsl[this->m_randcnt];
}

void isaac32_engine::discard(unsigned long long z)
{
	while (z) {
		this->operator()();
		--z;
	}
}

void isaac32_engine::init(void)
{
	std::size_t i;
	std::uint32_t a = 0x9E3779B9;
	std::uint32_t b = 0x9E3779B9;
	std::uint32_t c = 0x9E3779B9;
	std::uint32_t d = 0x9E3779B9;
	std::uint32_t e = 0x9E3779B9;
	std::uint32_t f = 0x9E3779B9;
	std::uint32_t g = 0x9E3779B9;
	std::uint32_t h = 0x9E3779B9;

	this->m_aa = 0;
	this->m_bb = 0;
	this->m_cc = 0;

	for (i = 0; i<4; ++i) {
		mix(a, b, c, d, e, f, g, h);
	}

	for (i = 0; i<randsiz(); i += 8) {
		a += this->m_randrsl[i + 0];
		b += this->m_randrsl[i + 1];
		c += this->m_randrsl[i + 2];
		d += this->m_randrsl[i + 3];
		e += this->m_randrsl[i + 4];
		f += this->m_randrsl[i + 5];
		g += this->m_randrsl[i + 6];
		h += this->m_randrsl[i + 7];

		mix(a, b, c, d, e, f, g, h);

		this->m_mm[i + 0] = a;
		this->m_mm[i + 1] = b;
		this->m_mm[i + 2] = c;
		this->m_mm[i + 3] = d;
		this->m_mm[i + 4] = e;
		this->m_mm[i + 5] = f;
		this->m_mm[i + 6] = g;
		this->m_mm[i + 7] = h;
	}

	for (i = 0; i<randsiz(); i += 8) {
		a += this->m_mm[i + 0];
		b += this->m_mm[i + 1];
		c += this->m_mm[i + 2];
		d += this->m_mm[i + 3];
		e += this->m_mm[i + 4];
		f += this->m_mm[i + 5];
		g += this->m_mm[i + 6];
		h += this->m_mm[i + 7];

		mix(a, b, c, d, e, f, g, h);

		this->m_mm[i + 0] = a;
		this->m_mm[i + 1] = b;
		this->m_mm[i + 2] = c;
		this->m_mm[i + 3] = d;
		this->m_mm[i + 4] = e;
		this->m_mm[i + 5] = f;
		this->m_mm[i + 6] = g;
		this->m_mm[i + 7] = h;
	}

	this->isaac32();
	this->m_randcnt = randsiz();
}

void isaac32_engine::mix(std::uint32_t & a, std::uint32_t & b, std::uint32_t & c, std::uint32_t & d, std::uint32_t & e, std::uint32_t & f, std::uint32_t & g, std::uint32_t & h)
{
	a ^= b << 11; d += a; b += c;
	b ^= c >> 2;  e += b; c += d;
	c ^= d << 8;  f += c; d += e;
	d ^= e >> 16; g += d; e += f;
	e ^= f << 10; h += e; f += g;
	f ^= g >> 4;  a += f; g += h;
	g ^= h << 8;  b += g; h += a;
	h ^= a >> 9;  c += h; a += b;
}

void isaac32_engine::isaac32(void)
{
	std::uint32_t x, y;
	unsigned int i = 0;

	++this->m_cc;
	this->m_bb += this->m_cc;

	while (i<randsiz()) {
		x = this->m_mm[i];
		this->m_aa ^= (this->m_aa << 13);
		this->m_aa += this->m_mm[std::uint8_t(i + 128)];
		y = this->m_mm[std::uint8_t(x >> 2)] + this->m_aa + this->m_bb;
		this->m_mm[i] = y;
		this->m_bb = this->m_mm[std::uint8_t(y >> 10)] + x;
		this->m_randrsl[i] = this->m_bb;
		++i;

		x = this->m_mm[i];
		this->m_aa ^= (this->m_aa >> 6);
		this->m_aa += this->m_mm[std::uint8_t(i + 128)];
		y = this->m_mm[std::uint8_t(x >> 2)] + this->m_aa + this->m_bb;
		this->m_mm[i] = y;
		this->m_bb = this->m_mm[std::uint8_t(y >> 10)] + x;
		this->m_randrsl[i] = this->m_bb;
		++i;

		x = this->m_mm[i];
		this->m_aa ^= (this->m_aa << 2);
		this->m_aa += this->m_mm[std::uint8_t(i + 128)];
		y = this->m_mm[std::uint8_t(x >> 2)] + this->m_aa + this->m_bb;
		this->m_mm[i] = y;
		this->m_bb = this->m_mm[std::uint8_t(y >> 10)] + x;
		this->m_randrsl[i] = this->m_bb;
		++i;

		x = this->m_mm[i];
		this->m_aa ^= (this->m_aa >> 16);
		this->m_aa += this->m_mm[std::uint8_t(i + 128)];
		y = this->m_mm[std::uint8_t(x >> 2)] + this->m_aa + this->m_bb;
		this->m_mm[i] = y;
		this->m_bb = this->m_mm[std::uint8_t(y >> 10)] + x;
		this->m_randrsl[i] = this->m_bb;
		++i;
	}
}

//template<typename sseq>
//isaac32_engine::isaac32_engine(sseq & s)
//: m_randcnt(0), m_randrsl(), m_mm(), m_aa(0), m_bb(0), m_cc(0)
//{
//	this->seed(s);
//}

//template<typename Sseq>
//void isaac32_engine::seed(Sseq & s)
//{
//	for (std::size_t i = 0; i<randsiz(); ++i) {
//		this->m_randrsl[i] = s();
//	}
//}

//template<>
//void isaac32_engine::seed(std::seed_seq& s)
//{
//	s.generate(this->m_randrsl, this->m_randrsl + randsiz());
//	this->init();
//}