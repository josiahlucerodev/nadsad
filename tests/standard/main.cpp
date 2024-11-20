
//nadsad
#include "../testUtils.h"
#include <natl/processing/serializationUtils.h>
#include <nadsad/nadsad.h>



int main() noexcept {
	natl::Path testFilePath = nadsad::getTestResourcePath();
	testFilePath /= "standard/integer.nadsad";

	natl::File testFile(testFilePath.c_str(), natl::FileOpenMode::readStart);
	natl::StringByteSize<10000> fileContents;
	const natl::LoadAllFileContentError error = natl::loadAllFileContent(testFile, fileContents);

	if (error != natl::LoadAllFileContentError::none) {
		natl::printlnf("failed to load ", testFilePath.toStringView(), ": ", natl::loadAllFileContentErrorToString(error));
		return 0;
	} 

	nadsad::ascii::LexicalInfo lexicalInfo;
	{
		natl::TestTimer test("lexical analysis");
		const natl::ConstAsciiStringView source = fileContents.toStringView();
		lexicalInfo = nadsad::ascii::lexicalAnalysis(source);
	}

	nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> serializer{};
	natl::serializeWriteNamed(serializer, "lexical", lexicalInfo);
	natl::println(serializer.output());

	nadsad::ascii::Deserializer<natl::DummyDeserializeElementInfo, natl::FullDeserializeErrorHandler<8>> deserializer;
	const natl::Bool sourceProccessed = deserializer.addSource(serializer.output());
	if(sourceProccessed) {
		auto deserializerScopeExpect = deserializer.begin();
		if(deserializerScopeExpect.hasValue()) {

			deserializer.end(deserializerScopeExpect.value());
		}
	} else {
		nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> errorSerializer{};
		deserializer.serializeSourceProccessState(errorSerializer);
		natl::println(errorSerializer.output());
	}
}