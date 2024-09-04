
//nadsad
#include "../testUtils.h"
#include <nadsad/nadsad.h>



int main() noexcept {
	natl::Path testFilePath = nadsad::getTestResourcePath();
	testFilePath /= "standard/sample.nadsad";

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
	natl::concatTemplateStringLiterals<"32 ", "42">();
	nadsad::ascii::Serializer<1000, natl::SerializeFlag::pretty> serializer{};
	natl::serializeWriteNamed(serializer, "lexical", lexicalInfo);
	natl::println(serializer.output());
}