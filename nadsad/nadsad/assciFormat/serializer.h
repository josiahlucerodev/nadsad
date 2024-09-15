#pragma once 

//natl
#include <natl/processing/serialization.h>

//interface 
namespace nadsad::ascii {
	template<
		natl::Size smallBufferByteSize, 
		natl::SerializeFlag SerializeFlag = natl::SerializeFlag::pretty,
		typename Alloc = natl::DefaultAllocator<natl::Ascii>>
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
			if constexpr (SerializeFlag == natl::SerializeFlag::pretty) {
				storage += '\n';
			}
		}
		constexpr void space() noexcept {
			if constexpr (SerializeFlag == natl::SerializeFlag::pretty) {
				storage += ' ';
			}
		}
		constexpr void indent() noexcept {
			if constexpr (SerializeFlag == natl::SerializeFlag::pretty) {
				for (natl::Size i = 0; i < indentValue; i++) {
					storage += '\t';
				}
			}
		}
		constexpr void increaseIndent() noexcept {
			if constexpr (SerializeFlag == natl::SerializeFlag::pretty) {
				indentValue += 1;
			}
		}
		constexpr void decreaseIndent() noexcept {
			if constexpr (SerializeFlag == natl::SerializeFlag::pretty) {
				indentValue -= 1;
			}
		}
	public:
		constexpr void beginWriteTable(const natl::Size size) noexcept {
			storage += "table ";
			newLine();
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
		constexpr natl::ui64 writeTableElement(const natl::ConstAsciiStringView name, const natl::ui64 id) noexcept {
			indent();
			writeStr(name);
			storage += ',';
			newLine();
		}
		constexpr void saveIndex(const natl::ui64 indexId, const natl::ConstAsciiStringView name) noexcept {
			indent();
			natl::intToStringDecimal(storage, indexId);
			writeStr(name);
			storage += ';';
			newLine();
;		}
		//write

	private:
		constexpr void beginWriteName(const natl::ConstAsciiStringView name) noexcept {
			writeStr(name);
			storage += ' ';
		}
		template<natl::SerializeID ID>
		constexpr void beginWriteID() noexcept {
			if constexpr ((ID & natl::SerializeID::name) != natl::SerializeID::none) {
				storage += "nameid ";
			}
			if constexpr ((ID & natl::SerializeID::type) != natl::SerializeID::none) {
				storage += "typeid ";
			}
		}
		constexpr void beginWriteNameTag(const natl::SerializeNameTag& serializeNameTag) noexcept {
			storage += "nametag ";
			writeStr(serializeNameTag.name);
			storage += ',';
			space();
		}
		template<typename SerializeNumberTagType>
			requires(natl::IsSerializeNumberTagC<SerializeNumberTagType>)
		constexpr void beginWriteNumberTag(const SerializeNumberTagType& serializeNumberTag) noexcept {
			storage += "numbertag ";
			storage += AsTypeToStringV<typename SerializeNumberTagType::number_type>::value.c_str();
			storage += " ";
			storage += natl::intToStringDecimal(serializeNumberTag.number);
			storage += ',';
			space();
		}
	public:
		constexpr void beginWrite(const natl::ConstAsciiStringView name) noexcept {
			indent();
			beginWriteName(name);
		}
		template<natl::SerializeID ID>
		constexpr void beginWrite(const natl::ConstAsciiStringView name) noexcept {
			indent();
			beginWriteID<ID>();
			beginWriteName(name);
		}
		template<natl::SerializeID ID>
		constexpr void beginWrite(const natl::ConstAsciiStringView name, const natl::SerializeNameTag& serializeNameTag) noexcept {
			indent();
			beginWriteNameTag(serializeNameTag);
			beginWriteID<ID>();
			beginWriteName(name);
		}
		template<natl::SerializeID ID, typename SerializeNumberTagType>
			requires(natl::IsSerializeNumberTagC<SerializeNumberTagType>)
		constexpr void beginWrite(const natl::ConstAsciiStringView name, const SerializeNumberTagType& serializeNumberTag) noexcept {
			indent();
			beginWriteNumberTag<SerializeNumberTagType>(serializeNumberTag);
			beginWriteID<ID>();
			beginWriteName(name);
		}
		template<natl::SerializeID ID, typename SerializeNumberTagType>
			requires(natl::IsSerializeNumberTagC<SerializeNumberTagType>)
		constexpr void beginWrite(const natl::ConstAsciiStringView name, const natl::SerializeNameTag& serializeNameTag, const SerializeNumberTagType& serializeNumberTag) noexcept {
			indent();
			beginWriteNameTag(serializeNameTag);
			beginWriteNumberTag<SerializeNumberTagType>(serializeNumberTag);
			beginWriteID<ID>();
			beginWriteName(name);
		}


		constexpr void endWrite() noexcept {
			storage += ',';
			newLine();
		}
		constexpr void writeValue() noexcept {
			storage += ':';
			space();
		}

		//as
	private:
		template<typename SerializeType>
		struct AsTypeToStringV;
		template<typename SerializeType>
		constexpr static natl::TemplateStringLiteral AsTypeToString = AsTypeToStringV<SerializeType>::value;

		template<> struct AsTypeToStringV<natl::i8> { 
			constexpr static natl::TemplateStringLiteral value = "i8";
		};
		template<> struct AsTypeToStringV<natl::i16> { 
			constexpr static natl::TemplateStringLiteral value = "i16";
		};
		template<> struct AsTypeToStringV<natl::i32> { 
			constexpr static natl::TemplateStringLiteral value = "i32";
		};
		template<> struct AsTypeToStringV<natl::i64> { 
			constexpr static natl::TemplateStringLiteral value = "i64";
		};
		template<> struct AsTypeToStringV<natl::ui8> { 
			constexpr static natl::TemplateStringLiteral value = "ui8";
		};
		template<> struct AsTypeToStringV<natl::ui16> { 
			constexpr static natl::TemplateStringLiteral value = "ui16";
		};
		template<> struct AsTypeToStringV<natl::ui32> { 
			constexpr static natl::TemplateStringLiteral value = "ui32";
		};
		template<> struct AsTypeToStringV<natl::ui64> { 
			constexpr static natl::TemplateStringLiteral value = "i64";
		};
		
		template<> struct AsTypeToStringV<natl::f32> { 
			constexpr static natl::TemplateStringLiteral value = "f32";
		};
		template<> struct AsTypeToStringV<natl::f64> { 
			constexpr static natl::TemplateStringLiteral value = "f64";
		};

		template<> struct AsTypeToStringV<natl::SerializeCharType> { 
			constexpr static natl::TemplateStringLiteral value = "char";
		};
		template<> struct AsTypeToStringV<natl::SerializeStrType> {
			constexpr static natl::TemplateStringLiteral value = "str";
		};
		template<> struct AsTypeToStringV<natl::SerializeFlagType> {
			constexpr static natl::TemplateStringLiteral value = "flag";
		};
		template<> struct AsTypeToStringV<natl::SerializeStructType> {
			constexpr static natl::TemplateStringLiteral value = "struct";
		};
		template<> struct AsTypeToStringV<natl::SerializeFileType> {
			constexpr static natl::TemplateStringLiteral value = "file";
		};
		template<> struct AsTypeToStringV<natl::SerializeBlobType> {
			constexpr static natl::TemplateStringLiteral value = "blob";
		};
		template<typename SerializeType> struct AsTypeToStringV<natl::SerializeOptionalType<SerializeType>> {
			constexpr static natl::TemplateStringLiteral value = natl::StringLiteral<"op ">::Concat<AsTypeToString<SerializeType>>;
		};
		template<typename ElementType> struct AsTypeToStringV<natl::SerializeArrayType<ElementType>> {
			constexpr static natl::TemplateStringLiteral value = natl::StringLiteral<"array[">::Concat<AsTypeToString<ElementType>, "]">;
		};
		template<typename KeyType, typename ValueType> struct AsTypeToStringV<natl::SerializeDicType<KeyType, ValueType>> {
			constexpr static natl::TemplateStringLiteral value = 
				natl::StringLiteral<"dic{">::Concat<AsTypeToString<KeyType>, ",", AsTypeToString<ValueType>, "}">;
		};

		template<typename IntegerType> struct AsTypeToStringV<natl::SerializeNumberTag<IntegerType>> {
			constexpr static natl::TemplateStringLiteral value = "blob";
		};

	public:
		template<typename SerializeType, natl::SerializeID ID = natl::SerializeID::none>
			requires(natl::IsSerializableC<SerializeType>)
		constexpr void as() noexcept {
			storage += AsTypeToStringV<SerializeType>::value.c_str();
		}

		//write
		constexpr void writeNull(const natl::i8 value) noexcept {
			storage += "null";
		}
		constexpr void writeI8(const natl::i8 value) noexcept { 
			natl::intToStringDecimal<container_type, natl::i8>(storage, value);
		}
		constexpr void writeI16(const natl::i16 value) noexcept { 
			natl::intToStringDecimal<container_type, natl::i16>(storage, value);
		}
		constexpr void writeI32(const natl::i32 value) noexcept { 
			natl::intToStringDecimal<container_type, natl::i32>(storage, value);
		}
		constexpr void writeI64(const natl::i64 value) noexcept { 
			natl::intToStringDecimal<container_type, natl::i64>(storage, value);
		}
		constexpr void writeUI8(const natl::ui8 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui8>(storage, value);
		}
		constexpr void writeUI16(const natl::ui16 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui16>(storage, value);
		}
		constexpr void writeUI32(const natl::ui32 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui32>(storage, value);
		}
		constexpr void writeUI64(const natl::ui64 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui64>(storage, value);
		}
		constexpr void writeF32(const natl::f32 value) noexcept {
			natl::floatToStringDecimal<container_type, natl::f32>(storage, value);
		}
		constexpr void writeF64(const natl::f64 value) noexcept {
			natl::floatToStringDecimal<container_type, natl::f32>(storage, value);
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
			natl::intToStringDecimal<container_type, natl::Size>(storage, value);
		}
		template<typename Functor>
			requires(natl::IsToFlagConvertFunctor<Functor>)
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
