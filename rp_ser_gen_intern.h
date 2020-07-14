#ifndef _RP_SER_GEN_INTERN_H
#define _RP_SER_GEN_INTERN_H

#define _SERIALIZE__CONCAT2(a, b, c) a ## _ ## b ## _ ## c
#define _SERIALIZE__CONCAT(a, b, c) _SERIALIZE__CONCAT2(a, b, c)
#define _SERIALIZE__UNIQUE() _SERIALIZE__CONCAT(_serialize_unique, __COUNTER__, __LINE__)

#define _SERIALIZE_ "__SERIALIZE__:USE"
#define _SERIALIZE_OUT(x) "__SERIALIZE__:OUT=" #x
#define _SERIALIZE_INOUT(x) "__SERIALIZE__:INOUT=" #x
#define _SERIALIZE_ARRAY_SIZE(x) "__SERIALIZE__:ARRAY_SIZE=" #x

#define _SERIALIZE_HOST_FILE(file) const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:HOST_FILE=" file
#define _SERIALIZE_CLIENT_FILE(file) const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:CLIENT_FILE=" file

#define SERIALIZE(...) _SERIALIZE_ ## __VA_ARGS__

//#include "nrf_rpc.h"

#undef NRF_RPC_CBOR_CMD_DECODER
#define NRF_RPC_CBOR_CMD_DECODER(grp, name, id, handler, param) \
	const char* _SERIALIZE__UNIQUE() = "__SERIALIZE__:REGISTER_DECODER=" #handler

#endif
