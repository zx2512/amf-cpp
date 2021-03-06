#include "amftest.hpp"

#include "deserializer.hpp"
#include "serializationcontext.hpp"

#include "types/amfarray.hpp"
#include "types/amfbool.hpp"
#include "types/amfbytearray.hpp"
#include "types/amfdate.hpp"
#include "types/amfdictionary.hpp"
#include "types/amfdouble.hpp"
#include "types/amfinteger.hpp"
#include "types/amfnull.hpp"
#include "types/amfobject.hpp"
#include "types/amfstring.hpp"
#include "types/amfundefined.hpp"
#include "types/amfvector.hpp"
#include "types/amfxml.hpp"
#include "types/amfxmldocument.hpp"

#include "utils/amfitemptr.hpp"

template<typename T>
static void deserializesTo(const T& expected, v8 data, int left = 0) {
	SCOPED_TRACE(::testing::PrintToString(expected) + " = " + ::testing::PrintToString(data));

	auto it = data.cbegin();
	try {
		Deserializer d;
		T i = d.deserialize(it, data.cend()).as<T>();
		ASSERT_EQ(expected, i);
		T j = *d.deserialize(data).asPtr<T>();
		ASSERT_EQ(expected, j);
		SerializationContext ctx;
		T k = Deserializer::deserialize(data, ctx).as<T>();
		ASSERT_EQ(expected, k);
	} catch(std::exception& e) {
		FAIL() << "Deserialization threw exception:\n"
		       << e.what() ;
	}

	ASSERT_EQ(left, data.cend() - it)
		<< "Expected " << left
		<< " bytes left, got " << (data.cend() - it)
		<< " bytes left";
}

TEST(Deserializer, Undefined) {
	deserializesTo(AmfUndefined(), { 0x00 });
	deserializesTo(AmfUndefined(), { 0x00, 0x00 }, 1);
}

TEST(Deserializer, Null) {
	deserializesTo(AmfNull(), { 0x01 });
	deserializesTo(AmfNull(), { 0x01, 0x01 }, 1);
}

TEST(Deserializer, Bool) {
	deserializesTo(AmfBool(false), { 0x02 });
	deserializesTo(AmfBool(false), { 0x02, 0x02 }, 1);
	deserializesTo(AmfBool(true), { 0x03 });
	deserializesTo(AmfBool(true), { 0x03, 0x03 }, 1);
}

TEST(Deserializer, Integer) {
	deserializesTo(AmfInteger(0x7e), { 0x04, 0x7e });
	deserializesTo(AmfInteger(0x7e), { 0x04, 0x7e, 0x04 }, 1);
	deserializesTo(AmfInteger(0xffffffe), { 0x04, 0xbf, 0xff, 0xff, 0xfe });
}

TEST(Deserializer, Double) {
	deserializesTo(AmfDouble(0.5), { 0x05, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
	deserializesTo(AmfDouble(0.5), { 0x05, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00 }, 3);
}

TEST(Deserializer, String) {
	deserializesTo(AmfString("foo"), { 0x06, 0x07, 0x66, 0x6f, 0x6f });
	deserializesTo(AmfString("foo"), { 0x06, 0x07, 0x66, 0x6f, 0x6f, 0x06 }, 1);
}

TEST(Deserializer, XmlDoc) {
	deserializesTo(AmfXmlDocument(""), { 0x07, 0x01 });
	deserializesTo(AmfXmlDocument(""), { 0x07, 0x01, 0x07 }, 1);
	deserializesTo(AmfXmlDocument("foo"), { 0x07, 0x07, 0x66, 0x6f, 0x6f });
}

TEST(Deserializer, Date) {
	deserializesTo(AmfDate(136969002755210ll), { 0x08, 0x01, 0x42, 0xdf, 0x24,
		0xa5, 0x30, 0x49, 0x22, 0x80 });
	deserializesTo(AmfDate(136969002755210ll), { 0x08, 0x01, 0x42, 0xdf, 0x24,
		0xa5, 0x30, 0x49, 0x22, 0x80, 0x08 }, 1);
}

TEST(Deserializer, Array) {
	deserializesTo(AmfArray(), { 0x09, 0x01, 0x01 });
	deserializesTo(AmfArray(std::vector<AmfInteger> { 1, 2, 3 } ), {
		0x09, 0x07, 0x01, 0x04, 0x01, 0x04, 0x02, 0x04, 0x03, 0xff }, 1);
}

TEST(Deserializer, Xml) {
	deserializesTo(AmfXml(""), { 0x0b, 0x01 });
	deserializesTo(AmfXml(""), { 0x0b, 0x01, 0x0b }, 1);
	deserializesTo(AmfXml("foo"), { 0x0b, 0x07, 0x66, 0x6f, 0x6f });
}

TEST(Deserializer, Object) {
	deserializesTo(AmfObject("", false, false), { 0x0a, 0x03, 0x01 });
	deserializesTo(AmfObject("", true, false), { 0x0a, 0x0b, 0x01, 0x01, 0xff }, 1);
}

TEST(Deserializer, ByteArray) {
	deserializesTo(AmfByteArray(v8 { 1, 2, 3 }), { 0x0c, 0x07, 0x01, 0x02, 0x03 });
	deserializesTo(AmfByteArray(v8 { 1, 2, 3 }), { 0x0c, 0x07, 0x01, 0x02, 0x03, 0x0c }, 1);
}

TEST(Deserializer, VectorInt) {
	deserializesTo(AmfVector<int> { { 1 }, false}, { 0x0d, 0x03, 0x00,
		0x00, 0x00, 0x00, 0x01 });
	deserializesTo(AmfVector<int> { { 2, 3 }, true}, { 0x0d, 0x05, 0x01,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0xff }, 1);
}

TEST(Deserializer, VectorUint) {
	deserializesTo(AmfVector<unsigned int> { { 1 }, false}, { 0x0e, 0x03, 0x00,
		0x00, 0x00, 0x00, 0x01 });
	deserializesTo(AmfVector<unsigned int> { { 2, 3 }, true}, { 0x0e, 0x05, 0x01,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0xff }, 1);
}

TEST(Deserializer, VectorDouble) {
	deserializesTo(AmfVector<double> { { 0.5 }, false }, { 0x0f, 0x03, 0x00,
		0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
	deserializesTo(AmfVector<double> { { -1.2 }, true }, { 0x0f, 0x03, 0x01,
		0xbf, 0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xff }, 1);
}

TEST(Deserializer, VectorAmfItem) {
	AmfVector<AmfInteger> v({1, 2, 3}, "foo", false);
	// Need to pass a AmfVector<AmfItem> to compare against, as that's what
	// Deserializer::deserialize returns
	AmfVector<AmfItem> vc = v;
	v8 data {
		0x10, 0x07, 0x00,
		0x07, 0x66, 0x6f, 0x6f,
		0x04, 0x01,
		0x04, 0x02,
		0x04, 0x03
	};

	deserializesTo(vc, data);
}

TEST(Deserializer, InstanceContext) {
	AmfByteArray ba(v8 { 0x1 });
	v8 data {
		0x0c, 0x03, 0x01,
		0x0c, 0x00,
	};

	Deserializer d;
	auto it = data.cbegin();
	auto end = data.cend();
	ASSERT_EQ(ba, d.deserialize(it, end).as<AmfByteArray>());
	ASSERT_EQ(data.cbegin() + 3, it);
	ASSERT_EQ(ba, d.deserialize(it, end).as<AmfByteArray>());
	ASSERT_EQ(end, it);
}

TEST(Deserializer, ClearContext) {
	AmfByteArray ba(v8 { 0x1 });
	v8 data {
		0x0c, 0x03, 0x01,
		0x0c, 0x00,
	};

	Deserializer d;
	auto it = data.cbegin();
	auto end = data.cend();
	ASSERT_EQ(ba, d.deserialize(it, end).as<AmfByteArray>());
	d.clearContext();
	ASSERT_THROW(d.deserialize(it, end), std::out_of_range);
}

TEST(Deserializer, UnknownType) {
	Deserializer d;
	ASSERT_THROW(d.deserialize({ AMF_DICTIONARY + 1 }), std::invalid_argument);
	ASSERT_THROW(d.deserialize({ 0xff }), std::invalid_argument);
}

TEST(Deserializer, IncorrectTypeRef) {
	Deserializer d;
	ASSERT_EQ(AmfArray(), d.deserialize(v8 { 0x09, 0x01, 0x01 }).as<AmfArray>());
	ASSERT_EQ(AmfArray(), d.deserialize(v8 { 0x09, 0x00 }).as<AmfArray>());
	ASSERT_THROW(d.deserialize(v8 { 0x10, 0x00 }), std::invalid_argument);
}
