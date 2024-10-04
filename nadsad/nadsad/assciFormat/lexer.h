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

		keywordI8,
		keywordI16,
		keywordI32,
		keywordI64,
		keywordUI8,
		keywordUI16,
		keywordUI32,
		keywordUI64,
		keywordF32,
		keywordF64,
		keywordBool,
		keywordStr,
		keywordChar,
		keywordBlob,
		keywordFile,
		keywordOp,
		keywordEnum,
		keywordFarray,
		keywordArray,
		keywordDic,
		keywordStruct,
		keywordVariant,
		keywordTable,
		keywordIndex,

		keywordTrue,
		keywordFalse
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
		case TokenType::keywordI8:
			return "keywordI8";
		case TokenType::keywordI16:
			return "keywordI16";
		case TokenType::keywordI32:
			return "keywordI32";
		case TokenType::keywordI64:
			return "keywordI64";
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
		case TokenType::keywordBool:
			return "keywordBool";
		case TokenType::keywordStr:
			return "keywordStr";
		case TokenType::keywordChar:
			return "keywordChar";
		case TokenType::keywordBlob:
			return "keywordBlob";
		case TokenType::keywordFile:
			return "keywordFile";
		case TokenType::keywordFarray:
			return "keywordFarray";
		case TokenType::keywordArray:
			return "keywordArray";
		case TokenType::keywordOp:
			return "keywordOp";
		case TokenType::keywordEnum:
			return "keywordEnum";
		case TokenType::keywordDic:
			return "keywordDic";
		case TokenType::keywordStruct:
			return "keywordStruct";
		case TokenType::keywordVariant:
			return "keywordVariant";
		case TokenType::keywordTable:
			return "keywordTable";
		case TokenType::keywordIndex:
			return "keywordIndex";
		case TokenType::keywordTrue:
			return "keywordTrue";
		case TokenType::keywordFalse:
			return "keywordFalse";
		default:
			natl::unreachable();
		}
	}

	constexpr natl::ConstAsciiStringView tokenTypeToDebugString(const TokenType tokenType) noexcept {
		switch (tokenType) {
		case TokenType::unknown:
			return "unknown";
		case TokenType::start:
			return "start";
		case TokenType::end:
			return "end";
		case TokenType::colon:
			return "colon :";
		case TokenType::simicolon:
			return "simicolon ;";
		case TokenType::comma:
			return "comma ,";
		case TokenType::leftCurly:
			return "left curly {";
		case TokenType::rightCurly:
			return "right curly }";
		case TokenType::leftSquare:
			return "left square [";
		case TokenType::rightSquare:
			return "right square ]";
		case TokenType::charLiteral:
			return "char literal";
		case TokenType::stringLiteral:
			return "string literal";
		case TokenType::dataStorage:
			return "data storage";
		case TokenType::decimalInteger:
			return "decimal integer";
		case TokenType::hexadecimalInteger:
			return "hexadecimal integer";
		case TokenType::binaryInteger:
			return "binary integer";
		case TokenType::decimalFloat:
			return "decimal float";
		case TokenType::hexadecimalFloat:
			return "hexadecimal float";
		case TokenType::binaryFloat:
			return "binary float";
		case TokenType::decimalIntegerWithType:
			return "decimal integer with type";
		case TokenType::hexadecimalIntegerWithType:
			return "hexadecimal integer with type";
		case TokenType::binaryIntegerWithType:
			return "binary integer with yype";
		case TokenType::decimalFloatWithType:
			return "decimal float with yype";
		case TokenType::hexadecimalFloatWithType:
			return "hexadecimal float with type";
		case TokenType::binaryFloatWithType:
			return "binary float with type";
		case TokenType::keywordI8:
			return "keyword i8";
		case TokenType::keywordI16:
			return "keyword i16";
		case TokenType::keywordI32:
			return "keyword i32";
		case TokenType::keywordI64:
			return "keyword i64";
		case TokenType::keywordUI8:
			return "keyword ui8";
		case TokenType::keywordUI16:
			return "keyword ui16";
		case TokenType::keywordUI32:
			return "keyword ui32";
		case TokenType::keywordUI64:
			return "keyword ui64";
		case TokenType::keywordF32:
			return "keyword f32";
		case TokenType::keywordF64:
			return "keyword f64";
		case TokenType::keywordBool:
			return "keyword bool";
		case TokenType::keywordStr:
			return "keyword str";
		case TokenType::keywordChar:
			return "keyword char";
		case TokenType::keywordFarray:
			return "keyword farray";
		case TokenType::keywordArray:
			return "keyword array";
		case TokenType::keywordOp:
			return "keyword op";
		case TokenType::keywordEnum:
			return "keyword enum";
		case TokenType::keywordDic:
			return "keyword dic";
		case TokenType::keywordBlob:
			return "keyword blob";
		case TokenType::keywordFile:
			return "keyword file";
		case TokenType::keywordStruct:
			return "keyword struct";
		case TokenType::keywordVariant:
			return "keyword variant";
		case TokenType::keywordTable:
			return "keyword table";
		case TokenType::keywordIndex:
			return "keyword index";
		case TokenType::keywordTrue:
			return "keyword true";
		case TokenType::keywordFalse:
			return "keyword false";
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
	constexpr natl::ConstAsciiStringView literalPostExtIntTypeToString(const LiteralPostExtIntType& intType) noexcept {
		switch(intType) {
		case LiteralPostExtIntType::i8:
			return "i8";
		case LiteralPostExtIntType::i16:
			return "i16";
		case LiteralPostExtIntType::i32:
			return "i32";
		case LiteralPostExtIntType::i64:
			return "i64";
		case LiteralPostExtIntType::ui8:
			return "ui8";
		case LiteralPostExtIntType::ui16:
			return "ui16";
		case LiteralPostExtIntType::ui32:
			return "ui32";
		case LiteralPostExtIntType::ui64:
			return "ui64";
		default:
			natl::unreachable();
		}
		
	}

	enum class LiteralPostExtFloatType : natl::ui8 {
		f32,
		f64,
	};

	constexpr natl::ConstAsciiStringView literalPostExtFloatTypeToString(const LiteralPostExtFloatType& floatType) noexcept {
		switch (floatType) {
		case LiteralPostExtFloatType::f32:
			return "f32";
		case LiteralPostExtFloatType::f64:
			return "f64";
		default:
			natl::unreachable();
		}
	}

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
			case natl::hashStringLessThan8("i8"):
				keywordTokenType = TokenType::keywordI8;
				break;
			case natl::hashStringLessThan8("i16"):
				keywordTokenType = TokenType::keywordI16;
				break;
			case natl::hashStringLessThan8("i32"):
				keywordTokenType = TokenType::keywordI32;
				break;
			case natl::hashStringLessThan8("i64"):
				keywordTokenType = TokenType::keywordI64;
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
			case natl::hashStringLessThan8("bool"):
				keywordTokenType = TokenType::keywordBool;
				break;
			case natl::hashStringLessThan8("char"):
				keywordTokenType = TokenType::keywordChar;
				break;
			case natl::hashStringLessThan8("str"):
				keywordTokenType = TokenType::keywordStr;
				break;
			case natl::hashStringLessThan8("op"):
				keywordTokenType = TokenType::keywordOp;
				break;
			case natl::hashStringLessThan8("enum"):
				keywordTokenType = TokenType::keywordEnum;
				break;
			case natl::hashStringLessThan8("farray"):
				keywordTokenType = TokenType::keywordFarray;
				break;
			case natl::hashStringLessThan8("array"):
				keywordTokenType = TokenType::keywordArray;
				break;
			case natl::hashStringLessThan8("dic"):
				keywordTokenType = TokenType::keywordDic;
				break;
			case natl::hashStringLessThan8("blob"):
				keywordTokenType = TokenType::keywordBlob;
				break;
			case natl::hashStringLessThan8("file"):
				keywordTokenType = TokenType::keywordFile;
				break;
			case natl::hashStringLessThan8("struct"):
				keywordTokenType = TokenType::keywordStruct;
				break;
			case natl::hashStringLessThan8("variant"):
				keywordTokenType = TokenType::keywordVariant;
				break;
			case natl::hashStringLessThan8("table"):
				keywordTokenType = TokenType::keywordTable;
				break;
			case natl::hashStringLessThan8("index"):
				keywordTokenType = TokenType::keywordIndex;
				break;
			case natl::hashStringLessThan8("true"):
				keywordTokenType = TokenType::keywordTrue;
				break;
			case natl::hashStringLessThan8("false"):
				keywordTokenType = TokenType::keywordTrue;
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

	constexpr natl::ConstAsciiStringView getViewOfTokenSource(const Token& token, const LexicalInfo& lexicalInfo) noexcept {
		return natl::ConstAsciiStringView(lexicalInfo.source.subview(token.offset, token.size));
	}

	constexpr natl::Size findTokenColumnNumber(
		const natl::Size tokenOffset,
		const natl::Size lineNumber,
		const natl::ArrayView<const natl::ui64> newLineOffsets) noexcept {
		if (!natl::isInRange(newLineOffsets, lineNumber)) {
			return 0;
		}
		return newLineOffsets[lineNumber] - tokenOffset;
	}

	template<typename DynStringContainer>
		requires(natl::IsConvertDynStringContainer<DynStringContainer>)
	constexpr void formatTokenAtToBack(DynStringContainer& outputDst, const Token& token, const LexicalInfo& lexicalInfo) {
		const natl::Size lineNumber = findTokenLineNumber(token.offset, lexicalInfo.newLineOffsets.toArrayView());
		const natl::Size columnNumber = findTokenColumnNumber(token.offset, lineNumber, lexicalInfo.newLineOffsets.toArrayView());
		natl::formatToBack(outputDst, "(line: ", lineNumber, ", column: ", columnNumber, ")");
	}

	constexpr natl::ConstAsciiStringView literalPreExtTypeToErrorString(LiteralPreExtType type) noexcept {
		switch (type) {
		case nadsad::ascii::LiteralPreExtType::decimalInteger:
			return "decimal integer";
		case nadsad::ascii::LiteralPreExtType::hexadecimalInteger:
			return "hexadecimal integer";
		case nadsad::ascii::LiteralPreExtType::binaryInteger:
			return "binary integer";
		case nadsad::ascii::LiteralPreExtType::decimalFloat:
			return "decimal float";
		case nadsad::ascii::LiteralPreExtType::hexadecimalFloat:
			return "hexadecimal float";
		case nadsad::ascii::LiteralPreExtType::binaryFloat:
			return "binary float";
		default:
			natl::unreachable();
		}
	}

	template<typename DynStringContainer>
		requires(natl::IsConvertDynStringContainer<DynStringContainer>)
	constexpr natl::Bool lexicalErrorToMessage(DynStringContainer& outputDst, const LexicalError& lexicalError, const LexicalInfo& lexicalInfo) noexcept {
		outputDst.reserve(outputDst.size() + 100);
		switch (lexicalError.type) {
		break; case LexicalErrorType::unknownToken: {
			const natl::ui64 tokenIndex = lexicalError.unknownToken.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "unknown token at");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::inputToBig:
			natl::formatToBack(outputDst, "input was too big");
			return true;
		break; case LexicalErrorType::unknownIdentifer: {
			const natl::ui64 tokenIndex = lexicalError.unknownToken.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "unknown identifer at");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::unknownLiteralExtention: {
			const natl::ui64 tokenIndex = lexicalError.unknownLiteralExtention.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "literal of type ",
				literalPreExtTypeToErrorString(lexicalError.unknownLiteralExtention.literalType),
				" with unknown literal extention at");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo),
				"with extenstion of ", getViewOfTokenSource(token, lexicalInfo).substr(lexicalError.unknownLiteralExtention.extentionOffset));
			return true;
		}
		break; case LexicalErrorType::unknownLiteralPreExt: {
			const natl::ui64 tokenIndex = lexicalError.unknownLiteralPreExt.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "unknown literal pre extention at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo),
				"with extenstion of ", getViewOfTokenSource(token, lexicalInfo).substr(lexicalError.unknownLiteralPreExt.literalPreExtEndIndex));
			return true;
		}
		break; case LexicalErrorType::expectedHashAtLiteralPreExtEnd: {
			const natl::ui64 tokenIndex = lexicalError.expectedHashAtLiteralPreExtEnd.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "expected hash at the literal pre end at");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::expectedDigitsAfterLiteralExt: {
			const natl::ui64 tokenIndex = lexicalError.expectedDigitsAfterLiteralExt.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "expected digits after literal ext at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::expectedDigitsAfterFloatDot: {
			const natl::ui64 tokenIndex = lexicalError.expectedDigitsAfterFloatDot.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "expected digits after float dot from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.expectedDigitsAfterFloatDot.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::expectedDigitsAfterFloatExp: {
			const natl::ui64 tokenIndex = lexicalError.expectedDigitsAfterFloatExp.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "expected digits after float exp from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.expectedDigitsAfterFloatExp.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart: {
			const natl::ui64 tokenIndex = lexicalError.expectedIdentiferAfterLiteralPostExtStart.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "expected identifer after literal post ext start from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.expectedIdentiferAfterLiteralPostExtStart.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; case LexicalErrorType::unknownLiteralPostExt: {
			const natl::ui64 tokenIndex = lexicalError.unknownLiteralPostExt.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokens, tokenIndex)) {
				return false;
			}
			const Token& token = lexicalInfo.tokens[tokenIndex];

			natl::formatToBack(outputDst, "unknown literal post ext \"",
				getViewOfTokenSource(token, lexicalInfo).substr(lexicalError.unknownLiteralPostExt.literalPostExtStartOffset),
				"\" from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.unknownLiteralPostExt.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, token, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, lexicalInfo));
			return true;
		}
		break; default:
			natl::unreachable();
		}
	}
}

namespace natl {
	template<>
	struct Serialize<nadsad::ascii::TokenType> {
		using serialize_as_type = SerializeEnum<SerializeTypeOf<UnderlyingType<nadsad::ascii::TokenType>>>;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::TokenType tokenType) noexcept {
			auto tokenTypeSizeToString = [](const Size value) noexcept -> ConstAsciiStringView {
				return nadsad::ascii::tokenTypeToString(static_cast<nadsad::ascii::TokenType>(value));
			};
			serializer.writeEnum(natl::enumToSize(tokenType), tokenTypeSizeToString);
		}
	};

	template<>
	struct Serialize<nadsad::ascii::Token> {
		using serialize_as_type = natl::SerializeStruct<
			natl::SerializeTypeOf<nadsad::ascii::TokenType>,
			natl::SerializeTypeOf<natl::Size>, 
			natl::SerializeTypeOf<natl::Size>, 
			natl::SerializeTypeOf<natl::Size>,
			natl::SerializeStr
		>;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::Token& token, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			serializer.beginWriteStruct();
			const natl::Size lineNumber = nadsad::ascii::findTokenLineNumber(token.offset, lexicalInfo.newLineOffsets);
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


	template<> struct natl::Serialize<nadsad::ascii::LexicalError> {
		using serialize_as_type = SerializeStruct<natl::SerializeStr>;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalError& lexicalError, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			natl::String errorMessage;
			if (nadsad::ascii::lexicalErrorToMessage(errorMessage, lexicalError, lexicalInfo)) {
				serializer.beginWriteStruct();
				serializeWriteNamed<Serializer>(serializer, "message", errorMessage.toStringView());
				serializer.endWriteStruct();
			}
		}
	};

	template<> struct natl::Serialize<nadsad::ascii::LexicalInfo> {
		using serialize_as_type = natl::SerializeStruct<
			natl::SerializeArray<natl::SerializeTypeOf<nadsad::ascii::LexicalError>>,
			natl::SerializeArray<natl::SerializeTypeOf<nadsad::ascii::Token>>
		>;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
			serializer.beginWriteStruct();
			
			serializer.beginWrite<serialize_as_type::member_types::template at<0>>("errors");

			serializer.writeValue();
			if (lexicalInfo.errors.empty()) {
				serializer.writeEmptyArray();
			} else {
				serializer.beginWriteArray();
				for (const nadsad::ascii::LexicalError& lexicalError : lexicalInfo.errors) {
					serializer.beginWriteArrayElement();
					serializeWrite<Serializer, nadsad::ascii::LexicalError>(serializer, lexicalError, lexicalInfo);
					serializer.endWriteArrayElement();
				}
				serializer.endWriteArray();
			}
			serializer.endWrite();

			serializer.beginWrite<serialize_as_type::member_types::template at<1>>("tokens");

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