#pragma once 

//natl
#include <natl\container\arrayView.h>

//interface 
namespace nadsad::ascii {
	enum class TokenType {
		unknown,
		start,
		end,

		identifier,

		colon,
		comma,
		leftCurly,
		rightCurly,
		leftSquare,
		rightSquare,

		charLiteral,
		stringLiteral,
		dataStorage,

		keywordOp,
		keywordU8,
		keywordU16,
		keywordU32,
		keywordU64,
		keywordUI8,
		keywordUI16,
		keywordUI32,
		keywordUI64,
		keywordF32,
		keywordF64,
		keywordStr,
		keywordChar,
		keywordFlag,
		keywordArray,
		keywordDic,
		keywordStruct,
		keywordBlob,
		keywordFile,
	};

	constexpr natl::ConstAsciiStringView tokenTypeToString(const TokenType tokenType) noexcept {
		switch (tokenType) {
		case TokenType::unknown:
			return "unknown";
		case TokenType::start:
			return "start";
		case TokenType::end:
			return "end";
		case TokenType::identifier:
			return "identifier";
		case TokenType::colon:
			return "colon";
		case TokenType::comma:
			return "comma";
		case TokenType::leftCurly:
			return "leftCurly";
		case TokenType::rightCurly:
			return "rightCurly";
		case TokenType::leftSquare:
			return "leftSquare";
		case TokenType::rightSquare:
			return "rightSquare";
		case TokenType::charLiteral:
			return "charLiteral";
		case TokenType::stringLiteral:
			return "stringLiteral";
		case TokenType::dataStorage:
			return "dataStorage";
		case TokenType::keywordOp:
			return "keywordOp";
		case TokenType::keywordU8:
			return "keywordU8";
		case TokenType::keywordU16:
			return "keywordU16";
		case TokenType::keywordU32:
			return "keywordU32";
		case TokenType::keywordU64:
			return "keywordU64";
		case TokenType::keywordUI8:
			return "keywordUI8";
		case TokenType::keywordUI16:
			return "keywordUI16";
		case TokenType::keywordUI32:
			return "keywordUI32";
		case TokenType::keywordUI64:
			return "keywordUI64";
		case TokenType::keywordF32:
			return "keywordF32";
		case TokenType::keywordF64:
			return "keywordF64";
		case TokenType::keywordStr:
			return "keywordStr";
		case TokenType::keywordChar:
			return "keywordChar";
		case TokenType::keywordArray:
			return "keywordArray";
		case TokenType::keywordDic:
			return "keywordDic";
		case TokenType::keywordStruct:
			return "keywordStruct";
		case TokenType::keywordBlob:
			return "keywordBlob";
		case TokenType::keywordFile:
			return "keywordFile";
		default:
			natl::unreachable();
		}
	}

	struct Token {
		TokenType tokenType;
		natl::ui32 offset;
	};


	namespace impl {
		constexpr natl::Size hashIdentifierLessThan8(const natl::Ascii* first, const natl::Ascii* last) noexcept {
			natl::Size identifierHash = 0;
			for (const natl::Ascii* iter = first; iter < last; ++iter) {
				identifierHash <<= 8;
				identifierHash += natl::Size(*iter);
			}
			return identifierHash;
		}
		constexpr natl::Size hashIdentifierLessThan8(natl::ConstAsciiStringView str) noexcept {
			return hashIdentifierLessThan8(str.data(), str.data() + str.size());
		}
	}

	enum class LexicalError {
		none,
		unknownTokens, 
		inputToBig,
	};

	struct LexicalInfo {
		natl::DynArray<Token> tokens;
		natl::DynArray<natl::ui32> newLineOffsets;
		LexicalError error;
	};

	constexpr LexicalInfo lexicalAnalysis(const natl::ArrayView<const natl::Ascii> source) noexcept {
		LexicalInfo lexicalInfo{};

		if (source.size() > static_cast<natl::Size>(natl::Limits<natl::ui32>::max())) {
			lexicalInfo.error = LexicalError::inputToBig;
			return lexicalInfo;
		}

		lexicalInfo.tokens.reserve((source.size() >> 5) + 100); //expect a token every 8 characters
		lexicalInfo.newLineOffsets.reserve((source.size() >> 3) + 100); //expect a newline every 32 characters
		lexicalInfo.tokens.push_back(Token{TokenType::start, 0});
		LexicalError lexicalError = LexicalError::none;

		natl::ui32 index = 0;
		while (index < source.size()) {
			const natl::Ascii character = source[index];

			TokenType tokenType = TokenType::unknown;
			switch (character) {
				case ' ': 
				case '\t': 
				case '\r': 
					index++;
					continue;
				case ':': 
					tokenType = TokenType::colon;
					break;
				case ',': 
					tokenType = TokenType::comma;
					break;
				case '{': 
					tokenType = TokenType::leftCurly;
					break;
				case '}': 
					tokenType = TokenType::rightCurly;
					break;
				case '[': 
					tokenType = TokenType::leftSquare;
					break;
				case ']': 
					tokenType = TokenType::rightSquare;
					break;
				case '\n':
					lexicalInfo.newLineOffsets.push_back(index);
					index++;
					continue;
				default:

					const natl::ui32 identifierStartIndex = index;
					if (natl::isAlphabetic(character) || character == '_') {
						index++;

						if (index == source.size()) {
							lexicalInfo.tokens.push_back(Token{ TokenType::identifier, index });
							break;
						}

						natl::Ascii nextCharacter = source[index];
						while (natl::isAlphanumeric(nextCharacter) || nextCharacter == '_' && index != source.size()) {
							nextCharacter = source[index];
							index++;
						}

						const natl::Size identifierSize = (index - identifierStartIndex) + 1;
						if (identifierSize < 8) { //could be keyword
							natl::Size identifierHash = 0;

							if (identifierStartIndex + 8 < source.size()) {
								identifierHash = *(natl::Size*)&character;
								identifierHash >>= identifierSize * 8;
							} else {
								const natl::Ascii* identifierPtr = &character;
								identifierHash = impl::hashIdentifierLessThan8(identifierPtr, identifierPtr + identifierSize);
							}

							TokenType keywordTokenType;
							switch (identifierHash) {
							case impl::hashIdentifierLessThan8("op"):
								keywordTokenType = TokenType::keywordOp;
								break;
							case impl::hashIdentifierLessThan8("u8"):
								keywordTokenType = TokenType::keywordU8;
								break;
							case impl::hashIdentifierLessThan8("u16"):
								keywordTokenType = TokenType::keywordU16;
								break;
							case impl::hashIdentifierLessThan8("u32"):
								keywordTokenType = TokenType::keywordU32;
								break;
							case impl::hashIdentifierLessThan8("u64"):
								keywordTokenType = TokenType::keywordU64;
								break;
							case impl::hashIdentifierLessThan8("ui8"):
								keywordTokenType = TokenType::keywordUI8;
								break;
							case impl::hashIdentifierLessThan8("ui16"):
								keywordTokenType = TokenType::keywordUI16;
								break;
							case impl::hashIdentifierLessThan8("ui32"):
								keywordTokenType = TokenType::keywordUI32;
								break;
							case impl::hashIdentifierLessThan8("ui64"):
								keywordTokenType = TokenType::keywordUI64;
								break;
							case impl::hashIdentifierLessThan8("f32"):
								keywordTokenType = TokenType::keywordF32;
								break;
							case impl::hashIdentifierLessThan8("f64"):
								keywordTokenType = TokenType::keywordF64;
								break;
							case impl::hashIdentifierLessThan8("char"):
								keywordTokenType = TokenType::keywordChar;
								break;
							case impl::hashIdentifierLessThan8("str"):
								keywordTokenType = TokenType::keywordStr;
								break;
							case impl::hashIdentifierLessThan8("flag"):
								keywordTokenType = TokenType::keywordFlag;
								break;
							case impl::hashIdentifierLessThan8("array"):
								keywordTokenType = TokenType::keywordArray;
								break;
							case impl::hashIdentifierLessThan8("dic"):
								keywordTokenType = TokenType::keywordDic;
								break;
							case impl::hashIdentifierLessThan8("struct"):
								keywordTokenType = TokenType::keywordStruct;
								break;
							case impl::hashIdentifierLessThan8("blob"):
								keywordTokenType = TokenType::keywordBlob;
								break;
							case impl::hashIdentifierLessThan8("file"):
								keywordTokenType = TokenType::keywordFile;
								break;
							default:
								lexicalInfo.tokens.push_back(Token{ TokenType::identifier, identifierStartIndex });
								continue;
							}

							lexicalInfo.tokens.push_back(Token{ keywordTokenType, identifierStartIndex });
						} else {
							lexicalInfo.tokens.push_back(Token{ TokenType::identifier, identifierStartIndex });
							continue;
						}
					}
			}

			lexicalInfo.tokens.push_back(Token{ tokenType, index });
			index++;
		}

		lexicalInfo.tokens.push_back(Token{ TokenType::end, static_cast<natl::ui32>(source.size()) });
		lexicalInfo.error = lexicalError;
		return lexicalInfo;
	}
	
}

namespace natl::serialization {
	template<>
	struct Serilize<nadsad::ascii::TokenType> {
		template<typename Serializer>
		constexpr static void as(Serializer& serializer) noexcept {
			serializer.asFlag();
		}
		template<typename Serializer>
		[[nodiscard]] constexpr static void write(Serializer& serializer, const nadsad::ascii::TokenType tokenType) noexcept {
			auto tokenTypeSizeToString = [](const Size value) noexcept -> ConstAsciiStringView {
				return nadsad::ascii::tokenTypeToString(static_cast<nadsad::ascii::TokenType>(value));
			};
			serializer.writeFlag(natl::enumToSize(tokenType), tokenTypeSizeToString);
		}
	};

	template<>
	struct Serilize<nadsad::ascii::LexicalInfo> {
		template<typename Serializer>
		constexpr static void as(Serializer& serializer) noexcept {
			serializer.asStruct();
		}
		template<typename Serializer>
		[[nodiscard]] constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			serializer.beginWriteStruct();
			
			serializer.beginWrite("tokens");
			serializer.asArrayOf(natls::BasicDataType::t_flag, lexicalInfo.tokens.size());

			serializer.beginWriteArray();
			for (const nadsad::ascii::Token& token : lexicalInfo.tokens) {
				serializer.beginWriteArrayElement();
				Serilize<nadsad::ascii::TokenType>::template write<Serializer>(serializer, token.tokenType);
				serializer.endWriteArrayElement();
			}
			serializer.endWriteArray();
			serializer.endWrite();

			serializer.endWriteStruct();
		}
	};
}