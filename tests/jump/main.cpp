
constexpr inline natl::ConstAsciiStringView natlTestFrom = "NadsadJumpTest";

//nadsad
#include "../testUtils.h"

#define NADSAD_TEST_SKIP(elementName, elementType, errorName)  \
	auto errorName = natl::deserializeSkip<custom_deserializer, readFlag, customReadFlag, natl::SerializeGlobalComponent, elementType>( \
		deserializer, globalScope, elementName); \
	natl::testAssert(test, !errorName.hasValue(), natl::String256("skip ") + elementName); \
	if (errorName.hasValue()) { \
		natl::println(errorName.value().toMessage<natl::String256>()); \
		return test; \
	} 

natl::Bool allIntegersJumpTest() noexcept {
	using custom_serializer = nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty>;
	using custom_deserializer = nadsad::ascii::Deserializer<
		natl::DeserializeFlags::shortenError,
		natl::DummyDeserializeElementInfo,
		natl::FullDeserializeErrorHandler<8>>;
	constexpr natl::DeserializeReadFlag readFlag = natl::DeserializeReadFlag::none;
	constexpr auto customReadFlag = natl::DefaultCustomDeserializeReadFlag<custom_deserializer>;

	natl::Test test;
	natl::String fileContent{};
	nadsad::ascii::LexicalInfo lexicalInfo{};
	custom_serializer serializer{};
	nadsad::baseTestFile<custom_serializer, custom_deserializer>(test, fileContent, lexicalInfo, serializer,
		"pretty assci serializer and deserializer", "jump/allIntegersJump.nadsada");

	custom_deserializer deserializer;
	auto processedSource = deserializer.addSource(fileContent.toArrayView());
	if (!processedSource) {
		natl::testAssert(test, false, "process source");
	}

	auto globalScopeExpect = deserializer.begin<readFlag, customReadFlag>();
	natl::testAssert(test, globalScopeExpect.hasValue(), "begin deserializer");
	if (globalScopeExpect.hasError()) {
		natl::println(globalScopeExpect.error().toMessage<natl::String256>());
		return test;
	}
	auto globalScope = globalScopeExpect.value();

	auto jumpTableExpect = natl::deserializeReadJumpTable<
		custom_deserializer, readFlag, customReadFlag, natl::SerializeGlobalComponent,
		10, natl::ui64
	>(deserializer, globalScope, "jumpTable");
	natl::testAssert(test, jumpTableExpect.hasValue(), "read jump table");
	if (jumpTableExpect.hasError()) {
		natl::println(jumpTableExpect.error().toMessage<natl::String256>());
		return test;
	}
	auto jumpTable = jumpTableExpect.value();

	natl::testAssert(test, jumpTable.jumps.size() == 3, "three jump locations");
	if (jumpTable.jumps.size() != 3) {
		return test;
	}

	natl::testAssert(test, jumpTable.jumps[0].jumpId.name == "pos1", "read jump pos1");
	natl::testAssert(test, jumpTable.jumps[1].jumpId.name == "pos2", "read jump pos2");
	natl::testAssert(test, jumpTable.jumps[2].jumpId.name == "pos3", "read jump pos3");	

	auto pos1JumpError = deserializer.jumpTo<readFlag, customReadFlag, natl::SerializeGlobalComponent, natl::ui64>(
		jumpTable.info, jumpTable.jumps[0], true);
	natl::testAssert(test, !pos1JumpError.hasValue(), "jump to pos1");
	if (pos1JumpError.hasValue()) {
		natl::println(pos1JumpError.value().toMessage<natl::String256>());
		return test;
	}

	auto pos2JumpError = deserializer.jumpTo<readFlag, customReadFlag, natl::SerializeGlobalComponent, natl::ui64>(
		jumpTable.info, jumpTable.jumps[1], true);
	natl::testAssert(test, !pos2JumpError.hasValue(), "jump to pos2");
	if (pos2JumpError.hasValue()) {
		natl::println(pos2JumpError.value().toMessage<natl::String256>());
		return test;
	}

	auto pos3JumpError = deserializer.jumpTo<readFlag, customReadFlag, natl::SerializeGlobalComponent, natl::ui64>(
		jumpTable.info, jumpTable.jumps[2], true);
	natl::testAssert(test, !pos3JumpError.hasValue(), "jump to pos3");
	if (pos3JumpError.hasValue()) {
		natl::println(pos3JumpError.value().toMessage<natl::String256>());
		return test;
	}
	
	auto deserializerEndError = deserializer.end<readFlag, customReadFlag>(globalScope);
	natl::testAssert(test, !deserializerEndError.hasValue(), "end deserializer");
	if (deserializerEndError.hasValue()) {
		natl::println(deserializerEndError.value().toMessage<natl::String256>());
		return test;
	}

	auto pos1JumpBackError = deserializer.jumpTo<readFlag, customReadFlag, natl::SerializeGlobalComponent, natl::ui64>(
		jumpTable.info, jumpTable.jumps[0], true);
	natl::testAssert(test, !pos1JumpBackError.hasValue(), "jump back to pos1");
	if (pos1JumpBackError.hasValue()) {
		natl::println(pos1JumpBackError.value().toMessage<natl::String256>());
		return test;
	}

	NADSAD_TEST_SKIP("i8", natl::i8, skipI8);
	NADSAD_TEST_SKIP("i16", natl::i16, skipI16);
	NADSAD_TEST_SKIP("i32", natl::i32, skipI32);
	NADSAD_TEST_SKIP("i64", natl::i64, skipI64);

	NADSAD_TEST_SKIP("ui8", natl::ui8, skipUI8);
	NADSAD_TEST_SKIP("ui16", natl::ui16, skipUI16);
	NADSAD_TEST_SKIP("ui32", natl::ui32, skipUI32);
	NADSAD_TEST_SKIP("ui64", natl::ui64, skipUI64);

	NADSAD_TEST_SKIP("f32", natl::f32, skipF32);
	NADSAD_TEST_SKIP("f64", natl::f64, skipF64);

	NADSAD_TEST_SKIP("option", natl::Option<natl::ui64>, skipOption);
	NADSAD_TEST_SKIP("emptyOption", natl::Option<natl::ui64>, skipEmptyOption);

	NADSAD_TEST_SKIP("farray", natl::DynArray<natl::ui64>, skipFarray);
	NADSAD_TEST_SKIP("array", natl::DynArray<natl::ui64>, skipArray);
	using dic_type = natl::FlatHashMap<natl::ui64, natl::ui64>;
	NADSAD_TEST_SKIP("dic", dic_type, skipDic);
	NADSAD_TEST_SKIP("struct", nadsad::ascii::TokenScopeUnit, skipStruct);
	NADSAD_TEST_SKIP("variant", nadsad::ascii::AdditionalTokenInfoVariant, skipVariant);
	NADSAD_TEST_SKIP("emptyVariant", nadsad::ascii::AdditionalTokenInfoVariant, skipEmptyVariant);

	auto pos2ReadError = deserializer.readJumpLocation<readFlag, customReadFlag, natl::SerializeGlobalComponent, natl::ui64>(
		jumpTable.info, jumpTable.jumps[1]);
	natl::testAssert(test, !pos2ReadError.hasValue(), "jump location pos2 read");
	if (pos2ReadError.hasValue()) {
		natl::println(pos2ReadError.value().toMessage<natl::String256>());
		return test;
	}

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