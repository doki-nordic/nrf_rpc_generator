
#include <stdint.h>
#include <stddef.h>

int test_func(uint8_t *data, size_t size, int u6g)
{
	SERIALIZE(OUT(data));

	struct nrf_rpc_cbor_ctx _ctx;                                                    /*######%AW*/
	size_t _buffer_size_max = 10;                                                    /*######Nbw*/
	int _result;                                                                     /*######@9Q*/

	NRF_RPC_CBOR_ALLOC(ctx, cbor_buffer_size_max);                                   /*##Ao2jWI0*/

	ser_encode_uint(&ctx.encoder, size);                                             /*####%AydB*/
	ser_encode_int(&ctx.encoder, u6g);                                               /*#####@owo*/

	nrf_rpc_cbor_cmd_no_err(&GROUP, SER_SAMPLE_ID, &_ctx, abc, &result);             /*##BOjH17c*/

	return _result;                                                                  /*##BX7TDLc*/
}
