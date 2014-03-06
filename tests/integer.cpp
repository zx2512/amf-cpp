#include "amftest.hpp"

#include "amf.hpp"
#include "types/amfinteger.hpp"

static void isEqual(const std::vector<u8>& expected, int value) {
	isEqual(expected, AmfInteger(value));
}

TEST(IntegerSerializationTest, PositiveInteger1Byte) {
	isEqual(v8 { 0x04, 0x00 }, 0);
	isEqual(v8 { 0x04, 0x01 }, 1);
	isEqual(v8 { 0x04, 0x7E }, 0x7e);
	isEqual(v8 { 0x04, 0x7F }, 0x7f);
}

TEST(IntegerSerializationTest, PositiveInteger2Byte) {
	isEqual(v8 { 0x04, 0x81, 0x00 }, 0x80);
	isEqual(v8 { 0x04, 0x87, 0x7E }, 0x3fe);
	isEqual(v8 { 0x04, 0x87, 0x7F }, 0x3ff);
	isEqual(v8 { 0x04, 0x88, 0x00 }, 0x400);
}

TEST(IntegerSerializationTest, PositiveInteger3Byte) {
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0x7E }, 0x1ffffe);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0x7F }, 0x1fffff);
}

TEST(IntegerSerializationTest, PositiveInteger4Byte) {
	isEqual(v8 { 0x04, 0x80, 0xC0, 0x80, 0x00 }, 0x200000);
	isEqual(v8 { 0x04, 0xBF, 0xFF, 0xFF, 0xFE }, 0xffffffe);
	isEqual(v8 { 0x04, 0xBF, 0xFF, 0xFF, 0xFF }, 0xfffffff);
}

TEST(IntegerSerializationTest, LargePositiveIntegerAsDouble) {
	isEqual(v8 { 0x05, 0x41, 0xBF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00 }, 0x1ffffffe);
	isEqual(v8 { 0x05, 0x41, 0xBF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 }, 0x1fffffff);
	isEqual(v8 { 0x05, 0x41, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, 0x20000000);
	isEqual(v8 { 0x05, 0x41, 0xDF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00 }, 0x7ffffffe);
	isEqual(v8 { 0x05, 0x41, 0xDF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00 }, 0x7fffffff);
	isEqual(v8 { 0x05, 0x41, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, 0x10000000);
}

TEST(IntegerSerializationTest, NegativeInteger) {
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xFF }, -1);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xFE }, -2);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xFD }, -3);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xFC }, -4);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xF9 }, -7);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xF8 }, -8);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xE1 }, -31);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0xE0 }, -32);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0x81 }, -127);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0x80 }, -128);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xFF, 0x7F }, -129);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xC0, 0x01 }, -16383);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xC0, 0x00 }, -16384);
	isEqual(v8 { 0x04, 0xFF, 0xFF, 0xBF, 0xFF }, -16385);
	isEqual(v8 { 0x04, 0xFF, 0xC0, 0x80, 0x01 }, -2097151);
	isEqual(v8 { 0x04, 0xFF, 0xC0, 0x80, 0x00 }, -2097152);
	isEqual(v8 { 0x04, 0xFF, 0xBF, 0xFF, 0xFF }, -2097153);
	isEqual(v8 { 0x04, 0xC0, 0x80, 0x80, 0x01 }, -268435455);
	isEqual(v8 { 0x04, 0xC0, 0x80, 0x80, 0x00 }, -268435456);
}

TEST(IntegerSerializationTest, LargeNegativeIntegerAsDouble) {
	isEqual(v8 { 0x05, 0xC1, 0xB0, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 }, -268435457);
	isEqual(v8 { 0x05, 0xC1, 0xDC, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00 }, -1879048193);
	isEqual(v8 { 0x05, 0xC1, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, -2147483647 - 1);
}

TEST(IntegerAsLengthTest, SimpleValue) {
	isEqual(v8 { 0x05, 0x07 }, AmfInteger(3).asLength(0x05));
	isEqual(v8 { 0x06, 0x07 }, AmfInteger(3).asLength(0x06));
	isEqual(v8 { 0x01, 0x87, 0x7F }, AmfInteger(511).asLength(0x01));
}

TEST(IntegerAsLengthTest, ValueNotModified) {
	// tests that a call to AmfInteger#asLength doesn't modify the actual
	// value of the AmfInteger
	AmfInteger val(17);
	isEqual(v8 { 0x0A, 0x23 }, val.asLength(0x0a));
	isEqual(v8 { 0x04, 0x11 }, val.serialize());
}

static void deserializesTo(int expected, const v8& data, int left = 0) {
	deserialize<AmfInteger>(expected, data, left);
}

TEST(IntegerDeserializationTest, PositiveInteger1Byte) {
	deserializesTo(0, { 0x00 });
	deserializesTo(1, { 0x01 });
	deserializesTo(0x7E, { 0x7E });
	deserializesTo(0x7F, { 0x7F });
}

TEST(IntegerDeserializationTest, PositiveInteger2Byte) {
	deserializesTo(0x80, { 0x81, 0x00 });
	deserializesTo(0x3fe, { 0x87, 0x7E });
	deserializesTo(0x3ff, { 0x87, 0x7F });
	deserializesTo(0x400, { 0x88, 0x00 });
}

TEST(IntegerDeserializationTest, PositiveInteger3Byte) {
	deserializesTo(0x1ffffe, { 0xFF, 0xFF, 0x7E });
	deserializesTo(0x1fffff, { 0xFF, 0xFF, 0x7F });
}

TEST(IntegerDeserializationTest, PositiveInteger4Byte) {
	deserializesTo(0x200000, { 0x80, 0xC0, 0x80, 0x00 });
	deserializesTo(0xffffffe, { 0xBF, 0xFF, 0xFF, 0xFE });
	deserializesTo(0xfffffff, { 0xBF, 0xFF, 0xFF, 0xFF });
}

TEST(IntegerDeserializationTest, NegativeInteger) {
	deserializesTo(-1, { 0xFF, 0xFF, 0xFF, 0xFF });
	deserializesTo(-2, { 0xFF, 0xFF, 0xFF, 0xFE });
	deserializesTo(-3, { 0xFF, 0xFF, 0xFF, 0xFD });
	deserializesTo(-4, { 0xFF, 0xFF, 0xFF, 0xFC });
	deserializesTo(-7, { 0xFF, 0xFF, 0xFF, 0xF9 });
	deserializesTo(-8, { 0xFF, 0xFF, 0xFF, 0xF8 });
	deserializesTo(-31, { 0xFF, 0xFF, 0xFF, 0xE1 });
	deserializesTo(-32, { 0xFF, 0xFF, 0xFF, 0xE0 });
	deserializesTo(-127, { 0xFF, 0xFF, 0xFF, 0x81 });
	deserializesTo(-128, { 0xFF, 0xFF, 0xFF, 0x80 });
	deserializesTo(-129, { 0xFF, 0xFF, 0xFF, 0x7F });
	deserializesTo(-16383, { 0xFF, 0xFF, 0xC0, 0x01 });
	deserializesTo(-16384, { 0xFF, 0xFF, 0xC0, 0x00 });
	deserializesTo(-16385, { 0xFF, 0xFF, 0xBF, 0xFF });
	deserializesTo(-2097151, { 0xFF, 0xC0, 0x80, 0x01 });
	deserializesTo(-2097152, { 0xFF, 0xC0, 0x80, 0x00 });
	deserializesTo(-2097153, { 0xFF, 0xBF, 0xFF, 0xFF });
	deserializesTo(-268435455, { 0xC0, 0x80, 0x80, 0x01 });
	deserializesTo(-268435456, { 0xC0, 0x80, 0x80, 0x00 });
}

TEST(IntegerDeserializationTest, BytesLeft) {
	deserializesTo(0, { 0x00, 0x00 }, 1);
	deserializesTo(1, { 0x01, 0x00 }, 1);
	deserializesTo(0x3ff, { 0x87, 0x7F, 0x7F }, 1);
	deserializesTo(0x3ff, { 0x87, 0x7F, 0x7F, 0x00 }, 2);
	deserializesTo(0x1fffff, { 0xFF, 0xFF, 0x7F, 0xFF }, 1);
	deserializesTo(0xffffffe, { 0xBF, 0xFF, 0xFF, 0xFE, 0xFF }, 1);
	deserializesTo(-1, { 0xFF, 0xFF, 0xFF, 0xFF, 0x00 }, 1);
}
