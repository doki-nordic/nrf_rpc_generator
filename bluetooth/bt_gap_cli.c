
#include <stdint.h>

#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_host.c"));
SERIALIZE(CMD_ID(, _RPC_CMD));
SERIALIZE(EVT_ID(, _RPC_EVT));
SERIALIZE(GROUP(bt_rpc_grp));

SERIALIZE(STRUCT_RAW(bt_addr_le_t));

#define MAX_BT_NAME_LENGTH 128


bool test1(int x)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ab*/
	size_t _buffer_size_max = 5;                                             /*######a4l*/
	bool _result;                                                            /*######@m8*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, x);                                        /*##A5xD4Ck*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, TEST1_RPC_CMD,                      /*####%BBUH*/
		&_ctx, ser_rsp_simple_bool, &_result);                           /*#####@6I4*/

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

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ENABLE_RPC_CMD,                  /*####%BKRK*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@M9g*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_set_name(const char *name)
{
	SERIALIZE(STR(name));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AV*/
	size_t _name_strlen;                                                     /*#######qo*/
	size_t _buffer_size_max = 5;                                             /*#######GI*/
	int _result;                                                             /*#######@o*/

	_name_strlen = strlen(name);                                             /*####%CFOk*/
	_buffer_size_max += _name_strlen;                                        /*#####@f8c*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_str(&_ctx.encoder, name, _name_strlen);                       /*##A/RUZRo*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_SET_NAME_RPC_CMD,                /*####%BKgG*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@0j4*/

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

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_GET_NAME_OUT_RPC_CMD,            /*####%BG0o*/
		&_ctx, bt_get_name_out_rpc_rsp, &_result);                       /*#####@sSg*/

	return _result._result;                                                  /*##BW0ge3U*/
}


const char *bt_get_name(void)
{
	static char bt_name_cache[MAX_BT_NAME_LENGTH];
	bool not_null;

	not_null = bt_get_name_out(bt_name_cache, sizeof(bt_name_cache));
	return not_null ? bt_name_cache : NULL;
}


int bt_id_delete(uint8_t id)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AX*/
	size_t _buffer_size_max = 2;                                             /*######eNB*/
	int _result;                                                             /*######@yA*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, id);                                      /*##A9BnOB4*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_DELETE_RPC_CMD,               /*####%BNTu*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@K8I*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_adv_stop(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AW*/
	size_t _buffer_size_max = 0;                                             /*######t1n*/
	int _result;                                                             /*######@QM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_ADV_STOP_RPC_CMD,             /*####%BIpx*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@3mk*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_scan_stop(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AW*/
	size_t _buffer_size_max = 0;                                             /*######t1n*/
	int _result;                                                             /*######@QM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_SCAN_STOP_RPC_CMD,            /*####%BG0S*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@eME*/

	return _result;                                                          /*##BX7TDLc*/
}


static void bt_le_scan_cb_reg_enable(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*####%ATMv*/
	size_t _buffer_size_max = 0;                                             /*#####@1d4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_SCAN_CB_REG_ENABLE_RPC_CMD,   /*####%BHTs*/
		&_ctx, ser_rsp_simple_void, NULL);                               /*#####@6UQ*/
}


static node_t scan_cb_list;


void bt_le_scan_cb_register(struct bt_le_scan_cb *cb)
{
	bool wasEmpty = true; // TODO: check if list is empty
	// TODO: add to scan_cb_list
	if (wasEmpty) {
		bt_le_scan_cb_reg_enable();
	}
}


int bt_le_whitelist_clear(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AW*/
	size_t _buffer_size_max = 0;                                             /*######t1n*/
	int _result;                                                             /*######@QM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_WHITELIST_CLEAR_RPC_CMD,      /*####%BBSY*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@ClQ*/

	return _result;                                                          /*##BX7TDLc*/
}

