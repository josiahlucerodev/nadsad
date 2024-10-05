#pragma once 

//natl
#include <natl/container/arrayView.h>

//own
#include "lexer.h"

//interface 
namespace nadsad::ascii {
	template<template<typename> typename DeserializeInfo, typename DeserilizeErrorHandler>
	struct Deserializer {
	public:
		using deserilize_error_handler = DeserilizeErrorHandler;
		using error_message_string_type = natl::String256;
	private:
		natl::ArrayView<natl::Byte> source;
		LexicalInfo lexicalInfo;
		natl::Size tokenIndex;
	public:
		constexpr natl::Bool addSource(const natl::ArrayView<natl::Byte>& newSource) noexcept {
			source = newSource;
			tokenIndex = 0;
			//TODO
		}

		
		constexpr natl::Bool processSource() noexcept {
			//TODO
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


		constexpr DeserilizeErrorHandler unexpectedTokenHandler(const TokenType& expectedTokenType,
			const natl::DeserilizeErrorLocation errorLocation,
			const natl::DeserilizeErrorFlag errorFlag) noexcept {
			error_message_string_type errorMessage = "expected ";
			errorMessage += tokenTypeToDebugString(expectedTokenType);
			errorMessage += " but got ";
			errorMessage += tokenTypeToDebugString(getCurrentToken());
			return DeserilizeErrorHandler(errorMessage, errorLocation, errorFlag);
		}

		constexpr DeserilizeErrorHandler unexpectedEndOfSourceHandler(const natl::DeserilizeErrorLocation errorLocation) noexcept {
			return DeserilizeErrorHandler(endOfSourceErrorMessage, errorLocation, natl::DeserilizeErrorFlag::endOfSource);
		}

		constexpr natl::Unexpected<DeserilizeErrorHandler> unexpectedToken(const TokenType& expectedTokenType,
			const natl::DeserilizeErrorLocation errorLocation,
			const natl::DeserilizeErrorFlag errorFlag) noexcept {
			return natl::unexpected(unexpectedTokenHandler(expectedTokenType, errorLocation, errorFlag));
		}

		constexpr natl::Option<DeserilizeErrorHandler> unexpectedEndOfSource(const natl::DeserilizeErrorLocation errorLocation) noexcept {
			return natl::unexpected(unexpectedEndOfSourceHandler(errorLocation));
		}

		constexpr natl::Unexpected<DeserilizeErrorHandler> unexpectedTokenOption(const TokenType& expectedTokenType,
			const natl::DeserilizeErrorLocation errorLocation,
			const natl::DeserilizeErrorFlag errorFlag) noexcept {
			return unexpectedTokenHandler(expectedTokenType, errorLocation, errorFlag);
		}

		constexpr natl::Option<DeserilizeErrorHandler> unexpectedEndOfSourceOption(const natl::DeserilizeErrorLocation errorLocation) noexcept {
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
		constexpr natl::Bool formatStringLiteral(const natl::ConstAsciiStringView& stringLiteral, OutputDstType& output) noexcept {
			for (natl::Size i = 0; i < stringLiteral.size() ; i++) {
				if (stringLiteral[i] == '\\') {
					i++;
				}
				output += stringLiteral[i];
			}
		}
		template<typename StringDstType>
		constexpr StringDstType formattedStringLiteralToString(const natl::ConstAsciiStringView& stringLiteral) noexcept {
			StringDstType output{};
			output.reserve(stringLiteral.size());

			for (natl::Size i = 0; i < stringLiteral.size(); i++) {
				if (stringLiteral[i] == '\\') {
					if (!(i < stringLiteral.size())) { break; }
					i++;
				}
				output.pushBack(stringLiteral[i]);
			}

			return output;
		}

		//test
		template<typename SerializeType>
		struct TestType;

		template<> struct TestType<natl::SerializeI8> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer, 
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI8)) {
					return unexpectedTokenOption(TokenType::keywordI8, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI16> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI16)) {
					return unexpectedTokenOption(TokenType::keywordI16, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI32> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI32)) {
					return unexpectedTokenOption(TokenType::keywordI32, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeI64> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordI64)) {
					return unexpectedTokenOption(TokenType::keywordI64, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI8> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI8)) {
					return unexpectedTokenOption(TokenType::keywordUI8, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI16> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI16)) {
					return unexpectedTokenOption(TokenType::keywordUI16, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI32> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI32)) {
					return unexpectedTokenOption(TokenType::keywordUI32, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeUI64> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordUI64)) {
					return unexpectedTokenOption(TokenType::keywordUI64, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};

		template<> struct TestType<natl::SerializeF32> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordF32)) {
					return unexpectedTokenOption(TokenType::keywordF32, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeF64> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordF64)) {
					return unexpectedTokenOption(TokenType::keywordF64, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeBool> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordBlob)) {
					return unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};

		template<> struct TestType<natl::SerializeChar> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordChar)) {
					return unexpectedTokenOption(TokenType::keywordChar, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeStr> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordStr)) {
					return unexpectedTokenOption(TokenType::keywordStr, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<> struct TestType<natl::SerializeBlob> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordBlob)) {
					return unexpectedTokenOption(TokenType::keywordBlob, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<typename SerializeType> struct TestType<natl::SerializeOptional<SerializeType>> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordOp)) {
					return unexpectedTokenOption(TokenType::keywordOp, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				return TestType<SerializeType>::test(deserializer, errorLocation);
			}
		};
		template<typename BaseSerializeType> struct TestType<natl::SerializeEnum<BaseSerializeType>> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordEnum)) {
					return unexpectedTokenOption(TokenType::keywordEnum, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				return TestType<BaseSerializeType>::test(deserializer, errorLocation);
			}
		};
		template<> struct TestType<natl::SerializeFile> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordFile)) {
					return unexpectedTokenOption(TokenType::keywordFile, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}
				return {};
			}
		};
		template<typename ElementType, natl::Size Number> struct TestType<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordFarray)) {
					return unexpectedTokenOption(TokenType::keywordFarray, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::keywordFile)) {
					return unexpectedTokenOption(TokenType::keywordFile, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}

				natl::Option<DeserilizeErrorHandler> infoTypeTest = TestType<ElementType>::test(deserializer, errorLocation);
				if (infoTypeTest.hasValue()) {
					return infoTypeTest;
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::comma)) {
					return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.isTokenInteger(getCurrentToken().tokenType)) {
					error_message_string_type errorMessage = "expected integer but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					return DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightSquare)) {
					return unexpectedTokenOption(TokenType::rightSquare, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename ElementType> struct TestType<natl::SerializeArray<ElementType>> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordArray)) {
					return unexpectedTokenOption(TokenType::keywordArray, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftSquare)) {
					return unexpectedTokenOption(TokenType::leftSquare, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserilizeErrorHandler> infoTypeTest = TestType<ElementType>::test(deserializer, errorLocation);
				if (infoTypeTest.hasValue()) {
					return infoTypeTest;
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightSquare)) {
					return unexpectedTokenOption(TokenType::rightSquare, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};

		template<typename KeyType, typename ValueType> struct TestType<natl::SerializeDic<KeyType, ValueType>> {
			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordDic)) {
					return unexpectedTokenOption(TokenType::keywordDic, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					return unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserilizeErrorHandler> keyTypeTest = TestType<KeyType>::test(deserializer, errorLocation);
				if (keyTypeTest.hasValue()) {
					return keyTypeTest;
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::comma)) {
					return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserilizeErrorHandler> valueTypeTest = TestType<ValueType>::test(deserializer, errorLocation);
				if (valueTypeTest.hasValue()) {
					return valueTypeTest;
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					return unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				return {};
			}
		};
		template<typename... MemberTypes> struct TestType<natl::SerializeStruct<MemberTypes...>> {
			template<natl::Size Index, typename MemberType, typename... OtherMemberTypes>
			constexpr static natl::Option<DeserilizeErrorHandler> testTypeMembers(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						return unexpectedEndOfSourceOption(errorLocation);
					}
					if (!deserializer.currentTokenIs(TokenType::comma)) {
						return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
					}
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserilizeErrorHandler> memberTypeTest = TestType<MemberType>::test(deserializer, errorLocation);
				if (memberTypeTest.hasValue()) {
					return memberTypeTest;
				}

				if constexpr (sizeof...(OtherMemberTypes) != 0) {
					return testTypeMembers<Index + 1, OtherMemberTypes...>(deserializer, errorLocation);
				} else {
					return {};
				}
			}


			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordStruct)) {
					return unexpectedTokenOption(TokenType::keywordStruct, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					return unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				natl::Option<DeserilizeErrorHandler> memberTypesTest = testTypeMembers<0, MemberTypes...>(deserializer, errorLocation);
				if (memberTypesTest.hasValue()) {
					return memberTypesTest;
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					return unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				return {};
			}
		};

		template<typename... Types> struct TestType<natl::SerializeVariant<Types...>> {
			template<natl::Size Index, typename MemberType, typename... OtherMemberTypes>
			constexpr static natl::Option<DeserilizeErrorHandler> testTypeMembers(Deserializer& deserializer, const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						return unexpectedEndOfSourceOption(errorLocation);
					}
					if (!deserializer.currentTokenIs(TokenType::comma)) {
						return unexpectedTokenOption(TokenType::comma, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
					}
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				natl::Option<DeserilizeErrorHandler> memberTypeTest = TestType<MemberType>::test(deserializer, errorLocation);
				if (memberTypeTest.hasValue()) {
					return memberTypeTest;
				}

				if constexpr(sizeof...(OtherMemberTypes) != 0) {
					return testTypeMembers<Index + 1, OtherMemberTypes...>(deserializer, errorLocation);
				} else {
					return {};
				}
			}

			constexpr static natl::Option<DeserilizeErrorHandler> test(Deserializer& deserializer,
				const natl::DeserilizeErrorLocation errorLocation) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordVariant)) {
					return unexpectedTokenOption(TokenType::keywordVariant, errorLocation, natl::DeserilizeErrorFlag::wrongType);
				}

				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					return unexpectedTokenOption(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				natl::Option<DeserilizeErrorHandler> memberTypesTest = testTypeMembers<0, Types...>(deserializer, errorLocation);
				if (memberTypesTest.hasValue()) {
					return memberTypesTest;
				}
	
				if (!deserializer.nextToken()) {
					return unexpectedEndOfSourceOption(errorLocation);
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					return unexpectedTokenOption(TokenType::rightCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
				}

				return true;
			}
		};
	public:


		constexpr natl::Expect<DeserializeInfo<natl::DeserilizeGlobalScope>, DeserilizeErrorHandler> 
			beginDeserialization() noexcept {
			//TODO
			return DeserializeInfo<natl::DeserilizeGlobalScope>{}
		}
		constexpr natl::Option<DeserilizeErrorHandler> endDeserialization(
			//TODO
			DeserializeInfo<natl::DeserilizeGlobalScope> info) noexcept {
			return {}
		}		

		template<typename SerializeType, typename ParentType>
		constexpr natl::Expect<DeserializeInfo<SerializeType>, DeserilizeErrorHandler> readNamed(
			DeserializeInfo<ParentType>& parrent, const natl::ConstAsciiStringView name) noexcept {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::readNamed;
			
			DeserializeInfo<SerializeType> info;
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}

			if(!currentTokenIs(TokenType::stringLiteral)) {
				return unexpectedToken(TokenType::stringLiteral, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}
			if(stringLiteralEqualsStr(currentTokenString(), name)) {
				error_message_string_type errorMessage = "expected name ";
				errorMessage += name;
				errorMessage += " but actual name was ";
				formatStringLiteral<error_message_string_type>(currentTokenString(), errorMessage);
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongName));
			}

			if(!nextToken()) {
				return unexpectedEndOfSource();
			}
			natl::Option<DeserilizeErrorHandler> typeError = TestType<SerializeType>::test(self(), natl::DeserilizeErrorLocation::readNamed);
			if(typeError.hasValue()) {
				return natl::unexpected(typeError.value());
			}

			if (!nextToken()) { 
				return unexpectedEndOfSource();
			}
			if(!currentTokenIs(TokenType::colon)) {
				return unexpectedToken(TokenType::colon, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}

			return info;
		}

	private:
		template<typename IntegerType, LiteralPostExtIntType PostExtIntType>
		constexpr natl::Expect<IntegerType, DeserilizeErrorHandler> readInteger(const natl::DeserilizeErrorLocation& errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isTokenInteger(getCurrentToken().tokenType)) {
				error_message_string_type errorMessage = "expected integer but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView intString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().tokenType)) {
				const TokenWithPostExtIntType intToken = natl::bitCast<TokenWithPostExtIntType, Token>(getCurrentToken());
				const LiteralPostExtIntType intType = natl::fromUnderlying<LiteralPostExtIntType>(intToken.postExtIntType);

				if (intType != PostExtIntType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += literalPostExtIntTypeToString(PostExtIntType);	
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType));
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
				value = natl::stringDecimalToIntExpect(intString);
				break;
			case TokenType::hexadecimalIntegerWithType:
			case TokenType::hexadecimalInteger:
				value = natl::stringHexadecimalToIntExpect(intString);
				break;
			case TokenType::binaryInteger:
			case TokenType::binaryIntegerWithType:
				value = natl::stringBinaryToIntExpect(intString);
				break;
			default:
				natl::unreachable();
			}


			if (value.error()) {
				error_message_string_type errorMessage = "failed to parse numeric of type ";
				errorMessage += tokenTypeToDebugString(getCurrentToken().tokenType);
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

		template<typename FloatType, LiteralPostExtFloatType PostExtFloatType>
		constexpr natl::Expect<FloatType, DeserilizeErrorHandler> readFloat(const natl::DeserilizeErrorLocation& errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (isTokenFloat(getCurrentToken().tokenType)) {
				error_message_string_type errorMessage = "expected float but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView floatString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().tokenType)) {
				const TokenWithPostExtFloatType floatToken = natl::bitCast<TokenWithPostExtFloatType, Token>(getCurrentToken());
				const LiteralPostExtFloatType floatType = natl::fromUnderlying<LiteralPostExtFloatType>(floatToken.postExtFloatType);

				if (floatType != PostExtFloatType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += literalPostExtFloatTypeToString(PostExtFloatType);
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType));
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
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

	public:
		constexpr natl::Expect<natl::i8, DeserilizeErrorHandler> readI8(DeserializeInfo<natl::SerializeI8>& info) noexcept {
			return readInteger<natl::i8, LiteralPostExtIntType::i8>(info, natl::DeserilizeErrorLocation::readI8);
		}
		constexpr natl::Expect<natl::i16, DeserilizeErrorHandler> readI16(DeserializeInfo<natl::SerializeI16>& info) noexcept {
			return readInteger<natl::i16, LiteralPostExtIntType::i16>(info, natl::DeserilizeErrorLocation::readI16);
		}
		constexpr natl::Expect<natl::i32, DeserilizeErrorHandler> readI32(DeserializeInfo<natl::SerializeI32>& info) noexcept {
			return readInteger<natl::i32, LiteralPostExtIntType::i32>(info, natl::DeserilizeErrorLocation::readI32);
		}
		constexpr natl::Expect<natl::i64, DeserilizeErrorHandler> readI64(DeserializeInfo<natl::SerializeI64>& info) noexcept {
			return readInteger<natl::i64, LiteralPostExtIntType::i64>(info, natl::DeserilizeErrorLocation::readI64);
		}

		constexpr natl::Expect<natl::ui8, DeserilizeErrorHandler> readUI8(DeserializeInfo<natl::SerializeUI8>& info) noexcept {
			return readInteger<natl::ui8, LiteralPostExtIntType::ui8>(info, natl::DeserilizeErrorLocation::readUI8);
		}
		constexpr natl::Expect<natl::ui16, DeserilizeErrorHandler> readUI16(DeserializeInfo<natl::SerializeUI16>& info) noexcept {
			return readInteger<natl::ui16, LiteralPostExtIntType::ui16>(info, natl::DeserilizeErrorLocation::readUI16);
		}
		constexpr natl::Expect<natl::ui32, DeserilizeErrorHandler> readUI32(DeserializeInfo<natl::SerializeUI32>& info) noexcept {
			return readInteger<natl::ui32, LiteralPostExtIntType::ui32>(info, natl::DeserilizeErrorLocation::readUI32);
		}
		constexpr natl::Expect<natl::ui64, DeserilizeErrorHandler> readUI64(DeserializeInfo<natl::SerializeUI64>& info) noexcept {
			return readInteger<natl::ui64, LiteralPostExtIntType::ui64>(info, natl::DeserilizeErrorLocation::readUI64);
		}

		constexpr natl::Expect<natl::f32, DeserilizeErrorHandler> readF32(DeserializeInfo<natl::SerializeF32>& info) noexcept {
			return readFloat<natl::f32, LiteralPostExtFloatType::f32>(info, natl::DeserilizeErrorLocation::readF32);
		}
		constexpr natl::Expect<natl::f64, DeserilizeErrorHandler> readF64(DeserializeInfo<natl::SerializeF64>& info) noexcept {
			return readFloat<natl::f64, LiteralPostExtFloatType::f64>(info, natl::DeserilizeErrorLocation::readF64);
		}

		constexpr natl::Expect<natl::Bool, DeserilizeErrorHandler> readBool(DeserializeInfo<natl::SerializeBool>& info) noexcept {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::readBool;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}

			if (currentTokenIs(TokenType::keywordTrue)) {
				return true;
			} else if(currentTokenIs(TokenType::keywordFalse)) {
				return false;
			} else {
				error_message_string_type errorMessage = "expected true or false literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType));
			}
		}

		constexpr natl::Expect<natl::Ascii, DeserilizeErrorHandler> readChar(DeserializeInfo<natl::SerializeChar>& info) noexcept {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::readChar;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::charLiteral)) {
				return unexpectedToken(TokenType::charLiteral, errorLocation, natl::DeserilizeErrorFlag::wrongType);
			}

			const natl::ConstAsciiStringView charLiteral = currentTokenString();
			if(charLiteral.size() == 1) {
				return charLiteral[0];
			} else {
				return charLiteral[1];
			}
		}

		template<typename StringDstType = error_message_string_type>
		constexpr natl::Expect<StringDstType, DeserilizeErrorHandler> readStr(DeserializeInfo<natl::SerializeStr>& info) noexcept {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::readStr;
			if (!nextToken(errorLocation)) {
				return unexpectedEndOfSource();
			}
			if (!currentTokenIs(TokenType::stringLiteral)) {
				return unexpectedToken(TokenType::stringLiteral, errorLocation, natl::DeserilizeErrorFlag::wrongType);
			}

			const natl::ConstAsciiStringView stringLiteral = currentTokenString();
			return formattedStringLiteralToString<StringDstType>(stringLiteral);
		}

		template<typename BaseSerilizeType, typename Functor>
			requires(natl::IsEnumBaseSerializeTypeC<BaseSerilizeType> && 
				natl::IsStringToSerializeFlagConvertFunctor<natl::BasicSerializeTypeToType<Functor>>)
		constexpr natl::Expect<natl::BasicSerializeTypeToType<BaseSerilizeType>, DeserilizeErrorHandler> 
			readEnum(DeserializeInfo<natl::SerializeEnum<BaseSerilizeType>>& info, Functor&& stringToEnum) noexcept {
			using enum_integer_type = natl::BasicSerializeTypeToType<BaseSerilizeType>;
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::readEnum;

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (currentTokenIs(TokenType::stringLiteral)) {
				const natl::ConstAsciiStringView enumValueStringLiteral = currentTokenString();
				const error_message_string_type enumValueString = formattedStringLiteralToString<error_message_string_type>(enumValueStringLiteral);
				natl::Option<enum_integer_type> value = stringToEnum(enumValueString.toStringView());
				
				if (value.hasValue()) {
					return value.value();
				} else {
					error_message_string_type errorMessage = "unexpected enum value of the string literal ";
					errorMessage += enumValueStringLiteral;
					return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::valueParsing));
				}
			} else if(isTokenInteger(getCurrentToken().tokenType)) {
				goBackAToken();
				natl::Expect<enum_integer_type, DeserilizeErrorHandler> value;
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
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongType));
			}
		}
	private:
		constexpr natl::Expect<natl::Size, DeserilizeErrorHandler> beginReadContainer(const natl::DeserilizeErrorLocation errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}


			natl::Size index = tokenIndex + 1;
			if (!(index < lexicalInfo.tokens.size())) {
				error_message_string_type errorMessage = "expected more tokens after array being";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting));
			}

			if (lexicalInfo.tokens[index].tokenType == TokenType::rightCurly) {
				nextToken();
				return 0;
			}

			natl::Size infoCount = 0;
			while (lexicalInfo.tokens[index].tokenType != TokenType::rightCurly) {
				const Token& token = lexicalInfo.tokens[index];
				switch (token.tokenType) {
				break; case TokenType::comma:
					infoCount++;
					break;
				case TokenType::leftCurly:
				case TokenType::leftSquare:
					if (token.size == 0) {
						error_message_string_type errorMessage = "scope error with token ";
						errorMessage += tokenTypeToDebugString(getCurrentToken());
						return natl::unexpected(DeserilizeErrorHandler(errorMessage, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting));
					}
					index += token.size;
				break; default:
					break;
				}
			}
		}
		constexpr natl::Option<DeserilizeErrorHandler> endReadContainer(const natl::DeserilizeErrorLocation errorLocation) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}

			return {};
		}

		constexpr natl::Option<DeserilizeErrorHandler> endReadContainerElement(const natl::DeserilizeErrorLocation errorLocation) {
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::comma)) {
				return unexpectedToken(TokenType::comma, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}

			return {};
		};
	public:

		//array
		template<typename SerializeElementType>
		constexpr natl::Expect<natl::Size, DeserilizeErrorHandler> beginReadArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return beginReadContainer(natl::DeserilizeErrorLocation::beginReadArray);
		}

		template<typename SerializeElementType>
		constexpr natl::Option<DeserilizeErrorHandler> endReadArray(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return endReadContainer(natl::DeserilizeErrorLocation::endReadArray);
		}

		template<typename SerializeElementType>
		constexpr natl::Expect<DeserializeInfo<SerializeElementType>, DeserilizeErrorHandler> beginReadArrayElement(
			DeserializeInfo<natl::SerializeArray<SerializeElementType>>& info) noexcept {
			return DeserializeInfo<SerializeElementType>{};
		}
		template<typename SerializeElementType>
		constexpr natl::Option<DeserilizeErrorHandler> endReadArrayElement(DeserializeInfo<SerializeElementType> info) {
			return endReadContainerElement(natl::DeserilizeErrorLocation::endReadArrayElement);
		};

		//dic
		template<typename SerializeKeyType, typename SerializeValueType>
		constexpr natl::Expect<natl::Size, DeserilizeErrorHandler> beginReadDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>> info) noexcept {
			return beginReadContainer(natl::DeserilizeErrorLocation::beginReadDic);
		}
		template<typename SerializeKeyType, typename SerializeValueType>
		constexpr natl::Option<DeserilizeErrorHandler> endReadDic(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>> info) {
			return endReadContainer(natl::DeserilizeErrorLocation::endReadDic);
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		constexpr natl::Expect<DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>, DeserilizeErrorHandler> beginReadDicElement(
			DeserializeInfo<natl::SerializeDic<SerializeKeyType, SerializeValueType>> info) noexcept {
			return DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>>{};
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		constexpr natl::Option<DeserilizeErrorHandler> endReadDicElement(
			DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>> info) noexcept {
			return endReadContainerElement(natl::DeserilizeErrorLocation::endReadDicElement);
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		constexpr natl::Option<DeserilizeErrorHandler> readDicKey(
			DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>> info) noexcept {
			return{};
		}

		template<typename SerializeKeyType, typename SerializeValueType>
		constexpr natl::Option<DeserilizeErrorHandler> readDicValue(
			DeserializeInfo<natl::SerializeDicElement<SerializeKeyType, SerializeValueType>> info) noexcept {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::readDicValue;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::colon)) {
				return unexpectedToken(TokenType::colon, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}
			return {};
		}

		//struct
		template<typename... SerializeMemberTypes>
		constexpr natl::Expect<DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>, DeserilizeErrorHandler> beginReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>> info) noexcept {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::beginReadStruct;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}
			return {};
		}
		template<typename... SerializeMemberTypes>
		constexpr natl::Option<DeserilizeErrorHandler> endReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>> info) {
			return endReadContainer(natl::DeserilizeErrorLocation::endReadStruct);
		}

		//varaint
		template<typename SerializeType, typename... VariantSerializeTypes>
			requires(natl::TemplatePackHasElementC<natl::IsSameV, SerializeType, VariantSerializeTypes...>)
		constexpr natl::Expect<DeserializeInfo<SerializeType>, DeserilizeErrorHandler> 
			beginReadVaraint(DeserializeInfo<natl::SerializeVariant<VariantSerializeTypes...>> info) {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::beginReadVariant;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::keywordVariant)) {
				return unexpectedToken(TokenType::keywordVariant, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}

			natl::Option<DeserilizeErrorHandler> typeTest = TestType<SerializeType>::test(self(), errorLocation);
			if (typeTest.hasValue()) {
				return natl::unexpected(typeTest.value());
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::rightCurly)) {
				return unexpectedToken(TokenType::rightCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}

			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::leftCurly)) {
				return unexpectedToken(TokenType::leftCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}
		}

		template<typename SerializeType, typename... VariantSerializeTypes>
		constexpr natl::Option<DeserilizeErrorHandler>
			endReadVaraint(DeserializeInfo<natl::SerializeVariant<VariantSerializeTypes...>> info) {
			constexpr natl::DeserilizeErrorLocation errorLocation = natl::DeserilizeErrorLocation::endReadVariant;
			if (!nextToken()) {
				return unexpectedEndOfSource(errorLocation);
			}
			if (!currentTokenIs(TokenType::rightCurly)) {
				return unexpectedToken(TokenType::rightCurly, errorLocation, natl::DeserilizeErrorFlag::wrongFormatting);
			}
			return {};
		}
	};
}