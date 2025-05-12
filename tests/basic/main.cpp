
constexpr inline natl::ConstAsciiStringView natlTestFrom = "NadsadIntegerTest";

//own
#include "../testUtils.h"

template<typename Type>
using global_c = natl::SerializeGlobalComponent<Type>;

template<typename Serializer, typename Deserializer>
constexpr natl::Bool testPrimitives(natl::Path256 serializeOutput) noexcept {
	natl::Test test(natlTestFrom, "primitives", natl::TestType::leaf);

	constexpr natl::SerializeWriteFlag writeFlags = natl::SerializeWriteFlag::none;
	constexpr natl::DeserializeReadFlag readFlags = natl::DeserializeReadFlag::none;
	constexpr natl::CustomSerializeWriteFlag<Serializer> customWriteFlags = Serializer::defaultCustomWriteFlag;
	constexpr natl::CustomDeserializeReadFlag<Deserializer> customReadFlags = Deserializer::defaultCustomReadFlag;
	
	constexpr natl::i8 originalI8 = natl::Limits<natl::i8>::max();
	constexpr natl::i16 originalI16 = natl::Limits<natl::i16>::max();
	constexpr natl::i32 originalI32 = natl::Limits<natl::i32>::max();
	constexpr natl::i64 originalI64 = natl::Limits<natl::i64>::max();
	constexpr natl::ui8 originalUI8 = natl::Limits<natl::ui8>::max();
	constexpr natl::ui16 originalUI16 = natl::Limits<natl::ui16>::max();
	constexpr natl::ui32 originalUI32 = natl::Limits<natl::ui32>::max();
	constexpr natl::ui64 originalUI64 = natl::Limits<natl::ui64>::max();
	constexpr natl::f32 originalF32 = natl::f32(32.32);
	constexpr natl::f64 originalF64 = natl::f32(64.64);
	constexpr natl::Char originalChar = 'A';
	constexpr natl::Bool originalBool = true;

	Serializer serializer;
	if (auto err = serializer.begin(); err.hasValue()) {
		natl::testAssertFail(test, "could not begin serialization");
		natl::testAssertFail(test, err.value().template getMessage<natl::String256>());
		return false;
	}


	auto handleSeError = [&]<typename ErrorType>(const ErrorType& error) {
		if (error.hasValue()) {
			natl::testAssertFail(test, error.value().template getMessage<natl::String256>());
			return true;
		}
		return false;
	};

	//se serialize error
	auto seI8 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::i8>(serializer, "i8", originalI8);
	auto seI16 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::i16>(serializer, "i16", originalI16);
	auto seI32 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::i32>(serializer, "i32", originalI32);
	auto seI64 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::i64>(serializer, "i64", originalI64);
	auto seUI8 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::ui8>(serializer, "ui8", originalUI8);
	auto seUI16 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::ui16>(serializer, "ui16", originalUI16);
	auto seUI32 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::ui32>(serializer, "ui32", originalUI32);
	auto seUI64 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::ui64>(serializer, "ui64", originalUI64);
	auto seF32 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::f32>(serializer, "f32", originalF32);
	auto seF64 = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::f64>(serializer, "f64", originalF64);
	auto seChar = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::Char>(serializer, "char", originalChar);
	auto seBool = natl::serializeWriteNamedDefaultGlobal<Serializer, natl::Bool>(serializer, "bool", originalBool);

	if(handleSeError(seI8)) { return false; }
	if(handleSeError(seI16)) { return false; }
	if(handleSeError(seI32)) { return false; }
	if(handleSeError(seI64)) { return false; }
	if(handleSeError(seUI8)) { return false; }
	if(handleSeError(seUI16)) { return false; }
	if(handleSeError(seUI32)) { return false; }
	if(handleSeError(seUI64)) { return false; }
	if(handleSeError(seF32)) { return false; }
	if(handleSeError(seF64)) { return false; }
	if(handleSeError(seChar)) { return false; }
	if(handleSeError(seBool)) { return false; }

	if (auto err = serializer.end(); err.hasValue()) {
		natl::testAssertFail(test, "could not end serialization");
		natl::testAssertFail(test, err.value().template getMessage<natl::String256>());
		return false;
	}

	if (not natl::isConstantEvaluated()) {
		auto source = serializer.output();
		nadsad::outputInfoFile(serializeOutput, source.size(), reinterpret_cast<natl::Byte*>(source.data()));
	}

	Deserializer deserializer;
	const natl::Bool addSourceSuccess = deserializer.addSource(serializer.output());
	if (not addSourceSuccess) {
		natl::testAssertFail(test, "could not add source to deserializer");
		return false;
	}

	auto rootInfoExpect = deserializer.template begin<readFlags, customReadFlags>();
	if (rootInfoExpect.hasError()) {
		natl::testAssertFail(test, "could not begin deserialization");
		natl::testAssertFail(test, rootInfoExpect.error().template getMessage<natl::String256>());
		return false;
	}
	auto rootInfo = rootInfoExpect.value();

	natl::i8 readI8;
	natl::i16 readI16;
	natl::i32 readI32;
	natl::i64 readI64;
	natl::ui8 readUI8;
	natl::ui16 readUI16;
	natl::ui32 readUI32;
	natl::ui64 readUI64;
	natl::f32 readF32;
	natl::f64 readF64;
	natl::Char readChar;
	natl::Bool readBool;

	auto handleDeError = [&]<typename ValueExpect>(const ValueExpect & valueExpect) {
		if (valueExpect.hasError()) {
			natl::testAssertFail(test, valueExpect.error().template getMessage<natl::String256>());
			return true;
		}
		return false;
	};

	auto readI8Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::i8>(deserializer, rootInfo, "i8");
		if (handleDeError(readI8Expect)) { return false; }
	readI8 = readI8Expect.value();
	auto readI16Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::i16>(deserializer, rootInfo, "i16");
		if (handleDeError(readI16Expect)) { return false; }
	readI16 = readI16Expect.value();
	auto readI32Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::i32>(deserializer, rootInfo, "i32");
		if (handleDeError(readI32Expect)) { return false; }
	readI32 = readI32Expect.value();
	auto readI64Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::i64>(deserializer, rootInfo, "i64");
		if (handleDeError(readI64Expect)) { return false; }
	readI64 = readI64Expect.value();
	auto readUI8Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::ui8>(deserializer, rootInfo, "ui8");
		if (handleDeError(readUI8Expect)) { return false; }
	readUI8 = readUI8Expect.value();
	auto readUI16Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::ui16>(deserializer, rootInfo, "ui16");
		if (handleDeError(readUI16Expect)) { return false; }
	readUI16 = readUI16Expect.value();
	auto readUI32Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::ui32>(deserializer, rootInfo, "ui32");
		if (handleDeError(readUI32Expect)) { return false; }
	readUI32 = readUI32Expect.value();
	auto readUI64Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::ui64>(deserializer, rootInfo, "ui64");
		if (handleDeError(readUI64Expect)) { return false; }
	readUI64 = readUI64Expect.value();
	auto readF32Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::f32>(deserializer, rootInfo, "f32");
		if (handleDeError(readF32Expect)) { return false; }
	readF32 = readF32Expect.value();
	auto readF64Expect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::f64>(deserializer, rootInfo, "f64");
		if (handleDeError(readF64Expect)) { return false; }
	readF64 = readF64Expect.value();
	auto readCharExpect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::Char>(deserializer, rootInfo, "char");
		if (handleDeError(readCharExpect)) { return false; }
	readChar = readCharExpect.value();
	auto readBoolExpect = natl::deserializeReadNamedDefaultGlobal<Deserializer, natl::Bool>(deserializer, rootInfo, "bool");
	if(handleDeError(readBoolExpect)) { return false; }
	readBool = readBoolExpect.value();

	auto endErr = deserializer.template end<readFlags, customReadFlags>(rootInfo);
	if (endErr.hasValue()) {
		natl::testAssertFail(test, "could not end deserialization");
		natl::testAssertFail(test, endErr.value().template getMessage<natl::String256>());
		return false;
	}

	natl::testAssertEquals(test, originalI8, readI8, "type i8");
	natl::testAssertEquals(test, originalI16, readI16, "type i16");
	natl::testAssertEquals(test, originalI32, readI32, "type i32");
	natl::testAssertEquals(test, originalI64, readI64, "type i64");
	natl::testAssertEquals(test, originalUI8, readUI8, "type ui8");
	natl::testAssertEquals(test, originalUI16, readUI16, "type ui16");
	natl::testAssertEquals(test, originalUI32, readUI32, "type ui32");
	natl::testAssertEquals(test, originalUI64, readUI64, "type ui64");
	natl::testAssert(test, natl::math::isRoughtlyEqualF32(originalF32, readF32, 0.001), "type f32");
	natl::testAssert(test, natl::math::isRoughtlyEqualF64(originalF64, readF64, 0.001), "type f64");
	natl::testAssertEquals(test, originalChar, readChar, "type char");
	natl::testAssertEquals(test, originalBool, readBool, "type bool");

	return test;
}


constexpr natl::Bool asciiOneIntegerTest() noexcept {
	return nadsad::testFile<
		nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>,
		nadsad::ascii::Deserializer<
			natl::DeserializeFlags::shortenError, 
			natl::DummyDeserializeElementInfo, 
			natl::FullDeserializeErrorHandler<8>>
	>("assci format one integer", "basic/oneInteger.nadsada");
}
constexpr natl::Bool asciiAllIntegersTest() noexcept {
	return nadsad::testFile<
		nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>,
		nadsad::ascii::Deserializer<
			natl::DeserializeFlags::shortenError,
			natl::DummyDeserializeElementInfo,
			natl::FullDeserializeErrorHandler<8>>
	>("assci all integer types test", "basic/allIntegers.nadsada");
}

constexpr natl::Bool asciiTest() {
	using serializer = nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>;
	using deserializer = nadsad::ascii::Deserializer<
		natl::DeserializeFlags::shortenError,
		natl::DummyDeserializeElementInfo,
		natl::FullDeserializeErrorHandler<8>>;

	natl::Test test(natlTestFrom, "ascii", natl::TestType::node);
	natl::subTestAssert(test, asciiOneIntegerTest());
	natl::subTestAssert(test, asciiAllIntegersTest());
	natl::subTestAssert(test, testPrimitives<serializer, deserializer>(
		natl::String(nadsad::getTestResourcePath()) += "/basic/bin/primitives.nadsada"));
	return test;
}

constexpr natl::Bool testUnalignedBinary() {
	natl::Test test(natlTestFrom, "unaligned binary", natl::TestType::node);

	using serializer = nadsad::binary::UnalignedSerializer<1024, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>;
	using deserializer = nadsad::binary::UnalignedDeserializer<
		natl::DeserializeFlags::shortenError,
		natl::DummyDeserializeElementInfo,
		natl::FullDeserializeErrorHandler<8>>;

	natl::subTestAssert(test, testPrimitives<serializer, deserializer>(
		natl::String(nadsad::getTestResourcePath()) += "/basic/bin/primitives.nadsadub"));

	return test;
}

constexpr natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, asciiTest());
	natl::subTestAssert(test, testUnalignedBinary());
	return test;
}

int main() noexcept {
	tests();
}