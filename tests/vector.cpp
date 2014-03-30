#include "amftest.hpp"

#include "amf.hpp"
#include "types/amfobject.hpp"
#include "types/amfvector.hpp"

TEST(VectorSerializationTest, VectorIntEmpty) {
	AmfVector<int> vec { {}, false };
	v8 expected { 0x0d, 0x01, 0x00 };
	isEqual(expected, vec);

	vec = { {}, true };
	expected = { 0x0d, 0x01, 0x01 };
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorIntSimple) {
	AmfVector<int> vec { { 1, 2, 3 }, false };
	v8 expected {
		0x0d, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03
	};
	isEqual(expected, vec);

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
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorInt32) {
	AmfVector<int> vec { {
		0x20000000,
		0x40000000,
		0x7fffffff
	}, true };
	v8 expected {
		0x0d, 0x07, 0x01,
		0x20, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xff
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorIntNegative) {
	AmfVector<int> vec { { -1, -2, -0xffff }, false };
	v8 expected {
		0x0d, 0x07, 0x00,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xfe,
		0xff, 0xff, 0x00, 0x01
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorInt32Negative) {
	AmfVector<int> vec { {
		-0x20000000,
		-0x40000000,
		-0x7fffffff,
		-2147483647 - 1
	}, true };
	v8 expected {
		0x0d, 0x09, 0x01,
		0xe0, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorIntFixedDefault) {
	AmfVector<int> vec { {1, 3, 5} };
	v8 expected {
		0x0d, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x05
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorInt2ByteLength) {
	std::vector<int> num(260);
	for (int i = 0; i < 260; ++i) num[i] = i;

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
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorUintEmpty) {
	AmfVector<unsigned int> vec { {}, false };
	v8 expected { 0x0e, 0x01, 0x00 };
	isEqual(expected, vec);

	vec = {{}, true};
	expected = { 0x0e, 0x01, 0x01 };
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorUintSimple) {
	AmfVector<unsigned int> vec { { 1, 2, 3 }, false };
	v8 expected {
		0x0e, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03
	};
	isEqual(expected, vec);

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
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorUint32) {
	AmfVector<unsigned int> vec { {
		0x20000000u,
		0x40000000u,
		0x7fffffffu,
		0x80000000u,
		0xffffffffu
	}, true };
	v8 expected {
		0x0e, 0x0b, 0x01,
		0x20, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xff,
		0x80, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorUintFixedDefault) {
	AmfVector<unsigned int> vec { {1, 3, 5} };
	v8 expected {
		0x0e, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x05
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorDoubleEmpty) {
	AmfVector<double> vec { {}, false };
	v8 expected { 0x0f, 0x01, 0x00 };
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorDoubleSimple) {
	AmfVector<double> vec { {0}, true };
	v8 expected {
		0x0f, 0x03, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	isEqual(expected, vec);

	vec = {{0.5}, false};
	expected = {
		0x0f, 0x03, 0x00,
		0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	isEqual(expected, vec);

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
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorDoubleFixedDefault) {
	AmfVector<double> vec { {3.14159} };
	v8 expected {
		0x0f, 0x03, 0x00,
		0x40, 0x09, 0x21, 0xf9, 0xf0, 0x1b, 0x86, 0x6e
	};
	isEqual(expected, vec);
}

TEST(VectorSerializationTest, VectorUtilityFunctions) {
	AmfVector<int> vector { {}, false };
	vector.push_back(1);
	v8 expected {
		0x0d, 0x03, 0x00,
		0x00, 0x00, 0x00, 0x01,
	};
	isEqual(expected, vector);
}

TEST(VectorSerializationTest, VectorAnonObjectEmpty) {
	AmfVector<AmfObject> vector { {}, "", false };
	v8 expected {
		0x10, 0x01, 0x00, 0x01
	};
	isEqual(expected, vector);

	vector = AmfVector<AmfObject>({}, "", true);
	expected = {
		0x10, 0x01, 0x01, 0x01
	};
	isEqual(expected, vector);
}

TEST(VectorSerializationTest, VectorNamedObjectEmpty) {
	AmfVector<AmfObject> vector { {}, "TestObject", false };
	v8 expected {
		0x10, 0x01, 0x00,
		0x15, 0x54, 0x65, 0x73, 0x74, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74
	};

	isEqual(expected, vector);
}

TEST(VectorSerializationTest, VectorAnonObject) {
	AmfVector<AmfObject> vector { {}, "", false };
	AmfObject obj("", true, false);

	obj.addDynamicProperty("prop", AmfString("val"));
	vector.push_back(obj);

	v8 expected {
		0x10, 0x03, 0x00,
		0x01,
		0x0a, 0x0b, 0x01,
		0x09, 0x70, 0x72, 0x6f, 0x70,
		0x06, 0x07, 0x76, 0x61, 0x6c, 0x01
	};
	isEqual(expected, vector);
}

TEST(VectorSerializationTest, VectorNamedObject) {
	AmfVector<AmfObject> vector { {}, "TestObject", false };
	// we're using a different class name (e.g subclass) for the actual object
	// to prevent running into any string reference serialization
	AmfObject obj("TestObject2", false, false);

	obj.addSealedProperty("sealedProp", AmfInteger(0xbeef));
	vector.push_back(obj);

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
	isEqual(expected, vector);
}

TEST(VectorTypeTest, VectorIntConstructible) {
	static_assert(std::is_constructible<AmfVector<int>>::value,
		"AmfVector<int> should be constructible");

	SUCCEED();
}

// disable these tests on Windows for now until I figured out a way to make them work
#ifndef _WIN32
TEST(VectorTypeTest, VectorLongLongNotConstructible) {
	static_assert(!std::is_constructible<AmfVector<long long>>::value,
		"AmfVector<long long> should not be constructible");

	SUCCEED();
}

TEST(VectorTypeTest, VectorFloatNotConstructible) {
	static_assert(!std::is_constructible<AmfVector<float>>::value,
		"AmfVector<float> should not be constructible");

	SUCCEED();
}
#endif

TEST(VectorEquality, IntVector) {
	AmfVector<int> vec { { 1, 2, 3 }, false };
	AmfVector<int> vec2 { { 1, 2, 3 }, false };
	EXPECT_EQ(vec, vec2);

	AmfVector<int> vec3 { { 1, 2, 3 }, true };
	EXPECT_NE(vec, vec3);

	AmfVector<int> vec4 { { -1, 2, 3 }, true };
	EXPECT_NE(vec, vec4);

	AmfVector<int> vec5 { { 1, 2, 4 }, false };
	EXPECT_NE(vec, vec5);

	AmfVector<int> vec6 { { 1, 2, 4 }, true };
	EXPECT_NE(vec, vec6);

	AmfVector<int> vec7 { { 1, 2, 3, 4, 5 }, false };
	EXPECT_NE(vec, vec7);

	AmfVector<int> vec8 { { 1, 2 }, false };
	EXPECT_NE(vec, vec8);

	AmfVector<int> vec9 { {}, false };
	AmfVector<int> vec10;
	EXPECT_EQ(vec9, vec10);
}

TEST(VectorEquality, UIntVector) {
	AmfVector<unsigned int> vec { { 1, 2, 3 }, false };
	AmfVector<unsigned int> vec2 { { 1, 2, 3 }, false };
	EXPECT_EQ(vec, vec2);

	AmfVector<unsigned int> vec3 { { 1, 2, 3 }, true };
	EXPECT_NE(vec, vec3);

	AmfVector<unsigned int> vec4 { { 1 }, true };
	EXPECT_NE(vec, vec4);

	AmfVector<unsigned int> vec5 { { 1, 2, 4 }, false };
	EXPECT_NE(vec, vec5);

	AmfVector<unsigned int> vec6 { { 1, 2, 4 }, true };
	EXPECT_NE(vec, vec6);
}

TEST(VectorEquality, DoubleVector) {
	AmfVector<double> vec { { 3.14159, 1.0, -0.5 } };
	AmfVector<double> vec2 { { 3.14159, 1.0, -0.5 } };
	EXPECT_EQ(vec, vec2);

	AmfVector<double> vec3 { { 3.14159, 1.0, -0.5 }, true };
	EXPECT_NE(vec, vec3);

	AmfVector<double> vec4 { { 3.141591, 1.0, -0.5 } };
	EXPECT_NE(vec, vec4);

	AmfVector<double> vec5 { { 3.141590, 1.000, -0.5 } };
	EXPECT_EQ(vec, vec5);

	AmfVector<double> vec6 { { 3.141590, 1.000, 0.5 } };
	EXPECT_NE(vec, vec6);
}

TEST(VectorEquality, VectorIntVector) {
	AmfVector<AmfVector<int>> vec { {}, "", false };
	vec.push_back(AmfVector<int> { { 1, 2, 3}, false });

	AmfVector<int> inner { { 1, 2, 3 }, false };
	AmfVector<AmfVector<int>> vec2 { { inner }, "", false };
	EXPECT_EQ(vec, vec2);

	AmfVector<int> inner2 { { 1, 2, 4 }, false };
	AmfVector<AmfVector<int>> vec3 { { inner2 }, "", false };
	EXPECT_NE(vec, vec3);

	AmfVector<AmfVector<int>> vec4 { { inner }, "", true };
	EXPECT_NE(vec, vec4);

	AmfVector<AmfVector<int>> vec5 { { inner }, "asd", false };
	EXPECT_NE(vec, vec5);

	AmfVector<AmfVector<int>> vec6 { { AmfVector<int> { {1, 2, 3 }, false } }, "", false };
	EXPECT_EQ(vec, vec6);
}

TEST(VectorEquality, ObjectVector) {
	AmfObject obj;
	AmfVector<AmfObject> v1 { { obj }, "", false };
	AmfVector<AmfObject> v2 { { obj }, "", false };
	EXPECT_EQ(v1, v2);

	AmfVector<AmfObject> v3(v2);
	EXPECT_EQ(v2, v3);

	AmfVector<AmfObject> v4 { { }, "" };
	v4.push_back(obj);
	EXPECT_EQ(v3, v4);

	AmfVector<AmfObject> v5 { { obj }, "", true };
	EXPECT_NE(v1, v5);

	AmfVector<AmfObject> v6 { { obj }, "x", false };
	EXPECT_NE(v1, v6);

	AmfVector<AmfObject> v7 { { }, "", false };
	EXPECT_NE(v1, v7);
}

TEST(VectorEquality, MixedTypes) {
	AmfVector<int> v1 { { 1 } };
	AmfVector<unsigned int> v2 { { 1 } };
	EXPECT_NE(v1, v2);

	AmfVector<AmfInteger> v3 { { AmfInteger(1) }, "", false };
	EXPECT_NE(v1, v3);

	AmfVector<double> v4 { { 1.0 } };
	EXPECT_NE(v1, v4);
}

TEST(VectorDeserialization, VectorInt) {
	AmfVector<int> vec { { 1, 2, 3 }, false };
	v8 data {
		0x0d, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03
	};

	deserialize(vec, data, 0);
}

TEST(VectorDeserialization, VectorIntEmpty) {
	AmfVector<int> vec { {}, false };
	v8 data { 0x0d, 0x01, 0x00 };

	deserialize(vec, data);
}

TEST(VectorDeserialization, VectorIntFixed) {
	AmfVector<int> vec { { 1, 2, 3 }, true };
	v8 data {
		0x0d, 0x07, 0x01,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03,
		0xff, 0xff, 0xff
	};

	deserialize(vec, data, 3);
}

TEST(VectorDeserialization, VectorNegativeInt) {
	AmfVector<int> vec { { -1, -2, -3 }, false };
	v8 data {
		0x0d, 0x07, 0x00,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xfe,
		0xff, 0xff, 0xff, 0xfd
	};

	deserialize(vec, data);
}

TEST(VectorDeserialization, VectorInt32) {
	AmfVector<int> vec { {
		0x20000000,
		0x40000000,
		0x7fffffff,
		-0x7fffffff
	}, true };
	v8 data {
		0x0d, 0x09, 0x01,
		0x20, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xff,
		0x80, 0x00, 0x00, 0x01
	};
	std::cout << ::testing::PrintToString(vec.serialize()) << std::endl;
	deserialize(vec, data);
}

TEST(VectorDeserialization, VectorIntMultiByteLength) {
	AmfVector<int> vec { {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
		22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
		60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
		79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
		98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
		114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
		129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
		144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
		159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
		174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
		189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
		204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
		219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
		234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
		249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259
	}, true };

	v8 data {
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
	deserialize(vec, data);
}

TEST(VectorDeserialization, VectorUint) {
	AmfVector<unsigned int> vec { { 1, 2, 3 }, false };
	v8 data {
		0x0e, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x03
	};

	deserialize(vec, data, 0);
}

TEST(VectorDeserialization, VectorUint32) {
	AmfVector<unsigned int> vec { { 0x7fffffff, 0x80000000, 0xffffffff }, false };
	v8 data {
		0x0e, 0x07, 0x00,
		0x7f, 0xff, 0xff, 0xff,
		0x80, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff
	};

	deserialize(vec, data);
}

TEST(VectorDeserialization, VectorDouble) {
	AmfVector<double> vec { { 0.5, -1.2 }, false };
	v8 data {
		0x0f, 0x05, 0x00,
		0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xbf, 0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33
	};

	deserialize(vec, data);
}

TEST(VectorDeserialization, ObjectCache) {
	DeserializationContext ctx;

	deserialize(AmfVector<int>({}, false), { 0x0d, 0x01, 0x00 }, 0, &ctx);
	deserialize(AmfVector<int>({}, false), { 0x0d, 0x00 }, 0, &ctx);
	deserialize(AmfVector<unsigned int>({}, false), { 0x0e, 0x01, 0x00 }, 0, &ctx);
	deserialize(AmfVector<double>({}, false), { 0x0f, 0x01, 0x00 }, 0, &ctx);
	deserialize(AmfVector<int>({}, false), { 0x0d, 0x00 }, 0, &ctx);
	deserialize(AmfVector<unsigned int>({}, false), { 0x0e, 0x02 }, 0, &ctx);
	deserialize(AmfVector<double>({}, false), { 0x0f, 0x04, 0xff }, 1, &ctx);
}

TEST(VectorDeserialization, VectorNumberEmptyIterator) {
	v8 data { };
	auto it = data.cbegin();
	auto end = data.cend();
	DeserializationContext ctx;
	EXPECT_THROW(AmfVector<int>::deserialize(it, end, ctx), std::invalid_argument);
	EXPECT_EQ(it, end);
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, end, ctx), std::invalid_argument);
	EXPECT_EQ(it, end);
	EXPECT_THROW(AmfVector<double>::deserialize(it, end, ctx), std::invalid_argument);
	EXPECT_EQ(it, end);
}

TEST(VectorDeserialization, VectorIntNotEnoughData) {
	DeserializationContext ctx;

	v8 data { 0x0d };
	auto it = data.cbegin();
	EXPECT_THROW(AmfVector<int>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0d, 0x01 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<int>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0d, 0x03, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<int>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0d, 0x03, 0x00, 0x00, 0x00, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<int>::deserialize(it, data.cend(), ctx), std::out_of_range);
}

TEST(VectorDeserialization, VectorUintNotEnoughData) {
	DeserializationContext ctx;

	v8 data { 0x0e };
	auto it = data.cbegin();
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0e, 0x01 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0e, 0x03, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, data.cend(), ctx), std::out_of_range);
}

TEST(VectorDeserialization, VectorDoubleNotEnoughData) {
	DeserializationContext ctx;

	v8 data { 0x0f };
	auto it = data.cbegin();
	EXPECT_THROW(AmfVector<double>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0f, 0x01 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<double>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0f, 0x03, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<double>::deserialize(it, data.cend(), ctx), std::out_of_range);

	data = { 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<double>::deserialize(it, data.cend(), ctx), std::out_of_range);
}

TEST(VectorDeserialization, VectorNumberInvalidMarker) {
	DeserializationContext ctx;

	v8 data { 0x0d, 0x01, 0x00 };
	auto it = data.cbegin();
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, data.cend(), ctx), std::invalid_argument);
	EXPECT_THROW(AmfVector<double>::deserialize(it, data.cend(), ctx), std::invalid_argument);

	data = { 0x0e, 0x01, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<int>::deserialize(it, data.cend(), ctx), std::invalid_argument);
	EXPECT_THROW(AmfVector<double>::deserialize(it, data.cend(), ctx), std::invalid_argument);

	data = { 0x0f, 0x01, 0x00 };
	it = data.cbegin();
	EXPECT_THROW(AmfVector<int>::deserialize(it, data.cend(), ctx), std::invalid_argument);
	EXPECT_THROW(AmfVector<unsigned int>::deserialize(it, data.cend(), ctx), std::invalid_argument);
}
