#include "gtest/gtest.h"

#include "amf.hpp"
#include "types/amfobject.hpp"
#include "types/amfvector.hpp"

template<typename T>
static void isEqual(const std::vector<u8>& expected, AmfVector<T> vector) {
	ASSERT_EQ(expected, vector.serialize());
}

TEST(VectorSerializationTest, VectorIntEmpty) {
	AmfVector<int> vec({}, false);
	v8 expected { 0x0d, 0x01, 0x00 };
	ASSERT_EQ(expected, vec.serialize());

	vec = {{}, true};
	expected = { 0x0d, 0x01, 0x01 };
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorIntSimple) {
	AmfVector<int> vec({ 1, 2, 3 }, false);
	v8 expected {
		0x0d, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03
	};
	ASSERT_EQ(expected, vec.serialize());

	vec = {{ 0xff, 0x100, 0xfffe, 0xffff, 0x123456, 0xfffffff }, true};
	expected = {
		0x0d, 0x0d, 0x01,
		0x00, 0x00, 0x00, 0xff,
		0x00, 0x00, 0x01, 0x00,
		0x00, 0x00, 0xff, 0xfe,
		0x00, 0x00, 0xff, 0xff,
		0x00, 0x12, 0x34, 0x56,
		0x0f, 0xff, 0xff, 0xff
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorInt32) {
	AmfVector<int> vec({
		0x20000000,
		0x40000000,
		0x7fffffff
	}, true);
	v8 expected {
		0x0d, 0x07, 0x01,
		0x20, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xff
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorIntNegative) {
	AmfVector<int> vec({ -1, -2, -0xffff }, false);
	v8 expected {
		0x0d, 0x07, 0x00,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xfe,
		0xff, 0xff, 0x00, 0x01
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorInt32Negative) {
	AmfVector<int> vec({
		-0x20000000,
		-0x40000000,
		-0x7fffffff,
		-2147483648
	}, true);
	v8 expected {
		0x0d, 0x09, 0x01,
		0xe0, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorIntFixedDefault) {
	AmfVector<int> vec({1, 3, 5});
	v8 expected {
		0x0d, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x05
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorInt2ByteLength) {
	std::vector<int> num(260);
	std::iota(num.begin(), num.end(), 0);

	AmfVector<int> vec(num, true);
	v8 expected {
		0x0d, 0x84, 0x09, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
		0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b,
		0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f,
		0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13,
		0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x17,
		0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1b,
		0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1f,
		0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x23,
		0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x27,
		0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x2b,
		0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x2f,
		0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x33,
		0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x37,
		0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x00, 0x00, 0x3b,
		0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f,
		0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x43,
		0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x47,
		0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x4b,
		0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x00, 0x4f,
		0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00, 0x53,
		0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00, 0x57,
		0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x5a, 0x00, 0x00, 0x00, 0x5b,
		0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x5e, 0x00, 0x00, 0x00, 0x5f,
		0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x63,
		0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x67,
		0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x6b,
		0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x6f,
		0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00, 0x72, 0x00, 0x00, 0x00, 0x73,
		0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x75, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x77,
		0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x79, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, 0x00, 0x7b,
		0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x7d, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7f,
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x83,
		0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, 0x87,
		0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x89, 0x00, 0x00, 0x00, 0x8a, 0x00, 0x00, 0x00, 0x8b,
		0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x00, 0x00, 0x8f,
		0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x93,
		0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x95, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x97,
		0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x9a, 0x00, 0x00, 0x00, 0x9b,
		0x00, 0x00, 0x00, 0x9c, 0x00, 0x00, 0x00, 0x9d, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0x9f,
		0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0xa1, 0x00, 0x00, 0x00, 0xa2, 0x00, 0x00, 0x00, 0xa3,
		0x00, 0x00, 0x00, 0xa4, 0x00, 0x00, 0x00, 0xa5, 0x00, 0x00, 0x00, 0xa6, 0x00, 0x00, 0x00, 0xa7,
		0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0xab,
		0x00, 0x00, 0x00, 0xac, 0x00, 0x00, 0x00, 0xad, 0x00, 0x00, 0x00, 0xae, 0x00, 0x00, 0x00, 0xaf,
		0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0xb1, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x00, 0x00, 0xb3,
		0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, 0xb5, 0x00, 0x00, 0x00, 0xb6, 0x00, 0x00, 0x00, 0xb7,
		0x00, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0xbb,
		0x00, 0x00, 0x00, 0xbc, 0x00, 0x00, 0x00, 0xbd, 0x00, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0xbf,
		0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, 0xc3,
		0x00, 0x00, 0x00, 0xc4, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0xc7,
		0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xc9, 0x00, 0x00, 0x00, 0xca, 0x00, 0x00, 0x00, 0xcb,
		0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0xcd, 0x00, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, 0xcf,
		0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0xd1, 0x00, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x00, 0xd3,
		0x00, 0x00, 0x00, 0xd4, 0x00, 0x00, 0x00, 0xd5, 0x00, 0x00, 0x00, 0xd6, 0x00, 0x00, 0x00, 0xd7,
		0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0xd9, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0xdb,
		0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, 0xdd, 0x00, 0x00, 0x00, 0xde, 0x00, 0x00, 0x00, 0xdf,
		0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xe1, 0x00, 0x00, 0x00, 0xe2, 0x00, 0x00, 0x00, 0xe3,
		0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0xe5, 0x00, 0x00, 0x00, 0xe6, 0x00, 0x00, 0x00, 0xe7,
		0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0xe9, 0x00, 0x00, 0x00, 0xea, 0x00, 0x00, 0x00, 0xeb,
		0x00, 0x00, 0x00, 0xec, 0x00, 0x00, 0x00, 0xed, 0x00, 0x00, 0x00, 0xee, 0x00, 0x00, 0x00, 0xef,
		0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf1, 0x00, 0x00, 0x00, 0xf2, 0x00, 0x00, 0x00, 0xf3,
		0x00, 0x00, 0x00, 0xf4, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x00, 0x00, 0xf6, 0x00, 0x00, 0x00, 0xf7,
		0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x00, 0x00, 0xfb,
		0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xff,
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x01, 0x03,
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorUintEmpty) {
	AmfVector<unsigned int> vec({}, false);
	v8 expected { 0x0e, 0x01, 0x00 };
	ASSERT_EQ(expected, vec.serialize());

	vec = {{}, true};
	expected = { 0x0e, 0x01, 0x01 };
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorUintSimple) {
	AmfVector<unsigned int> vec({ 1, 2, 3 }, false);
	v8 expected {
		0x0e, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03
	};
	ASSERT_EQ(expected, vec.serialize());

	vec = {{ 0xff, 0x100, 0xfffe, 0xffff, 0x123456, 0xfffffff }, true};
	expected = {
		0x0e, 0x0d, 0x01,
		0x00, 0x00, 0x00, 0xff,
		0x00, 0x00, 0x01, 0x00,
		0x00, 0x00, 0xff, 0xfe,
		0x00, 0x00, 0xff, 0xff,
		0x00, 0x12, 0x34, 0x56,
		0x0f, 0xff, 0xff, 0xff
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorUint32) {
	AmfVector<unsigned int> vec({
		0x20000000u,
		0x40000000u,
		0x7fffffffu,
		0x80000000u,
		0xffffffffu
	}, true);
	v8 expected {
		0x0e, 0x0b, 0x01,
		0x20, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xff,
		0x80, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorUintFixedDefault) {
	AmfVector<unsigned int> vec({1, 3, 5});
	v8 expected {
		0x0e, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x05
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorDoubleEmpty) {
	AmfVector<double> vec({}, false);
	v8 expected { 0x0f, 0x01, 0x00 };
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorDoubleSimple) {
	AmfVector<double> vec({0}, true);
	v8 expected {
		0x0f, 0x03, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	ASSERT_EQ(expected, vec.serialize());

	vec = {{0.5}, false};
	expected = {
		0x0f, 0x03, 0x00,
		0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	ASSERT_EQ(expected, vec.serialize());

	vec = {
		{ -1.2, 0.3333333333333333, -0.3333333333333333, 2.5e+51 },
		false
	};
	expected = {
		0x0f, 0x09, 0x00,
		0xbf, 0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
		0x3f, 0xd5, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0xbf, 0xd5, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x4a, 0x9a, 0xba, 0x47, 0x14, 0x95, 0x7d, 0x30
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorDoubleFixedDefault) {
	AmfVector<double> vec({3.14159});
	v8 expected {
		0x0f, 0x03, 0x00,
		0x40, 0x09, 0x21, 0xf9, 0xf0, 0x1b, 0x86, 0x6e
	};
	ASSERT_EQ(expected, vec.serialize());
}

TEST(VectorSerializationTest, VectorUtilityFunctions) {
	AmfVector<int> vector({}, false);
	vector.push_back(1);
	v8 expected {
		0x0d, 0x03, 0x00,
		0x00, 0x00, 0x00, 0x01,
	};
	ASSERT_EQ(expected, vector.serialize());

	std::vector<int> elems { 3, 5 };
	vector.insert(vector.end(), elems.begin(), elems.end());
	expected = {
		0x0d, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x05
	};
	ASSERT_EQ(expected, vector.serialize());
}

TEST(VectorSerializationTest, VectorAnonObjectEmpty) {
	AmfVector<AmfItem*> vector({}, "", false);
	v8 expected {
		0x10, 0x01, 0x00, 0x01
	};
	ASSERT_EQ(expected, vector.serialize());

	vector = AmfVector<AmfItem*>({}, "", true);
	expected = {
		0x10, 0x01, 0x01, 0x01
	};
	ASSERT_EQ(expected, vector.serialize());
}

TEST(VectorSerializationTest, VectorNamedObjectEmpty) {
	AmfVector<AmfItem*> vector({}, "TestObject", false);
	v8 expected {
		0x10, 0x01, 0x00,
		0x15, 0x54, 0x65, 0x73, 0x74, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74
	};

	ASSERT_EQ(expected, vector.serialize());
}

TEST(VectorSerializationTest, VectorAnonObject) {
	AmfVector<AmfItem*> vector({}, "", false);
	AmfObjectTraits traits("", true, false);
	AmfObject obj(traits);

	AmfString value("val");
	obj.dynamicProperties["prop"] = &value;
	vector.push_back(&obj);

	v8 expected {
		0x10, 0x03, 0x00,
		0x01,
		0x0a, 0x0b, 0x01,
		0x09, 0x70, 0x72, 0x6f, 0x70,
		0x06, 0x07, 0x76, 0x61, 0x6c, 0x01
	};
	ASSERT_EQ(expected, vector.serialize());
}

TEST(VectorSerializationTest, VectorNamedObject) {
	AmfVector<AmfItem*> vector({}, "TestObject", false);
	// we're using a different class name (e.g subclass) for the actual object
	// to prevent running into any string reference serialization
	AmfObjectTraits traits("TestObject2", false, false);
	traits.attributes.push_back("sealedProp");
	AmfObject obj(traits);

	AmfInteger val(0xbeef);
	obj.sealedProperties["sealedProp"] = &val;
	vector.push_back(&obj);

	v8 expected {
		// vector marker, 1 item, not fixed
		0x10, 0x03, 0x00,
		// class name "TestObject"
		0x15, 0x54, 0x65, 0x73, 0x74, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74,
		// object marker, not dynamic, 1 sealed property, class name 0x00???
		0x0a, 0x13,
		// object type "TestObject2"
		0x17, 0x54, 0x65, 0x73, 0x74, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x32,
		// property name sealedProp
		0x15, 0x73, 0x65, 0x61, 0x6c, 0x65, 0x64, 0x50, 0x72, 0x6f, 0x70,
		// value 0xbeef
		0x04, 0x82, 0xfd, 0x6f
	};
	ASSERT_EQ(expected, vector.serialize());
}

TEST(VectorSerializationTest, VectorLongLongNotConstructible) {
	static_assert(!std::is_constructible<AmfVector<long long>>::value,
		"AmfVector<long long> should not be constructible");

	SUCCEED();
}

TEST(VectorSerializationTest, VectorFloatNotConstructible) {
	static_assert(!std::is_constructible<AmfVector<float>>::value,
		"AmfVector<float> should not be constructible");

	SUCCEED();
}
