
constexpr inline natl::ConstAsciiStringView natlTestFrom = "NadsadIntegerTest";

//own
#include "../testUtils.h"

constexpr natl::Bool oneIntegerTest() noexcept {
	return nadsad::testFile<
		nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty>, 
		nadsad::ascii::Deserializer<
			natl::ErrorHandlingFlag::shorten, 
			natl::DummyDeserializeElementInfo, 
			natl::FullDeserializeErrorHandler<8>>
	>("pretty assci serializer and deserializer", "integer/oneInteger.nadsada");
}

constexpr natl::Bool allIntegersTest() noexcept {
	return nadsad::testFile<
		nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty>,
		nadsad::ascii::Deserializer<
		natl::ErrorHandlingFlag::shorten,
		natl::DummyDeserializeElementInfo,
		natl::FullDeserializeErrorHandler<8>>
	>("pretty assci serializer and deserializer", "integer/allIntegers.nadsada");
}

constexpr natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, oneIntegerTest());
	natl::subTestAssert(test, allIntegersTest());
	return test;
}

int main() noexcept {
	tests();
}