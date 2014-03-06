#include "amftest.hpp"

#include "amf.hpp"
#include "types/amfxml.hpp"

TEST(XmlSerializationTest, EmptyXmlString) {
	AmfXml val;

	v8 expected { 0x0b, 0x01 };
	ASSERT_EQ(expected, val.serialize());

	val = AmfXml("");
	ASSERT_EQ(expected, val.serialize());
}

TEST(XmlSerializationTest, XmlString) {
	AmfXml val("<asd><foo bar='asd'>qux</foo><boo/></asd>");

	isEqual(v8 {
		0x0b, // AMF_XML
		0x53, // U29X-value, length = 41
    0x3c, 0x61, 0x73, 0x64, 0x3e, 0x3c, 0x66, 0x6f, 0x6f, 0x20, 0x62, 0x61,
    0x72, 0x3d, 0x27, 0x61, 0x73, 0x64, 0x27, 0x3e, 0x71, 0x75, 0x78, 0x3c,
    0x2f, 0x66, 0x6f, 0x6f, 0x3e, 0x3c, 0x62, 0x6f, 0x6f, 0x2f, 0x3e, 0x3c,
    0x2f, 0x61, 0x73, 0x64, 0x3e
	}, val);
}

TEST(XmlSerializationTest, UnicodeXmlString) {
	AmfXml val("<foo>ħĸð@þæĸſ“ð</foo>");

	isEqual(v8 {
		0x0b,
		0x3f,
		0x3c, 0x66, 0x6f, 0x6f, 0x3e,
		0xc4, 0xa7, 0xc4, 0xb8, 0xc3, 0xb0, 0x40, 0xc3, 0xbe, 0xc3, 0xa6, 0xc4,
		0xb8, 0xc5, 0xbf, 0xe2, 0x80, 0x9c, 0xc3, 0xb0,
		0x3c, 0x2f, 0x66, 0x6f, 0x6f, 0x3e
 }, val);
}

TEST(XmlSerializationTest, MultiByteLengthString) {
	AmfXml val("<foo>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</foo>");

	isEqual(v8 {
		0x0b,
		0x84, 0x6f,
		0x3c, 0x66, 0x6f, 0x6f, 0x3e,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x3c, 0x2f, 0x66, 0x6f, 0x6f, 0x3e
	}, val);
}

static void deserializesTo(const char* expected, const v8& data, int left = 0,
	DeserializationContext* ctx = nullptr) {
	deserialize<AmfXml>(expected, data, left, ctx);
}

TEST(XmlDeserializationTest, SimpleValues) {
	deserializesTo("", v8 { 0x01 });
	deserializesTo("foo", v8 { 0x07, 0x66, 0x6f, 0x6f });
	deserializesTo("foo", v8 { 0x07, 0x66, 0x6f, 0x6f, 0x6f }, 1);
	deserializesTo("<asd><foo bar='asd'>qux</foo><boo/></asd>", v8 {
		0x53, // U29X-value, length = 41
    0x3c, 0x61, 0x73, 0x64, 0x3e, 0x3c, 0x66, 0x6f, 0x6f, 0x20, 0x62, 0x61,
    0x72, 0x3d, 0x27, 0x61, 0x73, 0x64, 0x27, 0x3e, 0x71, 0x75, 0x78, 0x3c,
    0x2f, 0x66, 0x6f, 0x6f, 0x3e, 0x3c, 0x62, 0x6f, 0x6f, 0x2f, 0x3e, 0x3c,
    0x2f, 0x61, 0x73, 0x64, 0x3e
	});
}

TEST(XmlDeserializationTest, UnicodeXmlString) {
	deserializesTo("<foo>ħĸð@þæĸſ“ð</foo>", v8 {
		0x3f,
		0x3c, 0x66, 0x6f, 0x6f, 0x3e,
		0xc4, 0xa7, 0xc4, 0xb8, 0xc3, 0xb0, 0x40, 0xc3, 0xbe, 0xc3, 0xa6, 0xc4,
		0xb8, 0xc5, 0xbf, 0xe2, 0x80, 0x9c, 0xc3, 0xb0,
		0x3c, 0x2f, 0x66, 0x6f, 0x6f, 0x3e
	});
}

TEST(XmlDeserializationTest, MultiByteLengthString) {
	std::string val(
		"<foo>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</foo>");

	v8 data {
		0x84, 0x6f,
		0x3c, 0x66, 0x6f, 0x6f, 0x3e,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
		0x3c, 0x2f, 0x66, 0x6f, 0x6f, 0x3e,
		0xff // extra byte
	};

	deserializesTo(val.c_str(), data, 1);
}

TEST(XmlDeserializationTest, ObjectReference) {
	DeserializationContext ctx;
	deserializesTo("foo", v8 { 0x07, 0x66, 0x6f, 0x6f }, 0, &ctx);
	deserializesTo("foo", v8 { 0x07, 0x66, 0x6f, 0x6f, 0x6f }, 1, &ctx);
	deserializesTo("foo", v8 { 0x00 }, 0, &ctx);
	deserializesTo("foo", v8 { 0x02 }, 0, &ctx);
	deserializesTo("bar", v8 { 0x07, 0x62, 0x61, 0x72 }, 0, &ctx);
	deserializesTo("foo", v8 { 0x02 }, 0, &ctx);
	deserializesTo("bar", v8 { 0x04 }, 0, &ctx);
}
