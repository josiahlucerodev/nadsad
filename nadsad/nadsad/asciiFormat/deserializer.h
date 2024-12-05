#pragma once 

//natl
#include <natl/container/arrayView.h>

//own
#include "lexer.h"

//interface 
namespace nadsad::ascii {
	template<natl::ErrorHandlingFlag errorFlag, template<typename> typename DeserializeInfo, typename DeserializeErrorHandler>
	struct Deserializer {
	public:
		template<typename SerializeType>
		using deserialize_info = DeserializeInfo<SerializeType>;
		using deserialize_error_handler = DeserializeErrorHandler;
		using error_message_string_type = natl::String256;
	private:
		natl::ArrayView<const natl::Char> source;
		LexicalInfo lexicalInfo;
		natl::Size tokenIndex;
		natl::Size totalOffset;
		natl::Size currentTokenOffset;
		natl::Size currentTokenSize;
	public:
		[[nodiscard]] constexpr natl::Bool addSource(const natl::ConstArrayView<natl::Char>& newSource) noexcept {
			source = newSource;
			tokenIndex = 0;
			totalOffset = 0;
			currentTokenOffset = 0;
			currentTokenSize = 0;
			lexicalInfo = lexicalAnalysis(natl::ConstAsciiStringView(source));
			return lexicalInfo.errors.isEmpty();
		}

		template<typename Serializer>
			requires(natl::IsSerializerC<Serializer>)
		constexpr void serializeErrors(Serializer& serializer) noexcept {
			using as_type = natl::SerializeArray<nadsad::ascii::LexicalError>;
			serializer.template beginWrite<as_type>("LexcialErrors");

			serializer.writeValue();
			if (lexicalInfo.errors.empty()) {
				serializer.writeEmptyArray();
			} else {
				serializer.beginWriteArray();
				for (const nadsad::ascii::LexicalError& lexicalError : lexicalInfo.errors) {
					serializer.beginWriteArrayElement();
					natl::serializeWrite<Serializer, nadsad::ascii::LexicalError>(serializer, lexicalError, lexicalInfo);
					serializer.endWriteArrayElement();
				}
				serializer.endWriteArray();
			}
			serializer.endWrite();
		}

		template<typename SerializerType>
			requires(natl::IsSerializerC<SerializerType>)
		constexpr void serializeSourceProccessState(SerializerType& serializer) noexcept {
			natl::serializeWriteNamed(serializer, "LexicalInfo", lexicalInfo);
		}

	private:
		//util
		constexpr static natl::ConstAsciiStringView endOfSourceErrorMessage = "end of source";
		constexpr Deserializer& self() noexcept { return *this; }
		constexpr const Deserializer& self() const noexcept { return *this; }

		constexpr Token getCurrentToken() noexcept {
			return getTokenAt(tokenIndex, lexicalInfo.tokenUnits);
		}
		constexpr natl::Bool isCurrentToken(const TokenType& type) noexcept {
			return getCurrentToken().type == type;
		}
		constexpr natl::ConstAsciiStringView currentTokenString() noexcept {
			return natl::ConstAsciiStringView(&source[currentTokenOffset], currentTokenSize);
		}
		constexpr natl::Bool nextToken() noexcept {
			const Token currentToken = getCurrentToken();
			tokenIndex = nextTokenIndex(tokenIndex, currentToken, lexicalInfo.tokenUnits);
			if(!(tokenIndex < lexicalInfo.tokenUnits.size())) {
				return false;
			}

			const Token token = getCurrentToken();
			currentTokenOffset = totalOffset + getTokenOffsetAt(tokenIndex, lexicalInfo.tokenUnits);
			currentTokenSize = getSizeOfToken(tokenIndex, token, lexicalInfo.tokenUnits);
			totalOffset = token.offset;
			currentTokenOffset = token.offset;
			return true;
		}

		constexpr natl::ConstAsciiStringView getErrorLocationDetails() noexcept {
			return natl::ConstAsciiStringView(lexicalInfo.source).substr(getCurrentToken().offset, 100);
		}

		constexpr DeserializeErrorHandler unexpectedTokenHandler(const TokenType& expectedTokenType,
			const natl::DeserializeErrorLocation errorLocation,
			const natl::DeserializeErrorFlag deserializeErrorFlag) noexcept {
			error_message_string_type errorMessage = "expected ";
			errorMessage += tokenTypeToDebugString(expectedTokenType);
			errorMessage += " but got ";
			errorMessage += tokenTypeToDebugString(getCurrentToken().type);
			return DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
				errorLocation, deserializeErrorFlag);
		}

		constexpr DeserializeErrorHandler unexpectedEndOfSourceHandler(const natl::DeserializeErrorLocation errorLocation) noexcept {
			return DeserializeErrorHandler(endOfSourceErrorMessage, getErrorLocationDetails(),
				errorLocation, natl::DeserializeErrorFlag::endOfSource);
		}

		constexpr natl::Unexpected<DeserializeErrorHandler> unexpectedToken(const TokenType& expectedTokenType,
			const natl::DeserializeErrorLocation errorLocation,
			const natl::DeserializeErrorFlag deserializeErrorFlag) noexcept {
			return natl::unexpected(unexpectedTokenHandler(expectedTokenType, errorLocation, deserializeErrorFlag));
		}

		constexpr natl::Unexpected<DeserializeErrorHandler> unexpectedEndOfSource(const natl::DeserializeErrorLocation errorLocation) noexcept {
			return natl::unexpected(unexpectedEndOfSourceHandler(errorLocation));
		}

		constexpr natl::Option<DeserializeErrorHandler> unexpectedTokenOption(const TokenType& expectedTokenType,
			const natl::DeserializeErrorLocation errorLocation,
			const natl::DeserializeErrorFlag deserializeErrorFlag) noexcept {
			return unexpectedTokenHandler(expectedTokenType, errorLocation, deserializeErrorFlag);
		}

		constexpr natl::Option<DeserializeErrorHandler> unexpectedEndOfSourceOption(const natl::DeserializeErrorLocation errorLocation) noexcept {
			return unexpectedEndOfSourceHandler(errorLocation);
		}

		constexpr natl::Bool isTokenNumericWithType(const TokenType& type) noexcept {
			switch (type) {
			case TokenType::decimalIntegerWithType:
			case TokenType::hexadecimalIntegerWithType:
			case TokenType::binaryIntegerWithType:
			case TokenType::decimalFloatWithType:
			case TokenType::hexadecimalFloatWithType:
			case TokenType::binaryFloatWithType:
				return true;
			default:
				return false;
			}
		}
		constexpr natl::Bool isTokenFloat(const TokenType& type) noexcept {
			switch (type) {
			case TokenType::decimalFloat:
			case TokenType::hexadecimalFloat:
			case TokenType::binaryFloat:
			case TokenType::decimalFloatWithType:
			case TokenType::hexadecimalFloatWithType:
			case TokenType::binaryFloatWithType:
				return true;
			default:
				return false;
			}
		}
		constexpr natl::Bool isTokenInteger(const TokenType& type) noexcept {
			switch (type) {
				case TokenType::decimalInteger:
				case TokenType::hexadecimalInteger:
				case TokenType::binaryInteger:
				case TokenType::decimalIntegerWithType:
				case TokenType::hexadecimalIntegerWithType:
				case TokenType::binaryIntegerWithType:
					return true;
			default:
				return false;
			}
		}

		constexpr natl::Bool stringLiteralEqualsStr(
			const natl::ConstAsciiStringView& stringLiteral, 
			const natl::ConstAsciiStringView& testString) noexcept {
			natl::Size stringLiteralIndex = 1;
			natl::Size testStringIndex = 0;
			for(;stringLiteralIndex < stringLiteral.size() && testStringIndex < testString.size()
				; stringLiteralIndex++, testStringIndex++) {
				if(stringLiteral[stringLiteralIndex] == '\\') {
					stringLiteralIndex++;
				}
				if(stringLiteral[stringLiteralIndex] != testString[testStringIndex]) {
					return false;
				}
			}
			return stringLiteralIndex == stringLiteral.size() - 1 && testStringIndex == testString.size();
		}
		
		template<typename OutputDstType>
		constexpr void formatStringLiteral(const natl::ConstAsciiStringView& stringLiteral, OutputDstType& output) noexcept {
			for (natl::Size i = 0; i < stringLiteral.size() ; i++) {
				if (stringLiteral[i] == '\\') {
					i++;
				}
				output += stringLiteral[i];
			}
		}
		template<typename StringDstType>
		constexpr void stringLiteralToDst(StringDstType& dst, const natl::ConstAsciiStringView& stringLiteral) noexcept {
			dst.reserve(stringLiteral.size() - 2);

			for (natl::Size i = 1; i < stringLiteral.size() - 1; i++) {
				if (stringLiteral[i] == '\\') {
					if (!(i < stringLiteral.size())) { break; }
					i++;
				}
				dst.pushBack(stringLiteral[i]);
			}
		}
		template<typename DataStroageDstType>
		constexpr void dataStorageToDst(DataStroageDstType& dst, const natl::ConstAsciiStringView& dataStorage) noexcept {
			dst.reserve(dataStorage.size());
			for (natl::Size i = 0; i < dataStorage.size(); i++) {
				if (dataStorage[i] == natl::Ascii(255)) {
					if (!(i < dataStorage.size())) { break; }
					i++;
				}
				dst.pushBack(dataStorage[i]);
			}
		}

		//test
		template<typename SerializeType>
		struct TestType;

		template<> struct TestType<natl::SerializeI8> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordI8)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI8, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI16> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordI16)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI16, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI32> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordI32)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI32, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI64> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordI64)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI64, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI8> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordUI8)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI8, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI16> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordUI16)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI16, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI32> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordUI32)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI32, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI64> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordUI64)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI64, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};

		template<> struct TestType<natl::SerializeF32> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordF32)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordF32, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeF64> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordF64)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordF64, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeBool> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordBlob)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};

		template<> struct TestType<natl::SerializeChar> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordChar)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordChar, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeStr> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordStr)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordStr, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeBlob> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordBlob)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<typename SerializeType> struct TestType<natl::SerializeOptional<SerializeType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordOp)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordOp, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				return TestType<SerializeType>::test(deserializer, errorLocation);
			}
		};
		template<typename BaseSerializeType> struct TestType<natl::SerializeEnum<BaseSerializeType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordEnum)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordEnum, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				return TestType<BaseSerializeType>::test(deserializer, errorLocation);
			}
		};
		template<> struct TestType<natl::SerializeFile> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordFile)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordFile, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<typename ElementType, natl::Size Number> struct TestType<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordFarray)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordFarray, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::keywordFile)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordFile, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}

				natl::Option<DeserializeErrorHandler> infoTypeTest = TestType<ElementType>::test(deserializer, errorLocation);
				if (infoTypeTest.hasValue()) {
					return infoTypeTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::comma)) {
					return deserializer.unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isTokenInteger(deserializer.getCurrentToken().type)) {
					error_message_string_type errorMessage = "expected integer but got ";
					errorMessage += tokenTypeToDebugString(deserializer.getCurrentToken().type);
					return DeserializeErrorHandler(errorMessage, deserializer.getErrorLocationDetails(),
						errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::rightSquare)) {
					return deserializer.unexpectedTokenOption(TokenType::rightSquare, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename ElementType> struct TestType<natl::SerializeArray<ElementType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordArray)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordArray, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::leftSquare)) {
					return deserializer.unexpectedTokenOption(TokenType::leftSquare, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserializeErrorHandler> infoTypeTest = TestType<ElementType>::test(deserializer, errorLocation);
				if (infoTypeTest.hasValue()) {
					return infoTypeTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::rightSquare)) {
					return deserializer.unexpectedTokenOption(TokenType::rightSquare, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename KeyType, typename ValueType> struct TestType<natl::SerializeDic<KeyType, ValueType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordDic)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordDic, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::leftCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserializeErrorHandler> keyTypeTest = TestType<KeyType>::test(deserializer, errorLocation);
				if (keyTypeTest.hasValue()) {
					return keyTypeTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::comma)) {
					return deserializer.unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserializeErrorHandler> valueTypeTest = TestType<ValueType>::test(deserializer, errorLocation);
				if (valueTypeTest.hasValue()) {
					return valueTypeTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::rightCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};
		template<typename... MemberTypes> struct TestType<natl::SerializeStruct<MemberTypes...>> {
			template<natl::Size Index, typename MemberType, typename... OtherMemberTypes>
			constexpr static natl::Option<DeserializeErrorHandler> testTypeMembers(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						return deserializer.unexpectedEndOfSourceOption(errorLocation);
					}
					if (!deserializer.isCurrentToken(TokenType::comma)) {
						return deserializer.unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
					}
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserializeErrorHandler> memberTypeTest = TestType<MemberType>::test(deserializer, errorLocation);
				if (memberTypeTest.hasValue()) {
					return memberTypeTest;
				}

				if constexpr (sizeof...(OtherMemberTypes) != 0) {
					return testTypeMembers<Index + 1, OtherMemberTypes...>(deserializer, errorLocation);
				} else {
					return {};
				}
			}


			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordStruct)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordStruct, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::leftCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				natl::Option<DeserializeErrorHandler> memberTypesTest = testTypeMembers<0, MemberTypes...>(deserializer, errorLocation);
				if (memberTypesTest.hasValue()) {
					return memberTypesTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::rightCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				return {};
			}
		};

		template<typename IndexSerializeType, typename... Types> struct TestType<natl::SerializeVariant<IndexSerializeType, Types...>> {
			template<natl::Size Index, typename MemberType, typename... OtherMemberTypes>
			constexpr static natl::Option<DeserializeErrorHandler> testTypeMembers(Deserializer& deserializer, const natl::DeserializeErrorLocation errorLocation) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						return deserializer.unexpectedEndOfSourceOption(errorLocation);
					}
					if (!deserializer.isCurrentToken(TokenType::comma)) {
						return deserializer.unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
					}
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserializeErrorHandler> memberTypeTest = TestType<MemberType>::test(deserializer, errorLocation);
				if (memberTypeTest.hasValue()) {
					return memberTypeTest;
				}

				if constexpr (sizeof...(OtherMemberTypes) != 0) {
					return testTypeMembers<Index + 1, OtherMemberTypes...>(deserializer, errorLocation);
				} else {
					return {};
				}
			}

			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.isCurrentToken(TokenType::keywordVariant)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordVariant, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserializeErrorHandler> indexTypeTest = TestType<IndexSerializeType>::test(deserializer, errorLocation);
				if (indexTypeTest.hasValue()) {
					return indexTypeTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::leftCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				natl::Option<DeserializeErrorHandler> memberTypesTest = testTypeMembers<0, Types...>(deserializer, errorLocation);
				if (memberTypesTest.hasValue()) {
					return memberTypesTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isCurrentToken(TokenType::rightCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename Type, template<typename> typename NameGetter>
		struct TestTypeAsNameBase {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (deserializer.isCurrentToken(TokenType::stringLiteral)) {
					if (deserializer.stringLiteralEqualsStr(deserializer.currentTokenString(), NameGetter<Type>::value)) {
						return {};
					} else {
						error_message_string_type errorMessage = "expected type name ";
						errorMessage += NameGetter<Type>::value;
						errorMessage += " but actual name was ";
						deserializer.formatStringLiteral<error_message_string_type>(deserializer.currentTokenString(), errorMessage);
						return DeserializeErrorHandler(
							errorMessage, deserializer.getErrorLocationDetails(),
							errorLocation, natl::DeserializeErrorFlag::wrongType);
					}
				}

				return  TestType<natl::DeserializeTypeOf<Type>>::test(deserializer, errorLocation);
			}
		};

		template<typename Type>
			requires(natl::IsDeserilizableC<Type>)
		struct TestType<Type> : TestType<natl::DeserializeTypeOf<Type>> {};
		template<typename Type>
			requires(natl::IsSerializableC<Type> && !natl::IsDeserilizableC<Type>)
		struct TestType<Type> : TestType<natl::SerializeTypeOf<Type>> {};

		template<typename Type>
			requires(natl::IsDeserilizableC<Type> && natl::HasDeserializeNameC<Type>)
		struct TestType<Type> : TestTypeAsNameBase<Type, natl::DeserializeNameV> {};
		template<typename Type>
			requires(natl::IsSerializableC<Type> && !natl::IsDeserilizableC<Type> && natl::HasSerializeNameC<Type>)
		struct TestType<Type> : TestTypeAsNameBase<Type, natl::SerializeNameV> {};

	public:
		template<typename DynStringType>
		constexpr void getLocationDetail(DynStringType& dst) noexcept {
			switch (errorFlag) {
			break; case natl::ErrorHandlingFlag::shorten:
				dst = natl::ConstAsciiStringView(lexicalInfo.source).substr(getCurrentToken().offset, 100);
			break; case natl::ErrorHandlingFlag::full:
				dst = natl::ConstAsciiStringView(lexicalInfo.source).substr(getCurrentToken().offset);
			break; default:
				natl::unreachable();
			}
		}

		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::DeserializeGlobalScope>, DeserializeErrorHandler> begin() noexcept {
			return DeserializeInfo<natl::DeserializeGlobalScope>{};
		}
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> end(
			DeserializeInfo<natl::DeserializeGlobalScope> info) noexcept {
			return {};
		}		

		template<typename Type, typename ParentType>
			//TODO requires()
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<Type>>, DeserializeErrorHandler> beginReadNamed(
			DeserializeInfo<ParentType>& parrent, const natl::ConstAsciiStringView name) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::beginReadNamed;
			
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}

			if(!isCurrentToken(TokenType::stringLiteral)) {
				return unexpectedToken(TokenType::stringLiteral, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			if(!stringLiteralEqualsStr(currentTokenString(), name)) {
				error_message_string_type errorMessage = "expected name ";
				errorMessage += name;
				errorMessage += " but actual name was ";
				formatStringLiteral<error_message_string_type>(currentTokenString(), errorMessage);
				return natl::unexpected(DeserializeErrorHandler(
					errorMessage, getErrorLocationDetails(), 
					errorLocation, natl::DeserializeErrorFlag::wrongName));
			}

			if(!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			natl::Option<DeserializeErrorHandler> typeError = TestType<Type>::test(self(), errorLocation);
			if(typeError.hasValue()) {
				return natl::unexpected(typeError.value());
			}

			if (!nextToken()) { 
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::colon)) {
				return unexpectedToken(TokenType::colon, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return DeserializeInfo<natl::SerializeTypeOf<Type>>{};
		}

		template<typename SerializeType>
			//TODO requires(IsSerializeTypeC<SerializeType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadNamed(DeserializeInfo<SerializeType>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::endReadNamed;
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if(!isCurrentToken(TokenType::comma)) {
				return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			return {};
		}

	private:
		template<typename IntegerType, NumericIntType PostExtIntType, natl::Bool TestIfIsInteger>
		[[nodiscard]] constexpr natl::Expect<IntegerType, DeserializeErrorHandler> readInteger(const natl::DeserializeErrorLocation& errorLocation) noexcept {
			if constexpr (TestIfIsInteger) {
				if (!nextToken()) {
					return unexpectedEndOfSource(errorLocation);
				}
				if (!isTokenInteger(getCurrentToken().type)) {
					error_message_string_type errorMessage = "expected integer but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken().type);
					return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
						errorLocation, natl::DeserializeErrorFlag::wrongType));
				}
			}

			natl::ConstAsciiStringView intString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().type)) {
				const natl::Size infoIndex = getTokenUnitIndexAfterToken(tokenIndex);
				const TokenNumericWithIntTypeUnit intUnit = natl::bitCast<TokenNumericWithIntTypeUnit, TokenUnit>(
					lexicalInfo.tokenUnits[infoIndex]
				);
				
				if (intUnit.intType != PostExtIntType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += numericIntTypeToString(PostExtIntType);
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
						errorLocation, natl::DeserializeErrorFlag::wrongType));
				}
				intString.removeSuffix(2);
			}

			if (intString[0] == '#') {
				intString.removePrefix(5);
			}

			natl::Expect<IntegerType, natl::StringNumericConvertError> value;
			switch (getCurrentToken().type) {
			case TokenType::decimalInteger:
			case TokenType::decimalIntegerWithType:
				value = natl::stringDecimalToInt<IntegerType>(intString);
				break;
			case TokenType::hexadecimalIntegerWithType:
			case TokenType::hexadecimalInteger:
				value = natl::stringHexadecimalToInt<IntegerType>(intString);
				break;
			case TokenType::binaryInteger:
			case TokenType::binaryIntegerWithType:
				value = natl::stringBinaryToInt<IntegerType>(intString);
				break;
			default:
				natl::unreachable();
			}


			if (value.hasError()) {
				error_message_string_type errorMessage = "failed to parse numeric of type ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().type);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(),
					errorLocation, natl::DeserializeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

		template<typename FloatType, NumericFloatType PostExtFloatType>
		[[nodiscard]] constexpr natl::Expect<FloatType, DeserializeErrorHandler> readFloat(const natl::DeserializeErrorLocation& errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isTokenFloat(getCurrentToken().type)) {
				error_message_string_type errorMessage = "expected float but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().type);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(),
					errorLocation, natl::DeserializeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView floatString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().type)) {
				const natl::Size infoIndex = getTokenUnitIndexAfterToken(tokenIndex, getCurrentToken());
				const TokenNumericWithFloatTypeUnit floatUnit = natl::bitCast<TokenNumericWithFloatTypeUnit, TokenUnit>(
					lexicalInfo.tokenUnits[infoIndex]
				);

				if (floatUnit.floatType != PostExtFloatType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += numericFloatTypeToString(PostExtFloatType);
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
						errorLocation, natl::DeserializeErrorFlag::wrongType));
				}
				floatString.removeSuffix(2);
			}

			if (floatString[0] == '#') {
				floatString.removePrefix(5);
			}

			natl::Expect<FloatType, natl::StringNumericConvertError> value;
			switch (getCurrentToken().type) {
			case TokenType::decimalFloat:
			case TokenType::decimalFloatWithType:
				value = natl::stringDecimalToFloat<FloatType>(floatString);
			case TokenType::hexadecimalFloatWithType:
			case TokenType::hexadecimalFloat:
			case TokenType::binaryFloat:
			case TokenType::binaryFloatWithType:
				//TODO
			default:
				natl::unreachable();
			}

			if (value.error()) {
				error_message_string_type errorMessage = "failed to parse numeric of type ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().type);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
					errorLocation, natl::DeserializeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

	public:
		[[nodiscard]] constexpr natl::Expect<natl::i8, DeserializeErrorHandler> readI8(DeserializeInfo<natl::SerializeI8>& info) noexcept {
			return readInteger<natl::i8, NumericIntType::i8, true>(natl::DeserializeErrorLocation::readI8);
		}
		[[nodiscard]] constexpr natl::Expect<natl::i16, DeserializeErrorHandler> readI16(DeserializeInfo<natl::SerializeI16>& info) noexcept {
			return readInteger<natl::i16, NumericIntType::i16, true>(natl::DeserializeErrorLocation::readI16);
		}
		[[nodiscard]] constexpr natl::Expect<natl::i32, DeserializeErrorHandler> readI32(DeserializeInfo<natl::SerializeI32>& info) noexcept {
			return readInteger<natl::i32, NumericIntType::i32, true>(natl::DeserializeErrorLocation::readI32);
		}
		[[nodiscard]] constexpr natl::Expect<natl::i64, DeserializeErrorHandler> readI64(DeserializeInfo<natl::SerializeI64>& info) noexcept {
			return readInteger<natl::i64, NumericIntType::i64, true>(natl::DeserializeErrorLocation::readI64);
		}

		[[nodiscard]] constexpr natl::Expect<natl::ui8, DeserializeErrorHandler> readUI8(DeserializeInfo<natl::SerializeUI8>& info) noexcept {
			return readInteger<natl::ui8, NumericIntType::ui8, true>(natl::DeserializeErrorLocation::readUI8);
		}
		[[nodiscard]] constexpr natl::Expect<natl::ui16, DeserializeErrorHandler> readUI16(DeserializeInfo<natl::SerializeUI16>& info) noexcept {
			return readInteger<natl::ui16, NumericIntType::ui16, true>(natl::DeserializeErrorLocation::readUI16);
		}
		[[nodiscard]] constexpr natl::Expect<natl::ui32, DeserializeErrorHandler> readUI32(DeserializeInfo<natl::SerializeUI32>& info) noexcept {
			return readInteger<natl::ui32, NumericIntType::ui32, true>(natl::DeserializeErrorLocation::readUI32);
		}
		[[nodiscard]] constexpr natl::Expect<natl::ui64, DeserializeErrorHandler> readUI64(DeserializeInfo<natl::SerializeUI64>& info) noexcept {
			return readInteger<natl::ui64, NumericIntType::ui64, true>(natl::DeserializeErrorLocation::readUI64);
		}

		[[nodiscard]] constexpr natl::Expect<natl::f32, DeserializeErrorHandler> readF32(DeserializeInfo<natl::SerializeF32>& info) noexcept {
			return readFloat<natl::f32, NumericFloatType::f32, true>(natl::DeserializeErrorLocation::readF32);
		}
		[[nodiscard]] constexpr natl::Expect<natl::f64, DeserializeErrorHandler> readF64(DeserializeInfo<natl::SerializeF64>& info) noexcept {
			return readFloat<natl::f64, NumericFloatType::f64, true>(natl::DeserializeErrorLocation::readF64);
		}

		[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler> readBool(DeserializeInfo<natl::SerializeBool>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readBool;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}

			if (isCurrentToken(TokenType::keywordTrue)) {
				return true;
			} else if (isCurrentToken(TokenType::keywordFalse)) {
				return false;
			} else {
				error_message_string_type errorMessage = "expected true or false literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().type);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
					errorLocation, natl::DeserializeErrorFlag::wrongType));
			}
		}

		[[nodiscard]] constexpr natl::Expect<natl::Ascii, DeserializeErrorHandler> readChar(DeserializeInfo<natl::SerializeChar>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readChar;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::length1CharLiteral) || !isCurrentToken(TokenType::length2CharLiteral)) {
				return unexpectedToken(TokenType::length1CharLiteral, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}

			const natl::ConstAsciiStringView charLiteral = currentTokenString();
			if (charLiteral.size() == 1) {
				return charLiteral[0];
			} else {
				return charLiteral[1];
			}
		}

		template<typename StringDstType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readStr(DeserializeInfo<natl::SerializeStr>& info, StringDstType& dst) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readStr;
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::stringLiteral)) {
				return unexpectedTokenOption(TokenType::stringLiteral, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}

			const natl::ConstAsciiStringView stringLiteral = currentTokenString();
			const natl::Size infoIndex = getTokenUnitIndexAfterToken(tokenIndex);
			const natl::Size escapeCount = getTokenUi64IntegerAt(infoIndex + sizeof(natl::ui64), lexicalInfo.tokenUnits);

			dst.reserve(dst.size() + (stringLiteral.size() - escapeCount));
			stringLiteralToDst<StringDstType>(dst, stringLiteral);
			return {};
		}

		template<typename FileNameDstType, typename FileContentDstType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
			readFile(DeserializeInfo<natl::SerializeFile>& info, FileNameDstType& filenameDst, FileContentDstType& fileContentDst) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readFile;

			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::stringLiteral)) {
				return unexpectedTokenOption(TokenType::stringLiteral, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}
			const natl::ConstAsciiStringView filenameStringLiteral = currentTokenString();
			const natl::Size filenameInfoIndex = getTokenUnitIndexAfterToken(tokenIndex);
			const natl::Size filenameEscapeCount = getTokenUi64IntegerAt(filenameInfoIndex + sizeof(natl::ui64), lexicalInfo.tokenUnits);

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::dataStorage)) {
				return unexpectedTokenOption(TokenType::dataStorage, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}
			const natl::ConstAsciiStringView fileContentDataStorage = currentTokenString();
			const natl::Size fileContentInfoIndex = getTokenUnitIndexAfterToken(tokenIndex);
			const natl::Size fileContentEscapeCount = getTokenUi64IntegerAt(fileContentInfoIndex + sizeof(natl::ui64), lexicalInfo.tokenUnits);

			filenameDst.reserve(filenameDst.size() + (filenameStringLiteral.size() - filenameEscapeCount));
			fileContentDst.reserve(fileContentDst.size() + (fileContentDataStorage.size() - fileContentEscapeCount));

			stringLiteralToDst<FileNameDstType>(filenameDst, filenameStringLiteral);
			dataStorageToDst<FileContentDstType>(fileContentDst, fileContentDataStorage);
			return {};
		}

		template<typename DataDstType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readBlob(DeserializeInfo<natl::SerializeBlob>& info, DataDstType& dst) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readBlob;
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::keywordBlob)) {
				return unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}

			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::dataStorage)) {
				return unexpectedTokenOption(TokenType::dataStorage, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}
			const natl::ConstAsciiStringView dataStorage = currentTokenString();
			const natl::Size infoIndex = getTokenUnitIndexAfterToken(tokenIndex);
			const natl::Size escapeCount = getTokenUi64IntegerAt(infoIndex + sizeof(natl::ui64), lexicalInfo.tokenUnits);

			dst.reserve(dst.size() + (dataStorage.size() - escapeCount));
			dataStorageToDst<DataDstType>(dst, dataStorage);
			return {};
		}



		template<typename SerilizeType>
		[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler>
			readIsOptionalNull(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(natl::DeserializeErrorLocation::readIsOptionalNull);
			}
			return isCurrentToken(TokenType::keywordNull);
		}

		template<typename SerilizeType>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerilizeType>, DeserializeErrorHandler>
			readAsOptional(const natl::Bool isNull, DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeOptional<SerilizeType>>{};
		}

		private:
		template<typename IntegerType>
		constexpr natl::Expect<IntegerType, DeserializeErrorHandler>
			readIntegerOfType(const natl::DeserializeErrorLocation errorLocation) noexcept {
			natl::Expect<IntegerType, DeserializeErrorHandler> value;
			using integer_deserialize_type = natl::DeserializeTypeOf<IntegerType>;
			if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeI8>) {
				value = readInteger<IntegerType, NumericIntType::i8, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeI16>) {
				value = readInteger<IntegerType, NumericIntType::i16, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeI32>) {
				value = readInteger<IntegerType, NumericIntType::i32, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeI64>) {
				value = readInteger<IntegerType, NumericIntType::i64, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeUI8>) {
				value = readInteger<IntegerType, NumericIntType::ui8, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeUI16>) {
				value = readInteger<IntegerType, NumericIntType::ui16, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeUI32>) {
				value = readInteger<IntegerType, NumericIntType::ui32, false>(errorLocation);
			} else if constexpr (natl::IsSame<integer_deserialize_type, natl::SerializeUI64>) {
				value = readInteger<IntegerType, NumericIntType::ui64, false>(errorLocation);
			} else {
				natl::unreachable();
			}
			return value;
		}

		public:
		template<typename BaseType, typename Functor>
			requires(natl::IsSerializableC<BaseType>
				&& natl::IsStringToSerializeFlagConvertFunctor<Functor, BaseType>)
		[[nodiscard]] constexpr natl::Expect<natl::BasicSerializeTypeToType<natl::SerializeTypeOf<BaseType>>, DeserializeErrorHandler>
			readEnum(DeserializeInfo<natl::SerializeEnum<BaseType>>& info, Functor&& stringToEnum) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readEnum;

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isCurrentToken(TokenType::stringLiteral)) {
				const natl::ConstAsciiStringView enumValueStringLiteral = currentTokenString();

				error_message_string_type enumValueString;
				stringLiteralToDst<error_message_string_type>(enumValueString, enumValueStringLiteral);

				natl::Option<BaseType> value = stringToEnum(enumValueString.toStringView());
				
				if (value.hasValue()) {
					return value.value();
				} else {
					error_message_string_type errorMessage = "unexpected enum value as string literal ";
					errorMessage += enumValueStringLiteral;
					return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(),
						errorLocation, natl::DeserializeErrorFlag::valueParsing));
				}
			} else if(isTokenInteger(getCurrentToken().type)) {
				return readIntegerOfType<BaseType>(errorLocation);
			} else {
				error_message_string_type errorMessage = "expected string literal or integer but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().type);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(),
					errorLocation, natl::DeserializeErrorFlag::wrongType));
			}
		}
	private:
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadContainer(
			const natl::DeserializeErrorLocation errorLocation,
			const TokenType scopeBegin
		) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(scopeBegin)) {
				return unexpectedToken(scopeBegin, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			const natl::Size infoIndex = getTokenUnitIndexAfterToken(tokenIndex);
			const natl::Size numberOfElement = getTokenUi64IntegerAt(infoIndex + sizeof(natl::ui64), lexicalInfo.tokenUnits);
			return numberOfElement;
		}
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadContainer(
			const natl::DeserializeErrorLocation errorLocation, const TokenType scopeEnd) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(scopeEnd)) {
				return unexpectedTokenOption(scopeEnd, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return {};
		}
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyContainer(
			const natl::DeserializeErrorLocation errorLocation, const TokenType scopeEnd) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(scopeEnd)) {
				return unexpectedTokenOption(scopeEnd, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return {};
		}

		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadContainerElement(const natl::DeserializeErrorLocation errorLocation) {
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::comma)) {
				return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return {};
		};
	public:

		//array
		template<typename SerializeElementType>
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return beginReadContainer(natl::DeserializeErrorLocation::beginReadArray, TokenType::leftSquare);
		}

		template<typename SerializeElementType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return endReadContainer(natl::DeserializeErrorLocation::endReadArray, TokenType::rightSquare);
		}
		template<typename SerializeElementType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return endReadEmptyContainer(natl::DeserializeErrorLocation::endReadEmptyArray, TokenType::rightSquare);
		}

		template<typename ElementType>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler> beginReadArrayElement(
			DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<ElementType>>{};
		}
		template<typename SerializeElementType>
			requires(natl::IsSerializeTypeC<SerializeElementType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArrayElement(DeserializeInfo<SerializeElementType>& info) {
			return endReadContainerElement(natl::DeserializeErrorLocation::endReadArrayElement);
		};

		//dic
		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>>& info) noexcept {
			return beginReadContainer(natl::DeserializeErrorLocation::beginReadDic, TokenType::leftCurly);
		}
		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>>& info) {
			return endReadContainer(natl::DeserializeErrorLocation::endReadDic, TokenType::rightCurly);
		}
		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>>& info) {
			return endReadEmptyContainer(natl::DeserializeErrorLocation::endReadEmptyDic, TokenType::rightCurly);
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>, DeserializeErrorHandler> beginReadDicElement(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>{};
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDicElement(
			DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>& info) noexcept {
			return endReadContainerElement(natl::DeserializeErrorLocation::endReadDicElement);
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readDicKey(
			DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>& info) noexcept {
			return{};
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readDicValue(
			DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readDicValue;
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::colon)) {
				return unexpectedTokenOption(TokenType::colon, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			return {};
		}

		//struct
		template<typename... SerializeMemberTypes>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>, DeserializeErrorHandler> beginReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>> info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::beginReadStruct;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			return DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>{};
		}
		template<typename... SerializeMemberTypes>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>& info) {
			return endReadContainer(natl::DeserializeErrorLocation::endReadStruct, TokenType::rightCurly);
		}

		//varaint
		template<typename IndexSerializeType, typename... VariantSerializeTypes>
		constexpr natl::Expect<natl::Bool, DeserializeErrorHandler> 
			readIsEmptyVariant(DeserializeInfo<natl::SerializeVariant<IndexSerializeType, VariantSerializeTypes...>>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readIsEmptyVariant;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			return isCurrentToken(TokenType::keywordNull);
		}

		template<typename StringToVariatIndexFunctor, typename IndexType, typename... VariantTypes>
			requires(natl::IsStringToSerializeVariantIndexConvertFunctor<StringToVariatIndexFunctor, IndexType>)
		[[nodiscard]] constexpr natl::Expect<IndexType, DeserializeErrorHandler>
			beginReadVaraintGetIndex(
				DeserializeInfo<natl::SerializeVariant<IndexType, VariantTypes...>>& info,
				const natl::Bool isEmpty,
				StringToVariatIndexFunctor&& stringToIndex) {
			
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::beginReadVariantGetIndex;
			if (!isCurrentToken(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			natl::Size index = 0;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isCurrentToken(TokenType::stringLiteral)) {
				const natl::ConstAsciiStringView indexValueStringLiteral = currentTokenString();

				error_message_string_type indexValueString;
				stringLiteralToDst<error_message_string_type>(indexValueString, indexValueStringLiteral);

				natl::Option<IndexType> value = stringToIndex(indexValueString.toStringView());

				if (value.hasValue()) {
					index = value.value();
				} else {
					error_message_string_type errorMessage = "unexpected varaint string index value of the string literal ";
					errorMessage += indexValueStringLiteral;
					return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
						errorLocation, natl::DeserializeErrorFlag::valueParsing));
				}
			} else if (isTokenInteger(getCurrentToken().type)) {
				auto indexValueExpect = readIntegerOfType<IndexType>(errorLocation);
				
				if(indexValueExpect.hasValue()) {
					index = indexValueExpect.value();
				} else {
					error_message_string_type integerErrorMessage = "";
					indexValueExpect.error().template getErrorMessage<error_message_string_type>(integerErrorMessage);

					error_message_string_type errorMessage = "failed to parse variant index";
					if(integerErrorMessage.isNotEmpty()) {
						errorMessage += ", ";
						errorMessage += integerErrorMessage;
					}

					return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(),
						errorLocation, natl::DeserializeErrorFlag::valueParsing));
				}
			} else {
				error_message_string_type errorMessage = "expected string literal or integer but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().type);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, getErrorLocationDetails(), 
					errorLocation, natl::DeserializeErrorFlag::wrongType));
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::comma)) {
				return unexpectedToken(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			
			return index;
		}

		template<typename ElementType, typename IndexSize, typename... VariantTypes>
			requires(natl::TemplatePackHasElementC<natl::IsSameV, ElementType, VariantTypes...>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler>
			beginReadVaraintOfType(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& variant) {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::beginReadVariantOfType;
			
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			natl::Option<DeserializeErrorHandler> typeTest = TestType<ElementType>::test(self(), errorLocation);
			if (typeTest.hasValue()) {
				return natl::unexpected(typeTest.value());
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::rightCurly)) {
				return unexpectedToken(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!isCurrentToken(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return DeserializeInfo<natl::SerializeTypeOf<ElementType>>{};
		}

		template<typename ElementSerializeType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
			endReadVariant(DeserializeInfo<ElementSerializeType>& info) {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::endReadVariant;
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!isCurrentToken(TokenType::rightCurly)) {
				return unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			return {};
		}
	};
}