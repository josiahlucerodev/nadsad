#pragma once 

//natl
#include <natl\container\flatHashMap.h>
#include <natl\container\arrayView.h>
#include <natl\util\bytes.h>

//interface 
namespace nadsad::ascii {
	/*
		TokenFormat:
		Tokens are stored as token units or ui8 chunks 
	 
		All tokens:
		Token: 5 chunk
		Has TokenType - ui8: the type of token stored in 1 chunk 
		Has Offset - ui64: the offset from the start off source
	 
		Token specific:
	 
		For TokenType::unknown:

			4 chunks:
			Has a size with integer type ui64 stored in the next 4 chunks

		For TokenType::leftCurly:
		TokenType::leftSquare

			16 chunks:
			Has a source interval size with type ui64 stored in 8 chunks
			Has a element count with type ui64 stored 8 chunks
			
		For TokenType::dataStorage:
			TokenType::stringLiteral:

			16 chunks:

			TokenStorageUnit:
			Has Size - ui64: token size with escape
			Has escapeCount - ui64: tag for how to handle escape 

		NOTE: max size of numeric literal is 32 characters 
	
		For TokenType::decimalInteger:
			TokenType::hexadecimalInteger:
			TokenType::binaryInteger:
			TokenType::decimalFloat:
			TokenType::hexadecimalFloat:
			TokenType::binaryFloat:

			TokenNumericUnit: 1 chunk
			Has Size - ui8 : size of numeric literal 
			
		For TokenType::decimalIntegerWithType:
			TokenType::hexadecimalIntegerWithType:
			TokenType::binaryIntegerWithType

			TokenNumericWithIntTypeUnit: 1 chunk
			Has Size - ui8 - 5 bits: size of numeric literal   
			Has intType - enum NumericIntType - 3 bits: int numeric type

		For TokenType::decimalFloatWithType:
			TokenType::hexadecimalFloatWithType:
			TokenType::binaryFloatWithType:
			
			TokenNumericWithFloatTypeUnit: 1 chunk
			Has Size - ui8 - 5 bits: size of numeric literal   
			Has floatType - enum NumericFloatType - 3 bits: float numeric type

	*/

	enum class TokenType : natl::ui8 {
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

		length1CharLiteral,
		length2CharLiteral,
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

		keywordNull,
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
		case TokenType::length1CharLiteral:
			return "length1CharLiteral";
		case TokenType::length2CharLiteral:
			return "length2CharLiteral";
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
		case TokenType::keywordOp:
			return "keywordOp";
		case TokenType::keywordEnum:
			return "keywordEnum";
		case TokenType::keywordFarray:
			return "keywordFarray";
		case TokenType::keywordArray:
			return "keywordArray";
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
		case TokenType::keywordNull:
			return "keywordNull";
		case TokenType::keywordTrue:
			return "keywordTrue";
		case TokenType::keywordFalse:
			return "keywordFalse";
		default:
			natl::unreachable();
		}
	}
	constexpr natl::Option<TokenType> stringToTokenType(const natl::ConstAsciiStringView& string) noexcept {
		if (string == "unknown") {
			return TokenType::unknown;
		} else if (string == "start") {
			return TokenType::start;
		} else if (string == "end") {
			return TokenType::end;
		} else if (string == "colon") {
			return TokenType::colon;
		} else if (string == "simicolon") {
			return TokenType::simicolon;
		} else if (string == "comma") {
			return TokenType::comma;
		} else if (string == "leftCurly") {
			return TokenType::leftCurly;
		} else if (string == "rightCurly") {
			return TokenType::rightCurly;
		} else if (string == "leftSquare") {
			return TokenType::leftSquare;
		} else if (string == "rightSquare") {
			return TokenType::rightSquare;
		} else if (string == "length1CharLiteral") {
			return TokenType::length1CharLiteral;
		} else if (string == "length2CharLiteral") {
			return TokenType::length2CharLiteral;
		} else if (string == "stringLiteral") {
			return TokenType::stringLiteral;
		} else if (string == "dataStorage") {
			return TokenType::dataStorage;
		} else if (string == "decimalInteger") {
			return TokenType::decimalInteger;
		} else if (string == "hexadecimalInteger") {
			return TokenType::hexadecimalInteger;
		} else if (string == "binaryInteger") {
			return TokenType::binaryInteger;
		} else if (string == "decimalFloat") {
			return TokenType::decimalFloat;
		} else if (string == "hexadecimalFloat") {
			return TokenType::hexadecimalFloat;
		} else if (string == "binaryFloat") {
			return TokenType::binaryFloat;
		} else if (string == "decimalIntegerWithType") {
			return TokenType::decimalIntegerWithType;
		} else if (string == "hexadecimalIntegerWithType") {
			return TokenType::hexadecimalIntegerWithType;
		} else if (string == "binaryIntegerWithType") {
			return TokenType::binaryIntegerWithType;
		} else if (string == "decimalFloatWithType") {
			return TokenType::decimalFloatWithType;
		} else if (string == "hexadecimalFloatWithType") {
			return TokenType::hexadecimalFloatWithType;
		} else if (string == "binaryFloatWithType") {
			return TokenType::binaryFloatWithType;
		} else if (string == "keywordI8") {
			return TokenType::keywordI8;
		} else if (string == "keywordI16") {
			return TokenType::keywordI16;
		} else if (string == "keywordI32") {
			return TokenType::keywordI32;
		} else if (string == "keywordI64") {
			return TokenType::keywordI64;
		} else if (string == "keywordUI8") {
			return TokenType::keywordUI8;
		} else if (string == "keywordUI16") {
			return TokenType::keywordUI16;
		} else if (string == "keywordUI32") {
			return TokenType::keywordUI32;
		} else if (string == "keywordUI64") {
			return TokenType::keywordUI64;
		} else if (string == "keywordF32") {
			return TokenType::keywordF32;
		} else if (string == "keywordF64") {
			return TokenType::keywordF64;
		} else if (string == "keywordBool") {
			return TokenType::keywordBool;
		} else if (string == "keywordStr") {
			return TokenType::keywordStr;
		} else if (string == "keywordChar") {
			return TokenType::keywordChar;
		} else if (string == "keywordBlob") {
			return TokenType::keywordBlob;
		} else if (string == "keywordFile") {
			return TokenType::keywordFile;
		} else if (string == "keywordOp") {
			return TokenType::keywordOp;
		} else if (string == "keywordEnum") {
			return TokenType::keywordEnum;
		} else if (string == "keywordFarray") {
			return TokenType::keywordFarray;
		} else if (string == "keywordArray") {
			return TokenType::keywordArray;
		} else if (string == "keywordDic") {
			return TokenType::keywordDic;
		} else if (string == "keywordStruct") {
			return TokenType::keywordStruct;
		} else if (string == "keywordVariant") {
			return TokenType::keywordVariant;
		} else if (string == "keywordTable") {
			return TokenType::keywordTable;
		} else if (string == "keywordIndex") {
			return TokenType::keywordIndex;
		} else if (string == "keywordNull") {
			return TokenType::keywordNull;
		} else if (string == "keywordTrue") {
			return TokenType::keywordTrue;
		} else if (string == "keywordFalse") {
			return TokenType::keywordFalse;
		} else {
			return {};
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
		case TokenType::length1CharLiteral:
			return "char literal";
		case TokenType::length2CharLiteral:
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
		case TokenType::keywordOp:
			return "keyword op";
		case TokenType::keywordEnum:
			return "keyword enum";
		case TokenType::keywordFarray:
			return "keyword farray";
		case TokenType::keywordArray:
			return "keyword array";
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
		case TokenType::keywordNull:
			return "keyword null";
		case TokenType::keywordTrue:
			return "keyword true";
		case TokenType::keywordFalse:
			return "keyword false";
		default:
			natl::unreachable();
		}
	}

	enum class NumericIntType : natl::ui8 {
		i8,
		i16,
		i32,
		i64,
		ui8,
		ui16,
		ui32,
		ui64,
	};
	constexpr natl::ConstAsciiStringView numericIntTypeToString(const NumericIntType& intType) noexcept {
		switch(intType) {
		case NumericIntType::i8:
			return "i8";
		case NumericIntType::i16:
			return "i16";
		case NumericIntType::i32:
			return "i32";
		case NumericIntType::i64:
			return "i64";
		case NumericIntType::ui8:
			return "ui8";
		case NumericIntType::ui16:
			return "ui16";
		case NumericIntType::ui32:
			return "ui32";
		case NumericIntType::ui64:
			return "ui64";
		default:
			natl::unreachable();
		}
	}

	constexpr natl::Option<NumericIntType> stringToNumericIntType(const natl::ConstAsciiStringView& str) noexcept {
		if (str == "i8") {
			return NumericIntType::i8;
		} else if (str == "i16") {
			return NumericIntType::i16;
		} else if (str == "i32") {
			return NumericIntType::i32;
		} else if (str == "i64") {
			return NumericIntType::i64;
		} else if (str == "ui8") {
			return NumericIntType::ui8;
		} else if (str == "ui16") {
			return NumericIntType::ui16;
		} else if (str == "ui32") {
			return NumericIntType::ui32;
		} else if (str == "ui64") {
			return NumericIntType::ui64;
		} else {
			return {};
		}
	}

	enum class NumericFloatType : natl::ui8 {
		f32,
		f64,
	};

	constexpr natl::ConstAsciiStringView numericFloatTypeToString(const NumericFloatType& floatType) noexcept {
		switch (floatType) {
		case NumericFloatType::f32:
			return "f32";
		case NumericFloatType::f64:
			return "f64";
		default:
			natl::unreachable();
		}
	}

	constexpr natl::Option<NumericFloatType> stringToNumericFloatType(const natl::ConstAsciiStringView& str) noexcept {
		if (str == "f32") {
			return NumericFloatType::f32;
		} else if (str == "f64") {
			return NumericFloatType::f64;
		} else {
			return {};
		}
	}


	using TokenUnit = natl::ui8;

	struct Token {
		TokenType type;
		natl::Size offset;
	};

	struct TokenStorageUnit {
		natl::ui64 size;
		natl::ui64 escapeCount;
	};

	struct TokenNumericUnit {
		natl::ui8 size;
	};

	struct TokenNumericWithIntTypeUnit {
		natl::ui8 size : 5;
		NumericIntType intType : 3;
	};
	
	struct TokenNumericWithFloatTypeUnit {
		natl::ui8 size : 5;
		NumericFloatType floatType : 3;
	};

	constexpr natl::ui64 getTokenUi64IntegerAt(const natl::ui64 index, const natl::ConstArrayView<TokenUnit>& tokenUnits) noexcept {
		return natl::bytesToUi64(tokenUnits.subview(index, 8));
	}
	constexpr natl::ui64 getTokenOffsetAt(const natl::ui64 tokenIndex, const natl::ConstArrayView<TokenUnit>& tokenUnits) noexcept {
		return getTokenUi64IntegerAt(tokenIndex + 1, tokenUnits);
	}

	constexpr TokenType convertTokenUnitToTokenType(const TokenUnit& tokenUnit) noexcept {
		return natl::bitCast<TokenType, TokenUnit>(tokenUnit);
	}

	constexpr Token getTokenAt(const natl::ui64 tokenIndex, const natl::ConstArrayView<TokenUnit>& tokenUnits) noexcept {
		Token token;
		token.type = convertTokenUnitToTokenType(tokenUnits[tokenIndex]);
		token.offset = getTokenUi64IntegerAt(tokenIndex + 1, tokenUnits);
		return token;
	}


	enum class LexicalErrorType {
		unknownToken, 
		unknownIdentifer,
		unknownLiteralExtention,
		unknownLiteralPreExt,
		expectedHashAtLiteralPreExtEnd,
		expectedDigitsAfterLiteralExt,
		expectedDigitsAfterFloatDot,
		expectedDigitsAfterFloatExp,
		expectedIdentiferAfterLiteralPostExtStart,
		unknownLiteralPostExt,
		numericTooBig,
		nonMatchingBeginScope,
		noBeingScope,
		noEndScope,
	};

	constexpr natl::ConstAsciiStringView lexcialErrorTypeToString(const LexicalErrorType type) noexcept {
		switch (type) {
		case LexicalErrorType::unknownToken:
			return "unknownToken";
		case LexicalErrorType::unknownIdentifer:
			return "unknownIdentifer";
		case LexicalErrorType::unknownLiteralExtention:
			return "unknownLiteralExtention";
		case LexicalErrorType::unknownLiteralPreExt:
			return "unknownLiteralPreExt";
		case LexicalErrorType::expectedHashAtLiteralPreExtEnd:
			return "expectedHashAtLiteralPreExtEnd";
		case LexicalErrorType::expectedDigitsAfterLiteralExt:
			return "expectedDigitsAfterLiteralExt";
		case LexicalErrorType::expectedDigitsAfterFloatDot:
			return "expectedDigitsAfterFloatDot";
		case LexicalErrorType::expectedDigitsAfterFloatExp:
			return "expectedDigitsAfterFloatExp";
		case LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart:
			return "expectedIdentiferAfterLiteralPostExtStart";
		case LexicalErrorType::unknownLiteralPostExt:
			return "numericTooBig";
		case LexicalErrorType::numericTooBig:
			return "unknownLiteralPostExt";
		case LexicalErrorType::nonMatchingBeginScope:
			return "nonMatchingBeginScope";
		case LexicalErrorType::noBeingScope:
			return "noBeingScope";
		case LexicalErrorType::noEndScope:
			return "noEndScope";
		default:
			natl::unreachable();
		}
	}

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

		union ErrorVariant {
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

			struct NumericTooBig {
				natl::ui64 tokenIndex;
			} numericTooBig;

			struct NonMatchingBeingScope {
				natl::ui64 endScopeTokenIndex;
				natl::ui64 wrongBeingScopeTokenIndex;
			} nonMatchingBeingScope;

			struct NoBeginScope {
				natl::ui64 endScopeTokenIndex;
			} noBeginScope;

			struct NoEndScope {
				natl::ui64 beginScopeTokenIndex;
			} noEndScope;
		} errorVariant;
	};

	struct LexicalInfo {
		natl::ui64 numberOfTokens;
		natl::ArrayView<const natl::Ascii> source;
		natl::DynArray<TokenUnit> tokenUnits;
		natl::DynArray<natl::ui64> newLineOffsets;
		natl::DynArray<LexicalError> errors;
	};

	struct LexicalAnalysisRunner  {
		struct ScopeInfo {
			natl::ui64 beginScopeTokenIndex;
			natl::ui64 beginScopeTokenInfoIndex;
			natl::ui64 beginScopeTokenTotalOffset;
		};

		LexicalInfo lexicalInfo;
		natl::ConstAsciiStringView source;
		natl::ui64 index;
		natl::SmallDynArray<ScopeInfo, 32> scopeStack;

		natl::ui64 previousEndOffset;
		natl::Bool previousTokenWasUnknown;
		natl::ui64 previousUnknownTokenEndOffset;

		constexpr void startup() noexcept {
			previousUnknownTokenEndOffset = 0;
			previousEndOffset = 0;
			previousTokenWasUnknown = false;
			index = 0;

			lexicalInfo.numberOfTokens = 0;
			lexicalInfo.source = source;
			lexicalInfo.tokenUnits.reserve((source.size() >> 5) + 1000); //expect a token every 8 characters
			lexicalInfo.tokenUnits.reserve(1000); //expect a token every 8 characters
			lexicalInfo.newLineOffsets.reserve((source.size() >> 3) + 100); //expect a newline every 32 characters
			addToken(TokenType::start, 0);

			return;
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
			return lexicalInfo.tokenUnits.size() - 1;
		}

		constexpr LexicalError& newError(const LexicalErrorType errorType) noexcept {
			LexicalError error;
			error.type = errorType;
			lexicalInfo.errors.pushBack(error);
			return lexicalInfo.errors.back();
		}
		constexpr natl::ui64 addUnknownToken(const natl::ui64 size,
			const natl::ui64& offset, 
			const natl::Bool& createError) noexcept {

			if (previousTokenWasUnknown && previousUnknownTokenEndOffset == offset) {
				addTo64BitIntegerAt(lexicalInfo.tokenUnits.size() - sizeof(natl::ui64), size);
				previousUnknownTokenEndOffset += size;
				previousTokenWasUnknown = true;
				return lexicalInfo.tokenUnits.size() - sizeof(natl::ui64) - 1;
			}

			const natl::ui64 unknownTokenIndex = addToken(TokenType::unknown, offset);
			add64BitInteger(size);

			if(createError) {
				LexicalError& error = newError(LexicalErrorType::unknownToken);
				error.errorVariant.unknownToken.tokenIndex = unknownTokenIndex;
				previousTokenWasUnknown = true;
				previousUnknownTokenEndOffset = offset + size;
			}

			return unknownTokenIndex;
		}

		constexpr natl::ui64 addUnknownTokenFromToCurrent(const natl::Size startPos, const natl::Bool& createError) noexcept {
			return addUnknownToken(index - startPos, startPos, createError);
		}
		constexpr natl::ui64 addUnknownTokenFromToCurrentMinusOne(const natl::Size startPos, const natl::Bool& createError) noexcept {
			return addUnknownToken(index - startPos - 1, startPos, createError);
		}

		constexpr void addTokenUnit(const TokenUnit tokenUnit) noexcept {
			lexicalInfo.tokenUnits.pushBack(tokenUnit);
		}
		constexpr void addTokenType(const TokenType& tokenType) noexcept {
			lexicalInfo.tokenUnits.pushBack(natl::bitCast<TokenUnit, TokenType>(tokenType));
			lexicalInfo.numberOfTokens += 1;
		}
		constexpr natl::ui64 addToken(const TokenType tokenType, const natl::ui64 totalOffset) noexcept {
			addTokenType(tokenType);
			add64BitInteger(totalOffset);
			previousTokenWasUnknown = false;
			return currentTokenIndex() - 8;
		}

		constexpr void add64BitInteger(const natl::ui64 value) noexcept {
			natl::Array<natl::ui8, 8> bytes
				= natl::ui64ToBytes(value);

			addTokenUnit(bytes[0]);
			addTokenUnit(bytes[1]);
			addTokenUnit(bytes[2]);
			addTokenUnit(bytes[3]);
			addTokenUnit(bytes[4]);
			addTokenUnit(bytes[5]);
			addTokenUnit(bytes[6]);
			addTokenUnit(bytes[7]);
		}

		constexpr void addTo64BitIntegerAt(const natl::Size index, const natl::Size& rhs) noexcept {
			const natl::ui64 value = natl::bytesToUi64(lexicalInfo.tokenUnits.subview(index, 8));
			natl::ui64ToBytesAt(value + rhs, lexicalInfo.tokenUnits.subview(index, 8));
			return;
		}

		constexpr void addTokenStorageUnit(const natl::Size size, const natl::Size escape) noexcept {
			add64BitInteger(size);
			add64BitInteger(escape);
		}


		constexpr void handleCharacter() noexcept {
			const natl::ui64 startIndex = index;
			natl::ui64 charLiteralSize = 2;

			if (startIndex + 2 >= source.size()) {
				addUnknownToken(charLiteralSize, startIndex, true);
				index = index + 2;
				return;
			}

			index += 1;
			charLiteralSize += 1;
			natl::Ascii scopedCharacter = source[index];
			if (scopedCharacter == '\\') {
				if (index + 2 >= source.size()) {
					addUnknownToken(charLiteralSize, startIndex, true);
					index = index + 2;
					const natl::ui64 startIndex = index;
					natl::ui64 charLiteralSize = 2;

				if (startIndex + 2 >= source.size()) {
					addUnknownToken(charLiteralSize, startIndex, true);
					index = index + 2;
					return;
				}

				index += 1;
				charLiteralSize += 1;
				natl::Ascii scopedCharacter = source[index];
				if (scopedCharacter == '\\') {
					if (index + 2 >= source.size()) {
						addUnknownToken(charLiteralSize, startIndex, true);
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
					addUnknownToken(charLiteralSize, startIndex, true);
					nextCharacter();
					return;
				}

				addToken(TokenType::length2CharLiteral, startIndex);
				nextCharacter();
				return;
				}

				index += 1;
				scopedCharacter = source[index];
				charLiteralSize += 1;
			}

			index += 1;
			const natl::Ascii endCharacter = source[index];
			if (endCharacter != '\'') {
				addUnknownToken(charLiteralSize, startIndex, true);
				nextCharacter();
				return;
			}

			addToken(TokenType::length1CharLiteral, startIndex);
			nextCharacter();
			return;
		}

		constexpr void handleString() noexcept {
			const natl::ui64 startIndex = index;
			nextCharacter(); //account for start "

			if (isAtEndOfSource()) {
				addUnknownToken(1, startIndex, true);
				return;
			}

			natl::ui64 escapeCount = 0;
			natl::Ascii stringLiteralCharacter = getCurrentCharacter();
			if(stringLiteralCharacter == '\"') {
				nextCharacter();
			} else {
				while (index != source.size() && stringLiteralCharacter != '\"') {
					stringLiteralCharacter = getCurrentCharacter();

					if (stringLiteralCharacter == '\\') {
						escapeCount++;
						nextCharacter();
						if (isAtEndOfSource()) {
							addUnknownToken(index - startIndex, startIndex, true);
							return;
						}
					}
					nextCharacter();
				}
			}

			const natl::ui64 stringLiteralSize = index - startIndex;
			addToken(TokenType::stringLiteral, startIndex);
			addTokenStorageUnit(stringLiteralSize, escapeCount);
			//index++; //remove end "
			return; 
		}
		constexpr void handleDataStorage() noexcept {
			const natl::ui64 startIndex = index;
			index += 1;

			if (isAtEndOfSource()) {
				addUnknownToken(1, startIndex, true);
				return;
			}

			natl::ui64 escapeCount = 0;
			natl::Ascii blobData = source[index];
			while (index != source.size() && blobData != ')') {
				blobData = source[index];

				if (blobData == natl::Ascii(255)) {
					escapeCount++;
					index += 1;
					if (isAtEndOfSource()) {
						addUnknownToken(index - startIndex, startIndex, true);
						return;
					}
				}

				nextCharacter();
			}

			const natl::ui64 dataStorageSize = index - startIndex;
			addToken(TokenType::dataStorage, startIndex);
			addTokenStorageUnit(dataStorageSize, escapeCount);
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
				const natl::ui64 unknownTokenIndex = addUnknownToken(identifierSize, identifierStartIndex, false);
				LexicalError& error = newError(LexicalErrorType::unknownIdentifer);
				error.errorVariant.unknownIdentifer.tokenIndex = unknownTokenIndex;
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
			case natl::hashStringLessThan8("null"):
				keywordTokenType = TokenType::keywordNull;
				break;
			case natl::hashStringLessThan8("true"):
				keywordTokenType = TokenType::keywordTrue;
				break;
			case natl::hashStringLessThan8("false"):
				keywordTokenType = TokenType::keywordTrue;
				break;
			default:
				const natl::ui64 unknownTokenIndex = addUnknownToken(identifierSize, identifierStartIndex, false);
				LexicalError& error = newError(LexicalErrorType::unknownIdentifer);
				error.errorVariant.unknownIdentifer.tokenIndex = unknownTokenIndex;
				return true;
			}

			addToken(keywordTokenType, identifierStartIndex);
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

		constexpr natl::Bool handleIfNumericTooBig(const natl::ui64 startIndex, const natl::ui64 size) noexcept {
			if (size > natl::Limits<natl::ui8>::max()) {
				const natl::ui64 unknownTokenIndex = addUnknownToken(size, startIndex, false);
				LexicalError& error = newError(LexicalErrorType::numericTooBig);
				error.errorVariant.numericTooBig.tokenIndex = unknownTokenIndex;
				return true;
			}

			return false;
		}

		constexpr void handlePostExtNumericLiteral(const natl::ui64 literalStartIndex, const TokenType type) noexcept {
			const natl::Ascii postExtStartCharacter = getCurrentCharacter();
			if (postExtStartCharacter != '_') {
				addNumericToken(literalStartIndex, type);
				return;
			}

			nextCharacter();
			if (isAtEndOfSource()) {
				const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrentMinusOne(literalStartIndex, false);
				LexicalError& error = newError(LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart);
				error.errorVariant.expectedIdentiferAfterLiteralPostExtStart.tokenIndex = unknownTokenIndex;
				error.errorVariant.expectedIdentiferAfterLiteralPostExtStart.literalType = tokenTypeToLiteralPreExtType(type);
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
				const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrentMinusOne(literalStartIndex, false);
				LexicalError& error = newError(LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart);
				error.errorVariant.expectedIdentiferAfterLiteralPostExtStart.tokenIndex = unknownTokenIndex;
				error.errorVariant.expectedIdentiferAfterLiteralPostExtStart.literalType = tokenTypeToLiteralPreExtType(type);
				return;
			}

			if (postExtIndentiferSize > 4) {
				const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrentMinusOne(literalStartIndex, false);
				LexicalError& error = newError(LexicalErrorType::unknownLiteralPostExt);
				error.errorVariant.unknownLiteralPostExt.literalType = tokenTypeToLiteralPreExtType(type);
				error.errorVariant.unknownLiteralPostExt.literalPostExtStartOffset = index - postExtIndentiferStartPos;
				error.errorVariant.unknownLiteralPostExt.tokenIndex = unknownTokenIndex;
				return;
			}

			const natl::ConstAsciiStringView literalPostExt(&source[postExtIndentiferStartPos], postExtIndentiferSize);
			switch (type) {
			case TokenType::decimalInteger: 
			case TokenType::hexadecimalInteger:
			case TokenType::binaryInteger: {
				NumericIntType intType;
				switch (natl::hashStringLessThan8(literalPostExt)) {
				break; case natl::hashStringLessThan8("i8"): intType = NumericIntType::i8;
				break; case natl::hashStringLessThan8("i16"): intType = NumericIntType::i16;
				break; case natl::hashStringLessThan8("i32"): intType = NumericIntType::i32;
				break; case natl::hashStringLessThan8("i64"): intType = NumericIntType::i64;
				break; case natl::hashStringLessThan8("ui8"): intType = NumericIntType::ui8;
				break; case natl::hashStringLessThan8("ui16"): intType = NumericIntType::ui16;
				break; case natl::hashStringLessThan8("ui32"): intType = NumericIntType::ui32;
				break; case natl::hashStringLessThan8("ui64"): intType = NumericIntType::ui64;
				break; default: {
					const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrentMinusOne(literalStartIndex, false);
					LexicalError& error = newError(LexicalErrorType::unknownLiteralPostExt);
					error.errorVariant.unknownLiteralPostExt.literalType = tokenTypeToLiteralPreExtType(type);
					error.errorVariant.unknownLiteralPostExt.literalPostExtStartOffset = index - postExtIndentiferStartPos;
					error.errorVariant.unknownLiteralPostExt.tokenIndex = unknownTokenIndex;
					return;
				}
				}

				const natl::ui64 size = index - literalStartIndex;
				if (handleIfNumericTooBig(literalStartIndex, index)) {
					return;
				}

				TokenNumericWithIntTypeUnit numericIntUnit;
				numericIntUnit.size = static_cast<natl::ui8>(size);
				numericIntUnit.intType = intType;

				addToken(type, literalStartIndex);
				addTokenUnit(natl::bitCast<TokenUnit, TokenNumericWithIntTypeUnit>(numericIntUnit));
				return;
			}
			case TokenType::hexadecimalFloat:
			case TokenType::decimalFloat:
			case TokenType::binaryFloat: {
				NumericFloatType floatType;
				switch (natl::hashStringLessThan8(literalPostExt)) {
				break; case natl::hashStringLessThan8("f32"): floatType = NumericFloatType::f32;
				break; case natl::hashStringLessThan8("f64"): floatType = NumericFloatType::f64;
				break; default: {
					const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrentMinusOne(literalStartIndex, false);
					LexicalError& error = newError(LexicalErrorType::unknownLiteralPostExt);
					error.errorVariant.unknownLiteralPostExt.literalType = tokenTypeToLiteralPreExtType(type);
					error.errorVariant.unknownLiteralPostExt.literalPostExtStartOffset = index - postExtIndentiferStartPos;
					error.errorVariant.unknownLiteralPostExt.tokenIndex = unknownTokenIndex;
					return;
				}
				}

				const natl::ui64 size = index - literalStartIndex;
				if (handleIfNumericTooBig(literalStartIndex, index)) {
					return;
				}

				TokenNumericWithFloatTypeUnit numericFloatUnit;
				numericFloatUnit.size = static_cast<natl::ui8>(size);
				numericFloatUnit.floatType = floatType;

				addToken(type, literalStartIndex);
				addTokenUnit(natl::bitCast<TokenUnit, TokenNumericWithFloatTypeUnit>(numericFloatUnit));
				return;
			}
			default:
				natl::unreachable();
			}

			return;
		}

		constexpr void addNumericToken(const natl::ui64 startIndex, const TokenType type) noexcept {
			const natl::ui64 size = index - startIndex;
			if(handleIfNumericTooBig(startIndex, size)) {
				return;
			}

			TokenNumericUnit numericUnit;
			numericUnit.size = static_cast<natl::ui8>(size);

			addToken(type, startIndex);
			addTokenUnit(natl::bitCast<TokenUnit, TokenNumericUnit>(numericUnit));
		}

		constexpr void handleNumericOfType(const natl::ui64 literalStartIndex, TokenType type, natl::Bool (*digitTest)(const natl::Ascii)) noexcept {
			if (isAtEndOfSource()) {
				const natl::ui64 unknownTokenIndex = addUnknownToken(index - literalStartIndex, index, false);
				LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterLiteralExt);
				error.errorVariant.expectedDigitsAfterLiteralExt.tokenIndex = unknownTokenIndex;
				error.errorVariant.expectedDigitsAfterLiteralExt.literalType = tokenTypeToLiteralPreExtType(type);
				return;
			}

			//digits
			natl::Ascii character = getCurrentCharacter();
			while (digitTest(character)) {
				nextCharacter();
				if (isAtEndOfSource()) {
					addNumericToken(literalStartIndex, type);
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
					const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrent(literalStartIndex, false);
					LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterFloatDot);
					error.errorVariant.expectedDigitsAfterFloatDot.tokenIndex = unknownTokenIndex;
					error.errorVariant.expectedDigitsAfterFloatDot.literalType = tokenTypeToLiteralPreExtType(type);
					return;
				}

				natl::Ascii character = getCurrentCharacter();
				while (digitTest(character)) {
					nextCharacter();
					if (isAtEndOfSource()) {
						addNumericToken(literalStartIndex, type);
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
					const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrent(literalStartIndex, false);
					LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterFloatExp);
					error.errorVariant.expectedDigitsAfterFloatExp.tokenIndex = unknownTokenIndex;
					error.errorVariant.expectedDigitsAfterFloatExp.literalType = tokenTypeToLiteralPreExtType(type);
					return;
				}
				
				natl::Ascii character = getCurrentCharacter();
				if (!natl::isDecimalDigit(character)) {
					const natl::ui64 unknownTokenIndex = addUnknownTokenFromToCurrent(literalStartIndex, false);
					LexicalError& error = newError(LexicalErrorType::expectedDigitsAfterFloatExp);
					error.errorVariant.expectedDigitsAfterFloatExp.tokenIndex = unknownTokenIndex;
					error.errorVariant.expectedDigitsAfterFloatExp.literalType = tokenTypeToLiteralPreExtType(type);
					return;
				}

				while (natl::isDecimalDigit(character)) {
					nextCharacter();
					if (isAtEndOfSource()) {
						addNumericToken(literalStartIndex, type);
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
					addUnknownToken(1, index, true);
					return true;
				}

				natl::Ascii preExtCharacter = getCurrentCharacter();
				while (natl::isAlphabetic(preExtCharacter) || preExtCharacter == '_') {
					nextCharacter();
					if (isAtEndOfSource()) {
						addUnknownToken(index - numberStartIndex, index, true);
						return true;
					}
					preExtCharacter = getCurrentCharacter();
				}

				const natl::Ascii literalPreExtEndCharacter = getCurrentCharacter();
				if (literalPreExtEndCharacter != '#') {
					const natl::ui64 unknownTokenIndex = addUnknownToken(index - numberStartIndex, index, false);
					LexicalError& error = newError(LexicalErrorType::expectedHashAtLiteralPreExtEnd);
					error.errorVariant.unknownLiteralPreExt.tokenIndex = unknownTokenIndex;
					error.errorVariant.unknownLiteralPreExt.literalPreExtEndIndex = index - numberStartIndex;
					return true;
				}
				nextCharacter();


				const natl::ui64 sizeOfLiteralPreExt = index - numberStartIndex - 2;
				if (sizeOfLiteralPreExt != 3) {
					const natl::ui64 unknownTokenIndex = addUnknownToken(index - numberStartIndex, index, false);
					LexicalError& error = newError(LexicalErrorType::unknownLiteralPreExt);
					error.errorVariant.unknownLiteralPreExt.tokenIndex = unknownTokenIndex;
					error.errorVariant.unknownLiteralPreExt.literalPreExtEndIndex = sizeOfLiteralPreExt;
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
						const natl::ui64 unknownTokenIndex = addUnknownToken(index - numberStartIndex, index, false);
						LexicalError& error = newError(LexicalErrorType::unknownLiteralPreExt);
						error.errorVariant.unknownLiteralPreExt.tokenIndex = unknownTokenIndex;
						error.errorVariant.unknownLiteralPreExt.literalPreExtEndIndex = sizeOfLiteralPreExt;
						return true;
					}
				}
			}


			if (natl::isDigit(character)) {
				nextCharacter();
				if (isAtEndOfSource()) {
					addNumericToken(numberStartIndex, TokenType::decimalInteger);
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

		constexpr void addBeginScopeToken(const TokenType type) noexcept {
			const natl::ui64 beginScopeTokenIndex = addToken(type, index); // token
			const natl::ui64 beginScopeTokenInfoIndex = currentTokenIndex() + 1;
			const natl::ui64 beginScopeTokenTotalOffset = index + 1;
			scopeStack.pushBack(ScopeInfo(beginScopeTokenIndex, beginScopeTokenInfoIndex, beginScopeTokenTotalOffset));

			add64BitInteger(0); //scope offset
			add64BitInteger(0); //element count
			nextCharacter();
		}

		constexpr void addEndScopeToken(const TokenType endType, const TokenType beginType) noexcept {
			natl::Size size = 0;
			const natl::Size endScopeTokenIndex = currentTokenIndex() + 1;

			if (scopeStack.isNotEmpty()) {
				const ScopeInfo scopeInfo = scopeStack.back();
				const TokenType beginScopeTokenType = natl::bitCast<TokenType, TokenUnit>(lexicalInfo.tokenUnits[scopeInfo.beginScopeTokenIndex]);


				if(beginType == beginScopeTokenType) {
					const natl::ui64 size = index - scopeInfo.beginScopeTokenTotalOffset;
					addTo64BitIntegerAt(scopeInfo.beginScopeTokenInfoIndex, size);
				} else {
					LexicalError& error = newError(LexicalErrorType::nonMatchingBeginScope);
					error.errorVariant.nonMatchingBeingScope.endScopeTokenIndex = endScopeTokenIndex;
					error.errorVariant.nonMatchingBeingScope.wrongBeingScopeTokenIndex = scopeInfo.beginScopeTokenIndex;
				}

				scopeStack.pop_back();
			} else {
				LexicalError& error = newError(LexicalErrorType::noBeingScope);
				error.errorVariant.noBeginScope.endScopeTokenIndex = endScopeTokenIndex;
			}

			addToken(endType, index);
			nextCharacter();
		}

		constexpr void nextState() noexcept {
			const natl::Ascii& character = source[index];

			TokenType tokenType = TokenType::unknown;
			switch (character) {
			case ' ':
			case '\t':
			case '\r':
				nextCharacter();
				return;
			break; case '\n':
				lexicalInfo.newLineOffsets.pushBack(index);
				nextCharacter();
				return;
			break; case ':':
				addToken(TokenType::colon, index);
				nextCharacter();
			break; case ';':
				addToken(TokenType::simicolon, index);
				nextCharacter();
			break; case ',':
				if(scopeStack.isNotEmpty()) {
					addTo64BitIntegerAt(scopeStack.back().beginScopeTokenInfoIndex + sizeof(natl::ui64), 1);
				}
				addToken(TokenType::comma, index);
				nextCharacter();
			break; case '[':
				addBeginScopeToken(TokenType::leftSquare);
			break; case ']':
				addEndScopeToken(TokenType::rightSquare, TokenType::leftSquare);
			break; case '{':
				addBeginScopeToken(TokenType::leftCurly);
			break; case '}': 
				addEndScopeToken(TokenType::rightCurly, TokenType::leftCurly);
			break; case '\'':
				handleCharacter();
			break; case '\"':
				handleString();
			break; case '(':
				handleDataStorage();
			break; default:
				if (isIdentifer()) { break; }
				if (isNumericLiteral()) { break; }

				addUnknownToken(1, index, false);
				nextCharacter();
			}
			previousEndOffset = index;
		}

		constexpr void loop() {
			while (index < source.size()) {
				nextState();
			}
		}

		constexpr void shutdown() noexcept {
			addToken(TokenType::end, index);

			for(const ScopeInfo& scopeInfo : scopeStack) {
				LexicalError& error = newError(LexicalErrorType::noBeingScope);
				error.errorVariant.noEndScope.beginScopeTokenIndex = scopeInfo.beginScopeTokenIndex;
			}
		}

		constexpr LexicalInfo run(const natl::ConstAsciiStringView sourceIn) noexcept {
			source = sourceIn;

			startup();
			loop();
			shutdown();

			return lexicalInfo;
		}
	};

	constexpr LexicalInfo lexicalAnalysis(const natl::ConstAsciiStringView source) noexcept {
		return LexicalAnalysisRunner().run(source);
	}

	constexpr natl::Option<natl::ui64> getTotalOffsetOfTokenAt(const natl::ui64 tokenIndex, const natl::ConstArrayView<TokenUnit>& tokenUnits) noexcept {
		return getTokenUi64IntegerAt(tokenIndex + 1, tokenUnits);
	}

	constexpr natl::ui64 getTokenUnitIndexAfterToken(const natl::ui64 tokenIndex) noexcept {
		return tokenIndex + sizeof(natl::ui64) + 1;
	}

	constexpr natl::ui64 getTokenStorageUnitSizeValueAt(
		const natl::ui64 tokenIndex, 
		const natl::ConstArrayView<TokenUnit>& tokenUnits) noexcept {
		return getTokenUi64IntegerAt(tokenIndex, tokenUnits);;
	}

	constexpr TokenStorageUnit getTokenStorageUnitAt(
		const natl::ui64 tokenIndex,
		const natl::ConstArrayView<TokenUnit>& tokenUnits) {
		TokenStorageUnit storageUnit;
		storageUnit.size = getTokenUi64IntegerAt(tokenIndex, tokenUnits);
		storageUnit.escapeCount = getTokenUi64IntegerAt(tokenIndex + sizeof(natl::ui64), tokenUnits);
		return storageUnit;
	}

	constexpr natl::ui64 getSizeOfToken(const natl::ui64 tokenIndex, const Token token, const natl::ConstArrayView<TokenUnit>& tokenUnits) noexcept {
		switch (token.type) {
		case TokenType::unknown:
			return getTokenUi64IntegerAt(
				getTokenUnitIndexAfterToken(tokenIndex), tokenUnits);
		case TokenType::dataStorage:
		case TokenType::stringLiteral:
			return getTokenStorageUnitSizeValueAt(
				getTokenUnitIndexAfterToken(tokenIndex), tokenUnits);
		case TokenType::start:
			return 0;
		case TokenType::end:
			return 0;
		case TokenType::colon:
			return 1;
		case TokenType::simicolon:
			return 1;
		case TokenType::comma:
			return 1;
		case TokenType::leftCurly:
			return 1;
		case TokenType::rightCurly:
			return 1;
		case TokenType::leftSquare:
			return 1;
		case TokenType::rightSquare:
			return 1;
		case TokenType::length1CharLiteral:
			return 1;
		case TokenType::length2CharLiteral:
			return 2;
		case TokenType::decimalInteger:
		case TokenType::hexadecimalInteger:
		case TokenType::binaryInteger:
		case TokenType::decimalFloat:
		case TokenType::hexadecimalFloat:
		case TokenType::binaryFloat: {
			const TokenNumericUnit numericUnit =
				natl::bitCast<TokenNumericUnit, TokenUnit>(
					tokenUnits[getTokenUnitIndexAfterToken(tokenIndex)]);
			return numericUnit.size;
		}
		case TokenType::decimalIntegerWithType:
		case TokenType::hexadecimalIntegerWithType:
		case TokenType::binaryIntegerWithType: {
			const TokenNumericWithIntTypeUnit numericIntUnit =
				natl::bitCast<TokenNumericWithIntTypeUnit, TokenUnit>(
					tokenUnits[getTokenUnitIndexAfterToken(tokenIndex)]);
			return numericIntUnit.size;
		}
		case TokenType::decimalFloatWithType:
		case TokenType::hexadecimalFloatWithType:
		case TokenType::binaryFloatWithType: {
			const TokenNumericWithFloatTypeUnit nuemricFloatUnit =
				natl::bitCast<TokenNumericWithFloatTypeUnit, TokenUnit>(
					tokenUnits[getTokenUnitIndexAfterToken(tokenIndex)]);
			return nuemricFloatUnit.size;
		}
		case TokenType::keywordI8:
			return 2; 
		case TokenType::keywordI16:
			return 3;
		case TokenType::keywordI32:
			return 3;
		case TokenType::keywordI64:
			return 3;
		case TokenType::keywordUI8:
			return 3;
		case TokenType::keywordUI16:
			return 4;
		case TokenType::keywordUI32:
			return 4;
		case TokenType::keywordUI64:
			return 4;
		case TokenType::keywordF32:
			return 3;
		case TokenType::keywordF64:
			return 3;
		case TokenType::keywordBool:
			return 4;
		case TokenType::keywordStr:
			return 3;
		case TokenType::keywordChar:
			return 4;
		case TokenType::keywordBlob:
			return 4;
		case TokenType::keywordFile:
			return 4;
		case TokenType::keywordOp:
			return 2;
		case TokenType::keywordEnum:
			return 4;
		case TokenType::keywordFarray:
			return 6;
		case TokenType::keywordArray:
			return 5;
		case TokenType::keywordDic:
			return 3;
		case TokenType::keywordStruct:
			return 6;
		case TokenType::keywordVariant:
			return 7;
		case TokenType::keywordTable:
			return 5;
		case TokenType::keywordIndex:
			return 5;
		case TokenType::keywordNull:
			return 4;
		case TokenType::keywordTrue:
			return 4;
		case TokenType::keywordFalse:
			return 5;
		default:
			natl::unreachable();
		}
	}

	constexpr natl::Size nextTokenIndex(
		const natl::Size tokenIndex, 
		const Token token,
		const natl::ConstArrayView<TokenUnit>& tokenUnits) {
		const natl::Size indexAfterToken = getTokenUnitIndexAfterToken(tokenIndex);

		natl::Size tokenIndexOffset = 0;
		switch (token.type) {
		case TokenType::leftCurly:
		case TokenType::leftSquare:
			tokenIndexOffset = sizeof(natl::ui64) * 2;
			break;
		case TokenType::dataStorage:
		case TokenType::stringLiteral:
			tokenIndexOffset = sizeof(natl::ui64) * 2; 
			break;
		case TokenType::unknown:
			tokenIndexOffset = sizeof(natl::ui64);
			break;
		case TokenType::decimalInteger:
		case TokenType::hexadecimalInteger:
		case TokenType::binaryInteger:
		case TokenType::decimalFloat:
		case TokenType::hexadecimalFloat:
		case TokenType::binaryFloat:
		case TokenType::decimalIntegerWithType:
		case TokenType::hexadecimalIntegerWithType:
		case TokenType::binaryIntegerWithType:
		case TokenType::decimalFloatWithType:
		case TokenType::hexadecimalFloatWithType:
		case TokenType::binaryFloatWithType:
			tokenIndexOffset = 1;
			break;
		case TokenType::rightCurly:
		case TokenType::rightSquare:
		case TokenType::length1CharLiteral:
		case TokenType::length2CharLiteral:
		case TokenType::start:
		case TokenType::end:
		case TokenType::colon:
		case TokenType::simicolon:
		case TokenType::comma:
		case TokenType::keywordI8:
		case TokenType::keywordI16:
		case TokenType::keywordI32:
		case TokenType::keywordI64:
		case TokenType::keywordUI8:
		case TokenType::keywordUI16:
		case TokenType::keywordUI32:
		case TokenType::keywordUI64:
		case TokenType::keywordF32:
		case TokenType::keywordF64:
		case TokenType::keywordBool:
		case TokenType::keywordStr:
		case TokenType::keywordChar:
		case TokenType::keywordBlob:
		case TokenType::keywordFile:
		case TokenType::keywordOp:
		case TokenType::keywordEnum:
		case TokenType::keywordFarray:
		case TokenType::keywordArray:
		case TokenType::keywordDic:
		case TokenType::keywordStruct:
		case TokenType::keywordVariant:
		case TokenType::keywordTable:
		case TokenType::keywordIndex:
		case TokenType::keywordNull:
		case TokenType::keywordTrue:
		case TokenType::keywordFalse:
			break;
		default:
			natl::unreachable();
		}
		return indexAfterToken + tokenIndexOffset;
	}

	constexpr natl::Size findTokenLineNumber(const natl::ui64 totalOffset, const natl::ConstArrayView<natl::ui64> newLineOffsets) noexcept {
		const natl::Size newlineIndex = natl::findLowerBoundIndex(totalOffset, newLineOffsets);
		return newlineIndex;
	}

	constexpr natl::ConstAsciiStringView getViewOfTokenSource(
		const Token token,
		const natl::ui64 tokenIndex, 
		const LexicalInfo& lexicalInfo) noexcept {
		const natl::Size size = getSizeOfToken(tokenIndex, token, lexicalInfo.tokenUnits);
		return natl::ConstAsciiStringView(lexicalInfo.source.subview(token.offset, size));
	}

	constexpr natl::Size findTokenColumnNumber(
		const natl::Size totalOffset,
		const natl::Size lineNumber,
		const natl::ConstArrayView<natl::ui64> newLineOffsets) noexcept {
		if (!natl::isInRange(newLineOffsets, lineNumber - 1)) {
			return totalOffset;
		}
		return totalOffset - newLineOffsets[lineNumber - 1];
	}

	template<typename DynStringContainer>
		requires(natl::IsConvertDynStringContainer<DynStringContainer>)
	constexpr void formatTokenAtToBack(DynStringContainer& outputDst, const natl::Size tokenIndex, const LexicalInfo& lexicalInfo) {
		const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);
		const natl::Size lineNumberIndex = findTokenLineNumber(token.offset, lexicalInfo.newLineOffsets.toArrayView());
		const natl::Size columnNumber = findTokenColumnNumber(token.offset, lineNumberIndex, lexicalInfo.newLineOffsets.toArrayView());
		natl::formatToBack(outputDst, "(line: ", lineNumberIndex + 1, ", column: ", columnNumber, ")");
		
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
		constexpr natl::Size maxValueLength = 100;
		switch (lexicalError.type) {
		break; case LexicalErrorType::unknownToken: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.unknownToken.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "unknown token at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::unknownIdentifer: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.unknownToken.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "unknown identifer at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::unknownLiteralExtention: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.unknownLiteralExtention.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "literal of type ",
				literalPreExtTypeToErrorString(lexicalError.errorVariant.unknownLiteralExtention.literalType),
				" with unknown literal extention at");

			const natl::ConstAsciiStringView tokenSource = getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength);
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", tokenSource,
				"with extenstion of ", tokenSource.substr(lexicalError.errorVariant.unknownLiteralExtention.extentionOffset));
			return true;
		}
		break; case LexicalErrorType::unknownLiteralPreExt: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.unknownLiteralPreExt.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);
			const natl::ConstAsciiStringView tokenSource = getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength);

			natl::formatToBack(outputDst, "unknown literal pre extention at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", tokenSource,
				"with extenstion of ", tokenSource.substr(lexicalError.errorVariant.unknownLiteralPreExt.literalPreExtEndIndex));
			return true;
		}
		break; case LexicalErrorType::expectedHashAtLiteralPreExtEnd: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.expectedHashAtLiteralPreExtEnd.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "expected hash at the literal pre end at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::expectedDigitsAfterLiteralExt: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.expectedDigitsAfterLiteralExt.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "expected digits after literal ext at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::expectedDigitsAfterFloatDot: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.expectedDigitsAfterFloatDot.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "expected digits after float dot from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.errorVariant.expectedDigitsAfterFloatDot.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::expectedDigitsAfterFloatExp: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.expectedDigitsAfterFloatExp.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "expected digits after float exp from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.errorVariant.expectedDigitsAfterFloatExp.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::expectedIdentiferAfterLiteralPostExtStart: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.expectedIdentiferAfterLiteralPostExtStart.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "expected identifer after literal post ext start from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.errorVariant.expectedIdentiferAfterLiteralPostExtStart.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::unknownLiteralPostExt: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.unknownLiteralPostExt.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);

			natl::formatToBack(outputDst, "unknown literal post ext \"",
				getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength).substr(lexicalError.errorVariant.unknownLiteralPostExt.literalPostExtStartOffset),
				"\" from literal of type ",
				literalPreExtTypeToErrorString(lexicalError.errorVariant.unknownLiteralPostExt.literalType), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::numericTooBig: {
			const natl::ui64 tokenIndex = lexicalError.errorVariant.numericTooBig.tokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, tokenIndex)) {
				return false;
			}
			const Token token = getTokenAt(tokenIndex, lexicalInfo.tokenUnits);
			natl::formatToBack(outputDst, "numeric too big at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, tokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " with value of ", getViewOfTokenSource(token, tokenIndex, lexicalInfo).substr(0, maxValueLength));
			return true;
		}
		break; case LexicalErrorType::nonMatchingBeginScope: {
			const natl::ui64 endScopeTokenIndex = lexicalError.errorVariant.nonMatchingBeingScope.endScopeTokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, endScopeTokenIndex)) {
				return false;
			}
			const Token endScopeToken = getTokenAt(endScopeTokenIndex, lexicalInfo.tokenUnits);

			const natl::ui64 wrongBeingScopeTokenIndex = lexicalError.errorVariant.nonMatchingBeingScope.wrongBeingScopeTokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, wrongBeingScopeTokenIndex)) {
				return false;
			}
			const Token wrongBeingScopeToken = getTokenAt(wrongBeingScopeTokenIndex, lexicalInfo.tokenUnits);
			natl::formatToBack(outputDst, "end scope token ",
				tokenTypeToDebugString(endScopeToken.type), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, endScopeTokenIndex, lexicalInfo);
			natl::formatToBack(outputDst,
				" had a nonmatching begin scope token ",
				tokenTypeToDebugString(wrongBeingScopeToken.type), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, wrongBeingScopeTokenIndex, lexicalInfo);
			return true;
		}
		break; case LexicalErrorType::noBeingScope: {
			const natl::ui64 endScopeTokenIndex = lexicalError.errorVariant.noBeginScope.endScopeTokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, endScopeTokenIndex)) {
				return false;
			}
			const Token endScopeToken = getTokenAt(endScopeTokenIndex, lexicalInfo.tokenUnits);
			natl::formatToBack(outputDst, "end scope token ",
				tokenTypeToDebugString(endScopeToken.type), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, endScopeTokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " does not have an associated being scope token");
			return true;
		}
		break; case LexicalErrorType::noEndScope: {
			const natl::ui64 beingScopeTokenIndex = lexicalError.errorVariant.noEndScope.beginScopeTokenIndex;
			if (not natl::isInRange(lexicalInfo.tokenUnits, beingScopeTokenIndex)) {
				return false;
			}
			const Token beingScopeToken = getTokenAt(beingScopeTokenIndex, lexicalInfo.tokenUnits);
			natl::formatToBack(outputDst, "begin scope token ",
				tokenTypeToDebugString(beingScopeToken.type), " at ");
			formatTokenAtToBack<DynStringContainer>(outputDst, beingScopeTokenIndex, lexicalInfo);
			natl::formatToBack(outputDst, " does not have an associated end scope token");
			return true;
		}
		break; default:
			natl::unreachable();
		}
		return false;
	}

	using TokenUnitSize = natl::ui64;

	struct TokenScopeUnit {
		natl::ui64 size;
		natl::ui64 elementCount;
	};

	using AdditionalTokenInfoVariant = natl::Variant<
		natl::NamedElement<"size", TokenUnitSize>,
		natl::NamedElement<"numeric", TokenNumericUnit>,
		natl::NamedElement<"numericWithIntType", TokenNumericWithIntTypeUnit>,
		natl::NamedElement<"numericWithFloatType", TokenNumericWithFloatTypeUnit>,
		natl::NamedElement<"storage", TokenStorageUnit>,
		natl::NamedElement<"scope", TokenScopeUnit>
	>;

	struct SerializeTokensFullInfo {};
}

template<> struct natl::Serialize<nadsad::ascii::TokenType> {
	using as_type = SerializeEnum<UnderlyingType<nadsad::ascii::TokenType>>;
	using type = nadsad::ascii::TokenType;
	template<typename Serializer> using error_type = void;

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const nadsad::ascii::TokenType tokenType) noexcept {
		serializer.template writeEnum<typename as_type::base_serialize_type>(natl::toUnderlying(tokenType), nadsad::ascii::tokenTypeToString(tokenType));
	}
};

template<> struct natl::Deserialize<nadsad::ascii::TokenType> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::TokenType>;
	using type = nadsad::ascii::TokenType;
	constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenType>::read";
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		auto stringToTokenTypeFunc = [](const ConstAsciiStringView& str) noexcept -> natl::Option<UnderlyingType<type>> {
			auto tokenTypeOption = nadsad::ascii::stringToTokenType(str);
			if (tokenTypeOption.hasValue()) {
				return natl::toUnderlying(tokenTypeOption.value());
			} else {
				return {};
			}
			};

		auto tokenTypeValueExpect = deserializer.readEnum(info, stringToTokenTypeFunc);
		if (tokenTypeValueExpect.hasValue()) {
			dst = natl::fromUnderlying<type>(tokenTypeValueExpect.value());
			return {};
		} else {
			return tokenTypeValueExpect.error();
		}
	}
};

template<> struct natl::Serialize<nadsad::ascii::NumericIntType> {
	using as_type = SerializeEnum<UnderlyingType<nadsad::ascii::NumericIntType>>;
	using type = nadsad::ascii::NumericIntType;
	template<typename Serializer> using error_type = void;

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type intType) noexcept {
		serializer.template writeEnum<typename as_type::base_serialize_type>(natl::toUnderlying(intType),
			nadsad::ascii::numericIntTypeToString(intType));
	}
};

template<> struct natl::Deserialize<nadsad::ascii::NumericIntType> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::NumericIntType>;
	using type = nadsad::ascii::NumericIntType;
	constexpr static natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::NumericIntType>::read";
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		auto stringToIntTypeFunc = [](const ConstAsciiStringView& str) noexcept -> natl::Option<UnderlyingType<type>> {
			auto intTypeOption = nadsad::ascii::stringToNumericIntType(str);
			if (intTypeOption.hasValue()) {
				return natl::toUnderlying<type>(intTypeOption.value());
			} else {
				return {};
			}
			};

		auto tokenTypeValueExpect = deserializer.readEnum(info, stringToIntTypeFunc);
		if (tokenTypeValueExpect.hasValue()) {
			dst = natl::fromUnderlying<type>(tokenTypeValueExpect.value());
			return {};
		} else {
			return tokenTypeValueExpect.error();
		}
	}
};

template<> struct natl::Serialize<nadsad::ascii::NumericFloatType> {
	using as_type = SerializeEnum<UnderlyingType<nadsad::ascii::NumericFloatType>>;
	using type = nadsad::ascii::NumericFloatType;
	template<typename Serializer> using error_type = void;

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type floatType) noexcept {
		serializer.template writeEnum<typename as_type::base_serialize_type>(natl::toUnderlying(floatType),
			nadsad::ascii::numericFloatTypeToString(floatType));
	}
};

template<> struct natl::Deserialize<nadsad::ascii::NumericFloatType> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::NumericFloatType>;
	using type = nadsad::ascii::NumericFloatType;
	constexpr static natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::NumericFloatType>::read";
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		auto stringToFloatTypeFunc = [](const ConstAsciiStringView& str) noexcept -> natl::Option<UnderlyingType<type>> {
			auto floatTypeOption = nadsad::ascii::stringToNumericFloatType(str);
			if (floatTypeOption.hasValue()) {
				return natl::toUnderlying<type>(floatTypeOption.value());
			} else {
				return {};
			}
			};

		auto tokenTypeValueExpect = deserializer.readEnum(info, stringToFloatTypeFunc);
		if (tokenTypeValueExpect.hasValue()) {
			dst = natl::fromUnderlying<type>(tokenTypeValueExpect.value());
			return {};
		} else {
			return tokenTypeValueExpect.error();
		}
	}
};


template<> struct natl::Serialize<nadsad::ascii::TokenNumericUnit> {
	using as_type = SerializeStruct<natl::ui8>;
	using type = nadsad::ascii::TokenNumericUnit;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "TokenNumericUnit";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type numericUnit) noexcept {
		using members = as_type::members_serialize_types;

		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer>(serializer, "size", numericUnit.size);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::TokenNumericUnit> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::TokenNumericUnit>;
	using type = nadsad::ascii::TokenNumericUnit;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenNumericUnit>::read";

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		using members = as_type::members_serialize_types;

		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<0>, Deserializer, natl::ui8>(
					deserializer, structInfo, "size", dst.size);
			}
		);
	}
};

template<> struct natl::Serialize<nadsad::ascii::TokenNumericWithIntTypeUnit> {
	using as_type = SerializeStruct<natl::ui8, nadsad::ascii::NumericIntType>;
	using type = nadsad::ascii::TokenNumericWithIntTypeUnit;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "TokenNumericWithIntTypeUnit";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type intUnit) noexcept {
		using members = as_type::members_serialize_types;

		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer>(serializer, "size", intUnit.size);
		serializeWriteNamedMatch<members::at<1>, Serializer>(serializer, "intType", intUnit.intType);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::TokenNumericWithIntTypeUnit> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::TokenNumericWithIntTypeUnit>;
	using type = nadsad::ascii::TokenNumericWithIntTypeUnit;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static inline natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenNumericWithIntTypeUnit>::read";

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info, type& dst) noexcept {
		using members = as_type::members_serialize_types;

		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) -> Option<error_type<Deserializer>> {
				auto valueExpect = natl::deserializeReadNamedMatch<members::at<0>, Deserializer, natl::ui8>(
					deserializer, structInfo, "size");

				if (valueExpect.hasValue()) {
					dst.size = valueExpect.value();
					return {};
				} else {
					return valueExpect.error();
				}
			}, [&](auto& deserializer, auto& structInfo) -> Option<error_type<Deserializer>> {
				auto valueExpect = natl::deserializeReadNamedMatch<members::at<1>, Deserializer, nadsad::ascii::NumericIntType>(
					deserializer, structInfo, "intType");
				if (valueExpect.hasValue()) {
					dst.intType = valueExpect.value();
					return {};
				} else {
					return valueExpect.error();
				}
			}
		);
	}
};

template<> struct natl::Serialize<nadsad::ascii::TokenNumericWithFloatTypeUnit> {
	using as_type = SerializeStruct<natl::ui8, nadsad::ascii::NumericFloatType>;
	using type = nadsad::ascii::TokenNumericWithFloatTypeUnit;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "TokenScopeUnit";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type floatUnit) noexcept {
		using members = as_type::members_serialize_types;

		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer>(serializer, "size", floatUnit.size);
		serializeWriteNamedMatch<members::at<1>, Serializer>(serializer, "floatType", floatUnit.floatType);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::TokenNumericWithFloatTypeUnit> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::TokenNumericWithFloatTypeUnit>;
	using type = nadsad::ascii::TokenNumericWithFloatTypeUnit;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static inline natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenNumericWithFloatTypeUnit>::read";

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info, type& dst) noexcept {
		using members = as_type::members_serialize_types;

		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) -> Option<error_type<Deserializer>> {
				auto valueExpect = natl::deserializeReadNamedMatch<members::at<0>, Deserializer, natl::ui8>(
					deserializer, structInfo, "size");

				if (valueExpect.hasValue()) {
					dst.size = valueExpect.value();
					return {};
				} else {
					return valueExpect.error();
				}
			}, [&](auto& deserializer, auto& structInfo) -> Option<error_type<Deserializer>> {
				auto valueExpect = natl::deserializeReadNamedMatch<members::at<1>, Deserializer, nadsad::ascii::NumericFloatType>(
					deserializer, structInfo, "floatType");
				if (valueExpect.hasValue()) {
					dst.floatType = valueExpect.value();
					return {};
				} else {
					return valueExpect.error();
				}
			}
		);
	}
};

template<> struct natl::Serialize<nadsad::ascii::TokenStorageUnit> {
	using as_type = SerializeStruct<natl::ui64, natl::ui64>;
	using type = nadsad::ascii::TokenStorageUnit;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "TokenScopeUnit";
	constexpr static inline natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenStorageUnit>::read";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type storageUnit) noexcept {
		using members = as_type::members_serialize_types;

		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer>(serializer, "size", storageUnit.size);
		serializeWriteNamedMatch<members::at<1>, Serializer>(serializer, "escapeCount", storageUnit.escapeCount);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::TokenStorageUnit> {
	using as_type = SerializeTypeOf<nadsad::ascii::TokenStorageUnit>;
	using type = nadsad::ascii::TokenStorageUnit;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenStorageUnit>::read";

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer, typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		using members = as_type::members_serialize_types;

		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<0>, Deserializer, natl::Size>(
					deserializer, structInfo, "size", dst.size);
			}, [&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<1>, Deserializer, natl::Size>(
					deserializer, structInfo, "escapeCount", dst.escapeCount);
				}
				);
	}
};


template<> struct natl::Serialize<nadsad::ascii::TokenScopeUnit> {
	using as_type = SerializeStruct<natl::ui64, natl::ui64>;
	using type = nadsad::ascii::TokenScopeUnit;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "TokenScopeUnit";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type scopeUnit) noexcept {
		using members = as_type::members_serialize_types;

		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer>(serializer, "size", scopeUnit.size);
		serializeWriteNamedMatch<members::at<1>, Serializer>(serializer, "elementCount", scopeUnit.elementCount);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::TokenScopeUnit> {
	using as_type = SerializeTypeOf<nadsad::ascii::TokenScopeUnit>;
	using type = nadsad::ascii::TokenScopeUnit;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static inline natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::TokenScopeUnit>::read";

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer, typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		using members = as_type::members_serialize_types;

		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<0>, Deserializer, natl::Size>(
					deserializer, structInfo, "size", dst.size);
			}, [&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<1>, Deserializer, natl::Size>(
					deserializer, structInfo, "elementCount", dst.elementCount);
				}
				);
	}
};

template<> struct natl::Serialize<nadsad::ascii::Token> {
	using as_type = natl::SerializeStruct<
		nadsad::ascii::TokenType, //type
		natl::Size, //lineNumber
		natl::Size, //columnNumber
		natl::Size, //offset
		natl::String, //value
		nadsad::ascii::AdditionalTokenInfoVariant //info
	>;
	using type = nadsad::ascii::Token;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "Token";

	template<typename Serializer>
	constexpr static void write(
		Serializer& serializer,
		const nadsad::ascii::Token& token,
		const natl::ui64& tokenIndex,
		const natl::ui64& tokenSize,
		const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
		using members = as_type::members_serialize_types;

		const natl::Size lineNumberIndex = nadsad::ascii::findTokenLineNumber(
			token.offset, lexicalInfo.newLineOffsets);
		const natl::Size columnNumber = nadsad::ascii::findTokenColumnNumber(
			token.offset, lineNumberIndex, lexicalInfo.newLineOffsets.toArrayView());
		const natl::Size lineNumber = lineNumberIndex + 1;

		const natl::ui64 infoIndex = nadsad::ascii::getTokenUnitIndexAfterToken(tokenIndex);
		nadsad::ascii::AdditionalTokenInfoVariant additionalTokenInfo;

		switch (token.type) {
		case nadsad::ascii::TokenType::leftCurly:
		case nadsad::ascii::TokenType::leftSquare:
			nadsad::ascii::TokenScopeUnit scopeUnit;
			scopeUnit.size = nadsad::ascii::getTokenUi64IntegerAt(infoIndex, lexicalInfo.tokenUnits);
			scopeUnit.elementCount = nadsad::ascii::getTokenUi64IntegerAt(infoIndex + sizeof(natl::ui64), lexicalInfo.tokenUnits);
			additionalTokenInfo.assign<"scope", nadsad::ascii::TokenScopeUnit>(scopeUnit);
			break;
		case nadsad::ascii::TokenType::dataStorage:
		case nadsad::ascii::TokenType::stringLiteral:
		{
			const nadsad::ascii::TokenStorageUnit storageUnit =
				nadsad::ascii::getTokenStorageUnitAt(infoIndex, lexicalInfo.tokenUnits);
			additionalTokenInfo.assign<"storage", nadsad::ascii::TokenStorageUnit>(storageUnit);
			break;
		}
		case nadsad::ascii::TokenType::unknown:
		{
			additionalTokenInfo.assign<"size", natl::Size>(tokenSize);
			break;
		}
		case nadsad::ascii::TokenType::decimalInteger:
		case nadsad::ascii::TokenType::hexadecimalInteger:
		case nadsad::ascii::TokenType::binaryInteger:
		case nadsad::ascii::TokenType::decimalFloat:
		case nadsad::ascii::TokenType::hexadecimalFloat:
		case nadsad::ascii::TokenType::binaryFloat:
		{
			const nadsad::ascii::TokenNumericUnit numericUnit =
				natl::bitCast<nadsad::ascii::TokenNumericUnit, nadsad::ascii::TokenUnit>(
					lexicalInfo.tokenUnits[infoIndex]);
			additionalTokenInfo.assign<"numeric", nadsad::ascii::TokenNumericUnit>(numericUnit);
			break;
		}
		case nadsad::ascii::TokenType::decimalIntegerWithType:
		case nadsad::ascii::TokenType::hexadecimalIntegerWithType:
		case nadsad::ascii::TokenType::binaryIntegerWithType:
		{
			const nadsad::ascii::TokenNumericWithIntTypeUnit intTypeUnit =
				natl::bitCast<nadsad::ascii::TokenNumericWithIntTypeUnit, nadsad::ascii::TokenUnit>(
					lexicalInfo.tokenUnits[infoIndex]);
			additionalTokenInfo.assign<"numericWithIntType", nadsad::ascii::TokenNumericWithIntTypeUnit>(intTypeUnit);
			break;
		}
		case nadsad::ascii::TokenType::decimalFloatWithType:
		case nadsad::ascii::TokenType::hexadecimalFloatWithType:
		case nadsad::ascii::TokenType::binaryFloatWithType:
		{
			const nadsad::ascii::TokenNumericWithFloatTypeUnit floatTypeUnit =
				natl::bitCast<nadsad::ascii::TokenNumericWithFloatTypeUnit, nadsad::ascii::TokenUnit>(
					lexicalInfo.tokenUnits[infoIndex]);
			additionalTokenInfo.assign<"numericWithFloatType", nadsad::ascii::TokenNumericWithFloatTypeUnit>(floatTypeUnit);
			break;
		}
		default:
			break;
		}

		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer, nadsad::ascii::TokenType>(serializer, "type", token.type);
		serializeWriteNamedMatch<members::at<1>, Serializer, natl::Size>(serializer, "offset", columnNumber);
		serializeWriteNamedMatch<members::at<2>, Serializer, natl::Size>(serializer, "lineNumber", lineNumber);
		serializeWriteNamedMatch<members::at<3>, Serializer, natl::Size>(serializer, "columnNumber", columnNumber);

		switch (token.type) {
		case nadsad::ascii::TokenType::start:
		case nadsad::ascii::TokenType::end:
			serializeWriteNamedMatch<members::at<4>, Serializer>(serializer, "value", "");
			break;
		default:
			natl::ConstAsciiStringView valueAsString(&lexicalInfo.source[token.offset], tokenSize);
			serializeWriteNamedMatch<members::at<4>, Serializer>(serializer, "value", valueAsString);
			break;
		}

		serializeWriteNamedMatch<members::at<5>, Serializer>(serializer, "info", additionalTokenInfo);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::Token> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::Token>;
	using type = nadsad::ascii::Token;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static inline natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::Token>::read";


	template<typename Deserializer, typename TokenUnitsDstDynArrayType>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			nadsad::ascii::Token& dst,
			TokenUnitsDstDynArrayType& dsTokenUnits) noexcept {
		using members = as_type::members_serialize_types;

		nadsad::ascii::TokenType type;
		natl::ui64 offset;
		[[maybe_unused]] natl::ui64 lineNumber;
		[[maybe_unused]] natl::ui64 columnNumber;
		[[maybe_unused]] natl::String256 value;
		nadsad::ascii::AdditionalTokenInfoVariant tokenInfo;

		auto error = natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<0>, Deserializer, nadsad::ascii::TokenType>(
					deserializer, structInfo, "type", type);
			}, [&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<1>, Deserializer, natl::Size>(
					deserializer, structInfo, "offset", offset);
				}, [&](auto& deserializer, auto& structInfo) {
					return natl::deserializeReadNamedToDstMatch<members::at<2>, Deserializer, natl::Size>(
						deserializer, structInfo, "lineNumber", lineNumber);
					}, [&](auto& deserializer, auto& structInfo) {
						return natl::deserializeReadNamedToDstMatch<members::at<3>, Deserializer, natl::Size>(
							deserializer, structInfo, "columnNumber", columnNumber);
						}, [&](auto& deserializer, auto& structInfo) {
							return natl::deserializeReadNamedToDstMatch<members::at<4>, Deserializer, decltype(value)>(
								deserializer, structInfo, "value", value);
							}, [&](auto& deserializer, auto& structInfo) {
								return natl::deserializeReadNamedToDstMatch<members::at<5>, Deserializer, nadsad::ascii::AdditionalTokenInfoVariant>(
									deserializer, structInfo, "info", tokenInfo);
								}
								);

		if (error.hasValue()) {
			return error.value().addSource(sourceName, "");
		}

		auto addUI64 = [&](const natl::ui64 value) {
			dsTokenUnits.resize(dsTokenUnits.size() + 8);
			natl::ui64ToBytesAt(value, dsTokenUnits.last(8));
			};

		//TODO
		dst.type = type;
		dst.offset = offset;

		dsTokenUnits.pushBack(
			natl::bitCast<nadsad::ascii::TokenUnit>(type));
		addUI64(offset);

		if (tokenInfo.doesNotHaveValue()) {
			return {};
		}

		switch (tokenInfo.getIndex()) {
		break; case tokenInfo.getIndexOf<"size">():
		{
			nadsad::ascii::TokenUnitSize value = tokenInfo.get<"size">();
			addUI64(value);
		}
		break; case tokenInfo.getIndexOf<"numeric">():
		{
			nadsad::ascii::TokenNumericUnit value = tokenInfo.get<"numeric">();
			dsTokenUnits.pushBack(natl::bitCast<nadsad::ascii::TokenUnit>(value));
		}
		break; case tokenInfo.getIndexOf<"numericWithIntType">():
		{
			nadsad::ascii::TokenNumericWithIntTypeUnit value = tokenInfo.get<"numericWithIntType">();
			dsTokenUnits.pushBack(natl::bitCast<nadsad::ascii::TokenUnit>(value));
		}
		break; case tokenInfo.getIndexOf<"numericWithFloatType">():
		{
			nadsad::ascii::TokenNumericWithFloatTypeUnit value = tokenInfo.get<"numericWithFloatType">();
			dsTokenUnits.pushBack(natl::bitCast<nadsad::ascii::TokenUnit>(value));
		}
		break; case tokenInfo.getIndexOf<"storage">():
		{
			nadsad::ascii::TokenStorageUnit value = tokenInfo.get<"storage">();
			addUI64(value.size);
			addUI64(value.escapeCount);
		}
		break; case tokenInfo.getIndexOf<"scope">():
		{
			nadsad::ascii::TokenScopeUnit value = tokenInfo.get<"scope">();
			addUI64(value.size);
			addUI64(value.elementCount);
		}
		break; default:
			natl::unreachable();
		}

		return{};
	}
};


template<> struct natl::Serialize<nadsad::ascii::LexicalError> {
	using as_type = natl::SerializeStruct<natl::String>;
	using type = nadsad::ascii::LexicalError;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "LexicalError";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalError& lexicalError, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
		serializer.beginWriteStruct();
		natl::String errorMessage;
		if (nadsad::ascii::lexicalErrorToMessage(errorMessage, lexicalError, lexicalInfo)) {
			serializeWriteNamed<Serializer>(serializer, "message", errorMessage.toStringView());
			natl::println(errorMessage.toStringView());
		} else {
			serializeWriteNamed<Serializer>(serializer, "message", "failed to format error");
		}
		serializer.endWriteStruct();

	}
};

template<> struct natl::Deserialize<nadsad::ascii::LexicalError> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::LexicalError>;
	using type = nadsad::ascii::LexicalError;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = RegularSerializeName<type>;
	constexpr static inline ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::LexicalError>::read";

	template<typename Deserializer>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		using members = as_type::members_serialize_types;

		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) {
				[[maybe_unused]] natl::String message;
				return natl::deserializeReadNamedToDstMatch<members::at<0>, Deserializer, natl::String>(
					deserializer, structInfo, "message", message);
			}
		);

		return {};
	}
};

template<> struct natl::Serialize<nadsad::ascii::SerializeTokensFullInfo> {
	using as_type = natl::SerializeArray<nadsad::ascii::Token>;
	using type = nadsad::ascii::SerializeTokensFullInfo;
	template<typename Serializer> using error_type = void;

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const type&, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
		if (lexicalInfo.tokenUnits.isEmpty()) {
			serializer.writeEmptyArray();
		} else {
			serializer.beginWriteArray();

			natl::Size tokenIndex = 0;
			while (tokenIndex < lexicalInfo.tokenUnits.size()) {
				const nadsad::ascii::Token token = nadsad::ascii::getTokenAt(tokenIndex, lexicalInfo.tokenUnits);
				const natl::Size tokenSize = nadsad::ascii::getSizeOfToken(tokenIndex, token, lexicalInfo.tokenUnits);

				serializer.beginWriteArrayElement();
				serializeWrite<Serializer>(serializer, token, tokenIndex, tokenSize, lexicalInfo);
				serializer.endWriteArrayElement();

				tokenIndex = nadsad::ascii::nextTokenIndex(tokenIndex, token, lexicalInfo.tokenUnits);
			}

			serializer.endWriteArray();
		}
	}

};

template<> struct natl::Deserialize<nadsad::ascii::SerializeTokensFullInfo> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::SerializeTokensFullInfo>;
	using type = nadsad::ascii::SerializeTokensFullInfo;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::SerializeTokensFullInfo>::read";

	template<typename Deserializer, typename TokenUnitsDstDynArrayType>
	constexpr static natl::Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type&, TokenUnitsDstDynArrayType& dsTokenUnits) noexcept {

		auto arraySizeExpect = deserializer.beginReadArray(info);
		if (arraySizeExpect.hasError()) {
			return arraySizeExpect.error().addSource(sourceName, "");
		}
		natl::Size arraySize = arraySizeExpect.value();

		if (arraySize == 0) {
			auto endArrayError = deserializer.endReadEmptyArray(info);
			if (endArrayError.hasValue()) {
				return endArrayError.value().addSource(sourceName, "");
			}
			return {};
		}

		for (natl::Size i = 0; i < arraySize; i++) {
			auto arrayElementExpect = deserializer.beginReadArrayElement(info);
			if (arrayElementExpect.hasError()) {
				return arrayElementExpect.error().addSource(sourceName, "");
			}
			auto arrayElement = arrayElementExpect.value();

			auto valueError = deserializeReadMatch<as_type::element_serialize_type, Deserializer, nadsad::ascii::Token>(
				deserializer, arrayElement, dsTokenUnits);
			if (valueError.hasError()) {
				return valueError.error().addSource(sourceName, "");
			}

			auto arrayElementEndError = deserializer.endReadArrayElement(arrayElement);
			if (arrayElementEndError.hasValue()) {
				return arrayElementEndError.value().addSource(sourceName, "");
			}
		}

		auto endArrayError = deserializer.endReadArray(info);
		if (endArrayError.hasValue()) {
			return endArrayError.value().addSource(sourceName, "");
		}
		return {};
	}
};

template<> struct natl::Serialize<nadsad::ascii::LexicalInfo> {
	using as_type = natl::SerializeStruct<
		natl::String,
		natl::ArrayView<natl::ui64>,
		natl::ArrayView<nadsad::ascii::LexicalError>,
		natl::ArrayView<nadsad::ascii::Token>
	>;
	using type = nadsad::ascii::LexicalInfo;
	template<typename Serializer> using error_type = void;
	constexpr static inline natl::ConstAsciiStringView name = "LexicalInfo";

	template<typename Serializer>
	constexpr static void write(Serializer& serializer, const nadsad::ascii::LexicalInfo& lexicalInfo) noexcept {
		using members = as_type::members_serialize_types;
		serializer.beginWriteStruct();
		serializeWriteNamedMatch<members::at<0>, Serializer>(serializer, "source", ConstAsciiStringView(lexicalInfo.source));
		serializeWriteNamedMatch<members::at<1>, Serializer>(serializer, "newlineOffsets", lexicalInfo.newLineOffsets.toArrayView());
		serializeWriteNamedMatch<members::at<2>, Serializer>(serializer, "errors", lexicalInfo.errors.toArrayView(), lexicalInfo);
		serializeWriteNamedMatch<members::at<3>, Serializer>(serializer, "tokens", nadsad::ascii::SerializeTokensFullInfo{}, lexicalInfo);
		serializer.endWriteStruct();
	}
};

template<> struct natl::Deserialize<nadsad::ascii::LexicalInfo> {
	using as_type = natl::SerializeTypeOf<nadsad::ascii::LexicalInfo>;
	using type = nadsad::ascii::LexicalInfo;
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;
	constexpr static inline natl::ConstAsciiStringView name = natl::RegularSerializeName<type>;
	constexpr static inline natl::ConstAsciiStringView sourceName = "natl::Deserialize<nadsad::ascii::LexicalInfo>::read";

	template<typename Deserializer, typename SourceDstType>
	constexpr static natl::Option<error_type<Deserializer>> read(Deserializer& deserializer,
		typename Deserializer::template deserialize_info<as_type>& info,
		type& dst, SourceDstType& sourceDst) noexcept {
		using members = as_type::members_serialize_types;
		return natl::deserializeReadStruct<error_type<Deserializer>, Deserializer>(
			sourceName, deserializer, info,
			[&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<0>, Deserializer, SourceDstType>(
					deserializer, structInfo, "source", sourceDst);
			}, [&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<1>, Deserializer, decltype(dst.newLineOffsets)>(
					deserializer, structInfo, "newlineOffsets", dst.newLineOffsets);
			}, [&](auto& deserializer, auto& structInfo) {
				return natl::deserializeReadNamedToDstMatch<members::at<2>, Deserializer, decltype(dst.errors)>(
					deserializer, structInfo, "errors", dst.errors);
			}, [&](auto& deserializer, auto& structInfo) {
				nadsad::ascii::SerializeTokensFullInfo dummy;
				return natl::deserializeReadNamedToDstMatch<members::at<3>, Deserializer, nadsad::ascii::SerializeTokensFullInfo>(
					deserializer, structInfo, "tokens", dummy, dst.tokenUnits);
			}
		);
	}
};