#pragma once

//natl
#include <natl/serialization.h>

//interface 
namespace nadsad::binary {
	/*
	* assumes:
	* - little endian 
	* - f32 is IEEE 754 32-bit floating point (single precision)
	* - f64 is IEEE 754 64-bit floating point (double precision)
	*/
	template<
		natl::Size smallBufferByteSize = 1024,
		natl::SerializeFlags serializeFlags = natl::SerializeFlags::pretty,
		typename SerializeErrorHandler = natl::DummySerializeErrorHandler,
		typename Alloc = natl::DefaultAllocator>
		requires(natl::IsAllocatorC<Alloc>)
	struct UnalignedSerializer {
	public:
		using allocator_type = Alloc;
		using code_point_type = natl::Byte;
		using container_type = natl::SmallDynArray<code_point_type, smallBufferByteSize, Alloc>;
		using custom_write_flag_type = natl::SerializeWriteFlag;
		using serialize_error_handler = SerializeErrorHandler;
		constexpr static custom_write_flag_type defaultCustomWriteFlag = natl::SerializeWriteFlag::none;
		constexpr static inline natl::Size smallBufferSize = smallBufferByteSize;
		constexpr static inline natl::SerializeFlags flag = serializeFlags;
		using container_size_type = natl::Size;

	private:
		container_type storage;
		natl::Size activeJumpTableSize;
		natl::Size activeJumpTableLocation;

	public:
		template<typename IntegerType>
			requires(natl::IsBuiltInIntegerC<IntegerType>)
		constexpr void writeBytes(const IntegerType value) noexcept {
			const natl::Size oldSize = storage.size();
			storage.resize(storage.size() + sizeof(IntegerType));
			natl::ArrayView<natl::Byte> valueDst = storage.subview(oldSize);
			natl::intToBytesAt<IntegerType>(value, valueDst);
		}
		template<typename ByteLikeType>
			requires(sizeof(ByteLikeType) == 1)
		constexpr void writeBytes(const ByteLikeType* src, const natl::Size size) noexcept {
			const natl::Size oldSize = storage.size();
			storage.resize(storage.size() + size);
			const natl::ArrayView<natl::Byte> dst = storage.subview(oldSize);
			for (natl::Size i = 0; i < size; i++) {
				dst[i] = natl::bitCast<natl::Byte, ByteLikeType>(src[i]);
			}
		}
		
	public:
		[[nodiscard]] constexpr natl::ArrayView<code_point_type> output() noexcept {
			return storage.toArrayView();
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> begin() noexcept {
			return natl::OptionEmpty{};
		}
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> end() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteNamed(const natl::ConstAsciiStringView name) noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteNamed() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeValue() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI8(const natl::i8 value) noexcept {
			writeBytes<natl::i8>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI16(const natl::i16 value) noexcept {
			writeBytes<natl::i16>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI32(const natl::i32 value) noexcept {
			writeBytes<natl::i32>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI64(const natl::i64 value) noexcept {
			writeBytes<natl::i64>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI8(const natl::ui8 value) noexcept {
			writeBytes<natl::ui8>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI16(const natl::ui16 value) noexcept {
			writeBytes<natl::ui16>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI32(const natl::ui32 value) noexcept {
			writeBytes<natl::ui32>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI64(const natl::ui64 value) noexcept {
			writeBytes<natl::ui64>(value);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF32(const natl::f32 value) noexcept {
			writeBytes<natl::ui32>(natl::bitCast<natl::ui32>(value));
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF64(const natl::f64 value) noexcept {
			writeBytes<natl::ui64>(natl::bitCast<natl::ui64>(value));
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBool(const natl::Bool value) noexcept {
			writeBytes<natl::ui8>(natl::bitCast<natl::ui8>(value));
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeChar(const natl::Char value) noexcept {
			writeBytes<natl::ui8>(natl::bitCast<natl::ui8>(value));
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::Ascii* str, const natl::Size size) noexcept {
			writeBytes<container_size_type>(size);
			writeBytes<natl::Ascii>(str, size);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::ConstAsciiStringView str) noexcept {
			writeBytes<container_size_type>(str.size());
			writeBytes<natl::Ascii>(str.data(), str.size());
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeFile(const natl::ConstAsciiStringView fileName, const natl::ConstArrayView<natl::Byte> data) noexcept {
			writeBytes<container_size_type>(fileName.size());
			writeBytes<natl::Ascii>(fileName.data(), fileName.size());
			writeBytes<container_size_type>(data.size());
			writeBytes<natl::Ascii>(data.data(), data.size());
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBlob(const natl::ConstArrayView<natl::Byte> data) noexcept {
			writeBytes<container_size_type>(data.size());
			writeBytes<natl::Byte>(data.data(), data.size());
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, typename BaseSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsEnumBaseSerializeTypeC<BaseSerializeType>
				&& natl::IsBuiltInIntegerC<natl::BasicSerializeTypeToType<BaseSerializeType>>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEnum(
			const natl::BasicSerializeTypeToType<BaseSerializeType> intValue,
			const natl::ConstAsciiStringView&) noexcept {
			using integer_type = natl::BasicSerializeTypeToType<BaseSerializeType>;
			writeBytes<integer_type>(intValue);
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteOptional() noexcept {
			writeBytes<natl::ui8>(1);
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
			writeBytes<natl::ui8>(0);
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, natl::Size FarraySize>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArray() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, natl::Size FarraySize>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArray() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArrayElement() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArrayElement() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyArray() noexcept {
			writeBytes<container_size_type>(0);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArray(const natl::Size size) noexcept {
			writeBytes<container_size_type>(size);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArray() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArrayElement() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArrayElement() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyDic() noexcept {
			writeBytes<container_size_type>(0);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDic(natl::Size size) noexcept {
			writeBytes<container_size_type>(size);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDic() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDicElement() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDicElement() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeDicKeyComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicKey() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeDicValueComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicValue() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteStruct() noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteStruct() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyVariant() noexcept {
			using index_type = VariantSerializeType::index_type;
			writeBytes<index_type>(0);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType, natl::Size Index>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteVariant(const natl::ConstAsciiStringView&) noexcept {
			using index_type = VariantSerializeType::index_type;
			writeBytes<index_type>(Index + 1);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteVariant() noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteJumpTable(const natl::ConstAsciiStringView name) noexcept {
			activeJumpTableSize = 0;
			activeJumpTableLocation = storage.size();
			writeBytes<container_size_type>(0);
			return natl::OptionEmpty{};
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeGlobalComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteTable() noexcept {
			const natl::ArrayView<natl::Byte> valueDst = storage.subview(activeJumpTableLocation, sizeof(container_size_type));
			natl::intToBytesAt<container_size_type>(activeJumpTableSize, valueDst);
			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpSaveId, SerializeErrorHandler> writeJumpTableElement(const natl::SerializeJumpId<IdNumberType>& element) noexcept {
			writeBytes<IdNumberType>(element.number);
			writeBytes<container_size_type>(0);
			natl::SerializeJumpSaveId saveId = natl::SerializeJumpSaveId(activeJumpTableSize);
			activeJumpTableSize++;
			return saveId;
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpLocation, SerializeErrorHandler> writeJumpLocation(const natl::SerializeJumpId<IdNumberType>& element,
				const natl::SerializeJumpSaveId& saveId) noexcept {
			const natl::Size saveLocation = activeJumpTableLocation //base
				+ sizeof(natl::Size) //jump table size 
				+ ((sizeof(IdNumberType) + sizeof(container_size_type)) /*size of jump table element*/ * saveId.value());
				+ sizeof(IdNumberType); //offset of jump table element id 

			const natl::SerializeJumpLocation jumpLocation = natl::SerializeJumpLocation(storage.size());
			const natl::ArrayView<natl::Byte> valueDst = storage.subview(saveLocation, sizeof(container_size_type));
			natl::intToBytesAt<container_size_type>(jumpLocation.value(), valueDst);
			return jumpLocation;
		}
	};
}