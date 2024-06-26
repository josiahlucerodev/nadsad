//nadsad
#include <nadsad/nadsad.h>


int main() {
	natl::ConstAsciiStringView sources = "the ui32: {}";
	nadsad::ascii::LexicalInfo lexicalInfo = nadsad::ascii::lexicalAnalysis(sources);

	nadsad::ascii::Serializer<1000, natls::Flag::pretty> serializer{};
	natls::write("lexical", serializer, lexicalInfo);
	natl::println(serializer.output());
}