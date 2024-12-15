
constexpr inline natl::ConstAsciiStringView natlTestFrom = "NadsadJumpTest";

//nadsad
#include "../testUtils.h"

constexpr natl::Bool allIntegersJumpTest() noexcept {
	using custom_serializer = nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty>;
	using custom_deserializer = nadsad::ascii::Deserializer<
		natl::ErrorHandlingFlag::shorten,
		natl::DummyDeserializeElementInfo,
		natl::FullDeserializeErrorHandler<8>>;

	natl::Test test;
	natl::String fileContent{};
	nadsad::ascii::LexicalInfo lexicalInfo{};
	custom_serializer serializer{};
	nadsad::baseTestFile<custom_serializer, custom_deserializer>(test, fileContent, lexicalInfo, serializer,
		"pretty assci serializer and deserializer", "jump/allIntegersJump.nadsada");

	return test;
}

constexpr natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, allIntegersJumpTest());
	return test;
}

int main() {
	tests();
}