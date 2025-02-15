# Keywords
- i8
- i16
- i32
- i64
- ui8
- ui16
- ui32
- ui64
- f32
- f64
- bool
- str
- char
- blob
- file
- op
- enum
- farray
- array
- dic
- struct
- variant
- table
- index
- null
- true
- false

# Definitions
- element_name: name element is addressed by
- type_name: generated name of some type
- type: any valid configuration of a type 
- type_or_name: type or "{type_name}"
- string_literal: "..." backslash (/) used as escape character
- numeric_literal: (+/-) digits... (optional . with preceding digits...) (optional e with preceding (+/-) digits...)  
- ui_numeric_literal: a numeric_literal that can be classified as unsigned integer
- ui_keyword: ui8 or ui16 or ui32 or ui64 keyword


# Type Syntax
- op (type_or_name)
- enum (ui_keyword) 
- variant {ui_keyword} { (type_or_name), ... }
- struct { (type_or_name), ... }
- farray [(ui_numeric_literal), (type_or_name)]
- array ["(type_or_name)"]
- dic {(type_or_name), (type_or_name)}


# Value Syntax
- op_value: null or type_value
- enum_value: string_literal or ui_numeric_literal
- variant_index_value: string_literal or ui_numeric_literal
- variant_value: {(variant_index_value), "(type_name)"} { (value) }
- array_value: [ (values), ... ]
- dic_value: [ { (key) : (value)}, ... ]
- struct_value: { (members); ... }
- blob_value: (( ... )) byte 0xff or 255 is used as escape character
- file_value: (string_literal) (blob_value) 

# Syntax 
"{element_name}" {type}: {value},

# Jump Syntax
- jtable_element_id: string_literal or ui_numeric_literal
- jtable_jump_pos: null or ui_numeric_literal
- jtable (ui_keyword) "element_name" { (jtable_element_id) : (jtable_jump_pos), ... }
- jindex {jtable_element_id}