#pragma once 

//natl
#include<natl/fundamental/option.h>
#include<natl/container/string.h>
#include<natl/system/filesystem.h>
#include <natl/serialization/utils.h>
#include <natl/util/test.h>

//nadsad
#include <nadsad/nadsad.h>

//interface
namespace nadsad {
	inline const char* getTestResourcePath() noexcept {
		return NADSAD_TEST_RESOURCE_PATH;
	}

	natl::Option<natl::String> loadTestFile(
		const natl::ConstAsciiStringView& relitivePath) noexcept {
		natl::Path testFilePath = nadsad::getTestResourcePath();
		testFilePath /= relitivePath;

		natl::File testFile(testFilePath.c_str(), natl::FileOpenMode::readStart);
		natl::String fileContents;
		const natl::LoadAllFileContentError error = natl::loadAllFileContent(testFile, fileContents);

		if (error != natl::LoadAllFileContentError::none) {
			natl::printlnf("nadsad: failed to load ", testFilePath.toStringView(), ": ", natl::loadAllFileContentErrorToString(error));
			return {};
		}
		return fileContents;
	}

	void outputErrorFile(const natl::ConstAsciiStringView& relitivePath, natl::Size size, natl::Byte* src) noexcept {
		natl::FileHandle file = natl::openFile(relitivePath.c_str(), natl::FileOpenMode::writeDestroy);
		natl::writeFile(file,
			natl::FileOffset(0),
			natl::FileCount(size),
			src);
		natl::closeFile(file);
	}

	void outputInfoFile(const natl::ConstAsciiStringView& relitivePath, natl::Size size, natl::Byte* src) noexcept {
		natl::FileHandle file = natl::openFile(relitivePath.c_str(), natl::FileOpenMode::writeDestroy);
		natl::writeFile(file,
			natl::FileOffset(0),
			natl::FileCount(size),
			src);
		natl::closeFile(file);
	}

	template<typename Serializer, typename Deserializer>
	constexpr natl::Bool baseTestFile(natl::Test& testDst, natl::String& fileContent,
		nadsad::ascii::LexicalInfo& lexicalInfoDst, Serializer& serializer,
		const natl::String& testName, const natl::String& fileName) noexcept {
		auto file = nadsad::loadTestFile(fileName);
		if (file.doesNotHaveValue()) {
			return false;
		}
		natl::construct(&testDst, natlTestFrom, testName + " with file " + fileName, natl::TestType::leaf);

		{
			fileContent = move(file.value());
			const natl::ConstAsciiStringView source = fileContent.toStringView();
			lexicalInfoDst = nadsad::ascii::lexicalAnalysis(source);
		}

		auto beginError = serializer.begin();
		if (beginError.hasValue()) { return false; }

		auto writeError = natl::serializeWriteNamed<Serializer, natl::SerializeWriteFlag::none, 
			typename Serializer::custom_write_flag_type{},  natl::SerializeGlobalComponent<nadsad::ascii::LexicalInfo>>(
				serializer, "lexical", lexicalInfoDst);
		if (writeError.hasValue()) { 
			natl::printlnf("Serialize error:\n", writeError.value().template getMessage<natl::String>());
			return false; 
		}
		auto endError = serializer.end();
		if (endError.hasValue()) { return false; }

		Deserializer deserializer;
		const natl::Bool sourceProccessed = deserializer.addSource(serializer.output());
		natl::testAssert(testDst, sourceProccessed);
		if (sourceProccessed) {
			constexpr auto readFlag = natl::DeserializeReadFlag::none;
			constexpr auto customReadFlag = natl::DefaultCustomDeserializeReadFlag<decltype(deserializer)>;


			auto deserializerScopeExpect = deserializer.begin<readFlag, customReadFlag>();
			if (deserializerScopeExpect.hasValue()) {
				auto deserializerScope = deserializerScopeExpect.value();

				nadsad::ascii::LexicalInfo deserializedLexicalInfo;
				natl::String sourceDst;

				auto lexicalError = natl::deserializeReadNamedToDst<decltype(deserializer), readFlag, customReadFlag, 
					natl::SerializeGlobalComponent<nadsad::ascii::LexicalInfo>, nadsad::ascii::LexicalInfo>(
					deserializer, deserializerScope, "lexical", deserializedLexicalInfo, sourceDst);

				if (lexicalError.hasValue()) {
					using error_serializer = nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>;
					error_serializer errorSerializer{};
					[[maybe_unused]] auto error = deserializer.serializeSourceProccessState<error_serializer, natl::SerializeWriteFlag::none,
						 natl::DefaultCustomSerializeWriteFlag<error_serializer>, natl::SerializeGlobalComponent<nadsad::ascii::LexicalInfo>>(errorSerializer);

					nadsad::outputErrorFile("error.nadsada", errorSerializer.output().size(), (natl::Byte*)errorSerializer.output().data());
					nadsad::outputErrorFile("state.nadsada", serializer.output().size(), (natl::Byte*)serializer.output().data());

					natl::println(lexicalError.value().getMessage<natl::String256>());
					return true;
				}

				auto deserializerEndError = deserializer.end<readFlag, customReadFlag>(deserializerScopeExpect.value());
			}
		} else {
			using error_serializer = nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>;
			error_serializer errorSerializer{};
			[[maybe_unused]] auto error = deserializer.serializeSourceProccessState<error_serializer, natl::SerializeWriteFlag::none,
				natl::DefaultCustomSerializeWriteFlag<error_serializer>, natl::SerializeGlobalComponent<nadsad::ascii::LexicalInfo>>(errorSerializer);
			natl::println(errorSerializer.output());
		}

		return true;
	}

	template<typename Serializer, typename Deserializer>
	constexpr natl::Bool testFile(const natl::String& testName, const natl::String& fileName) noexcept {
		natl::Test test;
		natl::String fileContent{};
		nadsad::ascii::LexicalInfo lexicalInfo{};
		Serializer serializer{};
		return baseTestFile<Serializer, Deserializer>(test, fileContent, lexicalInfo, serializer, testName, fileName);
	}
}