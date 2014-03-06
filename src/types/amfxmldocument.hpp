#pragma once
#ifndef AMFXMLDOCUMENT_HPP
#define AMFXMLDOCUMENT_HPP

#include "types/amfitem.hpp"
#include "types/amfxml.hpp"

namespace amf {

class AmfXmlDocument : public AmfItem {
public:
	AmfXmlDocument() { };
	AmfXmlDocument(std::string value) : value(value) { };

	std::vector<u8> serialize() const {
		// XMLDocument is identical to XML (except for the object marker),
		// so simply forward everything to the AmfXml implementation
		std::vector<u8> buf = AmfXml(value).serialize();
		buf[0] = AMF_XMLDOC;

		return buf;
	}

	template<typename Iter>
	static AmfXmlDocument deserialize(Iter& it, Iter end, DeserializationContext& ctx) {
		int type = AmfInteger::deserialize(it, end, ctx).value;
		if ((type & 0x01) == 0)
			return ctx.getObject<AmfXmlDocument>(type >> 1);

		int length = type >> 1;
		if (end - it < length)
			throw std::out_of_range("Not enough bytes for AmfXmlDocument");

		std::string val(it, it + length);
		it += length;

		AmfXmlDocument ret(val);
		ctx.addObject<AmfXmlDocument>(ret);

		return ret;
	}

	std::string value;
};

} // namespace amf

#endif
