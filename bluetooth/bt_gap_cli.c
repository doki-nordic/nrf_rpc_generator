


#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_host.c"));
SERIALIZE(CMD_ID(BT_RPC_GAP_, _CMD));
SERIALIZE(EVT_ID(BT_RPC_GAP_, _EVT));
SERIALIZE(GROUP(bt_rpc_grp));

SERIALIZE(STRUCT_RAW(bt_addr_le_t));

#define MAX_BT_NAME_LENGTH 128

static char bt_name_cache[MAX_BT_NAME_LENGTH];


bool test1(int x)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ab*/
	size_t _buffer_size_max = 5;                                             /*######a4l*/
	bool _result;                                                            /*######@m8*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, x);                                        /*##A5xD4Ck*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_TEST1_CMD,               /*####%BAU3*/
		&_ctx, ser_rsp_simple_bool, &_result);                           /*#####@9QM*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_enable(bt_ready_cb_t cb)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AS*/
	size_t _buffer_size_max = 5;                                             /*######rYb*/
	int _result;                                                             /*######@uM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_callback(&_ctx.encoder, cb);                                  /*##AxNS7A4*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_BT_ENABLE_CMD,           /*####%BBu1*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@lnE*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_set_name(const char *name)
{
	SERIALIZE(STR(name));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AU*/
	size_t _buffer_size_max = 5;                                             /*#######n7*/
	size_t _name_strlen;                                                     /*#######XX*/
	int _result;                                                             /*#######@c*/

	_name_strlen = strlen(name);                                             /*####%CFOk*/
	_buffer_size_max += _name_strlen;                                        /*#####@f8c*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_str(&_ctx.encoder, name, _name_strlen);                       /*##A/RUZRo*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_BT_SET_NAME_CMD,         /*####%BOrA*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@Slg*/

	return _result;                                                          /*##BX7TDLc*/
}


struct bt_get_name_out_rpc_res                                                   /*####%Bp8J*/
{                                                                                /*#####@zYc*/

	bool _result;                                                            /*######%CW*/
	size_t size;                                                             /*######pln*/
	char * name;                                                             /*######@DY*/

};                                                                               /*##B985gv0*/

static void bt_get_name_out_rpc_rsp(CborValue *_value, void *_handler_data)      /*####%BoL7*/
{                                                                                /*#####@fvE*/

	struct entropy_get_result *_res =                                        /*####%Afvn*/
		(struct bt_get_name_out_rpc_res *)_handler_data;                 /*#####@jyA*/

	ser_decode_bool(_value, &_res->_result);                                 /*####%Daor*/
	ser_decode_str(_value, _res->name, (_res->size));                        /*#####@roM*/

}                                                                                /*##B9ELNqo*/

static bool bt_get_name_out(char *name, size_t size)
{
	SERIALIZE(OUT(name));
	SERIALIZE(STR(name));
	SERIALIZE(SIZE_PARAM(name, size));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ad*/
	size_t _buffer_size_max = 5;                                             /*######bcZ*/
	struct bt_get_name_out_rpc_res _result;                                  /*######@j8*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, size);                                    /*##A2nPHkE*/

	_result.size = size;                                                     /*####%C5M6*/
	_result.name = name;                                                     /*#####@4K4*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_BT_GET_NAME_OUT_CMD,     /*####%BBLa*/
		&_ctx, bt_get_name_out_rpc_rsp, &_result);                       /*#####@vEg*/

	return _result._result;                                                  /*##BW0ge3U*/
}


const char *bt_get_name(void)
{
	bool not_null;

	not_null = bt_get_name_out(bt_name_cache, sizeof(bt_name_cache));
	return not_null ? bt_name_cache : NULL;
}


int bt_set_id_addr(const bt_addr_le_t *addr)
{
	//SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AS*/
	size_t _buffer_size_max = 5;                                             /*######rYb*/
	int _result;                                                             /*######@uM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, (*addr));                                  /*##A796MRY*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_CMD_BT_SET_ID_ADDR_ID,   /*####%BAWD*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@Lwg*/

	return _result;                                                          /*##BX7TDLc*/
}
