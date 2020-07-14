
#include <stdint.h>
#include <stddef.h>

SERIALIZE(GROUP(sample_group));
SERIALIZE(CMD_ID(SER_SAMPLE_CMD_, _ID));
SERIALIZE(EVT_ID(SER_SAMPLE_EVT_, _ID));



int test_func(uint8_t *data, size_t size, int p0, int extra)
{
	SERIALIZE(INOUT(data));

	struct nrf_rpc_cbor_ctx _ctx;                                                    /*######%AT*/
	size_t _buffer_size_max = 17;                                                    /*######Evj*/
	int _result;                                                                     /*######@LU*/

	NRF_RPC_CBOR_ALLOC(ctx, cbor_buffer_size_max);                                   /*##Ao2jWI0*/

	ser_encode_uint(&ctx.encoder, (*data));                                          /*######%A2*/
	ser_encode_uint(&ctx.encoder, size);                                             /*#######8i*/
	ser_encode_int(&ctx.encoder, p0);                                                /*#######3u*/
	ser_encode_int(&ctx.encoder, extra);                                             /*#######@I*/

	nrf_rpc_cbor_cmd_no_err(&GROUP, SER_SAMPLE_ID, &_ctx, abc, &result);             /*##BOjH17c*/

	return _result;                                                                  /*##BX7TDLc*/
}
