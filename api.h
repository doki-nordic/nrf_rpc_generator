

int func(int x);


/*
//#include "nonex.h"

//#define SERIALIZABLE(...) __attribute__((deprecated("RP_SER_GEN_SERIALIZABLE" __VA_ARGS__)))
#define SERIALIZABLE(...) "RP_SER_GEN:" "SERIALIZABLE" __VA_ARGS__
#define SERIALIZE_PRE "RP_SER_GEN:" "PRE_SERIALIZE"


#define __SERIALIZATION_PRE "PREPRE"
#define __SERIALIZATION_POST "PSOT"
#define __SERIALIZATION_END "ENDNED"
#define __SERIALIZATION_ENCODER __attribute__((deprecated("RP_SER_GEN_SERIALIZABLE")))
#define __SERIALIZATION_DECODER __attribute__((deprecated("RP_SER_GEN_SERIALIZABLE")))

#define SERIALIZATION(arg) __SERIALIZATION_ ## arg

#ifndef SERIALIZATION
#define SERIALIZATION(...)
#endif


SERIALIZATION(ENCODER)
int funct(int x)
{
	int result;
	SERIALIZATION(PRE);
	SERIALIZATION(END);
	//...
	SERIALIZATION(POST);
	SERIALIZATION(END);
	return result;
}

#define AUTOGENERATED(...) "AUTOGENERATED:" __VA_ARGS__

#ifndef AUTOGENERATED
#define AUTOGENERATED(...)
#endif


SERIALIZATION(DECODER)
void funct_decoder(CborDecoder* in)
{
	/ * AUTOGENERATED: local variables * /
	int param_x;
	int result;
	/ * AUTOGENERATED: end * /

	AUTOGENERATED("decode input");
	param_x = cbor_decode_int(in);
	AUTOGENERATED("end");

	AUTOGENERATED("call target");
	result = funct(param_x);
	AUTOGENERATED("end");

	AUTOGENERATED("encode output");
	CREATE_CBOR_ENCODER(out, 5);
	cbor_encode(out, result);
	AUTOGENERATED("end");

	AUTOGENERATED("send response");
	send_response(out);
	AUTOGENERATED("end");
}

*/
