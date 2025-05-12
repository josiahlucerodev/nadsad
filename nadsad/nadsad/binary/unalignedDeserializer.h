#pragma once

//interface 
namespace nadsad::binary {
	/*
	* assumes:
	* - little endian
	* - f32 is IEEE 754 32-bit floating point (single precision)
	* - f64 is IEEE 754 64-bit floating point (double precision)
	*/
	template<natl::DeserializeFlags deserializeFlags = natl::DeserializeFlags::shortenError,
		template<typename> typename DeserializeInfo = natl::DummyDeserializeElementInfo,
		typename DeserializeErrorHandler = natl::DummyDeserializeErrorHandler,
		typename Alloc = natl::DefaultAllocator>
	struct UnalignedDeserializer {
	public:
		using allocator_type = Alloc;
		using code_point_type = natl::Byte;
		template<typename SerializeType>
		using deserialize_info = DeserializeInfo<SerializeType>;
		using deserialize_error_handler = DeserializeErrorHandler;
		using error_message_string_type = natl::String256;
		using custom_read_flag_type = natl::CustomDeserializerReadFlagNone;
		constexpr static inline natl::DeserializeFlags flags = deserializeFlags;
		constexpr static inline custom_read_flag_type defaultCustomReadFlag = custom_read_flag_type::none;
		
	private:
		using container_size_type = natl::Size;

	private:
		natl::ConstArrayView<natl::Byte> source;
		natl::Size sourceIndex;
		natl::Size variantIndex;

	public:
		//constructor 
		constexpr UnalignedDeserializer() noexcept : source(), sourceIndex(0), variantIndex(0) {}

		//util
		constexpr UnalignedDeserializer& self() noexcept { return *this; };
		constexpr const UnalignedDeserializer& self() const noexcept { return *this; };

		template<typename IntegerType>
			requires(natl::IsBuiltInIntegerC<IntegerType>)
		[[nodiscard]] constexpr IntegerType readBytes() noexcept {
			const IntegerType value = natl::bytesToInt<IntegerType>(source.subview(sourceIndex, sizeof(IntegerType)));
			sourceIndex += sizeof(IntegerType);
			return value;
		}
		[[nodiscard]] constexpr natl::Bool hasEnoughSpaceFor(const natl::Size size) noexcept {
			return sourceIndex + size <= source.size();
		};
		template<typename Type>
		[[nodiscard]] constexpr natl::Bool hasEnoughSpaceFor() noexcept {
			return sourceIndex + sizeof(Type) <= source.size();
		};
		template<typename Type>
		[[nodiscard]] constexpr natl::Bool hasEnoughSpaceFor(const natl::Size count) noexcept {
			return sourceIndex + (sizeof(Type) * count) <= source.size();
		};
		[[nodiscard]] natl::ConstAsciiStringView getLocationDetails() noexcept {
			return natl::ConstAsciiStringView(reinterpret_cast<natl::Ascii*>(source + sourceIndex), source.size() - sourceIndex);
		};
		[[nodiscard]] DeserializeErrorHandler notEnoughSpaceError(natl::ConstAsciiStringView errorMessage, 
			const natl::DeserializeErrorLocation errorLocation) noexcept {
			return DeserializeErrorHandler(errorMessage, getLocationDetails(), errorLocation, natl::DeserializeErrorFlag::endOfSource);
		};
		[[nodiscard]] natl::Expect<natl::Size, DeserializeErrorHandler> readContainerSize(
			const natl::DeserializeErrorLocation errorLocation) noexcept {
			if (not hasEnoughSpaceFor<container_size_type>()) {
				return notEnoughSpaceError("not enough space for container size", errorLocation);
			}
			return readBytes<container_size_type>();
		}
		template<typename IntegerType>
			requires(natl::IsBuiltInIntegerC<IntegerType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> checkBufferSize(const natl::DeserializeErrorLocation errorLocation) noexcept {
			if (not hasEnoughSpaceFor<IntegerType>()) {
				return notEnoughSpaceError("not enough space for primitive type", errorLocation);
			}
			return natl::OptionEmpty{};
		}


		template<typename IntegerType>
			requires(natl::IsBuiltInIntegerC<IntegerType>)
		[[nodiscard]] constexpr natl::Expect<IntegerType, DeserializeErrorHandler> readInteger(const natl::DeserializeErrorLocation errorLocation) noexcept {
			auto error = checkBufferSize<IntegerType>(errorLocation);
			if (error.hasValue()) { return natl::unexpected(error.value()); }
			return readBytes<IntegerType>();
		}
		template<typename FloatType>
			requires(natl::IsBuiltInFloatingPointC<FloatType>)
		[[nodiscard]] constexpr natl::Expect<FloatType, DeserializeErrorHandler> readFloat(const natl::DeserializeErrorLocation errorLocation) noexcept {
			using integer_type = natl::UIntOfByteSize<sizeof(FloatType)>;
			auto error = checkBufferSize<integer_type>(errorLocation);
			if (error.hasValue()) { return natl::unexpected(error.value()); }
			const integer_type value = readBytes<integer_type>();
			return natl::bitCast<FloatType>(value);
		}

		template<typename ByteLikeType, typename BytesDstType>
			requires(sizeof(ByteLikeType) == 1)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readBytesToDst(
			const natl::DeserializeErrorLocation errorLocation, BytesDstType& dst) noexcept {
			if (not hasEnoughSpaceFor<container_size_type>()) {
				return notEnoughSpaceError("not enough space for container size element", errorLocation);
			}

			const natl::Size size = readBytes<container_size_type>();
			if (not hasEnoughSpaceFor<ByteLikeType>(size)) {
				return notEnoughSpaceError("not enough space for all elements of container", errorLocation);
			}

			dst.reserve(size);
			for (natl::Size i = 0; i < size; i++) {
				dst.pushBack(natl::bitCast<ByteLikeType>(source[sourceIndex + i]));
			}

			return natl::OptionEmpty{};
		}

	public:
		[[nodiscard]] constexpr natl::Bool addSource(const natl::ConstArrayView<natl::Byte>& newSource) noexcept {
			source = newSource;
			return true;
		}

		template<typename Serializer>
			requires(natl::IsSerializerC<Serializer>)
		constexpr void serializeErrors(Serializer& serializer) noexcept {}
		template<typename Serializer, natl::SerializeWriteFlag Flags,
			natl::CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeMemberType>
		constexpr void serializeSourceProccessState(Serializer& serializer) noexcept {}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags>
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::DeserializeGlobalScope>, DeserializeErrorHandler> begin() noexcept {
			sourceIndex = 0;
			return DeserializeInfo<natl::DeserializeGlobalScope>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> end(
			DeserializeInfo<natl::DeserializeGlobalScope> info) noexcept {
			sourceIndex = 0;
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename Type, typename ParentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<Type>>, DeserializeErrorHandler> beginReadNamed(
			DeserializeInfo<ParentType>& parrent, const natl::ConstAsciiStringView name) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<Type>>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadNamed(DeserializeInfo<SerializeType>& info) noexcept {
			return natl::OptionEmpty{};
		}


		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::i8, DeserializeErrorHandler> readI8(DeserializeInfo<natl::SerializeI8>& info) noexcept {
			return readInteger<natl::i8>(natl::DeserializeErrorLocation::readI8);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::i16, DeserializeErrorHandler> readI16(DeserializeInfo<natl::SerializeI16>& info) noexcept {
			return readInteger<natl::i16>(natl::DeserializeErrorLocation::readI16);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::i32, DeserializeErrorHandler> readI32(DeserializeInfo<natl::SerializeI32>& info) noexcept {
			return readInteger<natl::i32>(natl::DeserializeErrorLocation::readI32);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::i64, DeserializeErrorHandler> readI64(DeserializeInfo<natl::SerializeI64>& info) noexcept {
			return readInteger<natl::i64>(natl::DeserializeErrorLocation::readI64);
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::ui8, DeserializeErrorHandler> readUI8(DeserializeInfo<natl::SerializeUI8>& info) noexcept {
			return readInteger<natl::ui8>(natl::DeserializeErrorLocation::readUI8);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::ui16, DeserializeErrorHandler> readUI16(DeserializeInfo<natl::SerializeUI16>& info) noexcept {
			return readInteger<natl::ui16>(natl::DeserializeErrorLocation::readUI16);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::ui32, DeserializeErrorHandler> readUI32(DeserializeInfo<natl::SerializeUI32>& info) noexcept {
			return readInteger<natl::ui32>(natl::DeserializeErrorLocation::readUI32);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::ui64, DeserializeErrorHandler> readUI64(DeserializeInfo<natl::SerializeUI64>& info) noexcept {
			return readInteger<natl::ui64>(natl::DeserializeErrorLocation::readUI64);
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::f32, DeserializeErrorHandler> readF32(DeserializeInfo<natl::SerializeF32>& info) noexcept {
			return readFloat<natl::f32>(natl::DeserializeErrorLocation::readF32);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::f64, DeserializeErrorHandler> readF64(DeserializeInfo<natl::SerializeF64>& info) noexcept {
			return readFloat<natl::f64>(natl::DeserializeErrorLocation::readF64);
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler> readBool(DeserializeInfo<natl::SerializeBool>& info) noexcept {
			auto readValueExpect = readInteger<natl::ui8>(natl::DeserializeErrorLocation::readBool);
			if (readValueExpect.hasError()) { return unexpected(readValueExpect.error()); }
			return natl::bitCast<natl::Bool>(readValueExpect.value());
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::Ascii, DeserializeErrorHandler> readChar(DeserializeInfo<natl::SerializeChar>& info) noexcept {
			auto readValueExpect = readInteger<natl::ui8>(natl::DeserializeErrorLocation::readChar);
			if (readValueExpect.hasError()) { return unexpected(readValueExpect.error()); }
			return natl::bitCast<natl::Ascii>(readValueExpect.value());
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename StringDstType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readStr(DeserializeInfo<natl::SerializeStr>& info, StringDstType& dst) noexcept {
			return readBytesToDst<natl::Ascii>(dst, natl::DeserializeErrorLocation::readStr);
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename FileNameDstType, typename FileContentDstType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
			readFile(DeserializeInfo<natl::SerializeFile>& info, FileNameDstType& filenameDst, FileContentDstType& fileContentDst) noexcept {
			auto readFileNameError = readBytesToDst<natl::Ascii>(filenameDst, natl::DeserializeErrorLocation::readFile);
			if (readFileNameError.hasValue()) { return readFileNameError; }
			return readBytesToDst<natl::Byte>(fileContentDst, natl::DeserializeErrorLocation::readFile);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename DataDstType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readBlob(DeserializeInfo<natl::SerializeBlob>& info, DataDstType& dst) noexcept {
			return readBytesToDst<natl::Byte>(dst, natl::DeserializeErrorLocation::readBlob);
		}

		//optional
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler>
			readIsEmptyOptional(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {
			if (not hasEnoughSpaceFor<natl::i8>()) {
				return notEnoughSpaceError("not enough space for optional", natl::DeserializeErrorLocation::readIsEmptyOptional);
			}
			return !natl::bitCast<natl::Bool>(readBytes<natl::i8>());
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerilizeType>, DeserializeErrorHandler>
			beginReadOptional(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {
			return DeserializeInfo<SerilizeType>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
			endReadOptional(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		//enum
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename BaseType, typename Functor>
			requires(natl::IsSerializableC<BaseType>&& natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsStringToSerializeFlagConvertFunctor<Functor, BaseType>)
		[[nodiscard]] constexpr natl::Expect<natl::BasicSerializeTypeToType<natl::SerializeTypeOf<BaseType>>, DeserializeErrorHandler>
			readEnum(DeserializeInfo<natl::SerializeEnum<BaseType>>& info, Functor&& stringToEnum) noexcept {
			if (not hasEnoughSpaceFor<BaseType>()) {
				return notEnoughSpaceError("not enough space for enum value", natl::DeserializeErrorLocation::readEnum);
			}
			return readBytes<BaseType>();
		}

		//fixed array
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> beginReadFixedArray(
			DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadFixedArray(
			DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler> beginReadFixedArrayElement(
			DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<ElementType>>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename ElementType, natl::Size Number, typename ElementSerializeType = natl::SerializeTypeOf<ElementType>>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadFixedArrayElement(DeserializeInfo<ElementSerializeType>& info) noexcept {
			return natl::OptionEmpty{};
		}

		//array
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadArray(
			DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {
			return readContainerSize(natl::DeserializeErrorLocation::beginReadArray);
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArray(
			DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyArray(
			DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler> beginReadArrayElement(
			DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<ElementType>>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType,
			typename ElementSerializeType = natl::SerializeTypeOf<ElementType>>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArrayElement(DeserializeInfo<ElementSerializeType>& info) noexcept {
			return natl::OptionEmpty{};
		}

		//dic
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadDic(
			DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) noexcept {
			return readContainerSize(natl::DeserializeErrorLocation::beginReadDic);
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDic(
			DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyDic(
			DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>, DeserializeErrorHandler> beginReadDicElement(
			DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDicElement(
			DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<KeyType>>, DeserializeErrorHandler> readDicKey(
			DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<KeyType>>{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename KeyType, typename ValueType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ValueType>>, DeserializeErrorHandler> readDicValue(
			DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>& info) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<ValueType>>{};
		}

		//struct
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename... SerializeMemberTypes>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> beginReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>> info) noexcept {
			return natl::OptionEmpty{};
		}
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename... SerializeMemberTypes>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadStruct(
			DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		//variant
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename IndexType, typename... VariantTypes>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		constexpr natl::Expect<natl::Bool, DeserializeErrorHandler>
			readIsEmptyVariant(DeserializeInfo<natl::SerializeVariant<IndexType, VariantTypes...>>& info) noexcept {
			if (not hasEnoughSpaceFor<IndexType>()) {
				return natl::unexpected(
					notEnoughSpaceError("not enough space for variant index type", 
						natl::DeserializeErrorLocation::readIsEmptyVariant));
			}
			variantIndex = readBytes<IndexType>();

			if (variantIndex != 0 && variantIndex - 1 >= sizeof...(VariantTypes)) {
				return DeserializeErrorHandler(natl::format<natl::String256>("unexpected variant index of ", variantIndex, 
						" which is outside the accepted range of between [0, ", sizeof...(VariantTypes) - 1, "]"),
					getLocationDetails(), natl::DeserializeErrorFlag::valueParsing, natl::DeserializeErrorLocation::readIsEmptyOptional);
			}

			return variantIndex != 0;
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename StringToVariatIndexFunctor, typename IndexType, typename... VariantTypes>
			requires(natl::IsSerializeComponentC<SerializeComponentType>
				&& natl::IsStringToSerializeVariantIndexConvertFunctor<StringToVariatIndexFunctor, IndexType>)
		[[nodiscard]] constexpr natl::Expect<IndexType, DeserializeErrorHandler>
			beginReadVariantGetIndex(
				DeserializeInfo<natl::SerializeVariant<IndexType, VariantTypes...>>& info,
				StringToVariatIndexFunctor&& stringToIndex) noexcept {
			return variantIndex - 1;
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename ElementType, typename IndexSize, typename... VariantTypes>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::TemplatePackHasElementC<natl::IsSameV, ElementType, VariantTypes...>)
		[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler>
			beginReadVariantOfType(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& variant) noexcept {
			return DeserializeInfo<natl::SerializeTypeOf<ElementType>>{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename IndexSize, typename... VariantTypes>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
			endReadVariant(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& info) noexcept {
			return natl::OptionEmpty{};
		}

		//jump
		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType, typename ParentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpTableInfo<IdNumberType>, DeserializeErrorHandler> beginReadJumpTable(
			DeserializeInfo<ParentType>& info,
			const natl::ConstAsciiStringView& name) noexcept {

			natl::Expect<natl::Size, DeserializeErrorHandler> tableSizeExpect 
				= readContainerSize(natl::DeserializeErrorLocation::beginReadJumpTable);
			tableSizeExpect.hasValue();
			if (tableSizeExpect.hasValue()) { return tableSizeExpect.error(); }

			natl::SerializeJumpTableInfo<IdNumberType> jumpTableInfo{};
			jumpTableInfo.tableLocation = sourceIndex;
			jumpTableInfo.size = tableSizeExpect.value();

			return jumpTableInfo;
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType, typename ParentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadJumpTable(
			DeserializeInfo<ParentType>& info, natl::SerializeJumpTableInfo<IdNumberType>) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType, typename ParentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpInfo<IdNumberType>, DeserializeErrorHandler> readJumpTableElement(
			DeserializeInfo<ParentType>& info) noexcept {
			
			if (not hasEnoughSpaceFor<container_size_type>()) {
				return natl::unexpected(
					notEnoughSpaceError("not enough space for jump table element id",
						natl::DeserializeErrorLocation::readJumpTableElement));
			}
			const container_size_type id = readBytes<container_size_type>();
			
			if (not hasEnoughSpaceFor<container_size_type>()) {
				return natl::unexpected(
					notEnoughSpaceError("not enough space for jump table element jump location",
						natl::DeserializeErrorLocation::readJumpTableElement));
			}
			const container_size_type jumpLocation = readBytes<container_size_type>();


			natl::SerializeJumpInfo<IdNumberType> jumpInfo{};
			jumpInfo.jumpId = id;
			jumpInfo.jumpLocation = jumpLocation;
			return jumpInfo;
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readJumpLocation(
			const natl::SerializeJumpTableInfo<IdNumberType>& jumpTableInfo,
			natl::SerializeJumpInfo<IdNumberType>& jumpInfo) noexcept {
			return natl::OptionEmpty{};
		}

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> jumpTo(
			const natl::SerializeJumpTableInfo<IdNumberType>& jumpTableInfo,
			natl::SerializeJumpInfo<IdNumberType>& jumpInfo,
			const natl::Bool searchKnownLocations = true) noexcept {

			if (not natl::isInRange(source, jumpInfo.jumpLocation.value())) {
				return DeserializeErrorHandler(natl::format<natl::String256>("jump was not in the range. Tried jumping to ", 
						jumpInfo.jumpLocation.value(), " but not in the range (0 ", source.size()),
					getLocationDetails(), natl::DeserializeErrorFlag::jump, natl::DeserializeErrorLocation::jumpTo);
			}

			sourceIndex = jumpInfo.jumpLocation;
			variantIndex = 0;

			return natl::OptionEmpty{};
		}

		//Can OffsetSkip 
		//Check if type can be skip without considering stored data 
		template<typename SerializeType>
		struct CanOffsetSkipV;

		template<> struct CanOffsetSkipV<natl::SerializeI8> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeI16> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeI32> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeI64> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeUI8> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeUI16> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeUI32> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeUI64> {
		};

		template<> struct CanOffsetSkipV<natl::SerializeF32> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeF64> {
			constexpr static inline natl::Bool value = true;
		};

		template<> struct CanOffsetSkipV<natl::SerializeBool> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeChar> {
			constexpr static inline natl::Bool value = true;
		};
		template<> struct CanOffsetSkipV<natl::SerializeStr> {
			constexpr static inline natl::Bool value = false;
		};
		template<> struct CanOffsetSkipV<natl::SerializeFile> {
			constexpr static inline natl::Bool value = false;
		};
		template<> struct CanOffsetSkipV<natl::SerializeBlob> {
			constexpr static inline natl::Bool value = false;
		};
		template<typename SerializeType>
		struct CanOffsetSkipV<natl::SerializeOptional<SerializeType>> {
			constexpr static inline natl::Bool value = false;
		};
		template<typename BaseElementType>
		struct CanOffsetSkipV<natl::SerializeEnum<BaseElementType>> {
			constexpr static inline natl::Bool value = CanOffsetSkipV<BaseElementType>::value;
		};
		template<typename ElementType, natl::Size Number>
		struct CanOffsetSkipV<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static inline natl::Bool value = CanOffsetSkipV<ElementType>::value;
		};
		template<typename ElementType>
		struct CanOffsetSkipV<natl::SerializeArray<ElementType>> {
			constexpr static inline natl::Bool value = false;
		};
		template<typename KeyType, typename ValueType>
		struct CanOffsetSkipV<natl::SerializeDic<KeyType, ValueType>> {
			constexpr static inline natl::Bool value = false;
		};
		template<typename... MemberTypes>
		struct CanOffsetSkipV<natl::SerializeStruct<MemberTypes...>> {
			constexpr static inline natl::Bool value = (CanOffsetSkipV<MemberTypes>::value && ...);
		};

		template<typename IndexSerializeType, typename... Types>
		struct CanOffsetSkipV<natl::SerializeVariant<IndexSerializeType, Types...>> {
			constexpr static inline natl::Bool value = false;
		};

		template<typename Type>
			requires(natl::IsSerializableC<Type>)
		struct CanOffsetSkipV<Type> : CanOffsetSkipV<natl::SerializeTypeOf<Type>> {};

		//OffsetSkip 
		//Offset amount to skip type if can for type 
		template<typename SerializeType>
		struct OffsetSkipV;

		template<> struct OffsetSkipV<natl::SerializeI8> {
			constexpr static inline natl::Size value = sizeof(natl::i8);
		};
		template<> struct OffsetSkipV<natl::SerializeI16> {
			constexpr static inline natl::Size value = sizeof(natl::i16);
		};
		template<> struct OffsetSkipV<natl::SerializeI32> {
			constexpr static inline natl::Size value = sizeof(natl::i32);
		};
		template<> struct OffsetSkipV<natl::SerializeI64> {
			constexpr static inline natl::Size value = sizeof(natl::i64);
		};
		template<> struct OffsetSkipV<natl::SerializeUI8> {
			constexpr static inline natl::Size value = sizeof(natl::ui8);
		};
		template<> struct OffsetSkipV<natl::SerializeUI16> {
			constexpr static inline natl::Size value = sizeof(natl::ui16);
		};
		template<> struct OffsetSkipV<natl::SerializeUI32> {
			constexpr static inline natl::Size value = sizeof(natl::ui32);
		};
		template<> struct OffsetSkipV<natl::SerializeUI64> {
			constexpr static inline natl::Size value = sizeof(natl::ui64);
		};

		template<> struct OffsetSkipV<natl::SerializeF32> {
			constexpr static inline natl::Size value = sizeof(natl::f32);
		};
		template<> struct OffsetSkipV<natl::SerializeF64> {
			constexpr static inline natl::Size value = sizeof(natl::f64);
		};

		template<> struct OffsetSkipV<natl::SerializeBool> {
			constexpr static inline natl::Size value = sizeof(natl::Bool);
		};
		template<> struct OffsetSkipV<natl::SerializeChar> {
			constexpr static inline natl::Size value = sizeof(natl::Char);
		};
		template<typename BaseElementType>
		struct OffsetSkipV<natl::SerializeEnum<BaseElementType>> {
			constexpr static inline natl::Size value = OffsetSkipV<BaseElementType>::value;
		};
		template<typename ElementType, natl::Size Number>
		struct OffsetSkipV<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static inline natl::Size value = OffsetSkipV<ElementType>::value * Number;
		};
		template<typename... MemberTypes>
		struct OffsetSkipV<natl::SerializeStruct<MemberTypes...>> {
			constexpr static inline natl::Size value = (OffsetSkipV<MemberTypes>::value + ...);
		};

		template<typename Type>
			requires(natl::IsSerializableC<Type>)
		struct OffsetSkipV<Type> : OffsetSkipV<natl::SerializeTypeOf<Type>> {};

		//Skip
	private:
		template<typename PrimitiveType>
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> skipPrimitive(
			const natl::ConstAsciiStringView& primitiveName) noexcept {
			if (not hasEnoughSpaceFor<PrimitiveType>) {
				return notEnoughSpaceError(
					natl::format<natl::String256>("not enough space to skip primitive ", primitiveName),
					natl::DeserializeErrorLocation::skip);
			}
			sourceIndex += sizeof(PrimitiveType);
			return natl::OptionEmpty{};
		};
		[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> skipContainerSize(const natl::ConstAsciiStringView& containerName) noexcept {
			if (not hasEnoughSpaceFor<container_size_type>) {
				return natl::unexpected(notEnoughSpaceError(
					natl::format<natl::String256>("not enough space to skip container ", containerName, " size element"),
					natl::DeserializeErrorLocation::skip));
			}
			return readBytes<container_size_type>();
		}
		//a primitive container is sequentail set of bytes where the first 8 bytes is the size and the next bytes are byte sized elements
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> skipPrimitiveContainer(const natl::ConstAsciiStringView& containerName) noexcept {
			const natl::Expect<natl::Size, DeserializeErrorHandler> sizeExpect = skipContainerSize(containerName);
			if (sizeExpect.hasError()) { return sizeExpect.error(); }
			const natl::Size size = sizeExpect.value();

			if (not hasEnoughSpaceFor<natl::i8>(size)) {
				return notEnoughSpaceError(
					natl::format<natl::String256>("not enough space to skip all ", size, " elements of the container ", containerName)
					, natl::DeserializeErrorLocation::skip);
			}
			sourceIndex += size;
			return natl::OptionEmpty{};
		}

	public:
		//skip specializations for types 
		template<typename SerializeType>
		struct Skip;

		template<> struct Skip<natl::SerializeI8> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::i8>("i8");
			}
		};
		template<> struct Skip<natl::SerializeI16> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::i16>("i16");
			}
		};
		template<> struct Skip<natl::SerializeI32> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::i32>("i32");
			}
		};
		template<> struct Skip<natl::SerializeI64> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::i64>("i64");
			}
		};
		template<> struct Skip<natl::SerializeUI8> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::ui8>("ui8");
			}
		};
		template<> struct Skip<natl::SerializeUI16> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::ui16>("ui16");
			}
		};
		template<> struct Skip<natl::SerializeUI32> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::ui32>("ui32");
			}
		};
		template<> struct Skip<natl::SerializeUI64> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::ui32>("ui32");
			}
		};

		template<> struct Skip<natl::SerializeF32> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::f32>("f32");
			}
		};
		template<> struct Skip<natl::SerializeF64> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::f64>("f64");
			}
		};

		template<> struct Skip<natl::SerializeBool> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::Bool>("Bool");
			}
		};
		template<> struct Skip<natl::SerializeChar> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPrimitive<natl::Char>("Char");
			}
		};
		template<> struct Skip<natl::SerializeStr> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPimitiveContainer("str");
			}
		};
		template<> struct Skip<natl::SerializeFile> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				auto fileNameError = deserializer.skipPimitiveContainer("file name");
				if (fileNameError.hasValue()) { return fileNameError.value(); }
				return deserializer.skipPimitiveContainer("file contents");
			}
		};
		template<> struct Skip<natl::SerializeBlob> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return deserializer.skipPimitiveContainer("blob");
			}
		};
		template<typename SerializeType>
		struct Skip<natl::SerializeOptional<SerializeType>> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				if (not deserializer.hasEnoughSpaceFor<natl::i8>()) {
					return deserializer.notEnoughSpaceError(
						"not enough space for optional boolean", 
						natl::DeserializeErrorLocation::skip);
				}
				const natl::Bool hasValue = natl::bitCast<natl::Bool>(readBytes<natl::i8>());
				if (hasValue) {
					return Skip<SerializeType>::skip(deserializer);
				}
				return natl::OptionEmpty{};
			}
		};
		template<typename BaseElementType>
		struct Skip<natl::SerializeEnum<BaseElementType>> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				return Skip<BaseElementType>::skip(deserializer);
			}
		};
		template<typename ElementType, natl::Size Number>
		struct Skip<natl::SerializeFixedArray<ElementType, Number>> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				if constexpr (CanOffsetSkipV<ElementType>::value) {
					constexpr natl::Size skipSize = OffsetSkipV<ElementType>::value * Number;
					if (not deserializer.hasEnoughSpaceFor(skipSize)) {
						return deserializer.notEnoughSpaceError(
							natl::format<natl::String256>("not enough space to skip all ", Number, " fixed array elements"),
							natl::DeserializeErrorLocation::skip);
					}
					sourceIndex += skipSize;
				} else {
					for ([[maybe_unused]] const natl::Size index : natl::Repeat(Number)) {
						auto elementError = Skip<ElementType>::skip(deserializer);
						if (elementError.hasValue()) { return elementError.value(); }
					}
				}
				return natl::OptionEmpty{};
			}
		};
		template<typename ElementType>
		struct Skip<natl::SerializeArray<ElementType>> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				const natl::Expect<natl::Size, DeserializeErrorHandler> sizeExpect = skipContainerSize("array");
				if (sizeExpect.hasError()) { return sizeExpect.error(); }
				const natl::Size size = sizeExpect.value();

				if constexpr (CanOffsetSkipV<ElementType>::value) {
					const natl::Size skipSize = OffsetSkipV<ElementType>::value * size;
					if (not deserializer.hasEnoughSpaceFor(skipSize)) {
						return deserializer.notEnoughSpaceError(
							natl::format<natl::String256>("not enough space to skip all ", size, " array elements"),
							natl::DeserializeErrorLocation::skip);
					}
					sourceIndex += skipSize;
				} else {
					for ([[maybe_unused]] const natl::Size index : natl::Repeat(size)) {
						auto elementError = Skip<ElementType>::skip(deserializer);
						if (elementError.hasValue()) { return elementError.value(); }
					}
				}

				return natl::OptionEmpty{};
			}
		};
		template<typename KeyType, typename ValueType>
		struct Skip<natl::SerializeDic<KeyType, ValueType>> {
			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				const natl::Expect<natl::Size, DeserializeErrorHandler> sizeExpect = skipContainerSize("dic");
				if (sizeExpect.hasError()) { return sizeExpect.error(); }
				const natl::Size size = sizeExpect.value();

				if constexpr (CanOffsetSkipV<KeyType>::value && CanOffsetSkipV<ValueType>::value) {
					const natl::Size skipSize = OffsetSkipV<KeyType>::value * size + OffsetSkipV<KeyType>::value * size;
					if (not deserializer.hasEnoughSpaceFor(skipSize)) {
						return deserializer.notEnoughSpaceError(
							natl::format<natl::String256>("not enough space to skip all ", size, " dic elements"),
							natl::DeserializeErrorLocation::skip);
					}
					sourceIndex += skipSize;
				} else {
					for ([[maybe_unused]] const natl::Size index : natl::Repeat(size)) {
						auto keyError = Skip<KeyType>::skip(deserializer);
						if (keyError.hasValue()) { return keyError.value(); }
						auto valueError = Skip<ValueType>::skip(deserializer);
						if (valueError.hasValue()) { return valueError.value(); }
					}
				}
				return natl::OptionEmpty{};
			}
		};
		template<typename... MemberTypes>
		struct Skip<natl::SerializeStruct<MemberTypes...>> {
			
			constexpr static natl::Option<DeserializeErrorHandler> skipMembers(UnalignedDeserializer& deserializer) noexcept {
				return natl::OptionEmpty{};
			}
			template<typename MemberType, typename... OtherMembers>
			constexpr static natl::Option<DeserializeErrorHandler> skipMembers(UnalignedDeserializer& deserializer) noexcept {
				auto memberSkipError = Skip<MemberType>::skip(deserializer);
				if (memberSkipError.hasError()) { return memberSkipError.value(); }
				return skipMembers<OtherMembers...>(deserializer);
			}

			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				if constexpr (CanOffsetSkipV<natl::SerializeStruct<MemberTypes...>>::value) {
					constexpr natl::Size skipSize = OffsetSkipV<natl::SerializeStruct<MemberTypes...>>::value;
					if (not deserializer.hasEnoughSpaceFor(skipSize)) {
						return deserializer.notEnoughSpaceError(
							natl::format<natl::String256>("not enough space to skip struct"),
							natl::DeserializeErrorLocation::skip);
					}
					sourceIndex += skipSize;
					return natl::OptionEmpty{};
				} else {
					return skipMembers<MemberTypes...>();
				}
			}
		};

		template<typename IndexType, typename... Types>
		struct Skip<natl::SerializeVariant<IndexType, Types...>> {
			using variant_element_skip_func = natl::Option<DeserializeErrorHandler>(*)(UnalignedDeserializer&);
			template<typename Element>
			constexpr static variant_element_skip_func getSkipElementFunction() noexcept {
				return [](UnalignedDeserializer& deserializer) -> natl::Option<DeserializeErrorHandler> {
					return Skip<Element>::skip(deserializer);
				};
			}

			constexpr static natl::Option<DeserializeErrorHandler> skip(UnalignedDeserializer& deserializer) noexcept {
				if (not hasEnoughSpaceFor<IndexType>()) {
					return natl::unexpected(
						notEnoughSpaceError("not enough space for variant index type",
							natl::DeserializeErrorLocation::skip));
				}

				natl::Size variantIndex = readBytes<IndexType>();
				if (variantIndex != 0) {
					variantIndex -= 1;
					if (variantIndex >= sizeof...(Types)) {
						return DeserializeErrorHandler(natl::format<natl::String256>("unexpected variant index of ", variantIndex,
								" which is outside the accepted range of between [0, ", sizeof...(Types) - 1, "]"),
							getLocationDetails(), natl::DeserializeErrorFlag::valueParsing, natl::DeserializeErrorLocation::skip);
					}

					constexpr natl::Array<variant_element_skip_func, sizeof...(Types)> skipFuncs = {
						getSkipElementFunction<Types>()...
					};
					auto skipError = skipFuncs[variantIndex]();
					if (skipError.hasValue()) { return skipError.value(); }
				} 
				return natl::OptionEmpty{};
			}
		};

		template<typename Type>
			requires(natl::IsSerializableC<Type>)
		struct Skip<Type> : Skip<natl::SerializeTypeOf<Type>> {};

		template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
			typename Type, typename ParentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsDeserializableC<Type>)
		[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> skip(
			DeserializeInfo<ParentType>& parent, const natl::ConstAsciiStringView name) noexcept {
			return Skip<Type>::skip(self());
		}
	};
}