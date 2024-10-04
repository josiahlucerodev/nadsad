#pragma once 

//natl
#include <natl/container/arrayView.h>

//own
#include "lexer.h"

//interface 
namespace nadsad::ascii {
	template<template<typename> typename DeserializeElementInfo, typename DeserilizeErrorHandler>
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
		}

		
		constexpr natl::Bool processSource() noexcept {
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

		constexpr auto unexpectedEndOfSource() noexcept {
			return natl::unexpected(DeserilizeErrorHandler(endOfSourceErrorMessage, natl::DeserilizeErrorFlag::endOfSource));
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

		constexpr void reportEndOfSource(DeserilizeErrorHandler& errorHandler) noexcept {
			errorHandler.reportError(endOfSourceErrorMessage, natl::DeserilizeErrorFlag::endOfSource);
		}

		//test
		template<typename SerializeType>
		struct TestType;

		template<> struct TestType<natl::SerializeI8> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordI8)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of i8 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeI16> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordI16)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of i16 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeI32> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordI32)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of i32 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeI64> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordI64)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of i64 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeUI8> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordUI8)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of ui8 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeUI16> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordUI16)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of ui16 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeUI32> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordUI32)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of ui32 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeUI64> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordUI64)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of ui64 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};

		template<> struct TestType<natl::SerializeF32> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordF32)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of f32 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeF64> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordF64)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of f64 but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeBool> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordBool)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of bool but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};

		template<> struct TestType<natl::SerializeChar> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordChar)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of char but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeStr> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordStr)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of str but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<> struct TestType<natl::SerializeBlob> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordBlob)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of blob but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<typename SerializeType> struct TestType<natl::SerializeOptional<SerializeType>> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordOp)) {
					error_message_string_type errorMessage = "expected type of op but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<SerializeType>::test(deserializer, errorHandler)) {
					return false;
				}

				return true;
			}
		};
		template<typename BaseSerializeType> struct TestType<natl::SerializeEnum<BaseSerializeType>> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordFlag)) {
					error_message_string_type errorMessage = "expected type of str but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<BaseSerializeType>::test(deserializer, errorHandler)) {
					return false;
				}

				return true;
			}
		};
		template<> struct TestType<natl::SerializeFile> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (deserializer.currentTokenIs(TokenType::keywordFile)) {
					return true;
				}

				error_message_string_type errorMessage = "expected type of file but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
				return false;
			}
		};
		template<typename ElementType, natl::Size Number> struct TestType<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordFarray)) {
					error_message_string_type errorMessage = "expected type of farray but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::leftSquare)) {
					error_message_string_type errorMessage = "expected left square [ but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<ElementType>::test(deserializer, errorHandler)) {
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::comma)) {
					error_message_string_type errorMessage = "expected left comma , but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.isTokenInteger(getCurrentToken().tokenType)) {
					error_message_string_type errorMessage = "expected integer but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::rightSquare)) {
					error_message_string_type errorMessage = "expected right square ] but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				return true;
			}
		};

		template<typename ElementType> struct TestType<natl::SerializeArray<ElementType>> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordArray)) {
					error_message_string_type errorMessage = "expected type of array but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::leftSquare)) {
					error_message_string_type errorMessage = "expected left square [ but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<ElementType>::test(deserializer, errorHandler)) {
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::rightSquare)) {
					error_message_string_type errorMessage = "expected right square ] but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				return true;
			}
		};

		template<typename KeyType, typename ValueType> struct TestType<natl::SerializeDic<KeyType, ValueType>> {
			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordDic)) {
					error_message_string_type errorMessage = "expected type of array but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					error_message_string_type errorMessage = "expected left curly { but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<KeyType>::test(deserializer, errorHandler)) {
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::comma)) {
					error_message_string_type errorMessage = "expected comma , but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<ValueType>::test(deserializer, errorHandler)) {
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					error_message_string_type errorMessage = "expected rigth curly } but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				return true;
			}
		};
		template<typename... MemberTypes> struct TestType<natl::SerializeStruct<MemberTypes...>> {
			template<typename MemberType, natl::Size Index>
			constexpr static natl::Bool testMember(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						reportEndOfSource(errorHandler);
						return false;
					}
					if (!deserializer.currentTokenIs(TokenType::comma)) {
						error_message_string_type errorMessage = "expected comma , but got ";
						errorMessage += tokenTypeToDebugString(getCurrentToken());
						errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
						return false;
					}
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<MemberType>::test(deserializer, errorHandler)) {
					return false;
				}
			}


			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordStruct)) {
					error_message_string_type errorMessage = "expected type of struct but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					error_message_string_type errorMessage = "expected left curly { but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				[] <natl::Size... Indices>(natl::IndexSequence<Indices...>) noexcept {
					(testMember<MemberTypes, Indices>(deserializer, errorHandler), ...);
				}(natl::MakeIndexSequence<sizeof...(MemberTypes)>{});


				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					error_message_string_type errorMessage = "expected rigth curly } but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				return true;
			}
		};

		template<typename... Types> struct TestType<natl::SerializeVariant<Types...>> {
			template<typename TypeMember, natl::Size Index>
			constexpr static natl::Bool testTypeMember(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if constexpr (Index != 0) {
					if (!deserializer.nextToken()) {
						reportEndOfSource(errorHandler);
						return false;
					}
					if (!deserializer.currentTokenIs(TokenType::comma)) {
						error_message_string_type errorMessage = "expected comma , but got ";
						errorMessage += tokenTypeToDebugString(getCurrentToken());
						errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
						return false;
					}
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!TestType<TypeMember>::test(deserializer, errorHandler)) {
					return false;
				}
			}

			constexpr static natl::Bool test(Deserializer& deserializer, DeserilizeErrorHandler& errorHandler) noexcept {
				if (!deserializer.currentTokenIs(TokenType::keywordVariant)) {
					error_message_string_type errorMessage = "expected type of variant but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongType);
					return false;
				}

				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::leftCurly)) {
					error_message_string_type errorMessage = "expected left curly { but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				[] <natl::Size... Indices>(natl::IndexSequence<Indices...>) noexcept {
					(testTypeMember<Types, Indices>(deserializer, errorHandler), ...);
				}(natl::MakeIndexSequence<sizeof...(Types)>{});


				if (!deserializer.nextToken()) {
					reportEndOfSource(errorHandler);
					return false;
				}
				if (!deserializer.currentTokenIs(TokenType::rightCurly)) {
					error_message_string_type errorMessage = "expected rigth curly } but got ";
					errorMessage += tokenTypeToDebugString(getCurrentToken());
					errorHandler.reportError(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting);
					return false;
				}

				return true;
			}
		};



		template<typename SerializeType>
		constexpr natl::Bool isType() noexcept {
			TestType<SerializeType>::test(self());
		}
	public:


		constexpr natl::Bool beginDeserialization() noexcept {

		}
		constexpr natl::Bool endDeserialization() noexcept {
			
		}


		

		template<typename SerializeType, typename ParentType>
		constexpr natl::Expect<DeserializeElementInfo<SerializeType>, DeserilizeErrorHandler> readNamed(
			DeserializeElementInfo<ParentType>& parrent, const natl::ConstAsciiStringView name) noexcept {
			DeserializeElementInfo<SerializeType> elementInfo;
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}

			if(!currentTokenIs(TokenType::stringLiteral)) {
				error_message_string_type errorMessage = "expected string literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting));
			}
			if(stringLiteralEqualsStr(currentTokenString(), name)) {
				error_message_string_type errorMessage = "expected name ";
				errorMessage += name;
				errorMessage += " but actual element name was ";
				formatStringLiteral<error_message_string_type>(currentTokenString(), errorMessage);
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongName));
			}

			if(!nextToken()) {
				return unexpectedEndOfSource();
			}
			natl::Option<DeserilizeErrorHandler> typeError = isType<SerializeType>(elementInfo);
			if(typeError.hasValue()) {
				return natl::unexpected(typeError);
			}

			if (!nextToken()) { 
				return unexpectedEndOfSource();
			}
			if(!currentTokenIs(TokenType::colon)) {
				error_message_string_type errorMessage = "expected colon : but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongFormatting));
			}

			return elementInfo;
		}

	private:
		template<typename IntegerType, LiteralPostExtIntType PostExtIntType>
		constexpr natl::Expect<IntegerType, DeserilizeErrorHandler> readInteger() noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}
			if (isTokenInteger(getCurrentToken().tokenType)) {
				error_message_string_type errorMessage = "expected integer but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView intString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().tokenType)) {
				const TokenWithPostExtIntType intToken = natl::bitCast<TokenWithPostExtIntType, Token>(getCurrentToken());
				const LiteralPostExtIntType intType = natl::fromUnderlying<LiteralPostExtIntType>(intToken.postExtIntType);

				if (intType != PostExtIntType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += literalPostExtIntTypeToString(PostExtIntType);	
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
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
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

		template<typename FloatType, LiteralPostExtFloatType PostExtFloatType>
		constexpr natl::Expect<FloatType, DeserilizeErrorHandler> readFloat() noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}
			if (isTokenFloat(getCurrentToken().tokenType)) {
				error_message_string_type errorMessage = "expected float but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
			}

			natl::ConstAsciiStringView floatString = currentTokenString();
			if (isTokenNumericWithType(getCurrentToken().tokenType)) {
				const TokenWithPostExtFloatType floatToken = natl::bitCast<TokenWithPostExtFloatType, Token>(getCurrentToken());
				const LiteralPostExtFloatType floatType = natl::fromUnderlying<LiteralPostExtFloatType>(floatToken.postExtFloatType);

				if (floatType != PostExtFloatType) {
					error_message_string_type errorMessage = "expected ";
					errorMessage += literalPostExtFloatTypeToString(PostExtFloatType);
					errorMessage += "type on numeric but got ";
					return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
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
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::valueParsing));
			} else {
				return value.value();
			}
		}

	public:
		constexpr natl::Expect<natl::i8, DeserilizeErrorHandler> readI8(DeserializeElementInfo<natl::SerializeI8>& element) noexcept {
			return readInteger<natl::i8, LiteralPostExtIntType::i8>(element);
		}
		constexpr natl::Expect<natl::i16, DeserilizeErrorHandler> readI16(DeserializeElementInfo<natl::SerializeI16>& element) noexcept {
			return readInteger<natl::i16, LiteralPostExtIntType::i16>(element);
		}
		constexpr natl::Expect<natl::i32, DeserilizeErrorHandler> readI32(DeserializeElementInfo<natl::SerializeI32>& element) noexcept {
			return readInteger<natl::i32, LiteralPostExtIntType::i32>(element);
		}
		constexpr natl::Expect<natl::i64, DeserilizeErrorHandler> readI64(DeserializeElementInfo<natl::SerializeI64>& element) noexcept {
			return readInteger<natl::i64, LiteralPostExtIntType::i64>(element);
		}

		constexpr natl::Expect<natl::ui8, DeserilizeErrorHandler> readUI8(DeserializeElementInfo<natl::SerializeUI8>& element) noexcept {
			return readInteger<natl::ui8, LiteralPostExtIntType::ui8>(element);
		}
		constexpr natl::Expect<natl::ui16, DeserilizeErrorHandler> readUI16(DeserializeElementInfo<natl::SerializeUI16>& element) noexcept {
			return readInteger<natl::ui16, LiteralPostExtIntType::ui16>(element);
		}
		constexpr natl::Expect<natl::ui32, DeserilizeErrorHandler> readUI32(DeserializeElementInfo<natl::SerializeUI32>& element) noexcept {
			return readInteger<natl::ui32, LiteralPostExtIntType::ui32>(element);
		}
		constexpr natl::Expect<natl::ui64, DeserilizeErrorHandler> readUI64(DeserializeElementInfo<natl::SerializeUI64>& element) noexcept {
			return readInteger<natl::ui64, LiteralPostExtIntType::ui64>(element);
		}

		constexpr natl::Expect<natl::f32, DeserilizeErrorHandler> readF32(DeserializeElementInfo<natl::SerializeF32>& element) noexcept {
			return readFloat<natl::f32, LiteralPostExtFloatType::f32>(element);
		}
		constexpr natl::Expect<natl::f64, DeserilizeErrorHandler> readF64(DeserializeElementInfo<natl::SerializeF64>& element) noexcept {
			return readFloat<natl::f64, LiteralPostExtFloatType::f64>(element);
		}

		constexpr natl::Expect<natl::Bool, DeserilizeErrorHandler> readBool(DeserializeElementInfo<natl::SerializeBool>& element) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}

			if (currentTokenIs(TokenType::keywordTrue)) {
				return true;
			} else if(currentTokenIs(TokenType::keywordFalse)) {
				return false;
			} else {
				error_message_string_type errorMessage = "expected true or false literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
			}
		}

		constexpr natl::Expect<natl::Ascii, DeserilizeErrorHandler> readChar(DeserializeElementInfo<natl::SerializeChar>& element) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}
			if (!currentTokenIs(TokenType::charLiteral)) {
				error_message_string_type errorMessage = "expected char literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
			}

			const natl::ConstAsciiStringView charLiteral = currentTokenString();
			if(charLiteral.size() == 1) {
				return charLiteral[0];
			} else {
				return charLiteral[1];
			}
		}

		template<typename StringDstType = error_message_string_type>
		constexpr natl::Expect<StringDstType, DeserilizeErrorHandler> readStr(DeserializeElementInfo<natl::SerializeStr>& element) noexcept {
			if (!nextToken()) {
				return unexpectedEndOfSource();
			}
			if (!currentTokenIs(TokenType::stringLiteral)) {
				error_message_string_type errorMessage = "expected string literal but got ";
				errorMessage += tokenTypeToDebugString(getCurrentToken());
				return natl::unexpected(DeserilizeErrorHandler(errorMessage, natl::DeserilizeErrorFlag::wrongType));
			}

			const natl::ConstAsciiStringView stringLiteral = currentTokenString();
			StringDstType output{};
			output.reserve(stringLiteral.size());

			for(natl::Size i = 0; i < stringLiteral.size(); i++) {
				if(stringLiteral[i] == '\\') {
					if(!(i < stringLiteral.size())) { break; }
					i++;
				}
				output.pushBack(stringLiteral[i]);
			}

			return output;
		}

		template<typename BaseSerilizeType>
		constexpr natl::Expect<natl::BasicSerializeTypeToType<BaseSerilizeType>, DeserilizeErrorHandler> 
			readEnum(DeserializeElementInfo<natl::SerializeEnum<BaseSerilizeType>>& element) noexcept {



		}
	};
}