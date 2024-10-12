#pragma once 

//natl
#include <natl/container/arrayView.h>

//own
#include "lexer.h"

//interface 
namespace nadsad::ascii {
	template<template<typename> typename DeserializeInfo, typename DeserializeErrorHandler>
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
	public:
		[[nodiscard]] constexpr natl::Bool addSource(const natl::ArrayView<const natl::Char>& newSource) noexcept {
			source = newSource;
			tokenIndex = 0;
			lexicalInfo = lexicalAnalysis(natl::ConstAsciiStringView(source));
			return lexicalInfo.errors.isEmpty();
		}

		template<typename Serializer>
			requires(natl::IsSerializerC<Serializer>)
		constexpr void serializeErrors(Serializer& serializer) noexcept {
			using serialize_as_type = natl::SerializeArray<natl::SerializeTypeOf<nadsad::ascii::LexicalError>>;
			serializer.beginWrite<serialize_as_type>("LexcialErrors");

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

		constexpr const Token& getCurrentToken() noexcept {
			return lexicalInfo.tokens[tokenIndex];
		}
		constexpr natl::Bool currentTokenIs(const TokenType& tokenType) noexcept {
			return getCurrentToken().tokenType == tokenType;
		}
		constexpr natl::ConstAsciiStringView currentTokenString() noexcept {
			const Token& currentToken = getCurrentToken();
			return natl::ConstAsciiStringView((const char*)source[currentToken.offset], currentToken.size);
		}
		constexpr natl::Bool nextToken() noexcept {
			tokenIndex++;
			return tokenIndex >= lexicalInfo.tokens.size();
		}
		constexpr void goBackAToken() noexcept {
			tokenIndex--;
		}


		constexpr DeserializeErrorHandler unexpectedTokenHandler(const TokenType& expectedTokenType,
			const natl::DeserializeErrorLocation errorLocation,
			const natl::DeserializeErrorFlag errorFlag) noexcept {
			error_message_string_type errorMessage = "expected ";
			errorMessage += tokenTypeToDebugString(expectedTokenType);
			errorMessage += " but got ";
			errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
			return DeserializeErrorHandler(errorMessage, errorLocation, errorFlag);
		}

		constexpr DeserializeErrorHandler unexpectedEndOfSourceHandler(const natl::DeserializeErrorLocation errorLocation) noexcept {
			return DeserializeErrorHandler(endOfSourceErrorMessage, errorLocation, natl::DeserializeErrorFlag::endOfSource);
		}

		constexpr natl::Unexpected<DeserializeErrorHandler> unexpectedToken(const TokenType& expectedTokenType,
			const natl::DeserializeErrorLocation errorLocation,
			const natl::DeserializeErrorFlag errorFlag) noexcept {
			return natl::unexpected(unexpectedTokenHandler(expectedTokenType, errorLocation, errorFlag));
		}

		constexpr natl::Unexpected<DeserializeErrorHandler> unexpectedEndOfSource(const natl::DeserializeErrorLocation errorLocation) noexcept {
			return natl::unexpected(unexpectedEndOfSourceHandler(errorLocation));
		}

		constexpr natl::Option<DeserializeErrorHandler> unexpectedTokenOption(const TokenType& expectedTokenType,
			const natl::DeserializeErrorLocation errorLocation,
			const natl::DeserializeErrorFlag errorFlag) noexcept {
			return unexpectedTokenHandler(expectedTokenType, errorLocation, errorFlag);
		}

		constexpr natl::Option<DeserializeErrorHandler> unexpectedEndOfSourceOption(const natl::DeserializeErrorLocation errorLocation) noexcept {
			return unexpectedEndOfSourceHandler(errorLocation);
		}

	


		constexpr natl::Bool isTokenNumericWithType(const TokenType& tokenType) noexcept {
			switch (tokenType) {
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
		constexpr natl::Bool isTokenFloat(const TokenType& tokenType) noexcept {
			switch (tokenType) {
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
		constexpr natl::Bool isTokenInteger(const TokenType& tokenType) noexcept {
			switch (tokenType) {
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
			natl::Size stringLiteralIndex = 0;
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
			return stringLiteralIndex == stringLiteral.size() - 1 && testStringIndex == testString.size() - 1;
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
			dst.reserve(stringLiteral.size());

			for (natl::Size i = 0; i < stringLiteral.size(); i++) {
				if (stringLiteral[i] == '\\') {
					if (!(i < stringLiteral.size())) { break; }
					i++;
				}
				dst.push_back(stringLiteral[i]);
			}
		}
		template<typename DataStroageDstType>
		constexpr void dataStorageToDst(DataStroageDstType& dst, const natl::ConstAsciiStringView& dataStorage) noexcept {
			dst.reserve(dataStorage.size());
			for (natl::Size i = 0; i < dataStorage.size(); i++) {
				if (dataStorage[i] == 255) {
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
				if (!deserializer.currentTokenIs(TokenType::keywordI8)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI8, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI16> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI16)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI16, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI32> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI32)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI32, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI64> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI64)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordI64, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI8> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI8)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI8, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI16> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI16)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI16, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI32> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI32)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI32, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI64> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI64)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordUI64, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};

		template<> struct TestType<natl::SerializeF32> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordF32)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordF32, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeF64> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordF64)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordF64, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeBool> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordBlob)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};

		template<> struct TestType<natl::SerializeChar> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordChar)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordChar, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeStr> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordStr)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordStr, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeBlob> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordBlob)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<typename SerializeType> struct TestType<natl::SerializeOptional<SerializeType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordOp)) {
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
				if (!deserializer.currentTokenIs(TokenType::keywordEnum)) {
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
				if (!deserializer.currentTokenIs(TokenType::keywordFile)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordFile, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<typename ElementType, natl::Size Number> struct TestType<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordFarray)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordFarray, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::keywordFile)) {
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
				if (!deserializer.currentTokenIs(TokenType::comma)) {
					return deserializer.unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isTokenInteger(getCurrentToken().tokenType)) {
					error_message_string_type errorMessage = "expected integer but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
					return DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightSquare)) {
					return deserializer.unexpectedTokenOption(TokenType::rightSquare, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename ElementType> struct TestType<natl::SerializeArray<ElementType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordArray)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordArray, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftSquare)) {
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
				if (!deserializer.currentTokenIs(TokenType::rightSquare)) {
					return deserializer.unexpectedTokenOption(TokenType::rightSquare, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename KeyType, typename ValueType> struct TestType<natl::SerializeDic<KeyType, ValueType>> {
			constexpr static natl::Option<DeserializeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserializeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordDic)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordDic, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
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
				if (!deserializer.currentTokenIs(TokenType::comma)) {
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
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
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
					if (!deserializer.currentTokenIs(TokenType::comma)) {
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
				if (!deserializer.currentTokenIs(TokenType::keywordStruct)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordStruct, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				natl::Option<DeserializeErrorHandler> memberTypesTest = testTypeMembers<0, MemberTypes...>(deserializer, errorLocation);
				if (memberTypesTest.hasValue()) {
					return memberTypesTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				return {};
			}
		};

		template<typename... Types> struct TestType<natl::SerializeVariant<Types...>> {
			template<natl::Size Index, typename MemberType, typename... OtherMemberTypes>
			constexpr static natl::Option<DeserializeErrorHandler> testTypeMembers(Deserializer& deserializer, const natl::DeserializeErrorLocation errorLocation) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						return deserializer.unexpectedEndOfSourceOption(errorLocation);
					}
					if (!deserializer.currentTokenIs(TokenType::comma)) {
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
				if (!deserializer.currentTokenIs(TokenType::keywordVariant)) {
					return deserializer.unexpectedTokenOption(TokenType::keywordVariant, errorLocation, natl::DeserializeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				natl::Option<DeserializeErrorHandler> memberTypesTest = testTypeMembers<0, Types...>(deserializer, errorLocation);
				if (memberTypesTest.hasValue()) {
					return memberTypesTest;
				}

				if (!deserializer.nextToken()) {
					return deserializer.unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					return deserializer.unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};
	public:


		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::DeserializeGlobalScope>, DeserializeErrorHandler> begin() noexcept {
			//TODO
			return DeserializeInfo<natl::DeserializeGlobalScope>{};
		}
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> end(
			//TODO
			DeserializeInfo<natl::DeserializeGlobalScope> info) noexcept {
			return {};
		}		

		template<typename SerializeType, typename ParentType>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerializeType>, DeserializeErrorHandler> beginReadName(
			DeserializeInfo<ParentType>& parrent, const natl::ConstAsciiStringView name) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::beginReadArray;
			
			DeserializeInfo<SerializeType> info;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}

			if(!currentTokenIs(TokenType::stringLiteral)) {
				return unexpectedToken(TokenType::stringLiteral, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			if(stringLiteralEqualsStr(currentTokenString(), name)) {
				error_message_string_type errorMessage = "expected name ";
				errorMessage += name;
				errorMessage += " but actual name was ";
				formatStringLiteral<error_message_string_type>(currentTokenString(), errorMessage);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongName));
			}

			if(!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			natl::Option<DeserializeErrorHandler> typeError = TestType<SerializeType>::test(self(), errorLocation);
			if(typeError.hasValue()) {
				return natl::unexpected(typeError.value());
			}

			if (!nextToken()) { 
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::colon)) {
				return unexpectedToken(TokenType::colon, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return info;
		}

		template<typename SerializeType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadNamed(DeserializeInfo<SerializeType>& info) noexcept {
			return {};
		}

	private:
		template<typename IntegerType, LiteralPostExtIntType PostExtIntType>
		[[nodiscard]] constexpr natl::Expect<IntegerType, DeserializeErrorHandler> readInteger(const natl::DeserializeErrorLocation& errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isTokenInteger(getCurrentToken().tokenType)) {
				error_message_string_type errorMessage = "expected integer but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView intString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().tokenType)) {
				const TokenWithPostExtIntType intToken = natl::bitCast<TokenWithPostExtIntType, Token>(getCurrentToken());
				const LiteralPostExtIntType intType = natl::fromUnderlying<LiteralPostExtIntType>(intToken.postExtIntType);

				if (intType != PostExtIntType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += literalPostExtIntTypeToString(PostExtIntType);
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType));
				}
				intString.remove_suffix(2);
			}

			if (intString[0] == '#') {
				intString.remove_prefix(5);
			}

			natl::Expect<IntegerType, natl::StringNumericConvertError> value;
			switch (getCurrentToken().tokenType) {
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
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

		template<typename FloatType, LiteralPostExtFloatType PostExtFloatType>
		[[nodiscard]] constexpr natl::Expect<FloatType, DeserializeErrorHandler> readFloat(const natl::DeserializeErrorLocation& errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isTokenFloat(getCurrentToken().tokenType)) {
				error_message_string_type errorMessage = "expected float but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView floatString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().tokenType)) {
				const TokenWithPostExtFloatType floatToken = natl::bitCast<TokenWithPostExtFloatType, Token>(getCurrentToken());
				const LiteralPostExtFloatType floatType = natl::fromUnderlying<LiteralPostExtFloatType>(floatToken.postExtFloatType);

				if (floatType != PostExtFloatType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += literalPostExtFloatTypeToString(PostExtFloatType);
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType));
				}
				floatString.remove_suffix(2);
			}

			if (floatString[0] == '#') {
				floatString.remove_prefix(5);
			}

			natl::Expect<FloatType, natl::StringNumericConvertError> value;
			switch (getCurrentToken().tokenType) {
			case TokenType::decimalFloat:
			case TokenType::decimalFloatWithType:
				value = natl::stringDecimalToFloat(floatString);
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
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

	public:
		[[nodiscard]] constexpr natl::Expect<natl::i8, DeserializeErrorHandler> readI8(DeserializeInfo<natl::SerializeI8>& info) noexcept {
			return readInteger<natl::i8, LiteralPostExtIntType::i8>(natl::DeserializeErrorLocation::readI8);
		}
		[[nodiscard]] constexpr natl::Expect<natl::i16, DeserializeErrorHandler> readI16(DeserializeInfo<natl::SerializeI16>& info) noexcept {
			return readInteger<natl::i16, LiteralPostExtIntType::i16>(natl::DeserializeErrorLocation::readI16);
		}
		[[nodiscard]] constexpr natl::Expect<natl::i32, DeserializeErrorHandler> readI32(DeserializeInfo<natl::SerializeI32>& info) noexcept {
			return readInteger<natl::i32, LiteralPostExtIntType::i32>(natl::DeserializeErrorLocation::readI32);
		}
		[[nodiscard]] constexpr natl::Expect<natl::i64, DeserializeErrorHandler> readI64(DeserializeInfo<natl::SerializeI64>& info) noexcept {
			return readInteger<natl::i64, LiteralPostExtIntType::i64>(natl::DeserializeErrorLocation::readI64);
		}

		[[nodiscard]] constexpr natl::Expect<natl::ui8, DeserializeErrorHandler> readUI8(DeserializeInfo<natl::SerializeUI8>& info) noexcept {
			return readInteger<natl::ui8, LiteralPostExtIntType::ui8>(natl::DeserializeErrorLocation::readUI8);
		}
		[[nodiscard]] constexpr natl::Expect<natl::ui16, DeserializeErrorHandler> readUI16(DeserializeInfo<natl::SerializeUI16>& info) noexcept {
			return readInteger<natl::ui16, LiteralPostExtIntType::ui16>(natl::DeserializeErrorLocation::readUI16);
		}
		[[nodiscard]] constexpr natl::Expect<natl::ui32, DeserializeErrorHandler> readUI32(DeserializeInfo<natl::SerializeUI32>& info) noexcept {
			return readInteger<natl::ui32, LiteralPostExtIntType::ui32>(natl::DeserializeErrorLocation::readUI32);
		}
		[[nodiscard]] constexpr natl::Expect<natl::ui64, DeserializeErrorHandler> readUI64(DeserializeInfo<natl::SerializeUI64>& info) noexcept {
			return readInteger<natl::ui64, LiteralPostExtIntType::ui64>(natl::DeserializeErrorLocation::readUI64);
		}

		[[nodiscard]] constexpr natl::Expect<natl::f32, DeserializeErrorHandler> readF32(DeserializeInfo<natl::SerializeF32>& info) noexcept {
			return readFloat<natl::f32, LiteralPostExtFloatType::f32>(natl::DeserializeErrorLocation::readF32);
		}
		[[nodiscard]] constexpr natl::Expect<natl::f64, DeserializeErrorHandler> readF64(DeserializeInfo<natl::SerializeF64>& info) noexcept {
			return readFloat<natl::f64, LiteralPostExtFloatType::f64>(natl::DeserializeErrorLocation::readF64);
		}

		[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler> readBool(DeserializeInfo<natl::SerializeBool>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readBool;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}

			if (currentTokenIs(TokenType::keywordTrue)) {
				return true;
			} else if (currentTokenIs(TokenType::keywordFalse)) {
				return false;
			} else {
				error_message_string_type errorMessage = "expected true or false literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType));
			}
		}

		[[nodiscard]] constexpr natl::Expect<natl::Ascii, DeserializeErrorHandler> readChar(DeserializeInfo<natl::SerializeChar>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readChar;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::charLiteral)) {
				return unexpectedToken(TokenType::charLiteral, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}

			const natl::ConstAsciiStringView charLiteral = currentTokenString();
			if (charLiteral.size() == 1) {
				return charLiteral[0];
			} else {
				return charLiteral[1];
			}
		}

		template<typename StringDstType = error_message_string_type>
		[[nodiscard]] constexpr natl::Expect<StringDstType, DeserializeErrorHandler> readStr(DeserializeInfo<natl::SerializeStr>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readStr;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::stringLiteral)) {
				return unexpectedToken(TokenType::stringLiteral, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}

			const natl::ConstAsciiStringView stringLiteral = currentTokenString();
			StringDstType output;
			stringLiteralToDst<StringDstType>(output, stringLiteral);
			return output;
		}

		template<typename FileNameDstType, typename FileContentDstType>
		struct FileDeserializeReturn {
			FileNameDstType filename;
			FileContentDstType fileContent;
		};

		template<typename FileNameDstType = error_message_string_type, typename FileContentDstType = error_message_string_type>
		[[nodiscard]] constexpr natl::Expect<FileDeserializeReturn<FileNameDstType, FileContentDstType>, DeserializeErrorHandler>
			readFile(DeserializeInfo<natl::SerializeFile>& info) noexcept {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readFile;

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::stringLiteral)) {
				return unexpectedToken(TokenType::stringLiteral, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}
			const natl::ConstAsciiStringView fileNameStringLiteral = currentTokenString();

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::dataStorage)) {
				return unexpectedToken(TokenType::dataStorage, errorLocation, natl::DeserializeErrorFlag::wrongType);
			}
			const natl::ConstAsciiStringView fileContentDataStorage = currentTokenString();

			FileDeserializeReturn<FileNameDstType, FileContentDstType> fileReturn;
			stringLiteralToDst<FileNameDstType>(fileReturn.fileName, fileNameStringLiteral);
			dataStorageToDst<FileContentDstType>(fileReturn.fileContent, fileContentDataStorage);
			return fileReturn;
		}

		template<typename BaseSerilizeType, typename Functor>
			requires(natl::IsEnumBaseSerializeTypeC<BaseSerilizeType> 
				&& natl::IsStringToSerializeFlagConvertFunctor<Functor, natl::BasicSerializeTypeToType<BaseSerilizeType>>)
		[[nodiscard]] constexpr natl::Expect<natl::BasicSerializeTypeToType<BaseSerilizeType>, DeserializeErrorHandler>
			readEnum(DeserializeInfo<natl::SerializeEnum<BaseSerilizeType>>& info, Functor&& stringToEnum) noexcept {
			using enum_integer_type = natl::BasicSerializeTypeToType<BaseSerilizeType>;
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::readEnum;

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (currentTokenIs(TokenType::stringLiteral)) {
				const natl::ConstAsciiStringView enumValueStringLiteral = currentTokenString();

				error_message_string_type enumValueString;
				stringLiteralToDst<error_message_string_type>(enumValueString, enumValueStringLiteral);

				natl::Option<enum_integer_type> value = stringToEnum(enumValueString.toStringView());
				
				if (value.hasValue()) {
					return value.value();
				} else {
					error_message_string_type errorMessage = "unexpected enum value of the string literal ";
					errorMessage += enumValueStringLiteral;
					return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::valueParsing));
				}
			} else if(isTokenInteger(getCurrentToken().tokenType)) {
				goBackAToken();
				natl::Expect<enum_integer_type, DeserializeErrorHandler> value;
				if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeI8>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::i8>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeI16>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::i16>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeI32>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::i32>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeI64>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::i64>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeUI8>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::ui8>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeUI16>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::ui16>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeUI32>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::ui32>(errorLocation);
				} else if constexpr (natl::IsSame<BaseSerilizeType, natl::SerializeUI64>) {
					value = readInteger<enum_integer_type, LiteralPostExtIntType::ui64>(errorLocation);
				} else {
					natl::unreachable();
				}

				if(value.hasValue()) {
					return value.value();
				} else {
					return natl::unexpected(value.error());
				}
			} else {
				error_message_string_type errorMessage = "expected string literal or integer but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongType));
			}
		}
	private:
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadContainer(const natl::DeserializeErrorLocation errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}


			natl::Size index = tokenIndex + 1;
			if (!(index < lexicalInfo.tokens.size())) {
				error_message_string_type errorMessage = "expected more tokens after array being";
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongFormatting));
			}

			if (lexicalInfo.tokens[index].tokenType == TokenType::rightCurly) {
				nextToken();
				return 0;
			}

			natl::Size count = 0;
			while (lexicalInfo.tokens[index].tokenType != TokenType::rightCurly) {
				const Token& token = lexicalInfo.tokens[index];
				switch (token.tokenType) {
				break; case TokenType::comma:
					count++;
					break;
				case TokenType::leftCurly:
				case TokenType::leftSquare:
					if (token.size == 0) {
						error_message_string_type errorMessage = "scope error with token ";
						errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
						return natl::unexpected(DeserializeErrorHandler(errorMessage, errorLocation, natl::DeserializeErrorFlag::wrongFormatting));
					}
					index += token.size;
				break; default:
					break;
				}
			}
			return count;
		}
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadContainer(const natl::DeserializeErrorLocation errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return {};
		}

		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadContainerElement(const natl::DeserializeErrorLocation errorLocation) {
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!currentTokenIs(TokenType::comma)) {
				return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return {};
		};
	public:

		//array
		template<typename SerializeElementType>
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return beginReadContainer(natl::DeserializeErrorLocation::beginReadArray);
		}

		template<typename SerializeElementType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return endReadContainer(natl::DeserializeErrorLocation::endReadArray);
		}

		template<typename SerializeElementType>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerializeElementType>, DeserializeErrorHandler> beginReadArrayElement(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return DeserializeInfo<SerializeElementType>{};
		}
		template<typename SerializeElementType>
		constexpr natl::Option<DeserializeErrorHandler> endReadArrayElement(DeserializeInfo<SerializeElementType>& info) {
			return endReadContainerElement(natl::DeserializeErrorLocation::endReadArrayElement);
		};

		//dic
		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>>& info) noexcept {
			return beginReadContainer(natl::DeserializeErrorLocation::beginReadDic);
		}
		template<typename SerializeKeyType, typename SerializeValueType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>>& info) {
			return endReadContainer(natl::DeserializeErrorLocation::endReadDic);
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
			if (!currentTokenIs(TokenType::colon)) {
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
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			return {};
		}
		template<typename... SerializeMemberTypes>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>& info) {
			return endReadContainer(natl::DeserializeErrorLocation::endReadStruct);
		}

		//varaint
		template<typename SerializeType, typename... VariantSerializeTypes>
			requires(natl::TemplatePackHasElementC<natl::IsSameV, SerializeType, VariantSerializeTypes...>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerializeType>, DeserializeErrorHandler>
			beginReadVaraint(DeserializeInfo<natl::SerializeVariant<VariantSerializeTypes...>>& info) {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::beginReadVariant;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::keywordVariant)) {
				return unexpectedToken(TokenType::keywordVariant, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			natl::Option<DeserializeErrorHandler> typeTest = TestType<SerializeType>::test(self(), errorLocation);
			if (typeTest.hasValue()) {
				return natl::unexpected(typeTest.value());
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::rightCurly)) {
				return unexpectedToken(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}

			return DeserializeInfo<SerializeType>{};
		}

		template<typename SerializeType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
			endReadVariant(DeserializeInfo<SerializeType>& info) {
			constexpr natl::DeserializeErrorLocation errorLocation = natl::DeserializeErrorLocation::endReadVariant;
			if (!nextToken()) {
				return unexpectedEndOfSourceOption(errorLocation);
			}
			if (!currentTokenIs(TokenType::rightCurly)) {
				return unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserializeErrorFlag::wrongFormatting);
			}
			return {};
		}
	};
}