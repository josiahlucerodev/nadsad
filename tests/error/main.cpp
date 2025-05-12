
//natl
#include <natl/serialization/error.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NadsadErrorTest";

//own
#include "../testUtils.h"


constexpr natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	
	return test;
}

int main() noexcept {
	using Serializer = natl::SerializerEnhancedError<
		nadsad::ascii::Serializer,
		1024, natl::SerializeFlags::pretty, natl::FullSerializeErrorHandler<10>>;

	return nadsad::testFile<
		Serializer,
		nadsad::ascii::Deserializer<
		natl::DeserializeFlags::shortenError,
		natl::DummyDeserializeElementInfo,
		natl::FullDeserializeErrorHandler<8>>
		>("pretty assci serializer and deserializer", "integer/allIntegers.nadsada");

	tests();
}