#pragma once
#ifndef AMFDOUBLE_HPP
#define AMFDOUBLE_HPP

#include "types/amfitem.hpp"

namespace amf {

class DeserializationContext;

class AmfDouble : public AmfItem {
public:
	AmfDouble() : value(0) { }
	AmfDouble(double v) : value(v) { }
	operator double() const { return value; }

	bool operator==(const AmfItem& other) const;
	std::vector<u8> serialize() const;
	static AmfDouble deserialize(v8::const_iterator& it, v8::const_iterator end, DeserializationContext&);

	double value;
};

} // namespace amf

#endif
