


#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_cli.c"));


static void test1_rpc_handler(CborValue *_value, void *_handler_data)            /*####%BsgD*/
{                                                                                /*#####@rXg*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AW*/
	size_t _buffer_size_max = 1;                                             /*#######nh*/
	bool _result;                                                            /*#######al*/
	int x;                                                                   /*#######@U*/

	ser_decode_int(_value, &x);                                              /*##CjgxBkU*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = test1(x);                                                      /*##DvihPAY*/

	ser_encode_bool(&_ctx.encoder, &_result);                                /*##DND+Gd0*/

	return;                                                                  /*######%B1*/
decoding_error:                                                                  /*#######W+*/
	report_decoding_error(BT_RPC_GAP_TEST1_CMD, _handler_data);              /*#######DZ*/
}                                                                                /*#######@U*/

static void bt_enable_rpc_handler(CborValue *_value, void *_handler_data)        /*####%Bims*/
{                                                                                /*#####@U54*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AS*/
	size_t _buffer_size_max = 5;                                             /*######rYb*/
	int _result;                                                             /*######@uM*/

	nrf_rpc_cbor_decoding_done(_value);                                      /*##AGkSPWY*/

	_result = bt_enable();                                                   /*##DhG6/cg*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

}                                                                                /*##B9ELNqo*/

static void bt_set_name_rpc_handler(CborValue *_value, void *_handler_data)      /*####%BgLQ*/
{                                                                                /*#####@EdU*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AS*/
	size_t _buffer_size_max = 5;                                             /*######rYb*/
	int _result;                                                             /*######@uM*/

	char name[ser_decode_str_len(_value) + 1];                               /*####%CnvL*/
	ser_decode_str(_value, name);                                            /*#####@6BA*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_set_name(name);                                             /*##Du4mNCE*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

	return;                                                                  /*######%B1*/
decoding_error:                                                                  /*#######3e*/
	report_decoding_error(BT_RPC_GAP_BT_SET_NAME_CMD, _handler_data);        /*#######UH*/
}                                                                                /*#######@I*/


static bool bt_get_name_out(char *name, size_t size)
{
	const char *src;

	src = bt_get_name();

	if (src == NULL) {
		strcpy(name, "");
		return false;
	} else {
		strncpy(name, src, size);
		return true;
	}
}


static void bt_get_name_out_rpc_handler(CborValue *_value, void *_handler_data)  /*####%Buty*/
{                                                                                /*#####@Q0A*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	size_t _buffer_size_max = 6;                                             /*#######Yf*/
	bool _result;                                                            /*#######ER*/
	size_t size;                                                             /*#######Kc*/
	size_t _name_strlen;                                                     /*########@*/

	ser_decode_uint(_value, &size);                                          /*##Cik2Bmo*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	char name[size + 1];                                                     /*##D1X7iow*/

	_result = bt_get_name_out(size, name);                                   /*##Dn/FY8s*/

	_name_strlen = strlen(name);                                             /*####%CFOk*/
	_buffer_size_max += _name_strlen;                                        /*#####@f8c*/

	ser_encode_bool(&_ctx.encoder, &_result);                                /*####%DGKF*/
	ser_encode_str(&_ctx.encoder, name, -1);                                 /*#####@raA*/

	return;                                                                  /*######%B+*/
decoding_error:                                                                  /*#######0j*/
	report_decoding_error(BT_RPC_GAP_BT_GET_NAME_OUT_CMD, _handler_data);    /*#######94*/
}                                                                                /*#######@Y*/

