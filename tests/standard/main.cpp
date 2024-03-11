//nadsad
#include <nadsad/nadsad.h>

int main() {
	nadsad::AssciFormatSerializer serlizer;

	natl::SerializationStructType vec3Type(
		"vec3",
		{
			natl::SerializationStructTypeElement("x", natl::SerializationBasicType::dt_f32),
			natl::SerializationStructTypeElement("y", natl::SerializationBasicType::dt_f32),
			natl::SerializationStructTypeElement("z", natl::SerializationBasicType::dt_f32)
		}
	);
	serlizer.serializeBegin(10000);

	serlizer.serializeStructType(vec3Type);

	serlizer.serializeNamedElement<natl::SerializationTypeFlag::dt_tstruct>("pos");
	serlizer.serializeNamedTstructInfo(vec3Type);

	serlizer.serializeTypedStruct(vec3Type);
	serlizer.serializeNamedElement<natl::SerializationTypeFlag::dt_i8>("x"); serlizer.serialize_i8(2);
	serlizer.serializeNamedElement<natl::SerializationTypeFlag::dt_i8>("y"); serlizer.serialize_i8(2);
	serlizer.serializeNamedElement<natl::SerializationTypeFlag::dt_i8>("z"); serlizer.serialize_i8(2);
	serlizer.endSerializeTypedStruct();

	serlizer.serializeEnd();
	
	auto moveOutput = serlizer.moveOutputAsAssci();
	natl::String source = natl::move(moveOutput);
	std::cout << source;

	//natl::DynArray<natl::AssciCode> output(serlizer.moveOutput());

	nadsad::AssciFormatTokenStream tokenStream;
	nadsad::AssciFromatLexicalErrorStream errorStream;
	nadsad::assciFromatLexicalAnalysis(source.toStringView(), tokenStream, errorStream);

	nadsad::AssciFormatSerializer lexicalSerializer;
	lexicalSerializer.serializeBegin();
	nadsad::serializeAssicTokenStream<nadsad::AssciFormatSerializer<>>(lexicalSerializer, tokenStream, errorStream);
	lexicalSerializer.serializeEnd();

	std::cout << "\n";
	natl::String lexicalAnalysisOutput = natl::move(lexicalSerializer.moveOutputAsAssci());
	std::cout << lexicalAnalysisOutput;

}