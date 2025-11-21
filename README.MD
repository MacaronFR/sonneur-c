# S.O.N.N.E.U.R

The lib S.O.N.N.E.U.R (Simple Object Notation for Network Exchange of Universal Ressources) is a implementation of the S.O.N.E.U.R. data format in C providing a simple API for serialization and deserialization.

# Usage

Download the lastest release package (containing headers and precompiled binaries), extract and include them in your project.  
You will define a structure to be parsed by using this template.
```c
#define struct_to_serialize(field, struct_field)\
    field(type, name)\
    struct_field(struct_type, name)
```

Next you will need to call the macro to generate the struct, the serializer and the deserializer by using
```c
serializable_struct(name, struct_to_serialize);
```

Then you are set. To serialize an object, you will use `uint8_t *buffer = serialize(pointer_to_object, get_serializer(name_of_struct), pointer_to_size_of_resulting_buffer);`.  
The buffer will need to be freed afterward.  
  
To deserialize a buffer into an object, you will use `name_of_struct object = deserialize(name_of_struct, buffer);`.  
The buffer is not freed by the deserialize function.

## Supported types

| C Type   | Format in SONNEUR |
|----------|-------------------|
| char     | INT8              |
| int8_t   | INT8              |
| int8_t   | INT8              |
| uint8_t  | INT8              |
| short    | INT16             |
| int16_t  | INT16             |
| uint16_t | INT16             |
| int      | INT32             |
| int32_t  | INT32             |
| uint32_t | INT32             |
| long     | INT64             |
| int64_t  | INT64             |
| uint64_t | INT64             |
| float    | FLOAT32           |
| double   | FLOAT64           |

