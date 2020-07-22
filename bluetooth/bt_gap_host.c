
#include <stdint.h>

#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_cli.c"));


static void test1_rpc_handler(CborValue *_value, void *_handler_data)            /*####%BsgD*/
{                                                                                /*#####@rXg*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AR*/
	bool _result;                                                            /*#######jF*/
	int x;                                                                   /*#######Pu*/
	size_t _buffer_size_max = 1;                                             /*#######@w*/

	ser_decode_int(_value, &x);                                              /*##CjgxBkU*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = test1(x);                                                      /*##DvihPAY*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_bool(&_ctx.encoder, &_result);                                /*##DND+Gd0*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*######%B/*/
decoding_error:                                                                  /*#######YT*/
	report_decoding_error(TEST1_RPC_CMD, _handler_data);                     /*#######bN*/
}                                                                                /*#######@0*/


static void bt_enable_rpc_handler(CborValue *_value, void *_handler_data)        /*####%Bims*/
{                                                                                /*#####@U54*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	nrf_rpc_cbor_decoding_done(_value);                                      /*##AGkSPWY*/

	_result = bt_enable();                                                   /*##DhG6/cg*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

}                                                                                /*##B9ELNqo*/


static void bt_set_name_rpc_handler(CborValue *_value, void *_handler_data)      /*####%BgLQ*/
{                                                                                /*#####@EdU*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	char name[ser_decode_str_len(_value) + 1];                               /*####%CnvL*/
	ser_decode_str(_value, name);                                            /*#####@6BA*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_set_name(name);                                             /*##Du4mNCE*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*######%B7*/
decoding_error:                                                                  /*#######BB*/
	report_decoding_error(BT_SET_NAME_RPC_CMD, _handler_data);               /*#######fU*/
}                                                                                /*#######@Y*/


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
	bool _result;                                                            /*#######fe*/
	size_t size;                                                             /*#######Dy*/
	size_t _name_strlen;                                                     /*#######q8*/
	size_t _buffer_size_max = 6;                                             /*########@*/

	ser_decode_uint(_value, &size);                                          /*##Cik2Bmo*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	char name[size + 1];                                                     /*##D1X7iow*/

	_result = bt_get_name_out(size, name);                                   /*##Dn/FY8s*/

	_name_strlen = strlen(name);                                             /*####%CFOk*/
	_buffer_size_max += _name_strlen;                                        /*#####@f8c*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_bool(&_ctx.encoder, &_result);                                /*####%DI6/*/
	ser_encode_str(&_ctx.encoder, name, _name_strlen);                       /*#####@Xnk*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*######%B1*/
decoding_error:                                                                  /*#######bO*/
	report_decoding_error(BT_GET_NAME_OUT_RPC_CMD, _handler_data);           /*#######kq*/
}                                                                                /*#######@0*/

static void bt_id_delete_rpc_handler(CborValue *_value, void *_handler_data)     /*####%Bn4o*/
{                                                                                /*#####@h9M*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AV*/
	int _result;                                                             /*#######o2*/
	uint8_t id;                                                              /*#######yK*/
	size_t _buffer_size_max = 5;                                             /*#######@8*/

	ser_decode_uint(_value, &id);                                            /*##CufGSKw*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_id_delete(id);                                              /*##DilWuoM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*######%B5*/
decoding_error:                                                                  /*#######iP*/
	report_decoding_error(BT_ID_DELETE_RPC_CMD, _handler_data);              /*#######YY*/
}                                                                                /*#######@Y*/

static void bt_le_adv_stop_rpc_handler(CborValue *_value, void *_handler_data)   /*####%BlBQ*/
{                                                                                /*#####@4G8*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	nrf_rpc_cbor_decoding_done(_value);                                      /*##AGkSPWY*/

	_result = bt_le_adv_stop();                                              /*##Du9+9xY*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

}                                                                                /*##B9ELNqo*/

static void bt_le_scan_stop_rpc_handler(CborValue *_value, void *_handler_data)  /*####%BjGj*/
{                                                                                /*#####@TTc*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	nrf_rpc_cbor_decoding_done(_value);                                      /*##AGkSPWY*/

	_result = bt_le_scan_stop();                                             /*##DjTVckk*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*##DD1pDyU*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

}                                                                                /*##B9ELNqo*/


static void bt_le_whitelist_clear_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bs7f*/
{                                                                                    /*#####@2mU*/

	struct nrf_rpc_cbor_ctx _ctx;                                                /*######%Aa*/
	int _result;                                                                 /*######Qso*/
	size_t _buffer_size_max = 5;                                                 /*######@uA*/

	nrf_rpc_cbor_decoding_done(_value);                                          /*##AGkSPWY*/

	_result = bt_le_whitelist_clear();                                           /*##DjMtZbs*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                                  /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                     /*##DD1pDyU*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                              /*##BFLm1vw*/

}                                                                                    /*##B9ELNqo*/

static void bt_le_scan_recv(const struct bt_le_scan_recv_info *info,
		     struct net_buf_simple *buf)
{
	//SERIALIZE(); TODO
}

static void bt_le_scan_timeout(void)
{
	//SERIALIZE();
}


static struct bt_le_scan_cb scan_cb_send = {
	.recv = bt_le_scan_recv,
	.timeout = bt_le_scan_timeout,
}

static void bt_le_scan_cb_reg_enable(void)
{
	bt_le_scan_cb_register(&scan_cb_send);
}

static void bt_le_scan_cb_reg_enable_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bq/A*/
{                                                                                       /*#####@9a4*/

	struct nrf_rpc_cbor_ctx _ctx;                                                   /*####%ATMv*/
	size_t _buffer_size_max = 0;                                                    /*#####@1d4*/

	nrf_rpc_cbor_decoding_done(_value);                                             /*##AGkSPWY*/

	bt_le_scan_cb_reg_enable();                                                     /*##DmupDOk*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                                     /*##AvrU03s*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                                 /*##BFLm1vw*/

}                                                                                       /*##B9ELNqo*/

