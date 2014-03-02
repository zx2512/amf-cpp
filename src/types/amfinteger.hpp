#pragma once
#ifndef AMFINTEGER_HPP
#define AMFINTEGER_HPP

#include "amfitem.hpp"
#include "amfdouble.hpp"

namespace amf {

class AmfInteger : public AmfItem {
public:
	AmfInteger() : value(0) { };
	AmfInteger(int v) : value(v) { };
	operator int() const { return value; }

	std::vector<u8> serialize() const {
		// According to the spec:
		// If the value of an unsigned integer (uint) or signed integer (int)
		// is greater than or equal to 2^28, or if a signed integer (int) is
		// less than -2^28, it will be serialized using the AMF 3 double type
		if (value < -0x10000000 || value >= 0x10000000)
			return AmfDouble(value).serialize();

		if (value >= 0 && value <= 0x7F) {
			return std::vector<u8> {
				AMF_INTEGER,
				u8(value)
			};
		} else if (value > 0x7F && value <= 0x3FFF) {
			return std::vector<u8> {
				AMF_INTEGER,
				u8(value >> 7 | 0x80),
				u8(value & 0x7F)
			};
		} else if (value > 0x3FFF && value <= 0x1FFFFF) {
			return std::vector<u8> {
				AMF_INTEGER,
				u8(value >> 14 | 0x80),
				u8(((value >> 7) & 0x7F) | 0x80),
				u8(value & 0x7F)
			};
		} else {
			return std::vector<u8> {
				AMF_INTEGER,
				u8(value >> 22 | 0x80),
				u8(((value >> 15) & 0x7F) | 0x80),
				u8(((value >> 8 ) & 0x7F) | 0x80),
				u8(value & 0xFF)
			};
		}
	}

	std::vector<u8> asLength(u8 marker) {
		AmfInteger length(value << 1 | 1);

		std::vector<u8> buf = length.serialize();
		buf[0] = marker;

		return buf;
	}

	template<typename Iter>
	static AmfInteger deserialize(Iter& it, Iter end) {
		v8 data(it, end);
		int i = 0;
		int val = 0;

		// up to 3 bytes with high bit set for values > 255
		while (it != end && i < 3 && data.at(i) & 0x80) {
			val <<= 7;
			val |= data.at(i++) & 0x7F;
		}

		// last byte
		val <<= i < 3 ? 7 : 8;
		val |= data.at(i++);

		// set sign bit to handle negative integers
		val <<= 3;
		val >>= 3;

		it += i;

		return AmfInteger(val);
	}

	int value;
};

} // namespace amf

#endif
