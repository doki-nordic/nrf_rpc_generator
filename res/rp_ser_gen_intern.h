#ifndef _RP_SER_GEN_INTERN_H
#define _RP_SER_GEN_INTERN_H

#define _SERIALIZE__CONCAT2(a, b, c) a ## _ ## b ## _ ## c
#define _SERIALIZE__CONCAT(a, b, c) _SERIALIZE__CONCAT2(a, b, c)
#define _SERIALIZE__UNIQUE() _SERIALIZE__CONCAT(_serialize_unique_, __COUNTER__, __LINE__)

#define _SERIALIZE_ "__SERIALIZE__:USE"
#define _SERIALIZE_OUT(x) "__SERIALIZE__:OUT=" #x
#define _SERIALIZE_INOUT(x) "__SERIALIZE__:INOUT=" #x
#define _SERIALIZE_STR(x) "__SERIALIZE__:STR=" #x
#define _SERIALIZE_INLINE_RESPONSE "__SERIALIZE__:INLINE_RESPONSE"
#define _SERIALIZE_EVENT "__SERIALIZE__:EVENT"
#define _SERIALIZE_IGNORE_RETURN "__SERIALIZE__:IGNORE_RETURN"
#define _SERIALIZE_CUSTOM_RESPONSE "__SERIALIZE__:CUSTOM_RESPONSE"
#define _SERIALIZE_ADD(dir, type, name) "__SERIALIZE__:ADD=" #dir "`" #type "`" #name
#define _SERIALIZE_DEL(name) "__SERIALIZE__:DEL=" #name
#define _SERIALIZE_SIZE(param, size) "__SERIALIZE__:SIZE=" #param "`" #size
#define _SERIALIZE_SIZE_PARAM(param, size_param) "__SERIALIZE__:SIZE_PARAM=" #param "`" #size_param
#define _SERIALIZE_SIZE_PARAM_EX(param, size_pattern, size_param) "__SERIALIZE__:SIZE_PARAM_EX=" #param "`" #size_pattern "`" #size_param
#define _SERIALIZE_NULLABLE(param) "__SERIALIZE__:NULLABLE=" #param
#define _SERIALIZE_STRUCT_INLINE(field) "__SERIALIZE__:STRUCT_INLINE=" #field
#define _SERIALIZE_CALLBACK(type) "__SERIALIZE__:CALLBACK=" #type
#define _SERIALIZE_STRUCT(type) "__SERIALIZE__:STRUCT=" #type
#define _SERIALIZE_CUSTOM_STRUCT(type) "__SERIALIZE__:CUSTOM_STRUCT=" #type
#define _SERIALIZE_CUSTOM_EXECUTE "__SERIALIZE__:CUSTOM_EXECUTE"


#define _SERIALIZE_GROUP(group) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:GROUP=" #group
#define _SERIALIZE_FUNC(name) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:FUNC=" #name
#define _SERIALIZE_RAW_STRUCT(type) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:RAW_STRUCT=" #type
#define _SERIALIZE_OPAQUE_STRUCT(type) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:OPAQUE_STRUCT=" #type
#define _SERIALIZE_FILTERED_STRUCT(type, buf_size, encoder, decoder) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:FILTERED_STRUCT=" #type "`" #buf_size "`" #encoder "`" #decoder
#define _SERIALIZE_ENUM(type) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:ENUM=" #type
#define _SERIALIZE_FIELD_TYPE(struct, type, name) static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:FIELD_TYPE=" #struct "`" #type "`" #name


#define SERIALIZE(...) _SERIALIZE_ ## __VA_ARGS__

#include "nrf_rpc.h"
#include "nrf_rpc_cbor.h"

#undef NRF_RPC_CBOR_CMD_DECODER
#define NRF_RPC_CBOR_CMD_DECODER(grp, name, id, handler, ...) \
	static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:REGISTER_DECODER=" #handler

#undef NRF_RPC_CBOR_EVT_DECODER
#define NRF_RPC_CBOR_EVT_DECODER(grp, name, id, handler, ...) \
	static const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:REGISTER_DECODER=" #handler

#endif
