#pragma once 

#include<natl/fundamental/option.h>
#include<natl/container/string.h>
#include<natl/system/filesystem.h>

//interface
namespace nadsad {
	inline const char* getTestResourcePath() noexcept {
		return NADSAD_TEST_RESOURCE_PATH;
	}

	template<natl::Size NumberSmallBufferBytes>
	natl::Option<natl::StringByteSize<NumberSmallBufferBytes>> loadTestFile(
		const natl::ConstAsciiStringView& relitivePath) noexcept {
		natl::Path testFilePath = nadsad::getTestResourcePath();
		testFilePath /= relitivePath;

		natl::File testFile(testFilePath.c_str(), natl::FileOpenMode::readStart);
		natl::StringByteSize<10000> fileContents;
		const natl::LoadAllFileContentError error = natl::loadAllFileContent(testFile, fileContents);

		if (error != natl::LoadAllFileContentError::none) {
			natl::printlnf("nadsad: failed to load ", testFilePath.toStringView(), ": ", natl::loadAllFileContentErrorToString(error));
			return {};
		}
		return fileContents;
	}

	
}