#pragma once 

//natl
#include <natl\stringView.h>
#include <natl\flatHashMap.h>
#include <natl\dynArray.h>

//interface 
namespace nadsad {
	enum class AssciFormatTokenType {
		unknown = 0,

		keywordI8,
		keywordI16,
		keywordI32,
		keywordI64,

		keywordUI8,
		keywordUI16,
		keywordUI32,
		keywordUI64,

		keywordSize,

		keywordF32,
		keywordF64,

		keywordStr,
		keywordDic,
		keywordArray,
		keywordStruct,
		keywordStructType,
		keywordTstruct,
		keywordVariant,
		keywordVariantType,
		keywordEnum,
		keywordEnumType,
		keywordOp,

		leftCurly,
		rightCurly,
		leftSquare,
		rightSquare,
		colon,
		comma,
		doubleQuotes,

		identifier,
		numericFloat,
		numericInt,
	};


	struct AssciFormatToken {
		AssciFormatTokenType type;
		natl::ConstAsciiStringView identifier;
		natl::Size column;
		natl::Size lineNumber;
	};

	using AssciFormatTokenStream = natl::DynArray<AssciFormatToken>;
	using AssciFromatLexicalErrorStream = natl::DynArray<AssciFormatToken>;

	namespace impl {
		bool isDigit(const char character) noexcept {
			switch (character) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return true;
			default:
				return false;
			}
		}

		bool isIdentifierChar(const char character) noexcept {
			switch (character) {
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case '_':
				return true;
			default:
				return false;
			}
		}

		class AssciFormatLexer {
		private:
			natl::Size currentColumn;
			natl::Size currentLineNumber;
			natl::Size index;
			natl::FlatHashMap<natl::StringView, AssciFormatTokenType> keywordMap;

			const natl::AssciCode* sourcePtr;
			AssciFormatTokenStream* tokenStreamPtr;
			AssciFromatLexicalErrorStream* lexicalErrorStreamPtr;
		private:
			constexpr void initKeywordMap() noexcept {
				keywordMap.insert("i8", AssciFormatTokenType::keywordI8);
				keywordMap.insert("i16", AssciFormatTokenType::keywordI16);
				keywordMap.insert("i32", AssciFormatTokenType::keywordI32);
				keywordMap.insert("i64", AssciFormatTokenType::keywordI64);
				keywordMap.insert("ui8", AssciFormatTokenType::keywordUI8);
				keywordMap.insert("ui16", AssciFormatTokenType::keywordUI16);
				keywordMap.insert("ui32", AssciFormatTokenType::keywordUI32);
				keywordMap.insert("ui64", AssciFormatTokenType::keywordUI64);
				keywordMap.insert("size", AssciFormatTokenType::keywordSize);
				keywordMap.insert("f32", AssciFormatTokenType::keywordF32);
				keywordMap.insert("f64", AssciFormatTokenType::keywordF64);
				keywordMap.insert("str", AssciFormatTokenType::keywordStr);
				keywordMap.insert("dic", AssciFormatTokenType::keywordDic);
				keywordMap.insert("array", AssciFormatTokenType::keywordArray);
				keywordMap.insert("struct", AssciFormatTokenType::keywordStruct);
				keywordMap.insert("struct_type", AssciFormatTokenType::keywordStructType);
				keywordMap.insert("tstruct", AssciFormatTokenType::keywordTstruct);
				keywordMap.insert("variant", AssciFormatTokenType::keywordVariant);
				keywordMap.insert("variant_type", AssciFormatTokenType::keywordVariantType);
				keywordMap.insert("enum", AssciFormatTokenType::keywordEnum);
				keywordMap.insert("enum_type", AssciFormatTokenType::keywordEnumType);
				keywordMap.insert("op", AssciFormatTokenType::keywordOp);
			}
		public:
			constexpr AssciFormatLexer() noexcept :
				currentColumn(0),
				currentLineNumber(0),
				index(0),
				keywordMap(),
				tokenStreamPtr(nullptr),
				lexicalErrorStreamPtr(nullptr) {
				initKeywordMap();
			}
		private:

			constexpr void addToken(const AssciFormatTokenType tokenType, const natl::ConstAsciiStringView& identifier) noexcept {
				tokenStreamPtr->push_back(AssciFormatToken(tokenType, identifier, currentLineNumber, currentColumn - identifier.length()));
			}

			constexpr natl::ConstAsciiStringView getCurrentCharacterStringView() noexcept {
				return natl::ConstAsciiStringView(getCurrentCharacterPtr(), 1);
			}

			constexpr const char getCurrentCharacter() noexcept {
				return *sourcePtr;
			}
			constexpr const char* getCurrentCharacterPtr() noexcept {
				return sourcePtr;
			}

			constexpr bool atEnd() noexcept {
				return *sourcePtr == '\0';
			}
			constexpr bool notAtEnd() noexcept {
				return *sourcePtr != '\0';
			}

			constexpr void nextCharacter() noexcept {
				switch (getCurrentCharacter()) {
				case '\n':
					currentColumn = 2;
					currentLineNumber += 1;
					break;
				default:
					currentColumn += 1;
					break;
				}
				sourcePtr++;
			}
		public:
			constexpr void next() noexcept {
				const char currentCharacter = getCurrentCharacter();
				switch (currentCharacter) {
				case '\t':
					break;
				case '\n':
					break;
				case '\r':
					break;
				case ' ':
					break;
				case ':':
					addToken(AssciFormatTokenType::colon, getCurrentCharacterStringView());
					break;
				case '{':
					addToken(AssciFormatTokenType::leftCurly, getCurrentCharacterStringView());
					break;
				case '}':
					addToken(AssciFormatTokenType::rightCurly, getCurrentCharacterStringView());
					break;
				case '[':
					addToken(AssciFormatTokenType::leftSquare, getCurrentCharacterStringView());
					break;
				case ']':
					addToken(AssciFormatTokenType::rightSquare, getCurrentCharacterStringView());
					break;
				case '"':
					addToken(AssciFormatTokenType::doubleQuotes, getCurrentCharacterStringView());
					break;
				default:
					if (isDigit(currentCharacter)) {
						natl::Size numericIdentifierSize = 1;
						const char* numericIdentifierPtr = getCurrentCharacterPtr();
						AssciFormatTokenType numericType = AssciFormatTokenType::numericInt;

						while (notAtEnd() && isDigit(getCurrentCharacter())) {
							numericIdentifierSize += 1;
							nextCharacter();
						}

						if (notAtEnd() && getCurrentCharacter() == '.') {
							numericType = AssciFormatTokenType::numericFloat;

							while (notAtEnd() && isDigit(getCurrentCharacter())) {
								numericIdentifierSize += 1;
								nextCharacter();
							}
						}

						natl::StringView identifier = natl::StringView(numericIdentifierPtr, numericIdentifierSize);
						addToken(numericType, identifier);

						return;
					}
					if (isIdentifierChar(currentCharacter)) {
						natl::Size identifierSize = 1;
						const char* identifierPtr = getCurrentCharacterPtr();
						nextCharacter();

						while (notAtEnd() && isIdentifierChar(getCurrentCharacter())) {
							identifierSize += 1;
							nextCharacter();
						}

						natl::StringView identifier = natl::StringView(identifierPtr, identifierSize);
						natl::FlatHashMap<natl::StringView, AssciFormatTokenType>::optional_pointer keywordType = keywordMap.find(identifier);

						if (keywordType) {
							addToken(keywordType.value()->data, identifier);
						} else {
							addToken(AssciFormatTokenType::identifier, identifier);
						}

						return;
					}

					if (!tokenStreamPtr->empty() && tokenStreamPtr->back().type == AssciFormatTokenType::unknown) {
						AssciFormatToken& unknownToken = tokenStreamPtr->back();
						unknownToken.identifier = natl::StringView(unknownToken.identifier.data(), sourcePtr - unknownToken.identifier.data());
						lexicalErrorStreamPtr->back() = unknownToken;
					} else {
						addToken(AssciFormatTokenType::unknown, getCurrentCharacterStringView());
						lexicalErrorStreamPtr->push_back(tokenStreamPtr->back());
					}
					break;
				}
				nextCharacter();
			}


			constexpr void lex(
				const natl::ConstAsciiStringView& source,
				AssciFormatTokenStream& tokenStream, 
				AssciFromatLexicalErrorStream& lexicalErrorStream) noexcept {
				sourcePtr = source.c_str();
				currentColumn = 0;
				currentLineNumber = 0;
				index = 0;
				tokenStreamPtr = &tokenStream;
				lexicalErrorStreamPtr = &lexicalErrorStream;
				tokenStreamPtr->clear();
				tokenStreamPtr->reserve(100);
				
				while (notAtEnd()) { next(); }
			}
		};
	}

	template<typename Serializer>
		requires(natl::IsCompleteSerializer<Serializer>)
	constexpr void serializeAssicTokenStream(
		Serializer& serializer,
		AssciFormatTokenStream& tokenStream,
		AssciFromatLexicalErrorStream& lexicalErrorStream) noexcept {
		natl::SerializationEnumType serializationAssciFormatTokenType("AssciFormatTokenType", {
				"unknown", //0
				"keywordI8", //1
				"keywordI16", //2
				"keywordI32", //3
				"keywordI64", //4
				"keywordUI8", //5
				"keywordUI16", //6
				"keywordUI32", //7
				"keywordUI64", //8
				"keywordSize", //9
				"keywordF32", //10
				"keywordF64", //11
				"keywordStr", //12
				"keywordDic", //13
				"keywordArray", //14
				"keywordStruct", //15
				"keywordStructType", //16
				"keywordTstruct", //17
				"keywordVariant", //18
				"keywordVariantType", //19
				"keywordEnum", //20
				"keywordEnumType", //21
				"keywordOp", //22
				"leftCurly", //23
				"rightCurly", //24
				"leftSquare", //25
				"rightSquare", //26
				"colon", //27
				"comma", //28
				"doubleQuotes", //29
				"identifier",  //30
				"numericFloat", //31
				"numericInt", //32
			});

		auto tokenTypeToIndexCovert = [](const AssciFormatTokenType& tokenType) -> natl::Size {
			switch (tokenType) {
			case AssciFormatTokenType::unknown: return 0;
			case AssciFormatTokenType::keywordI8: return 1;
			case AssciFormatTokenType::keywordI16: return 2;
			case AssciFormatTokenType::keywordI32: return 3;
			case AssciFormatTokenType::keywordI64: return 4;
			case AssciFormatTokenType::keywordUI8: return 5;
			case AssciFormatTokenType::keywordUI16: return 6;
			case AssciFormatTokenType::keywordUI32: return 7;
			case AssciFormatTokenType::keywordUI64: return 8;
			case AssciFormatTokenType::keywordSize: return 9;
			case AssciFormatTokenType::keywordF32: return 10;
			case AssciFormatTokenType::keywordF64: return 11;
			case AssciFormatTokenType::keywordStr: return 12;
			case AssciFormatTokenType::keywordDic: return 13;
			case AssciFormatTokenType::keywordArray: return 14;
			case AssciFormatTokenType::keywordStruct: return 15;
			case AssciFormatTokenType::keywordStructType: return 16;
			case AssciFormatTokenType::keywordTstruct: return 17;
			case AssciFormatTokenType::keywordVariant: return 18;
			case AssciFormatTokenType::keywordVariantType: return 19;
			case AssciFormatTokenType::keywordEnum: return 20;
			case AssciFormatTokenType::keywordEnumType: return 21;
			case AssciFormatTokenType::keywordOp: return 22;
			case AssciFormatTokenType::leftCurly: return 23;
			case AssciFormatTokenType::rightCurly: return 24;
			case AssciFormatTokenType::leftSquare: return 25;
			case AssciFormatTokenType::rightSquare: return 26;
			case AssciFormatTokenType::colon: return 27;
			case AssciFormatTokenType::comma: return 28;
			case AssciFormatTokenType::doubleQuotes: return 29;
			case AssciFormatTokenType::identifier: return 30;
			case AssciFormatTokenType::numericFloat: return 31;
			case AssciFormatTokenType::numericInt: return 32;
			default:
				natl::unreachable();
			}

			};

		natl::SerializationStructType serializationAssciFormatToken("AssciFormatToken",
			{
				natl::SerializationStructTypeElement("tokenType", &serializationAssciFormatTokenType),
				natl::SerializationStructTypeElement("identifier", natl::SerializationBasicType::dt_ascci_str),
				natl::SerializationStructTypeElement("lineNumber", natl::SerializationBasicType::dt_size),
				natl::SerializationStructTypeElement("column", natl::SerializationBasicType::dt_size)
			}
		);


		serializer.serializeNamedElement<natl::SerializationTypeFlag::dt_array>("tokens");

		auto tokensArrayInfo = natl::SerializationArrayInfo(
			natl::SerializationTypeElementsInfo(&serializationAssciFormatToken)
		);
		serializer.serializeNamedArrayInfo(tokensArrayInfo);

		serializer.serializeArray(tokensArrayInfo);

		for (const AssciFormatToken& token : tokenStream) {
			serializer.serializeTypedStruct(serializationAssciFormatToken);
			serializer.serializeNamedElement<natl::SerializationTypeFlag::dt_enum>("tokenType");
			serializer.serializeEnum(serializationAssciFormatTokenType, tokenTypeToIndexCovert(token.type));

			serializer.serializeNamedElement<natl::SerializationTypeFlag::dt_ascci_str>("identifier");
			serializer.serializeAssicStr(token.identifier);

			serializer.serializeNamedElement<natl::SerializationTypeFlag::dt_size>("lineNumber");
			serializer.serialize_size(token.lineNumber);

			serializer.serializeNamedElement<natl::SerializationTypeFlag::dt_size>("column");
			serializer.serialize_size(token.column);

			serializer.endSerializeTypedStruct();

			if (&token != tokenStream.endPtr()) {
				serializer.serializeNextArrayElement();
			}
		}

		serializer.endSerializeArray();
	}

	constexpr void assciFromatLexicalAnalysis(const natl::ConstAsciiStringView& source,
		AssciFormatTokenStream& tokenStream, 
		AssciFromatLexicalErrorStream& lexicalErrorStream) noexcept {
		impl::AssciFormatLexer lexer{};
		lexer.lex(source, tokenStream, lexicalErrorStream);
	}
}