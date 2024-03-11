#pragma once 

//natl
#include <natl/serialization.h>
#include <natl/dynArray.h>
#include <natl/basicTypes.h>
#include <natl/flatHashMap.h>
#include <natl/batchPool.h>
#include <natl/stringConvert.h>

//interface 
namespace nadsad {
	template<typename Alloc = natl::DefaultAllocatorByte>
		requires(natl::IsAllocator<Alloc>)
	class AssciFormatSerializer {
		using allocator_type = Alloc;
		using allocation_move_adapater = natl::AllocationMoveAdapater<natl::Byte>;
	public:
		natl::DynArray<natl::AssciCode, typename Alloc::template rebind_alloc<natl::AssciCode>> data;
		natl::SmallBatchPool<natl::SerializationStructType, 10> types;
		natl::FlatHashMap<natl::String, natl::SerializationStructType*> nameToSchemaMap;
		natl::SmallDynArray<const natl::SerializationStructType*, 2> schemaStack;
		natl::Size indent;
	public:
		constexpr AssciFormatSerializer() noexcept : data(), types(), nameToSchemaMap(), schemaStack(), indent(0) {}
		using ByteAllocationMoveAdapter = natl::AllocationMoveAdapater<natl::Byte, typename Alloc::template rebind_alloc<natl::Byte>>;
		using AssciAllocationMoveAdapter = natl::AllocationMoveAdapater<natl::AssciCode, typename Alloc::template rebind_alloc<natl::AssciCode>>;
		[[nodiscard]] constexpr ByteAllocationMoveAdapter moveOutputAsByte() noexcept {
			data.reserve(data.size() + 1);
			natl::AssciCode* assciOutputEnd = &*data.end();
			*assciOutputEnd = 0;
			return natl::equivalentCastAllocationMoveAdapter<natl::Byte>(data.getAlloctionMoveAdapater());
		}
		[[nodiscard]] constexpr AssciAllocationMoveAdapter moveOutputAsAssci() noexcept {
			data.reserve(data.size() + 1);
			natl::AssciCode* assciOutputEnd = &*data.end();
			*assciOutputEnd = 0;
			return data.getAlloctionMoveAdapater();
		}
	private:
		constexpr void add(const natl::ConstAsciiStringView& name) noexcept {
			data.append(name.data(), name.size());
		}
		constexpr void addSpace() noexcept {
			data.push_back(' ');
		}
		constexpr void addColon() noexcept {
			data.push_back(':');
		}
		constexpr void addNewLine() noexcept {
			data.push_back('\n');
		}
		constexpr void addNull() noexcept {
			addNull();
		}

		constexpr void addIndent() noexcept {
			for (const natl::Size i : natl::Repeat(indent)) {
				data.push_back('\t');
			}
		}

		template<typename Interger>
		constexpr void serializeInt(const Interger value) noexcept {
			add(natl::intToStringDecimal(value).toStringView());
		}
		template<typename Float>
		constexpr void serializeFloat(const Float value) noexcept {
			add(natl::floatToStringDecimal(value).toStringView());
		}

		constexpr natl::StringView converttypeFlagToStringView(const natl::SerializationTypeFlag schemaType) noexcept {
			switch (schemaType) {
			case natl::SerializationTypeFlag::dt_i8:
				return "i8";
			case natl::SerializationTypeFlag::dt_i16:
				return "i16";
			case natl::SerializationTypeFlag::dt_i32:
				return "i32";
			case natl::SerializationTypeFlag::dt_i64:
				return "i64";

			case natl::SerializationTypeFlag::dt_ui8:
				return "ui8";
			case natl::SerializationTypeFlag::dt_ui16:
				return "ui16";
			case natl::SerializationTypeFlag::dt_ui32:
				return "ui32";
			case natl::SerializationTypeFlag::dt_ui64:
				return "ui64";
			case natl::SerializationTypeFlag::dt_size:
				return "size";

			case natl::SerializationTypeFlag::dt_f32:
				return "f32";
			case natl::SerializationTypeFlag::dt_f64:
				return "f64";

			case natl::SerializationTypeFlag::dt_ascci_str:
				return "str";
			case natl::SerializationTypeFlag::dt_tstruct:
				return "tstruct";
			case natl::SerializationTypeFlag::dt_struct:
				return "struct";
			case natl::SerializationTypeFlag::dt_array:
				return "array";
			case natl::SerializationTypeFlag::dt_dic:
				return "dic";
			case natl::SerializationTypeFlag::dt_variant:
				return "variant";
			case natl::SerializationTypeFlag::dt_enum:
				return "enum";

			case natl::SerializationTypeFlag::dt_structType:
				return "struct_type";
			case natl::SerializationTypeFlag::dt_enumType:
				return "enum_type";
			case natl::SerializationTypeFlag::dt_variantType:
				return "variant_tye";
			default:
				natl::unreachable();
			}
		}

	public:
		constexpr void serializeBegin(const natl::Size bytesReserved = 10000) noexcept {
			data.reserve(bytesReserved);
		}
		constexpr void serializeEnd() noexcept {}

		template<natl::SerializationTypeFlag serializationType, 
			natl::SerializationOptionalType optionalFlag = natl::SerializationOptionalType::False>
		constexpr void serializeNamedElement(const natl::ConstAsciiStringView& name) noexcept {
			addIndent();
			add(name);
			if constexpr (optionalFlag == natl::SerializationOptionalType::True) {
				add(" op ");
			}
			add(" ");
			add(converttypeFlagToStringView(serializationType));
			
			if constexpr ((serializationType != natl::SerializationTypeFlag::dt_array) ||
				(serializationType != natl::SerializationTypeFlag::dt_dic) ||
				(serializationType != natl::SerializationTypeFlag::dt_enum) ||
				(serializationType != natl::SerializationTypeFlag::dt_variant)) {
				add(": ");
			}
		}

		//type 
	private:
		constexpr void serializeTypeElementType(const natl::SerializationType& type) noexcept {
			if (type.isOptionalFlag == natl::SerializationOptionalType::True) {
				add("op ");
			}

			switch (type.typeFlag) {
			case natl::SerializationTypeFlag::dt_struct:
				add("struct");
			case natl::SerializationTypeFlag::dt_tstruct:
				add("tstruct ");
				add(type.data.structType->name);
				break;
			case natl::SerializationTypeFlag::dt_array:
				add("array ");
				serializeArrayType(type.data.arrayInfo);
				break;
			case natl::SerializationTypeFlag::dt_dic:
				add("dic {");
				serializeDicType(type.data.dicInfo);
				add("}");
				break;
			case natl::SerializationTypeFlag::dt_variant:
				add("variant ");
				add(type.data.variantType->name);
				break;
			default:
				serializeObjectBasicType(type.typeFlag, natl::SerializationOptionalType::False);
				break;
			}
		}
	public:
		constexpr void serializeStructType(const natl::SerializationStructType& type) noexcept {
			addIndent();
			add(type.name);
			add(" struct_type: {\n");
			indent++;

			for (const natl::SerializationStructTypeElement& typeElement : type.elements) {
				addIndent();
				add(typeElement.name);
				add(": ");
				serializeTypeElementType(typeElement.type);
				add(",\n");
			}

			add("}\n");
			indent--;
		}

		//basic types
#define NADSAD_ASSCI_SERLIZE_BASIC_TYPE(NameOfTypeCatagory, NameOfType, FunctionEndName) \
		constexpr void serialize_##FunctionEndName(const natl::##NameOfType value) noexcept { \
			serialize##NameOfTypeCatagory<natl::##NameOfType>(value); \
			addNewLine(); \
		} \
		constexpr void serializeNull_##FunctionEndName() noexcept { \
			addNull(); \
		} 

		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, i8, i8)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, i16, i16)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, i32, i32)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, i64, i64)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, ui8, ui8)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, ui16, ui16)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, ui32, ui32)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, ui64, ui64)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Int, Size, size)

		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Float, f32, f32)
		NADSAD_ASSCI_SERLIZE_BASIC_TYPE(Float, f64, f64)

#undef NADSAD_ASSCI_SERLIZE_BASIC_TYPE

			//assci str 
	private:
		constexpr void addAssicStr(const natl::ConstAsciiStringView& assicStringView) noexcept {
			if (assicStringView.isEmpty()) {
				add("\"\"");
				return;
			}

			add("\"");
			data.reserve(data.size() + assicStringView.size());
			for (const char& character : assicStringView) {
				switch (character) {
				case '"':
					data.push_back('\\');
					data.push_back('"');
					break;
				case '\n':
					data.push_back('\\');
					data.push_back('n');
					break;
				case '\t':
					data.push_back('\\');
					data.push_back('t');
					break;
				default:
					data.push_back(character);
					break;
				}
			}

			add("\"");
		}
	public:
		constexpr void serializeAssicStr(const natl::ConstAsciiStringView& assciStringView) noexcept {
			addAssicStr(assciStringView);
		}
		constexpr void serializeNullAssicStr() noexcept {
			addNull();
		}

		//TypedStruct
		constexpr void serializeNamedTstructInfo(const natl::SerializationStructType& type) noexcept {
			add(type.name);
			add(": ");
		}

		constexpr void serializeTypedStruct(const natl::SerializationStructType& type) noexcept {
			addIndent();
			add("{\n");
			indent++;
		}
		constexpr void endSerializeTypedStruct() noexcept {
			indent--;
			addIndent();
			add("}\n");
		}

		constexpr void serializeNullTypedStruct(const natl::SerializationStructType& type) noexcept {
			addNull();
		}

		//struct
		constexpr void serializeStruct() noexcept {
			add("{\n");
			indent++;
		}
		constexpr void endSerializeStruct() noexcept {
			indent--;
			addIndent();
			add("}\n");
		}

		constexpr void serializeNullStruct() noexcept {
			addNull();
		}

		//dic 
	private:
		constexpr void serializeObjectBasicType(const natl::SerializationTypeFlag typeFlag, 
			const natl::SerializationOptionalType isOptional) noexcept {
			if (isOptional == natl::SerializationOptionalType::True) {
				add("op ");
			}
			add(converttypeFlagToStringView(typeFlag));
		}
		constexpr void serializeObjectEnumType(const natl::SerializationEnumType* enumType, 
			const natl::SerializationOptionalType isOptional) {
			if (isOptional == natl::SerializationOptionalType::True) {
				add("op enum ");
			} else {
				add("enum ");
			}

			add(enumType->name);
		}
		constexpr void serializeObjectVariantType(const natl::SerializationVariantType* variantType, 
			const natl::SerializationOptionalType isOptional) {
			if (isOptional == natl::SerializationOptionalType::True) {
				add("op variant ");
			} else {
				add("variant ");
			}

			add(variantType->name);
		}
		constexpr void serializeObjectType(const natl::SerializationStructType* type, 
			const natl::SerializationOptionalType isOptional) {
			if (isOptional == natl::SerializationOptionalType::True) {
				add("op ");
			}
			add(type->name);
		}
		
		constexpr void serializeDicType(const natl::SerializationDicInfo& dicInfo) noexcept {
			switch (dicInfo.keyTypeCategory) {
			case natl::SerializationTypeCategory::basicType:
				serializeObjectBasicType(dicInfo.keyTypeVariant.basicType, natl::SerializationOptionalType::False);
				break;
			case natl::SerializationTypeCategory::enumType:
				serializeObjectEnumType(dicInfo.keyTypeVariant.enumType, natl::SerializationOptionalType::False);
				break;
			case natl::SerializationTypeCategory::variantType:
				serializeObjectVariantType(dicInfo.keyTypeVariant.variantType, natl::SerializationOptionalType::False);
				break;
			case natl::SerializationTypeCategory::structType:
				serializeObjectType(dicInfo.keyTypeVariant.structType, natl::SerializationOptionalType::False);
				break;
			}

			add(":");

			switch (dicInfo.valueTypeCategory) {
			case natl::SerializationTypeCategory::basicType:
				serializeObjectBasicType(dicInfo.valueTypeVariant.basicType, dicInfo.isValueOptionalFlag);
				break;
			case natl::SerializationTypeCategory::enumType:
				serializeObjectEnumType(dicInfo.keyTypeVariant.enumType, dicInfo.isValueOptionalFlag);
				break;
			case natl::SerializationTypeCategory::variantType:
				serializeObjectVariantType(dicInfo.keyTypeVariant.variantType, dicInfo.isValueOptionalFlag);
				break;
			case natl::SerializationTypeCategory::structType:
				serializeObjectType(dicInfo.keyTypeVariant.structType, dicInfo.isValueOptionalFlag);
				break;
			}
		}
	public:
		constexpr void serializeNamedDicInfo(const natl::SerializationDicInfo& dicInfo) noexcept {
			add("{");
			serializeDicType(dicInfo);
			add("}: ");
		}

		constexpr void serializeDic(const natl::SerializationDicInfo& dicInfo) noexcept {
			add("{\n");
			indent++;
		}
		constexpr void endSerializeDic() noexcept {
			indent--;
			addIndent();
			add("}\n");
		}

		constexpr void serializeDicKey() noexcept {
			add(",\n");
		}
		constexpr void serializeDicValue() noexcept {
			add(": ");
		}

		
		constexpr void serializeNullDic(const natl::SerializationDicInfo& dicInfo) noexcept {
			addNull();
		}

		//array
	private:
		constexpr void serializeArrayType(const natl::SerializationArrayInfo& arrayInfo) noexcept {
			switch (arrayInfo.elementTypeCategory) {
			case natl::SerializationTypeCategory::basicType:
				serializeObjectBasicType(arrayInfo.elementTypeVariant.basicType, arrayInfo.isElementOptionalFlag);
				break;
			case natl::SerializationTypeCategory::enumType:
				serializeObjectEnumType(arrayInfo.elementTypeVariant.enumType, arrayInfo.isElementOptionalFlag);
				break;
			case natl::SerializationTypeCategory::variantType:
				serializeObjectVariantType(arrayInfo.elementTypeVariant.variantType, arrayInfo.isElementOptionalFlag);
				break;
			case natl::SerializationTypeCategory::structType:
				serializeObjectType(arrayInfo.elementTypeVariant.structType, arrayInfo.isElementOptionalFlag);
				break;
			}
		}
	public:
		constexpr void serializeNamedArrayInfo(const natl::SerializationArrayInfo& arrayInfo) noexcept {
			serializeArrayType(arrayInfo);
			add(": ");
		}

		constexpr void serializeNextArrayElement() noexcept {
			add(",\n");
		}

		constexpr void serializeArray(const natl::SerializationArrayInfo& arrayInfo) noexcept {
			add("[\n");
			indent++;
		}
		constexpr void endSerializeArray() noexcept {
			indent--;
			addIndent();
			add("]\n");
		}

		constexpr void serializeNullArray(const natl::SerializationArrayInfo& arrayInfo) noexcept {
			addNull();
		}

		//variant 
		constexpr void serializeVariantType(const natl::SerializationVariantType& variantType) {
			addIndent();
			add(variantType.name);
			add(" variant_type: {\n");
			indent++;

			for (const natl::SerializationType& type: variantType.types) {
				addIndent();
				serializeTypeElementType(type);
				add(",\n");
			}

			indent--;
			addIndent();
			add("}");
		}

		constexpr void serializeNamedVariantInfo(
			const natl::SerializationVariantType& variantType,
			const natl::SerializationType& variantElementType) noexcept {
			add("variant ");
			add(variantType.name);
			add(": ");
		}

		constexpr void serializeVariant(
			const natl::SerializationVariantType& variantType,
			const natl::SerializationType& variantElementType) noexcept {
			serializeTypeElementType(variantElementType);
			add(" ");
		}
		constexpr void endSerializeVariant() noexcept {
			add("\n");
		}

		constexpr void serializeNullVariant(const natl::SerializationVariantType& variantType) noexcept {
			addNull();
		}

		//enum
		constexpr void serializeEnumType(const natl::SerializationEnumType& enumType) noexcept {
			addIndent();
			add(enumType.name);
			add(" enum_type: {\n");
			indent++;

			for (const natl::String& nameValue : enumType.nameValues) {
				addIndent();
				add(nameValue);
				add("\n");
			}

			indent--;
			addIndent();
			add("}\n");
		}

		constexpr void serializeNamedEnumInfo(const natl::SerializationEnumType& enumType) noexcept {
			add("enum ");
			add(enumType.name);
			add(": ");
		}

		constexpr void serializeEnum(const natl::SerializationEnumType& enumType, const natl::Size nameIndex) noexcept {
			add(enumType.nameValues[nameIndex]);
			addNewLine();
		}
		constexpr void serializeNullEnum(const natl::SerializationEnumType& enumType) noexcept {
			addNull();
		}
	};
	static_assert(natl::IsCompleteSerializer<AssciFormatSerializer<>>, "nadsad: error - AssciFormatSerializer is not a complete serializer");
}