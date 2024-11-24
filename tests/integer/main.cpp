
//nadsad
#include "../testUtils.h"
#include <natl/processing/serializationUtils.h>
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

	nadsad::ascii::Deserializer<natl::ErrorHandlingFlag::shorten, natl::DummyDeserializeElementInfo, natl::FullDeserializeErrorHandler<8>> deserializer;
	const natl::Bool sourceProccessed = deserializer.addSource(serializer.output());
	if (sourceProccessed) {
		auto deserializerScopeExpect = deserializer.begin();
		if (deserializerScopeExpect.hasValue()) {
			auto deserializerScope = deserializerScopeExpect.value();

			nadsad::ascii::LexicalInfo deserializedLexicalInfo;
			natl::String sourceDst;

			auto lexicalError = natl::deserializeReadNamedToDst<decltype(deserializer), nadsad::ascii::LexicalInfo>(
				deserializer, deserializerScope, "lexical", deserializedLexicalInfo, sourceDst);

			if(lexicalError.hasValue()) {
				nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> errorSerializer{};
				deserializer.serializeSourceProccessState(errorSerializer);
				natl::println(errorSerializer.output());
				natl::println(lexicalError.value().toMessage<natl::String256>());
				return 0;
			}

			auto deserializerEndError = deserializer.end(deserializerScopeExpect.value());
		}
	} else {
		nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> errorSerializer{};
		deserializer.serializeSourceProccessState(errorSerializer);


		natl::println(errorSerializer.output());
	}
}