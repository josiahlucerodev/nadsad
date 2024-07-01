#pragma once 

//natl
#include <natl/processing/serialization.h>

//interface 
namespace nadsad::ascii {
	

	template<
		natl::Size smallBufferByteSize, 
		natls::Flag SerializationFlag = natls::Flag::pretty,
		typename Alloc = natl::DefaultAllocator<natl::Ascii>
	>
		requires(natl::IsAllocator<Alloc>)
	struct Serializer {
		using allocator_type = Alloc::template rebind_alloc<natl::Ascii>;
		using code_point_type = natl::Ascii;
		using container_type = natl::AsciiStringByteSize<smallBufferByteSize, allocator_type>;
		using mapping_allocator_type = Alloc::template rebind_alloc<natl::String32>;
		using mapping_container_type = natl::SmallDynArray<natl::String32, (smallBufferByteSize / 100 + 10), mapping_allocator_type>;
	private:
		container_type storage;
		natl::Size indentValue = 0;
	public:
		constexpr Serializer() noexcept {
			indentValue = 0;
		}

		constexpr natl::ConstAsciiStringView output() noexcept {
			return storage.toStringView();
		}
		constexpr void newLine() noexcept {
			if constexpr (SerializationFlag == natls::Flag::pretty) {
				storage += '\n';
			}
		}
		constexpr void space() noexcept {
			if constexpr (SerializationFlag == natls::Flag::pretty) {
				storage += ' ';
			}
		}
		constexpr void indent() noexcept {
			if constexpr (SerializationFlag == natls::Flag::pretty) {
				for (natl::Size i = 0; i < indentValue; i++) {
					storage += '\t';
				}
			}
		}
		constexpr void increaseIndent() noexcept {
			if constexpr (SerializationFlag == natls::Flag::pretty) {
				indentValue += 1;
			}
		}
		constexpr void decreaseIndent() noexcept {
			if constexpr (SerializationFlag == natls::Flag::pretty) {
				indentValue -= 1;
			}
		}
	public:
		constexpr void writeTable(const natl::Size size) noexcept {
			storage += "table ";
			newLine();
		}

		constexpr void beginWriteNamedTable(const natl::Size size) noexcept {
			indent();
			storage += "ntable"; 
			space();
			storage += '{';
			newLine();
			increaseIndent();
		}
		constexpr void endWriteTable() noexcept {
			decreaseIndent();
			indent();
			storage += '}';
		}
		constexpr void writeNamedTableElement(const natl::ConstAsciiStringView name) noexcept {
			indent();
			writeStr(name);
			storage += ',';
			newLine();
		}

		//write
		constexpr void beginWrite(const natl::ConstAsciiStringView name) noexcept {
			indent();
			writeStr(name);
			storage += ' ';
		}
		constexpr void endWrite() noexcept {
			storage += ',';
			newLine();
		}

		//as
		constexpr void asOptional() noexcept { storage += "op "; }
		constexpr void asI8() noexcept { storage += "i8:"; space(); }
		constexpr void asI16() noexcept { storage += "i16:"; space(); }
		constexpr void asI32() noexcept { storage += "i32:"; space(); }
		constexpr void asI64() noexcept { storage += "i64:"; space(); }
		constexpr void asUI8() noexcept { storage += "ui8:"; space(); }
		constexpr void asUI16() noexcept { storage += "ui16:"; space(); }
		constexpr void asUI32() noexcept { storage += "ui32:"; space(); }
		constexpr void asUI64() noexcept { storage += "ui64:"; space(); }
		constexpr void asF32() noexcept { storage += "f32:"; space(); }
		constexpr void asF64() noexcept { storage += "f64:"; space(); }
		constexpr void asChar() noexcept { storage += "char:"; space(); }
		constexpr void asStr() noexcept { storage += "str:"; space(); }
		constexpr void asFlag() noexcept { storage += "flag:"; space(); }
		constexpr void asArrayOf(const natls::BasicDataType type, const natl::Size count) noexcept {
			storage.reserve(storage.size() + (count * 16));
			storage += "array ";
			storage += natls::basicDataTypeToString(type);
			storage += ':';
			space();
		}
		constexpr void asDicOf(const natls::BasicDataType key, const natls::BasicDataType value, const natl::Size count) noexcept {
			storage.reserve(storage.size() + (count * 16));
			storage += "dic ";
			storage += natls::basicDataTypeToString(key);
			storage += ' ';
			storage += natls::basicDataTypeToString(value);
			storage += ':';
			space();
		}
		constexpr void asStruct() noexcept { storage += "struct:"; space(); }
		constexpr void asFile() noexcept { storage += "file:"; space(); }
		constexpr void asBlob() noexcept { storage += "blob:"; space(); }

		//write
		constexpr void writeNull(const natl::i8 value) noexcept {
			storage += "null";
		}
		constexpr void writeI8(const natl::i8 value) noexcept { 
			natl::intToStringDecimalStringType<container_type, natl::i8>(storage, value);
		}
		constexpr void writeI16(const natl::i16 value) noexcept { 
			natl::intToStringDecimalStringType<container_type, natl::i16>(storage, value);
		}
		constexpr void writeI32(const natl::i32 value) noexcept { 
			natl::intToStringDecimalStringType<container_type, natl::i32>(storage, value);
		}
		constexpr void writeI64(const natl::i64 value) noexcept { 
			natl::intToStringDecimalStringType<container_type, natl::i64>(storage, value);
		}
		constexpr void writeUI8(const natl::ui8 value) noexcept {
			natl::intToStringDecimalStringType<container_type, natl::ui8>(storage, value);
		}
		constexpr void writeUI16(const natl::ui16 value) noexcept {
			natl::intToStringDecimalStringType<container_type, natl::ui16>(storage, value);
		}
		constexpr void writeUI32(const natl::ui32 value) noexcept {
			natl::intToStringDecimalStringType<container_type, natl::ui32>(storage, value);
		}
		constexpr void writeUI64(const natl::ui64 value) noexcept {
			natl::intToStringDecimalStringType<container_type, natl::ui64>(storage, value);
		}
		constexpr void writeF32(const natl::f32 value) noexcept {
			natl::floatToStringStringType<container_type, natl::f32>(storage, value);
		}
		constexpr void writeF64(const natl::f64 value) noexcept {
			natl::floatToStringStringType<container_type, natl::f32>(storage, value);
		}
		constexpr void writeChar(const natl::Char value) noexcept { 
			storage += '\'';
			switch (value) {
				break; case '\a': storage += "\\a";
				break; case '\b': storage += "\\b";
				break; case '\f': storage += "\\f";
				break; case '\n': storage += "\\n";
				break; case '\r': storage += "\\r";
				break; case '\t': storage += "\\t";
				break; case '\v': storage += "\\v";
				break; case '\\': storage += "\\\\";
				break; case '\"': storage += "\\\"";
				break; case '\'': storage += "\\\'";
				break; default: storage += value;
			}
			storage += '\'';
		}
		constexpr void writeStr(const natl::Ascii* str, const natl::Size size) noexcept {
			writeStr(natl::ConstAsciiStringView(str, size));
		}
		constexpr void writeStr(const natl::ConstAsciiStringView str) noexcept { 
			storage += '\"';

			for (const natl::Ascii character : str) {
				switch (character) {
				break; case '\a': storage += "\\a";
				break; case '\b': storage += "\\b";
				break; case '\f': storage += "\\f";
				break; case '\n': storage += "\\n";
				break; case '\r': storage += "\\r";
				break; case '\t': storage += "\\t";
				break; case '\v': storage += "\\v";
				break; case '\\': storage += "\\\\";
				break; case '\"': storage += "\\\"";
				break; case '\'': storage += "\\\'";
				break; default: storage += character;
				}
			}

			storage += '\"';
		}

		constexpr void writeFlag(const natl::Size value) noexcept {
			natl::intToStringDecimalStringType<container_type, natl::Size>(storage, value);
		}
		template<typename Functor>
			requires(natls::IsToFlagConvertFunctor<Functor>)
		constexpr void writeFlag(const natl::Size value, Functor&& toFlagFunction) noexcept {
			storage += '\"';
			storage += toFlagFunction(value);
			storage += '\"';
		}

		constexpr void writeEmptyArray() noexcept {
			storage += "[]";
		}
		constexpr void beginWriteArray() noexcept {
			storage += '[';
			newLine();
			increaseIndent();
		}
		constexpr void endWriteArray() noexcept {
			decreaseIndent();
			indent();
			storage += ']';
		}
		constexpr void beginWriteArrayElement() noexcept {
			indent();
		}
		constexpr void endWriteArrayElement() noexcept { 
			storage += ',';
			newLine();
		}

		constexpr void writeEmptyDic() noexcept {
			storage += "{}";
		}
		constexpr void beginWritDic() noexcept {
			storage += '{';
			newLine();
			increaseIndent();
		}
		constexpr void endWriteDic() noexcept {
			decreaseIndent();
			indent();
			storage += '}';
		}

		constexpr void beginWriteDicElement() noexcept {
			indent();
		}
		constexpr void endWriteDicElement() noexcept {
			storage += ',';
			newLine();
		}

		constexpr void writeDicKey() noexcept {}
		constexpr void writeDicValue() noexcept {
			storage += ':';
			space();
		}


		constexpr void beginWriteStruct() noexcept {
			storage += '{';
			newLine();
			increaseIndent();
		}
		constexpr void endWriteStruct() noexcept {
			decreaseIndent();
			indent();
			storage += '}';
		}

		constexpr void writeFileName(const natl::ConstAsciiStringView fileName) noexcept {
			writeStr(fileName);
		}
		constexpr void writeFileContent(const natl::ArrayView<const natl::Byte> data) noexcept {
			writeBlob(data);
		}

		constexpr void writeBlob(const natl::ArrayView<const natl::Byte> data) noexcept {
			storage += '(';

			for (const natl::Byte part : data) {
				switch (static_cast<natl::ui8>(part)) {
				case 255:
					storage += static_cast<natl::Ascii>(255);
					storage += static_cast<natl::Ascii>(255);
				case static_cast<natl::ui8>(')'):
					storage += static_cast<natl::Ascii>(255);
					storage += ')';
				default:
					storage += part;
					break;
				}
			}

			storage += ')';
			newLine();
		}
	};
	//serilie()
}
