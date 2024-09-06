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
		simicolon,
		comma,
		leftCurly,
		rightCurly,
		leftSquare,
		rightSquare,

		charLiteral,
		stringLiteral,
		dataStorage,

		decimalInteger,
		hexadecimalInteger,
		binaryInteger,
		decimalFloat,
		hexadecimalFloat,
		binaryFloat,

		decimalIntegerWithType,
		hexadecimalIntegerWithType,
		binaryIntegerWithType,
		decimalFloatWithType,
		hexadecimalFloatWithType,
		binaryFloatWithType,

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
		keywordTag,
		keywordNameTag,
		keywordTypeTag,
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
		case TokenType::simicolon:
			return "simicolon";
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
		case TokenType::decimalInteger:
			return "decimalInteger";
		case TokenType::hexadecimalInteger:
			return "hexadecimalInteger";
		case TokenType::binaryInteger:
			return "binaryInteger";
		case TokenType::decimalFloat:
			return "decimalFloat";
		case TokenType::hexadecimalFloat:
			return "hexadecimalFloat";
		case TokenType::binaryFloat:
			return "binaryFloat";
		case TokenType::decimalIntegerWithType:
			return "decimalIntegerWithType";
		case TokenType::hexadecimalIntegerWithType:
			return "hexadecimalIntegerWithType";
		case TokenType::binaryIntegerWithType:
			return "binaryIntegerWithType";
		case TokenType::decimalFloatWithType:
			return "decimalFloatWithType";
		case TokenType::hexadecimalFloatWithType:
			return "hexadecimalFloatWithType";
		case TokenType::binaryFloatWithType:
			return "binaryFloatWithType";
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
		case TokenType::keywordTag:
			return "keywordTag";
		case TokenType::keywordNameTag:
			return "keywordNameTag";
		case TokenType::keywordTypeTag:
			return "keywordTypeTag";
		default:
			natl::unreachable();
		}
	}

	enum class LiteralPostExtIntType : natl::ui8 {
		i8,
		i16,
		i32,
		i64,
		ui8,
		ui16,
		ui32,
		ui64,
	};
	enum class LiteralPostExtFloatType : natl::ui8 {
		f32,
		f64,
	};

	struct TokenWithPostExtIntType {
		natl::ui64 tokenType : 8;
		natl::ui64 postExtIntType : 8;
		natl::ui64 size : 64 - 8 - 8;
		natl::ui64 offset;
	};

	struct TokenWithPostExtFloatType {
		natl::ui64 tokenType : 8;
		natl::ui64 postExtFloatType : 8;
		natl::ui64 size : 64 - 8 - 8;
		natl::ui64 offset;
	};

	struct Token {
		TokenType tokenType : 8;
		natl::ui64 size : 64 - 8;
		natl::ui64 offset : 64;
	};

	enum class LexicalErrorType {
		none,
		unknownToken, 
		inputToBig,
		unknownIdentifer,
		unknownLiteralExtention,
		unknownLiteralPreExt,
		expectedHashAtLiteralPreExtEnd,
		expectedDigitsAfterLiteralExt,
		expectedDigitsAfterFloatDot,
		expectedDigitsAfterFloatExp,
		expectedIdentiferAfterLiteralPostExtStart,
		unknownLiteralPostExt,
	};

	enum class LiteralPreExtType {
		decimalInteger,
		hexadecimalInteger,
		binaryInteger,
		decimalFloat,
		hexadecimalFloat,
		binaryFloat,
	};

	constexpr LiteralPreExtType tokenTypeToLiteralPreExtType(const TokenType tokenType) noexcept {
		switch (tokenType) {
		case TokenType::decimalInteger: return LiteralPreExtType::decimalInteger;
		case TokenType::binaryInteger: return LiteralPreExtType::binaryInteger;
		case TokenType::hexadecimalInteger: return LiteralPreExtType::hexadecimalInteger;
		case TokenType::decimalFloat: return LiteralPreExtType::decimalFloat;
		case TokenType::hexadecimalFloat: return LiteralPreExtType::hexadecimalFloat;
		case TokenType::binaryFloat: return LiteralPreExtType::binaryFloat;
		default:
			natl::unreachable();
		}
	}


	struct LexicalError {
		LexicalErrorType type;
		union {
			struct UnknownIdentifer {
				natl::ui64 tokenIndex;
			} unknownIdentifer;

			struct ExpectedDigitsAfterLiteralExt {
				LiteralPreExtType literalType;
				natl::ui64 tokenIndex;
			} expectedDigitsAfterLiteralExt;

			struct ExpectedHashAtLiteralPreExtEnd {
				natl::ui64 literalPreExtEndIndex;
				natl::ui64 tokenIndex;
			} expectedHashAtLiteralPreExtEnd;

			struct UnknownLiteralPreExt {
				natl::ui64 literalPreExtEndIndex;
				natl::ui64 tokenIndex;
			} unknownLiteralPreExt;

			struct UnknownLiteralExtention {
				LiteralPreExtType literalType;
				natl::ui32 extentionOffset;
				natl::ui64 tokenIndex;
			} unknownLiteralExtention;

			struct ExpectedDigitsAfterFloatDot {
				LiteralPreExtType literalType;
				natl::ui64 tokenIndex;
			} expectedDigitsAfterFloatDot;
			
			struct ExpectedDigitsAfterFloatExp {
				LiteralPreExtType literalType;
				natl::ui64 tokenIndex;
			} expectedDigitsAfterFloatExp;

			struct ExpectedIdentiferAfterLiteralPostExtStart {
				LiteralPreExtType literalType;
				natl::ui64 tokenIndex;
			} expectedIdentiferAfterLiteralPostExtStart;
			
			struct UnknownLiteralPostExt {
				LiteralPreExtType literalType : 8;
				natl::ui64 literalPostExtStartOffset;
				natl::ui64 tokenIndex;
			} unknownLiteralPostExt;

			struct UnknownToken {
				natl::ui64 tokenIndex;
			} unknownToken;
		};
	};

	struct LexicalInfo {
		natl::ArrayView<const natl::Ascii> source;
		natl::DynArray<Token> tokens;
		natl::DynArray<natl::ui64> newLineOffsets;
		natl::DynArray<LexicalError> errors;
	};

	
	struct LexicalAnalysisRunner  {
		LexicalInfo lexicalInfo;
		natl::ConstAsciiStringView source;
		natl::Bool enableFastIndexing;
		natl::ui64 index;
		natl::SmallDynArray<Token*, 10> curlyScopeStack;

		constexpr natl::Bool startup() noexcept {
			if (source.size() > static_cast<natl::Size>(natl::Limits<natl::ui64>::max())) {
				LexicalError error;
				error.type = LexicalErrorType::inputToBig;
				lexicalInfo.errors.push_back(error);
				return false;
			}

			lexicalInfo.source = source;
			lexicalInfo.tokens.reserve((source.size() >> 5) + 100); //expect a token every 8 characters
			lexicalInfo.newLineOffsets.reserve((source.size() >> 3) + 100); //expect a newline every 32 characters
			lexicalInfo.tokens.push_back(Token{ TokenType::start, 0 });

			return true;
		}
		
		constexpr natl::Bool isAtEndOfSource() noexcept {
			return index == source.size();
		}

		constexpr natl::Ascii getCurrentCharacter() noexcept {
			return source[index];
		}
		constexpr void nextCharacter() noexcept {
			index++;
		}

		constexpr natl::ui64 currentTokenIndex() noexcept {
			return lexicalInfo.tokens.size() - 1;
		}

		constexpr LexicalError& newError(const LexicalErrorType errorType) noexcept {
			LexicalError error;
			error.type = LexicalErrorType::unknownToken;
			lexicalInfo.errors.push_back(error);
			return lexicalInfo.errors.back();
		}
		constexpr void addUnknownToken(const natl::ui64 size, const natl::ui64 offset) noexcept {
			if (lexicalInfo.tokens.isNotEmpty()) {
				Token& previousToken = lexicalInfo.tokens.back();
				if (previousToken.tokenType == TokenType::unknown &&
					previousToken.offset + previousToken.size == offset) {
					previousToken.size += size;
					return;
				}
			}
			lexicalInfo.tokens.push_back(Token{ TokenType::unknown, size, offset });

			LexicalError& error = newError(LexicalErrorType::unknownToken);
			error.unknownToken.tokenIndex = lexicalInfo.tokens.size() - 1;
		}

		constexpr void addUnknownTokenFromToCurrent(const natl::Size startPos) noexcept {
			addUnknownToken(index - startPos, startPos);
		}
		constexpr void addUnknownTokenFromToCurrentMinusOne(const natl::Size startPos) noexcept {
			addUnknownToken(index - startPos - 1, startPos);
		}

		constexpr void addToken(const Token token) noexcept {
			lexicalInfo.tokens.push_back(token);
		}
		constexpr void addToken(const TokenType tokenType, const natl::ui64 size, const natl::ui64 offset) noexcept {
			lexicalInfo.tokens.push_back(Token{ tokenType, size, offset });
		}

		constexpr void addTokenFromToCurrent(const TokenType tokenType, const natl::Size startPos) noexcept {
			lexicalInfo.tokens.push_back(Token{ tokenType, index - startPos, startPos });
		}
		constexpr void addTokenFromToCurrentMinusOne(const TokenType tokenType, const natl::Size startPos) noexcept {
			lexicalInfo.tokens.push_back(Token{ tokenType, index - startPos - 1, startPos });
		}

		constexpr void handleCharacter() noexcept {
			const natl::ui64 startIndex = index;
			natl::ui64 charLiteralSize = 2;

			if (startIndex + 2 >= source.size()) {
				addUnknownToken(charLiteralSize, startIndex);
				index = index + 2;
				return;
			}

			index += 1;
			charLiteralSize += 1;
			natl::Ascii scopedCharacter = source[index];
			if (scopedCharacter == '\\') {
				if (index + 2 >= source.size()) {
					addUnknownToken(charLiteralSize, startIndex);
					index = index + 2;
					const natl::ui64 startIndex = index;
					natl::ui64 charLiteralSize = 2;

				if (startIndex + 2 >= source.size()) {
					addUnknownToken(charLiteralSize, startIndex);
					index = index + 2;
					return;
				}

				index += 1;
				charLiteralSize += 1;
				natl::Ascii scopedCharacter = source[index];
				if (scopedCharacter == '\\') {
					if (index + 2 >= source.size()) {
						addUnknownToken(charLiteralSize, startIndex);
						index = index + 2;
						return;
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
					return;
				}

				addToken(TokenType::charLiteral, charLiteralSize, startIndex);
				index++;
				return;
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
				return;
			}

			addToken(TokenType::charLiteral, charLiteralSize, startIndex);
			index++;
			return;
		}

		constexpr void handleString() noexcept {
			const natl::ui64 startIndex = index;
			index += 1;

			if (isAtEndOfSource()) {
				addUnknownToken(1, startIndex);
				return;
			}

			natl::Ascii stringLiteralCharacter = getCurrentCharacter();
			while (index != source.size() && stringLiteralCharacter != '\"') {
				stringLiteralCharacter = getCurrentCharacter();

				if (stringLiteralCharacter == '\\') {
					nextCharacter();
					if (isAtEndOfSource()) {
						addUnknownToken(index - startIndex, startIndex);
						return;
					}
				}
				nextCharacter();
			}

			addToken(TokenType::stringLiteral, index - startIndex, startIndex);
			return;
		}
		constexpr void handleDataStorage() noexcept {
			const natl::ui64 startIndex = index;
			index += 1;

			if (isAtEndOfSource()) {
				addUnknownToken(1, startIndex);
				return;
			}

			natl::Ascii blobData = source[index];
			while (index != source.size() && blobData != ')') {
				blobData = source[index];

				if (blobData == natl::Ascii(255)) {
					index += 1;
					if (isAtEndOfSource()) {
						addUnknownToken(index - startIndex, startIndex);
						return;
					}
				}

				index++;
			}

			addToken(TokenType::dataStorage, index - startIndex, startIndex);
		}

		constexpr natl::Bool isIdentifer() noexcept {
			const natl::Ascii& character = source[index];
			const natl::ui64 identifierStartIndex = index;

			if (!(natl::isAlphabetic(character) || character == '_')) {
				return false;
			}

			natl::Ascii identifierCharacter = getCurrentCharacter();
			while ((natl::isAlphanumeric(identifierCharacter) || identifierCharacter == '_') && !isAtEndOfSource()) {
				nextCharacter();
				identifierCharacter = getCurrentCharacter();
			}

			const natl::ui64 identifierSize = natl::ui64(index - identifierStartIndex);
			if (identifierSize > 8) { //too big to be a keyword 
				addUnknownToken(identifierSize, identifierStartIndex);
				LexicalError& error = newError(LexicalErrorType::unknownIdentifer);
				error.unknownIdentifer.tokenIndex = currentTokenIndex();
				return true;
			}

			natl::Size identifierHash = 0;

			const natl::Ascii* identifierPtr = &character;
			if (identifierStartIndex + 8 < source.size()) {
				identifierHash = natl::hashStringLessThan8Unbounded(identifierPtr, identifierSize);
			} else {
				identifierHash = natl::hashStringLessThan8(identifierPtr, identifierSize);
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
			case natl::hashStringLessThan8("tag"):
				keywordTokenType = TokenType::keywordTag;
				break;
			case natl::hashStringLessThan8("nametag"):
				keywordTokenType = TokenType::keywordNameTag;
				break;
			case natl::hashStringLessThan8("typetag"):
				keywordTokenType = TokenType::keywordTypeTag;
				break;
			default:
				addUnknownToken(identifierSize, identifierStartIndex);
				LexicalError& error = newError(LexicalErrorType::unknownIdentifer);
				error.unknownIdentifer.tokenIndex = currentTokenIndex();
				return true;
			}

			addToken(keywordTokenType, identifierSize, identifierStartIndex);
			return true;
		}

		constexpr TokenType numericTokenTypeToNumericWithType(const TokenType type) noexcept {
			switch (type) {
			case TokenType::decimalInteger:
				return TokenType::decimalIntegerWithType;
			case TokenType::hexadecimalInteger:
				return TokenType::hexadecimalIntegerWithType;
			case TokenType::binaryInteger:
				return TokenType::binaryIntegerWithType;
			case TokenType::decimalFloat:
				return TokenType::decimalFloatWithType;
			case TokenType::hexadecimalFloat:
				return TokenType::hexadecimalFloatWithType;
			case TokenType::binaryFloat:
				return TokenType::binaryFloatWithType;
			default:
				natl::unreachable();
			}
		}

		constexpr void handlePostExtNumericLiteral(const natl::ui64 literalStartIndex, const TokenType type) noexcept {
			const natl::Ascii postExtStartCharacter = getCurrentCharacter();
			if (postExtStartCharacter != '_') {
				addTokenFromToCurrent(type, literalStartIndex);
				return;
			}

			nextCharacter();
			if (isAtEndOfSource()) {
				addUnknownTokenFromToCurrentMinusOne(literalStartIndex);
				LexicalError& error = newError(LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart);
				error.expectedIdentiferAfterLiteralPostExtStart.tokenIndex = currentTokenIndex();
				error.expectedIdentiferAfterLiteralPostExtStart.literalType = tokenTypeToLiteralPreExtType(type);
				return;
			}

			natl::Ascii character = getCurrentCharacter();
			const natl::ui64 postExtIndentiferStartPos = index;
			while (natl::isAlphanumeric(character) || character == '_') {
				nextCharacter();
				if (isAtEndOfSource()) {
					break;
				}
				character = getCurrentCharacter();
			}

			natl::ui64 postExtIndentiferSize = index - postExtIndentiferStartPos;
			if (postExtIndentiferSize == 0) {
				addUnknownTokenFromToCurrentMinusOne(literalStartIndex);
				LexicalError& error = newError(LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart);
				error.expectedIdentiferAfterLiteralPostExtStart.tokenIndex = currentTokenIndex();
				error.expectedIdentiferAfterLiteralPostExtStart.literalType = tokenTypeToLiteralPreExtType(type);
				return;
			}

			if (postExtIndentiferSize > 4) {
				addUnknownTokenFromToCurrentMinusOne(literalStartIndex);
				LexicalError& error = newError(LexicalErrorType::unknownLiteralPostExt);
				error.unknownLiteralPostExt.literalType = tokenTypeToLiteralPreExtType(type);
				error.unknownLiteralPostExt.literalPostExtStartOffset = index - postExtIndentiferStartPos;
				error.unknownLiteralPostExt.tokenIndex = currentTokenIndex();
				return;
			}

			const natl::ConstAsciiStringView literalPostExt(&source[postExtIndentiferStartPos], postExtIndentiferSize);
			switch (type) {
			case TokenType::decimalInteger: 
			case TokenType::hexadecimalInteger:
			case TokenType::binaryInteger: {
				LiteralPostExtIntType intType;
				switch (natl::hashStringLessThan8(literalPostExt)) {
				break; case natl::hashStringLessThan8("i8"): intType = LiteralPostExtIntType::i8;
				break; case natl::hashStringLessThan8("i16"): intType = LiteralPostExtIntType::i16;
				break; case natl::hashStringLessThan8("i32"): intType = LiteralPostExtIntType::i32;
				break; case natl::hashStringLessThan8("i64"): intType = LiteralPostExtIntType::i64;
				break; case natl::hashStringLessThan8("ui8"): intType = LiteralPostExtIntType::ui8;
				break; case natl::hashStringLessThan8("ui16"): intType = LiteralPostExtIntType::ui16;
				break; case natl::hashStringLessThan8("ui32"): intType = LiteralPostExtIntType::ui32;
				break; case natl::hashStringLessThan8("ui64"): intType = LiteralPostExtIntType::ui64;
				break; default: {
					addUnknownTokenFromToCurrentMinusOne(literalStartIndex);
					LexicalError& error = newError(LexicalErrorType::unknownLiteralPostExt);
					error.unknownLiteralPostExt.literalType = tokenTypeToLiteralPreExtType(type);
					error.unknownLiteralPostExt.literalPostExtStartOffset = index - postExtIndentiferStartPos;
					error.unknownLiteralPostExt.tokenIndex = currentTokenIndex();
					return;
				}
				}

				TokenWithPostExtIntType token;
				token.tokenType = natl::toUnderlying<TokenType>(numericTokenTypeToNumericWithType(type));
				token.postExtIntType = natl::toUnderlying<LiteralPostExtIntType>(intType);
				token.offset = literalStartIndex;
				token.size = index - literalStartIndex;

				addToken(natl::bitCast<Token, TokenWithPostExtIntType>(token));
				return;
			}
			case TokenType::hexadecimalFloat:
			case TokenType::decimalFloat:
			case TokenType::binaryFloat: {
				LiteralPostExtFloatType floatType;
				switch (natl::hashStringLessThan8(literalPostExt)) {
				break; case natl::hashStringLessThan8("f32"): floatType = LiteralPostExtFloatType::f32;
				break; case natl::hashStringLessThan8("f64"): floatType = LiteralPostExtFloatType::f64;
				break; default: {
					addUnknownTokenFromToCurrentMinusOne(literalStartIndex);
					LexicalError& error = newError(LexicalErrorType::unknownLiteralPostExt);
					error.unknownLiteralPostExt.literalType = tokenTypeToLiteralPreExtType(type);
					error.unknownLiteralPostExt.literalPostExtStartOffset = index - postExtIndentiferStartPos;
					error.unknownLiteralPostExt.tokenIndex = currentTokenIndex();
					return;
				}
				}

				TokenWithPostExtFloatType token;
				token.tokenType = natl::toUnderlying<TokenType>(numericTokenTypeToNumericWithType(type));
				token.postExtFloatType = natl::toUnderlying<LiteralPostExtFloatType>(floatType);
				token.offset = literalStartIndex;
				token.size = index - literalStartIndex;

				addToken(natl::bitCast<Token, TokenWithPostExtFloatType>(token));
				return;
			}
			default:
				natl::unreachable();
			}

			return;
		}

		constexpr void handleNumericOfType(const natl::ui64 literalStartIndex, TokenType type, natl::Bool (*digitTest)(const natl::Ascii)) noexcept {
			if (isAtEndOfSource()) {
				addUnknownToken(index - literalStartIndex, index);

				LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterLiteralExt);
				error.expectedDigitsAfterLiteralExt.tokenIndex = currentTokenIndex();
				error.expectedDigitsAfterLiteralExt.literalType = tokenTypeToLiteralPreExtType(type);
				return;
			}

			//digits
			natl::Ascii character = getCurrentCharacter();
			while (digitTest(character)) {
				nextCharacter();
				if (isAtEndOfSource()) {
					addTokenFromToCurrent(type, literalStartIndex);
					return;
				}
				character = getCurrentCharacter();
			}

			//decimal digits 
			const natl::Ascii dotCharacter = getCurrentCharacter();
			if (dotCharacter == '.') {
				switch (type) {
				break; case TokenType::decimalInteger:
					type = TokenType::decimalFloat;
				break; case TokenType::hexadecimalInteger:
					type = TokenType::hexadecimalFloat;
				break; case TokenType::binaryInteger:
					type = TokenType::binaryFloat;
				break; default:
					natl::unreachable();
				}

				nextCharacter();
				if (isAtEndOfSource()) {
					addUnknownTokenFromToCurrent(literalStartIndex);

					LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterFloatDot);
					error.expectedDigitsAfterFloatDot.tokenIndex = currentTokenIndex();
					error.expectedDigitsAfterFloatDot.literalType = tokenTypeToLiteralPreExtType(type);
					return;
				}

				natl::Ascii character = getCurrentCharacter();
				while (digitTest(character)) {
					nextCharacter();
					if (isAtEndOfSource()) {
						addTokenFromToCurrent(type, literalStartIndex);
						return;
					}
					character = getCurrentCharacter();
				}
			}

			//exponent
			natl::Ascii expCharacter = getCurrentCharacter();
			if (expCharacter == 'e') {
				switch (type) {
				break; case TokenType::decimalInteger:
					type = TokenType::decimalFloat;
				break; case TokenType::hexadecimalInteger:
					type = TokenType::hexadecimalFloat;
				break; case TokenType::binaryInteger:
					type = TokenType::binaryFloat;
				break; default: break;
				}


				nextCharacter();
				if (isAtEndOfSource()) {
					addUnknownTokenFromToCurrent(literalStartIndex);

					LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterFloatExp);
					error.expectedDigitsAfterFloatExp.tokenIndex = currentTokenIndex();
					error.expectedDigitsAfterFloatExp.literalType = tokenTypeToLiteralPreExtType(type);
					return;
				}
				
				natl::Ascii character = getCurrentCharacter();
				if (!natl::isDecimalDigit(character)) {
					addUnknownTokenFromToCurrent(literalStartIndex);

					LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterFloatExp);
					error.expectedDigitsAfterFloatExp.tokenIndex = currentTokenIndex();
					error.expectedDigitsAfterFloatExp.literalType = tokenTypeToLiteralPreExtType(type);
					return;
				}

				while (natl::isDecimalDigit(character)) {
					nextCharacter();
					if (isAtEndOfSource()) {
						addTokenFromToCurrent(type, literalStartIndex);
						return;
					}
					character = getCurrentCharacter();
				}
			}

			handlePostExtNumericLiteral(literalStartIndex, type);
			return;
		}

		constexpr natl::Bool isNumericLiteral() noexcept {
			const natl::Ascii character = getCurrentCharacter();
			const natl::ui64 numberStartIndex = index;

			if (character == '#') {
				nextCharacter();
				if (isAtEndOfSource()) {
					addUnknownToken(1, index);
					return true;
				}

				natl::Ascii preExtCharacter = getCurrentCharacter();
				while (natl::isAlphabetic(preExtCharacter) || preExtCharacter == '_') {
					nextCharacter();
					if (isAtEndOfSource()) {
						addUnknownToken(index - numberStartIndex, index);
						return true;
					}
					preExtCharacter = getCurrentCharacter();
				}

				const natl::Ascii literalPreExtEndCharacter = getCurrentCharacter();
				if (literalPreExtEndCharacter != '#') {
					addUnknownToken(index - numberStartIndex, index);

					LexicalError& error = newError(LexicalErrorType::expectedHashAtLiteralPreExtEnd);
					error.unknownLiteralPreExt.tokenIndex = currentTokenIndex();
					error.unknownLiteralPreExt.literalPreExtEndIndex = index - numberStartIndex;
					return true;
				}
				nextCharacter();


				const natl::ui64 sizeOfLiteralPreExt = index - numberStartIndex - 2;
				if (sizeOfLiteralPreExt != 3) {
					LexicalError& error = newError(LexicalErrorType::unknownLiteralPreExt);
					error.unknownLiteralPreExt.tokenIndex = currentTokenIndex();
					error.unknownLiteralPreExt.literalPreExtEndIndex = sizeOfLiteralPreExt;
				} else {
					const natl::ConstAsciiStringView literalPreExt(&source[numberStartIndex + 1], sizeOfLiteralPreExt);
					if (literalPreExt == "dec") {
						handleNumericOfType(numberStartIndex, TokenType::decimalInteger, natl::isDecimalDigit);
						return true;
					} else if (literalPreExt == "hex") {
						handleNumericOfType(numberStartIndex, TokenType::hexadecimalInteger, natl::isHexadecimalDigit);
						return true;
					} else if (literalPreExt == "bin") {
						handleNumericOfType(numberStartIndex, TokenType::binaryInteger, natl::isBinaryDigit);
						return true;
					} else {
						LexicalError& error = newError(LexicalErrorType::unknownLiteralPreExt);
						error.unknownLiteralPreExt.tokenIndex = currentTokenIndex();
						error.unknownLiteralPreExt.literalPreExtEndIndex = sizeOfLiteralPreExt;
						return true;
					}
				}
			}


			if (natl::isDigit(character)) {
				nextCharacter();
				if (isAtEndOfSource()) {
					addToken(TokenType::decimalInteger, 1, index - 1);
					return true;
				}

				natl::Ascii nextCharacter = source[index];
				if (nextCharacter == 'x') {
					handleNumericOfType(numberStartIndex, TokenType::hexadecimalInteger, natl::isHexadecimalDigit);
					return true;
				} else if (nextCharacter == 'b') {
					handleNumericOfType(numberStartIndex, TokenType::binaryInteger, natl::isBinaryDigit);
					return true;
				} else {
					handleNumericOfType(numberStartIndex, TokenType::decimalInteger, natl::isDecimalDigit);
					return true;
				}
			} else {
				return false;
			}
		}

		constexpr void nextState() noexcept {
			const natl::Ascii& character = source[index];

			TokenType tokenType = TokenType::unknown;
			switch (character) {
			case ' ':
			case '\t':
			case '\r':
				index++;
				break;
			break; case ':':
				addToken(TokenType::colon, 1, index);
				index++;
			break; case ';':
				addToken(TokenType::simicolon, 1, index);
				index++;
			break; case ',':
				addToken(TokenType::comma, 1, index);
				index++;
			break; case '[':
				addToken(TokenType::leftSquare, 1, index);
				index++;
			break; case ']':
				addToken(TokenType::rightSquare, 1, index);
				index++;
			break; case '\n':
				lexicalInfo.newLineOffsets.push_back(index);
				index++;
			break; case '{':
				addToken(TokenType::leftCurly, 1, index);
				index++;
				if (enableFastIndexing) {
					curlyScopeStack.push_back(&lexicalInfo.tokens.back());
				} 
			break; case '}':
				if (enableFastIndexing) {
					natl::Size size = 1;
					if (curlyScopeStack.isNotEmpty()) {
						Token& curlyStart = *curlyScopeStack.back();
						size = index - curlyStart.offset;
						curlyStart.size = size;
					}

					addToken(TokenType::rightCurly, size, index);
					index++;
				} else {
					addToken(TokenType::rightCurly, 1, index);
					index++;
				}
			break; case '\'':
				handleCharacter();
			break; case '\"':
				handleString();
			break; case '(':
				handleDataStorage();
			break; default:
				if (isIdentifer()) { break; }
				if (isNumericLiteral()) { break; }

				addUnknownToken(1, index);
				index++;
			}
		}

		constexpr void loop() {
			while (index < source.size()) {
				nextState();
			}
		}

		constexpr void shutdown() noexcept {
			addToken(TokenType::end, 0, static_cast<natl::ui64>(source.size()));
		}

		constexpr LexicalInfo run(const natl::ConstAsciiStringView sourceIn, const natl::Bool enableFastIndexingIn) noexcept {
			source = sourceIn;
			enableFastIndexing = enableFastIndexingIn;

			if (!startup()) { 
				return lexicalInfo; 
			}
			loop();
			shutdown();

			return lexicalInfo;
		}
	};

	constexpr LexicalInfo lexicalAnalysis(const natl::ConstAsciiStringView source, const natl::Bool enableFastIndexing = true) noexcept {
		return LexicalAnalysisRunner().run(source, enableFastIndexing);
	}

	constexpr natl::Size findTokenLineNumber(const natl::ui64 offset, const natl::ArrayView<const natl::ui64> newLineOffsets) noexcept {
		const natl::Size newlineIndex = natl::findLowerBoundIndex(offset, newLineOffsets);
		return newlineIndex;
	}

	template<typename DynStringContainer>
		requires(natl::IsConvertDynStringContainer<DynStringContainer>)
	constexpr void lexicalErrorToMessage(DynStringContainer& outputDst, LexicalError& lexicalError, const natl::ArrayView<const Token>& tokens) noexcept {
		outputDst.reserve(outputDst.size() + 100);
		switch (lexicalError.type) {
		case LexicalErrorType::unknownToken:
			natl::formatTo();
			outputDst.append("unknown token at ");

		case LexicalErrorType::inputToBig:

		case LexicalErrorType::unknownIdentifer:

		case LexicalErrorType::unknownLiteralExtention:

		case LexicalErrorType::unknownLiteralPreExt:

		case LexicalErrorType::expectedHashAtLiteralPreExtEnd:

		case LexicalErrorType::expectedDigitsAfterLiteralExt:

		case LexicalErrorType::expectedDigitsAfterFloatDot:

		case LexicalErrorType::expectedDigitsAfterFloatExp:

		case LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart:

		case LexicalErrorType::unknownLiteralPostExt:
			break;
		default:
			natl::unreachable();
		}
	}
}

namespace natl {
	template<>
	struct Serialize<nadsad::ascii::TokenType> {
		using serialize_as_type = SerializeFlagType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::TokenType tokenType) noexcept {
			auto tokenTypeSizeToString = [](const Size value) noexcept -> ConstAsciiStringView {
				return nadsad::ascii::tokenTypeToString(static_cast<nadsad::ascii::TokenType>(value));
			};
			serializer.writeFlag(natl::enumToSize(tokenType), tokenTypeSizeToString);
		}
	};

	template<>
	struct Serialize<nadsad::ascii::Token> {
		using serialize_as_type = SerializeStructType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::Token& token, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			serializer.beginWriteStruct();
			const natl::Size lineNumber = nadsad::ascii::findTokenLineNumber(token.offset, lexicalInfo.newLineOffsets.toArrayView());
			serializeWriteNamed<Serializer, nadsad::ascii::TokenType>(serializer, "type", token.tokenType);
			serializeWriteNamed<Serializer, natl::Size>(serializer, "lineNumber", lineNumber);

			switch (token.tokenType) {
			case nadsad::ascii::TokenType::start:
			case nadsad::ascii::TokenType::end:
				serializeWriteNamed<Serializer>(serializer, "value", "");
				break;
			case nadsad::ascii::TokenType::leftCurly:
				serializeWriteNamed<Serializer>(serializer, "value", "{");
				break;
			case nadsad::ascii::TokenType::rightCurly:
				serializeWriteNamed<Serializer>(serializer, "value", "}");
				break;
			case nadsad::ascii::TokenType::decimalIntegerWithType:
			case nadsad::ascii::TokenType::hexadecimalIntegerWithType:
			case nadsad::ascii::TokenType::binaryIntegerWithType: {
				const nadsad::ascii::TokenWithPostExtIntType intToken =
					natl::bitCast<nadsad::ascii::TokenWithPostExtIntType, nadsad::ascii::Token>(token);
				natl::ConstAsciiStringView valueAsString(&lexicalInfo.source[intToken.offset], intToken.size);
				serializeWriteNamed<Serializer>(serializer, "value", valueAsString);
				break;
			}
			case nadsad::ascii::TokenType::decimalFloatWithType:
			case nadsad::ascii::TokenType::hexadecimalFloatWithType:
			case nadsad::ascii::TokenType::binaryFloatWithType: {
				const nadsad::ascii::TokenWithPostExtFloatType floatToken =
					natl::bitCast<nadsad::ascii::TokenWithPostExtFloatType, nadsad::ascii::Token>(token);
				natl::ConstAsciiStringView valueAsString(&lexicalInfo.source[floatToken.offset], floatToken.size);
				serializeWriteNamed<Serializer>(serializer, "value", valueAsString);
				break;
			}
			default:
			{
				natl::ConstAsciiStringView valueAsString(&lexicalInfo.source[token.offset], token.size);
				serializeWriteNamed<Serializer>(serializer, "value", valueAsString);
				break;
			}
			}

			serializer.endWriteStruct();
		}
	};

	template<>
	struct Serialize<nadsad::ascii::LexicalInfo> {
		using serialize_as_type = SerializeStructType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			serializer.beginWriteStruct();
			
			serializer.beginWrite<SerializeID::none>("tokens");
			serializer.as<natl::SerializeArrayType<natl::SerializeStructType>>();
			serializer.writeValue();

			serializer.beginWriteArray();
			for (const nadsad::ascii::Token& token : lexicalInfo.tokens) {
				serializer.beginWriteArrayElement();
				serializeWrite<Serializer, nadsad::ascii::Token>(serializer, token, lexicalInfo);
				serializer.endWriteArrayElement();
			}
			serializer.endWriteArray();
			serializer.endWrite();

			serializer.endWriteStruct();
		}
	};
}