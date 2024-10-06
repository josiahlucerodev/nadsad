
//nadsad
#include "../testUtils.h"
#include <nadsad/nadsad.h>

int main() noexcept {
	natl::Option<natl::StringByteSize<10000>> simpleTestFile = nadsad::loadTestFile<10000>("integer/simpleTest.nadsad");
	if (simpleTestFile.doesNotHaveValue()) { 
		return 0; 
	}

	nadsad::ascii::LexicalInfo lexicalInfo;
	{
		natl::TestTimer test("lexical analysis");
		const natl::ConstAsciiStringView source = simpleTestFile.value().toStringView();
		lexicalInfo = nadsad::ascii::lexicalAnalysis(source);
	}

	nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> serializer{};
	natl::serializeWriteNamed(serializer, "lexical", lexicalInfo);
	natl::println(serializer.output());

	nadsad::ascii::Deserializer<natl::DummyDeserializeElementInfo, natl::DummyDeserializeErrorHandler> deserializer;
	const natl::Bool sourceProccessed = deserializer.addSource(serializer.output());
	if (sourceProccessed) {
		auto deserializerScopeExpect = deserializer.begin();
		if (deserializerScopeExpect.hasValue()) {

			auto deserializerEndError = deserializer.end(deserializerScopeExpect.value());
		}
	} else {
		nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> errorSerializer{};
		deserializer.serializeSourceProccessState(errorSerializer);
		natl::println(errorSerializer.output());
	}
}