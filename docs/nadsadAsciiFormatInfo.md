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
- op {type_or_name}
- enum {ui_keyword} 
- variant {ui_keyword} { {type_or_name}, ... }
- struct { {type_or_name}, ... }
- farray [{ui_numeric_literal}, {type_or_name}]
- array ["{type_or_name}"]
- dic {{type_or_name}, {type_or_name}}


# Value Syntax
- op value: null or type_value
- enum_value: string_literal or ui_numeric_literal
- varaint_index_value: string_literal or ui_numeric_literal
- variant_value: {{varaint_index_value}, "{type_name}"} { value }
- array_value: [ {values}, ... ]
- dic_value: [ { key : value}, ... ]
- struct: { {members}; ... }

# Syntax 
"{element_name}" {type}: {value}; 