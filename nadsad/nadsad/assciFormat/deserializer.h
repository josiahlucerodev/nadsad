#pragma once 

//natl
#include <natl/container/arrayView.h>

//own
#include "lexer.h"

//interface 
namespace nadsad::ascii {
	struct Deserializer {
	private:
		natl::ArrayView<natl::Byte> source;
		LexicalInfo lexicalInfo;
		natl::Size tokenIndex;
	public:
		constexpr natl::Bool addSource(const natl::ArrayView<natl::Byte>& newSource) noexcept {
			source = newSource;
			tokenIndex = 0;


		}

		constexpr natl::Bool beginDeserialization() noexcept {

		}
		constexpr natl::Bool endDeserialization() noexcept {

		}

	};
}