#pragma once 

//natl
#include <natl/processing/serialization.h>

//interface 
namespace nadsad::ascii {
	namespace impl {
		template<typename SerializeType>
		struct SerializeTypeToString;

		template<typename Type>
			requires(natl::IsSerializableC<Type>)
		struct SerializeTypeToString<Type> : SerializeTypeToString<natl::SerializeTypeOf<Type>> {};

		template<> struct SerializeTypeToString<natl::SerializeI8> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i8";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeI16> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i16";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeI32> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i32";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeI64> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i64";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeUI8> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui8";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeUI16> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui16";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeUI32> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui32";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeUI64> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui64";
			}
		};

		template<> struct SerializeTypeToString<natl::SerializeF32> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "f32";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeF64> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "f64";
			}
		};

		template<> struct SerializeTypeToString<natl::SerializeBool> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "bool";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeChar> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "char";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeStr> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "str";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeFile> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "file";
			}
		};
		template<> struct SerializeTypeToString<natl::SerializeBlob> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "blob";
			}
		};
		template<typename SerializeType> struct SerializeTypeToString<natl::SerializeOptional<SerializeType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "op ";
				SerializeTypeToString<SerializeType>::template write<OutputDstType>(output);
			}
		};
		template<typename BaseElementType> struct SerializeTypeToString<natl::SerializeEnum<BaseElementType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "enum ";
				SerializeTypeToString<BaseElementType>::template write<OutputDstType>(output);
			}
		};
		template<typename ElementType, natl::Size Number> struct SerializeTypeToString<natl::SerializeFixedArray<ElementType, Number>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "farray";
				output += "[";
				SerializeTypeToString<ElementType>::template write<OutputDstType>(output);
				output += ",";
				output += natl::intToStringDecimal(Number);
				output += "]";
			}
		};
		template<typename ElementType> struct SerializeTypeToString<natl::SerializeArray<ElementType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "array";
				output += "[";
				SerializeTypeToString<ElementType>::template write<OutputDstType>(output);
				output += "]";
			}
		};
		template<typename KeyType, typename ValueType> struct SerializeTypeToString<natl::SerializeDic<KeyType, ValueType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "dic";
				output += "{";
				SerializeTypeToString<KeyType>::template write<OutputDstType>(output);
				output += ",";
				SerializeTypeToString<ValueType>::template write<OutputDstType>(output);
				output += "}";
			}
		};
		template<typename... MemberTypes> struct SerializeTypeToString<natl::SerializeStruct<MemberTypes...>> {
			template<typename OutputDstType, typename MemberType, natl::Size Index>
			constexpr static void writeMember(OutputDstType& output) noexcept {
				if constexpr (Index != 0) {
					output += ", ";
				}
				SerializeTypeToString<MemberType>::template write<OutputDstType>(output);
			}

			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) {
				output += "struct{";
				[&] <natl::Size... Indices>(natl::IndexSequence<Indices...>) noexcept {
					(writeMember<OutputDstType, MemberTypes, Indices>(output), ...);
				}(natl::MakeIndexSequence<sizeof...(MemberTypes)>{});
				output += "}";
			}
		};

		template<typename IndexSerializeType, typename... Types> struct SerializeTypeToString<natl::SerializeVariant<IndexSerializeType, Types...>> {
			template<typename OutputDstType, typename Type, natl::Size Index>
			constexpr static void writeType(OutputDstType& output) noexcept {
				if constexpr (Index != 0) {
					output += ", ";
				}
				SerializeTypeToString<Type>::template write<OutputDstType>(output);
			}
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) {
				output += "variant ";
				SerializeTypeToString<IndexSerializeType>::template write<OutputDstType>(output);
				output += " {";
				[&] <natl::Size... Indices>(natl::IndexSequence<Indices...>) noexcept {
					(writeType<OutputDstType, Types, Indices>(output), ...);
				}(natl::MakeIndexSequence<sizeof...(Types)>{});
				output += "}";
			}
		};
	}

	template<typename SerializeType, typename OutputDstType>
		requires(natl::IsSerializeTypeC<SerializeType> || natl::IsSerializableC<SerializeType>)
	constexpr void serializeTypeToString(OutputDstType& output) noexcept {
		impl::SerializeTypeToString<SerializeType>::write(output);
	}


	template<
		natl::Size smallBufferByteSize,
		natl::SerializeFlag SerializeFlag = natl::SerializeFlag::pretty,
		typename Alloc = natl::DefaultAllocator<natl::Ascii>>
		requires(natl::IsAllocator<Alloc>)
	struct Serializer {
		using allocator_type = Alloc::template rebind_alloc<natl::Ascii>;
		using code_point_type = natl::Ascii;
		using container_type = natl::AsciiStringByteSize<smallBufferByteSize, allocator_type>;

		constexpr static inline natl::Size smallBufferSize = smallBufferByteSize;
		constexpr static inline natl::SerializeFlag flag = SerializeFlag;
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
		constexpr void begin() noexcept {}
		constexpr void end() noexcept {}

		template<typename IndexIDSizeType>
			requires(natl::IsBuiltInIntegerC<IndexIDSizeType>)
		constexpr void beginWriteTable(const natl::Size size) noexcept {
			storage += "table ";
			serializeTypeToString<IndexIDSizeType, container_type>(storage);
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
		template<typename IndexIDSizeType>
			requires(natl::IsBuiltInIntegerC<IndexIDSizeType>)
		constexpr natl::ui64 writeTableElement(const natl::ConstAsciiStringView name, const IndexIDSizeType id) noexcept {
			indent();
			writeStr(name);
			storage += ',';
			newLine();
			return 0;
		}

		template<typename IndexIDSizeType>
			requires(natl::IsBuiltInIntegerC<IndexIDSizeType>)
		constexpr void saveIndex(const IndexIDSizeType indexId, const natl::ConstAsciiStringView name) noexcept {
			indent();
			storage += "index ";
			serializeTypeToString<IndexIDSizeType, container_type>(storage);
			storage += " ";
			natl::intToStringDecimal(storage, indexId);
			writeStr(name);
			storage += ';';
			newLine();
			;
		}
	public:
		template<typename SerializeType>
		constexpr void beginWrite(const natl::ConstAsciiStringView name) noexcept {
			indent();
			writeStr(name);
			storage += ' ';
			serializeTypeToString<SerializeType, container_type>(storage);
		}
		constexpr void endWrite() noexcept {
			storage += ',';
			newLine();
		}
		constexpr void writeValue() noexcept {
			storage += ':';
			space();
		}

		constexpr void writeNull() noexcept {
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
		constexpr void writeBool(const natl::Bool value) noexcept {
			if (value) {
				storage += "true";
			} else {
				storage += "false";
			}

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

		constexpr void writeFile(const natl::ConstAsciiStringView fileName, const natl::ConstArrayView<natl::Byte> data) noexcept {
			writeStr(fileName);
			writeBlob(data);
		}

		constexpr void writeBlob(const natl::ConstArrayView<natl::Byte> data) noexcept {
			storage += '(';

			for (const natl::Byte part : data) {
				switch (static_cast<natl::ui8>(part)) {
				break; case 255:
					storage += static_cast<natl::Ascii>(255);
					storage += static_cast<natl::Ascii>(255);
				break; case static_cast<natl::ui8>(')'):
					storage += static_cast<natl::Ascii>(255);
					storage += ')';
				break; default:
					storage += part;
					break;
				}
			}

			storage += ')';
			newLine();
		}
		template<typename BaseSerializeType>
			requires(natl::IsEnumBaseSerializeTypeC<BaseSerializeType>)
		constexpr void writeEnum(
			const natl::BasicSerializeTypeToType<BaseSerializeType> intValue, 
			const natl::ConstAsciiStringView& strValue) noexcept {
			storage += '\"';
			storage += strValue;
			storage += '\"';
		}

		constexpr void beginWriteFixedArray() noexcept {
			storage += '[';
			newLine();
			increaseIndent();
		}
		constexpr void endWriteFixedArray() noexcept {
			decreaseIndent();
			indent();
			storage += ']';
		}
		constexpr void beginWriteFixedArrayElement() noexcept {
			indent();
		}
		constexpr void endWriteFixedArrayElement() noexcept {
			storage += ',';
			newLine();
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
		constexpr void beginWriteDic() noexcept {
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

		template<typename VariantSerializeType>
			requires(natl::IsSerializeVariantC<VariantSerializeType>)
		constexpr void writeEmptyVariant() noexcept {
			storage += "null";
		}

		template<typename VariantSerializeType, natl::Size Index>
			requires(natl::IsSerializeVariantC<VariantSerializeType>)
		constexpr void beginWriteVariant(const natl::ConstAsciiStringView& strValue) noexcept {
			storage += "variant{";
			serializeTypeToString<typename VariantSerializeType::index_type>(storage);
			storage += " ";
			writeStr(strValue);
			storage += ",";
			space();
			serializeTypeToString<typename VariantSerializeType::types::template at<Index>>(storage);
			storage += '}';

			newLine();
			indent();
			storage += "{";

			newLine();
			increaseIndent();
			indent();
		}
		constexpr void endWriteVariant() noexcept {
			newLine();
			decreaseIndent();
			indent();
			storage += '}';
		}
	};
}

static_assert(natl::IsFullSerializer<nadsad::ascii::Serializer<1000>>, "nadsad: nadsad::ascii::Serializer is not a full serializer");