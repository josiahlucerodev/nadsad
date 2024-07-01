#pragma once 

//natl
#include <natl\container\arrayView.h>

//interface 
namespace nadsad::ascii {
	enum class TokenType : natl::ui64 {
		unknown,
		start,
		end,

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

		keywordTable,
		keywordNtable,
		keywordIndex,
	};

	constexpr natl::ConstAsciiStringView tokenTypeToString(const TokenType tokenType) noexcept {
		switch (tokenType) {
		case TokenType::unknown:
			return "unknown";
		case TokenType::start:
			return "start";
		case TokenType::end:
			return "end";
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
		case TokenType::keywordFlag:
			return "keywordFlag";
		case TokenType::keywordDic:
			return "keywordDic";
		case TokenType::keywordStruct:
			return "keywordStruct";
		case TokenType::keywordBlob:
			return "keywordBlob";
		case TokenType::keywordFile:
			return "keywordFile";
		case TokenType::keywordTable:
			return "keywordTable";
		case TokenType::keywordNtable:
			return "keywordNtable";
		case TokenType::keywordIndex:
			return "keywordIndex";
		default:
			natl::unreachable();
		}
	}

	struct Token {
		TokenType tokenType : 8;
		natl::ui64 size : 64 - 8;
		natl::ui64 offset;
	};

	enum class LexicalError {
		none,
		unknownTokens, 
		inputToBig,
	};

	struct LexicalInfo {
		natl::ArrayView<const natl::Ascii> source;
		natl::DynArray<Token> tokens;
		natl::DynArray<natl::ui64> newLineOffsets;
		LexicalError error;
	};

	constexpr LexicalInfo lexicalAnalysis(const natl::ArrayView<const natl::Ascii> source) noexcept {
		LexicalInfo lexicalInfo{};

		if (source.size() > static_cast<natl::Size>(natl::Limits<natl::ui64>::max())) {
			lexicalInfo.error = LexicalError::inputToBig;
			return lexicalInfo;
		}

		lexicalInfo.source = source;
		lexicalInfo.tokens.reserve((source.size() >> 5) + 100); //expect a token every 8 characters
		lexicalInfo.newLineOffsets.reserve((source.size() >> 3) + 100); //expect a newline every 32 characters
		lexicalInfo.tokens.push_back(Token{TokenType::start, 0});
		LexicalError lexicalError = LexicalError::none;

		auto addUnknownToken = [&](const natl::ui64 size, const natl::ui64 offset) noexcept {
			if (lexicalInfo.tokens.isNotEmpty()) {
				Token& previousToken = lexicalInfo.tokens.back();
				if (previousToken.tokenType == TokenType::unknown && 
					previousToken.offset + previousToken.size == offset) {
					previousToken.size += size;
					return;
				}
			}
			lexicalInfo.tokens.push_back(Token{ TokenType::unknown, size, offset });
		};

		auto addToken = [&](const TokenType tokenType, const natl::ui64 size, const natl::ui64 offset) noexcept {
			lexicalInfo.tokens.push_back(Token{ tokenType, size, offset });
		};

		natl::ui64 index = 0;
		while (index < source.size()) {
			const natl::Ascii& character = source[index];

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
				case '\'':
				{
					const natl::ui64 startIndex = index;
					natl::ui64 charLiteralSize = 2;

					if (startIndex + 2 >= source.size()) {
						addUnknownToken(charLiteralSize, startIndex);
						index = index + 2;
						continue;
					}

					index += 1;
					charLiteralSize += 1;
					natl::Ascii scopedCharacter = source[index];
					if (scopedCharacter == '\\') {
						if (index + 2 >= source.size()) {
							addUnknownToken(charLiteralSize, startIndex);
							index = index + 2;
							continue;
						}

						index += 1;
						scopedCharacter = source[index];
						charLiteralSize += 1;
					} 

					index += 1;
					const natl::Ascii endCharacter = source[index];
					if (endCharacter != '\'') {
						addUnknownToken(charLiteralSize, startIndex);
						index++;
						continue;
					}

					addToken(TokenType::charLiteral, charLiteralSize, startIndex);
					index++;
					continue;
				}
				case '\"':
				{
					const natl::ui64 startIndex = index;
					index += 1;

					if (index == source.size()) {
						addUnknownToken(1, startIndex);
						continue;
					}

					natl::Ascii stringLiteralCharacter = source[index];
					while (index != source.size() && stringLiteralCharacter != '\"') {
						stringLiteralCharacter = source[index];

						if (stringLiteralCharacter == '\\') {
							index += 1;
							if (index == source.size()) {
								addUnknownToken(index - startIndex, startIndex);
								continue;
							}
						} 

						index++;
					}

					addToken(TokenType::stringLiteral, index - startIndex, startIndex);
					continue;
				}
				case '(':
				{
					const natl::ui64 startIndex = index;
					index += 1;

					if (index == source.size()) {
						addUnknownToken(1, startIndex);
						continue;
					}

					natl::Ascii blobData = source[index];
					while (index != source.size() && blobData != ')') {
						blobData = source[index];

						if (blobData == natl::Ascii(255)) {
							index += 1;
							if (index == source.size()) {
								addUnknownToken(index - startIndex, startIndex);
								continue;
							}
						}

						index++;
					}

					addToken(TokenType::dataStorage, index - startIndex, startIndex);
					continue;
				}
				default:
					//is identifier 
					const natl::ui64 identifierStartIndex = index;
					if (natl::isAlphabetic(character) || character == '_') {
						index++;

						if (index == source.size()) {
							lexicalInfo.tokens.push_back(Token{ TokenType::unknown, 1, index });
							break;
						}

						natl::Ascii nextCharacter = source[index];
						while (natl::isAlphanumeric(nextCharacter) || nextCharacter == '_' && index + 1 != source.size()) {
							index++;
							nextCharacter = source[index];
						}

						const natl::ui64 identifierSize = natl::ui64(index - identifierStartIndex);
						if (identifierSize < 8) { //could be keyword
							natl::Size identifierHash = 0;

							const natl::Ascii* identifierPtr = &character;
							if (identifierStartIndex + 8 < source.size()) {
								constexpr natl::Size testHash = natl::hashStringLessThan8("ntable");
								identifierHash = natl::hashStringLessThan8Unbounded(identifierPtr, identifierSize);
							} else {
								identifierHash = natl::hashStringLessThan8(identifierPtr, identifierPtr + identifierSize);
							}

							TokenType keywordTokenType;
							switch (identifierHash) {
							case natl::hashStringLessThan8("op"):
								keywordTokenType = TokenType::keywordOp;
								break;
							case natl::hashStringLessThan8("u8"):
								keywordTokenType = TokenType::keywordU8;
								break;
							case natl::hashStringLessThan8("u16"):
								keywordTokenType = TokenType::keywordU16;
								break;
							case natl::hashStringLessThan8("u32"):
								keywordTokenType = TokenType::keywordU32;
								break;
							case natl::hashStringLessThan8("u64"):
								keywordTokenType = TokenType::keywordU64;
								break;
							case natl::hashStringLessThan8("ui8"):
								keywordTokenType = TokenType::keywordUI8;
								break;
							case natl::hashStringLessThan8("ui16"):
								keywordTokenType = TokenType::keywordUI16;
								break;
							case natl::hashStringLessThan8("ui32"):
								keywordTokenType = TokenType::keywordUI32;
								break;
							case natl::hashStringLessThan8("ui64"):
								keywordTokenType = TokenType::keywordUI64;
								break;
							case natl::hashStringLessThan8("f32"):
								keywordTokenType = TokenType::keywordF32;
								break;
							case natl::hashStringLessThan8("f64"):
								keywordTokenType = TokenType::keywordF64;
								break;
							case natl::hashStringLessThan8("char"):
								keywordTokenType = TokenType::keywordChar;
								break;
							case natl::hashStringLessThan8("str"):
								keywordTokenType = TokenType::keywordStr;
								break;
							case natl::hashStringLessThan8("flag"):
								keywordTokenType = TokenType::keywordFlag;
								break;
							case natl::hashStringLessThan8("array"):
								keywordTokenType = TokenType::keywordArray;
								break;
							case natl::hashStringLessThan8("dic"):
								keywordTokenType = TokenType::keywordDic;
								break;
							case natl::hashStringLessThan8("struct"):
								keywordTokenType = TokenType::keywordStruct;
								break;
							case natl::hashStringLessThan8("blob"):
								keywordTokenType = TokenType::keywordBlob;
								break;
							case natl::hashStringLessThan8("file"):
								keywordTokenType = TokenType::keywordFile;
								break;
							case natl::hashStringLessThan8("table"):
								keywordTokenType = TokenType::keywordTable;
								break;
							case natl::hashStringLessThan8("ntable"):
								keywordTokenType = TokenType::keywordNtable;
								break;
							case natl::hashStringLessThan8("index"):
								keywordTokenType = TokenType::keywordIndex;
								break;
							default:
								addUnknownToken(identifierSize, identifierStartIndex);
								continue;
							}

							addToken(keywordTokenType, identifierSize, identifierStartIndex);
							continue;
						} else {
							addUnknownToken(identifierSize, identifierStartIndex);
							continue;
						}
					}

					//is number 

					//unknown
					addUnknownToken(1, index);
					index++;
					continue;
			}

			addToken(tokenType, 1, index);
			index++;
		}

		addToken(TokenType::end, 0, static_cast<natl::ui64>(source.size()));
		lexicalInfo.error = lexicalError;
		return lexicalInfo;
	}

	constexpr natl::Size findTokenLineNumber(const natl::ui64 offset, const natl::ArrayView<const natl::ui64> newLineOffsets) noexcept {
		const natl::Size newlineIndex = natl::lowerBoundIndex(offset, newLineOffsets);
		return newlineIndex;
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
		constexpr static void write(Serializer& serializer, const nadsad::ascii::TokenType tokenType) noexcept {
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
		constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			serializer.beginWriteStruct();
			
			serializer.beginWrite("tokens");
			serializer.asArrayOf(natls::BasicDataType::t_struct, lexicalInfo.tokens.size());

			serializer.beginWriteArray();
			for (natl::Size i = 0; i < lexicalInfo.tokens.size(); ++i) {
				const nadsad::ascii::Token token = lexicalInfo.tokens[i];
				serializer.beginWriteArrayElement();

				serializer.beginWriteStruct();
				const natl::Size lineNumber = nadsad::ascii::findTokenLineNumber(token.offset, lexicalInfo.newLineOffsets.toArrayView());
				natls::write<Serializer, nadsad::ascii::TokenType>(serializer, "type", token.tokenType);
				natls::write<Serializer, natl::Size>(serializer, "lineNumber", lineNumber);

				if (i == 0 || i == lexicalInfo.tokens.size() - 1) {
					natls::write<Serializer>(serializer, "value", "");
				} else {
					natl::ConstAsciiStringView value(&lexicalInfo.source[token.offset], token.size);
					natls::write<Serializer>(serializer, "value", value);
				}
				serializer.endWriteStruct();

				serializer.endWriteArrayElement();
			}
			serializer.endWriteArray();
			serializer.endWrite();

			serializer.endWriteStruct();
		}
	};
}