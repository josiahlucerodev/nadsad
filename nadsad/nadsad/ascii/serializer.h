
#pragma once 

//natl
#include <natl/serialization.h>
#include <natl/util/enum.h>

//interface 
namespace nadsad::ascii {
	namespace impl {
		template<typename OutputDstType>
		constexpr void serializeWriteCharLiteral(OutputDstType& output, const natl::Ascii character) noexcept {
			output += '\'';
			switch (character) {
			break; case '\a': output += "\\a";
			break; case '\b': output += "\\b";
			break; case '\f': output += "\\f";
			break; case '\n': output += "\\n";
			break; case '\r': output += "\\r";
			break; case '\t': output += "\\t";
			break; case '\v': output += "\\v";
			break; case '\\': output += "\\\\";
			break; case '\"': output += "\\\"";
			break; case '\'': output += "\\\'";
			break; default: output += character;
			}
			output += '\'';
		}
		template<typename OutputDstType>
		constexpr void serializeWriteStringLiteral(OutputDstType& output, const natl::ConstAsciiStringView str) noexcept {
			output += '\"';

			for (const natl::Ascii character : str) {
				switch (character) {
				break; case '\a': output += "\\a";
				break; case '\b': output += "\\b";
				break; case '\f': output += "\\f";
				break; case '\n': output += "\\n";
				break; case '\r': output += "\\r";
				break; case '\t': output += "\\t";
				break; case '\v': output += "\\v";
				break; case '\\': output += "\\\\";
				break; case '\"': output += "\\\"";
				break; case '\'': output += "\\\'";
				break; default: output += character;
				}
			}

			output += '\"';
		}

		template<natl::Bool FullTypes, typename SerializeType>
		struct SerializeTypeToString;

		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeI8> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i8";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeI16> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i16";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeI32> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i32";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeI64> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "i64";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeUI8> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui8";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeUI16> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui16";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeUI32> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui32";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeUI64> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "ui64";
			}
		};

		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeF32> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "f32";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeF64> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "f64";
			}
		};

		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeBool> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "bool";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeChar> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "char";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeStr> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "str";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeFile> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "file";
			}
		};
		template<natl::Bool FullTypes> struct SerializeTypeToString<FullTypes, natl::SerializeBlob> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "blob";
			}
		};
		template<natl::Bool FullTypes, typename SerializeType>
		struct SerializeTypeToString<FullTypes, natl::SerializeOptional<SerializeType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "op ";
				SerializeTypeToString<FullTypes, SerializeType>::template write<OutputDstType>(output);
			}
		};
		template<natl::Bool FullTypes, typename BaseElementType>
		struct SerializeTypeToString<FullTypes, natl::SerializeEnum<BaseElementType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "enum ";
				SerializeTypeToString<FullTypes, BaseElementType>::template write<OutputDstType>(output);
			}
		};
		template<natl::Bool FullTypes, typename ElementType, natl::Size Number>
		struct SerializeTypeToString<FullTypes, natl::SerializeFixedArray<ElementType, Number>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "farray";
				output += "[";
				output += natl::intToStringDecimal(Number);
				output += ",";
				SerializeTypeToString<FullTypes, ElementType>::template write<OutputDstType>(output);
				output += "]";
			}
		};
		template<natl::Bool FullTypes, typename ElementType>
		struct SerializeTypeToString<FullTypes, natl::SerializeArray<ElementType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "array";
				output += "[";
				SerializeTypeToString<FullTypes, ElementType>::template write<OutputDstType>(output);
				output += "]";
			}
		};
		template<natl::Bool FullTypes, typename KeyType, typename ValueType>
		struct SerializeTypeToString<FullTypes, natl::SerializeDic<KeyType, ValueType>> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "dic";
				output += "{";
				SerializeTypeToString<FullTypes, KeyType>::template write<OutputDstType>(output);
				output += ",";
				SerializeTypeToString<FullTypes, ValueType>::template write<OutputDstType>(output);
				output += "}";
			}
		};
		template<natl::Bool FullTypes, typename... MemberTypes>
		struct SerializeTypeToString<FullTypes, natl::SerializeStruct<MemberTypes...>> {
			template<typename OutputDstType, typename MemberType, natl::Size Index>
			constexpr static void writeMember(OutputDstType& output) noexcept {
				if constexpr (Index != 0) {
					output += ", ";
				}
				SerializeTypeToString<FullTypes, MemberType>::template write<OutputDstType>(output);
			}

			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "struct{";
				[&] <natl::Size... Indices>(natl::IndexSequence<Indices...>) noexcept {
					(writeMember<OutputDstType, MemberTypes, Indices>(output), ...);
				}(natl::MakeIndexSequence<sizeof...(MemberTypes)>{});
				output += "}";
			}
		};

		template<natl::Bool FullTypes, typename IndexSerializeType, typename... Types>
		struct SerializeTypeToString<FullTypes, natl::SerializeVariant<IndexSerializeType, Types...>> {
			template<typename OutputDstType, typename Type, natl::Size Index>
			constexpr static void writeType(OutputDstType& output) noexcept {
				if constexpr (Index != 0) {
					output += ", ";
				}
				SerializeTypeToString<FullTypes, Type>::template write<OutputDstType>(output);
			}
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				output += "variant ";
				SerializeTypeToString<FullTypes, IndexSerializeType>::template write<OutputDstType>(output);
				output += " {";
				[&] <natl::Size... Indices>(natl::IndexSequence<Indices...>) noexcept {
					(writeType<OutputDstType, Types, Indices>(output), ...);
				}(natl::MakeIndexSequence<sizeof...(Types)>{});
				output += "}";
			}
		};

		template<natl::Bool FullTypes, typename Type>
			requires(natl::IsSerializableC<Type>)
		struct SerializeTypeToString<FullTypes, Type> : SerializeTypeToString<FullTypes, natl::SerializeTypeOf<Type>> {};

		template<typename Type>
			requires(natl::IsSerializableC<Type>&& natl::HasSerializeNameC<Type>)
		struct SerializeTypeToString<false, Type> {
			template<typename OutputDstType>
			constexpr static void write(OutputDstType& output) noexcept {
				serializeWriteStringLiteral<OutputDstType>(output, natl::SerializeName<Type>);
			}
		};
	}

	template<natl::Bool FullTypes, typename SerializeType, typename OutputDstType>
		requires(natl::IsSerializeTypeC<SerializeType> || natl::IsSerializableC<SerializeType>)
	constexpr void serializeTypeToString(OutputDstType& output) noexcept {
		impl::SerializeTypeToString<FullTypes, SerializeType>::write(output);
	}

	enum class WriteFlag {
		none = 0,
		fullname,
	};

	template<
		natl::Size smallBufferByteSize = 1024,
		natl::SerializeFlags serializeFlags = natl::SerializeFlags::pretty,
		typename SerializeErrorHandler = natl::DummySerializeErrorHandler,
		typename Alloc = natl::DefaultAllocator>
		requires(natl::IsAllocatorC<Alloc> && natl::IsSerializeErrorHandlerC<SerializeErrorHandler>)
	struct Serializer {
	public:
		using allocator_type = Alloc;
		using code_point_type = natl::Ascii;
		using container_type = natl::AsciiStringByteSize<smallBufferByteSize, allocator_type>;
		using custom_write_flag_type = WriteFlag;
		using serialize_error_handler = SerializeErrorHandler;
		constexpr static custom_write_flag_type defaultCustomWriteFlag = WriteFlag::none;
		constexpr static inline natl::Size smallBufferSize = smallBufferByteSize;
		constexpr static inline natl::SerializeFlags flags = serializeFlags;
		constexpr static inline natl::Bool fullTypes = natl::bitFlagTest(serializeFlags & natl::SerializeFlags::fullTypes);

	private:
		container_type storage;
		natl::Size indentValue = 0;

	public:
		constexpr Serializer() noexcept {
			indentValue = 0;
		}

		[[nodiscard]] constexpr natl::ArrayView<code_point_type> output() noexcept {
			return storage.toArrayView();
		}

	private:
		constexpr void newLine() noexcept {
			if constexpr (natl::bitFlagTest(serializeFlags & natl::SerializeFlags::pretty)) {
				storage += '\n';
			}
		}
		constexpr void space() noexcept {
			if constexpr (natl::bitFlagTest(serializeFlags & natl::SerializeFlags::pretty)) {
				storage += ' ';
			}
		}
		constexpr void indent() noexcept {
			if constexpr (natl::bitFlagTest(serializeFlags & natl::SerializeFlags::pretty)) {
				for (natl::Size i = 0; i < indentValue; i++) {
					storage += '\t';
				}
			}
		}
		constexpr void increaseIndent() noexcept {
			if constexpr (natl::bitFlagTest(serializeFlags & natl::SerializeFlags::pretty)) {
				indentValue += 1;
			}
		}
		constexpr void decreaseIndent() noexcept {
			if constexpr (natl::bitFlagTest(serializeFlags & natl::SerializeFlags::pretty)) {
				indentValue -= 1;
			}
		}
		constexpr void writeStrLiteral(const natl::ConstAsciiStringView str) noexcept {
			impl::serializeWriteStringLiteral(storage, str);
		}

	public:
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> begin() noexcept {
			return natl::OptionEmpty{};
		}
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> end() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename SerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteNamed(const natl::ConstAsciiStringView name) noexcept {
			indent();
			writeStrLiteral(name);
			storage += ' ';
			serializeTypeToString<fullTypes, SerializeType, container_type>(storage);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteNamed() noexcept {
			storage += ',';
			newLine();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeValue() noexcept {
			storage += ':';
			space();
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI8(const natl::i8 value) noexcept {
			natl::intToStringDecimal<container_type, natl::i8>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI16(const natl::i16 value) noexcept {
			natl::intToStringDecimal<container_type, natl::i16>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI32(const natl::i32 value) noexcept {
			natl::intToStringDecimal<container_type, natl::i32>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI64(const natl::i64 value) noexcept {
			natl::intToStringDecimal<container_type, natl::i64>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI8(const natl::ui8 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui8>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI16(const natl::ui16 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui16>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI32(const natl::ui32 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui32>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI64(const natl::ui64 value) noexcept {
			natl::intToStringDecimal<container_type, natl::ui64>(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF32(const natl::f32 value) noexcept {
			natl::floatToStringDecimal<container_type, natl::f32>(storage, value, 8);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF64(const natl::f64 value) noexcept {
			natl::floatToStringDecimal<container_type, natl::f64>(storage, value, 18);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBool(const natl::Bool value) noexcept {
			if (value) {
				storage += "true";
			} else {
				storage += "false";
			}
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeChar(const natl::Char value) noexcept {
			impl::serializeWriteCharLiteral(storage, value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::Ascii* str, const natl::Size size) noexcept {
			writeStr<Flags, CustomFlags, SerializeComponentType >(natl::ConstAsciiStringView(str, size));
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::ConstAsciiStringView str) noexcept {
			impl::serializeWriteStringLiteral(storage, str);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeFile(const natl::ConstAsciiStringView fileName, const natl::ConstArrayView<natl::Byte> data) noexcept {
			writeStrLiteral(fileName);
			writeBlob(data);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBlob(const natl::ConstArrayView<natl::Byte> data) noexcept {
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
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename BaseSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsEnumBaseSerializeTypeC<BaseSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEnum(
			const natl::BasicSerializeTypeToType<BaseSerializeType> intValue,
			const natl::ConstAsciiStringView& strValue) noexcept {
			storage += '\"';
			storage += strValue;
			storage += '\"';
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteOptional() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteOptional() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyOptional() noexcept {
			storage += "null";
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, natl::Size>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArray() noexcept {
			storage += '[';
			newLine();
			increaseIndent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, natl::Size>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArray() noexcept {
			decreaseIndent();
			indent();
			storage += ']';
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArrayElement() noexcept {
			indent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArrayElement() noexcept {
			storage += ',';
			newLine();
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyArray() noexcept {
			storage += "[]";
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArray(natl::Size) noexcept {
			storage += '[';
			newLine();
			increaseIndent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArray() noexcept {
			decreaseIndent();
			indent();
			storage += ']';
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArrayElement() noexcept {
			indent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArrayElement() noexcept {
			storage += ',';
			newLine();
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyDic() noexcept {
			storage += "{}";
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDic(natl::Size) noexcept {
			storage += '{';
			newLine();
			increaseIndent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDic() noexcept {
			decreaseIndent();
			indent();
			storage += '}';
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDicElement() noexcept {
			indent();
			storage += '{';
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDicElement() noexcept {
			storage += "},";
			newLine();
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeDicKeyComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicKey() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeDicValueComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicValue() noexcept {
			space();
			storage += ':';
			space();
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteStruct() noexcept {
			storage += '{';
			newLine();
			increaseIndent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteStruct() noexcept {
			decreaseIndent();
			indent();
			storage += '}';
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyVariant() noexcept {
			storage += "null";
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType, natl::Size Index>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteVariant(const natl::ConstAsciiStringView& strValue) noexcept {
			storage += "{";
			writeStrLiteral(strValue);
			storage += ",";
			space();
			serializeTypeToString<fullTypes, typename VariantSerializeType::types::template at<Index>, container_type>(storage);
			storage += '}';

			newLine();
			indent();
			storage += "{";

			newLine();
			increaseIndent();
			indent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteVariant() noexcept {
			newLine();
			decreaseIndent();
			indent();
			storage += '}';
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IndexIDSizeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> 
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInIntegerC<IndexIDSizeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteJumpTable(const natl::ConstAsciiStringView name) noexcept {
			storage += "jtable ";
			serializeTypeToString<fullTypes, IndexIDSizeType, container_type>(storage);
			space();
			storage += '{';
			newLine();
			increaseIndent();
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeGlobalComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteTable() noexcept {
			decreaseIndent();
			indent();
			storage += '}';
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpSaveId, SerializeErrorHandler> writeJumpTableElement(const natl::SerializeJumpId<IdNumberType>& element) noexcept {
			indent();
			writeStrLiteral(element.name);
			storage += ':';

			const natl::SerializeJumpSaveId saveId(storage.size());
			for (natl::Size i : natl::Repeat(20)) {
				storage += '\0';
			}
			storage += ",";

			newLine();
			return saveId;
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> 
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpLocation, SerializeErrorHandler> writeJumpLocation(const natl::SerializeJumpId<IdNumberType>& element,
			const natl::SerializeJumpSaveId& saveId) noexcept {
			indent();
			natl::SerializeJumpLocation jumpLocation(storage.size());
			storage += "jindex ";
			storage += " ";
			writeStrLiteral(element.name);
			newLine();
			return jumpLocation;
		}
	};
}

static_assert(
	natl::IsFullSerializerC<nadsad::ascii::Serializer<1000, natl::SerializeFlags::pretty, natl::DummySerializeErrorHandler>>,
	"nadsad: nadsad::ascii::Serializer is not a full natl::serializer");