
#include <stdint.h>

#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_host.c"));
SERIALIZE(CMD_ID(, _RPC_CMD));
SERIALIZE(EVT_ID(, _RPC_EVT));
SERIALIZE(GROUP(bt_rpc_grp));

SERIALIZE(STRUCT_RAW(bt_addr_le_t));

#define MAX_BT_NAME_LENGTH 128


int bt_enable(bt_ready_cb_t cb)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_callback(&_ctx.encoder, cb, bt_ready_cb_t_decoder);           /*##A7a4EXE*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ENABLE_RPC_CMD,                  /*####%BKRK*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@M9g*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_set_name(const char *name)
{
	SERIALIZE(STR(name));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	size_t _name_strlen;                                                     /*#######bk*/
	int _result;                                                             /*#######HH*/
	size_t _scratchpad_size = 0;                                             /*#######eY*/
	size_t _buffer_size_max = 10;                                            /*########@*/

	_name_strlen = strlen(name);                                             /*####%CFOk*/
	_buffer_size_max += _name_strlen;                                        /*#####@f8c*/

	_scratchpad_size += SCRATCHPAD_ALIGN(_name_strlen + 1);                  /*##EObZcVc*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

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

	_res->_result = ser_decode_bool(_value);                                 /*####%DYid*/
	ser_decode_str(_value, _res->name, (_res->size));                        /*#####@AWM*/

}                                                                                /*##B9ELNqo*/

static bool bt_get_name_out(char *name, size_t size)
{
	SERIALIZE(OUT(name));
	SERIALIZE(STR(name));
	SERIALIZE(SIZE_PARAM(name, size));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ac*/
	struct bt_get_name_out_rpc_res _result;                                  /*######WP+*/
	size_t _buffer_size_max = 5;                                             /*######@64*/

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


int bt_set_id_addr(const bt_addr_le_t *addr)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AR*/
	int _result;                                                             /*######RDP*/
	size_t _buffer_size_max = 3;                                             /*######@sI*/

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*##CHCgvU0*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*##A9rQrRg*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_SET_ID_ADDR_RPC_CMD,             /*####%BG6w*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@qi0*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_id_delete(uint8_t id)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AW*/
	int _result;                                                             /*######sOU*/
	size_t _buffer_size_max = 2;                                             /*######@TQ*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, id);                                      /*##A9BnOB4*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_DELETE_RPC_CMD,               /*####%BNTu*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@K8I*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_adv_stop(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AX*/
	int _result;                                                             /*######56+*/
	size_t _buffer_size_max = 0;                                             /*######@io*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_ADV_STOP_RPC_CMD,             /*####%BIpx*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@3mk*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_scan_stop(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AX*/
	int _result;                                                             /*######56+*/
	size_t _buffer_size_max = 0;                                             /*######@io*/

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

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AX*/
	int _result;                                                             /*######56+*/
	size_t _buffer_size_max = 0;                                             /*######@io*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_WHITELIST_CLEAR_RPC_CMD,      /*####%BBSY*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@ClQ*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_set_chan_map(uint8_t chan_map[5])
{
	SERIALIZE(SIZE(chan_map, 5));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Af*/
	size_t _chan_map_size;                                                   /*#######hK*/
	int _result;                                                             /*#######Kr*/
	size_t _buffer_size_max = 5;                                             /*#######@Y*/

	_chan_map_size = sizeof(uint8_t) * 5;                                    /*####%CGlM*/
	_buffer_size_max += _chan_map_size;                                      /*#####@6lU*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_buffer(&_ctx.encoder, chan_map, _chan_map_size);              /*##A4qUuxk*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_SET_CHAN_MAP_RPC_CMD,         /*####%BCta*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@i9M*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_br_discovery_stop(void)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AX*/
	int _result;                                                             /*######56+*/
	size_t _buffer_size_max = 0;                                             /*######@io*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_BR_DISCOVERY_STOP_RPC_CMD,       /*####%BB+X*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@piE*/

	return _result;                                                          /*##BX7TDLc*/
}

int bt_br_set_discoverable(bool enable)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AU*/
	int _result;                                                             /*######ulr*/
	size_t _buffer_size_max = 1;                                             /*######@jw*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_bool(&_ctx.encoder, enable);                                  /*##AyLT/1M*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_BR_SET_DISCOVERABLE_RPC_CMD,     /*####%BLiS*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@PD8*/

	return _result;                                                          /*##BX7TDLc*/
}

int bt_br_set_connectable(bool enable)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AU*/
	int _result;                                                             /*######ulr*/
	size_t _buffer_size_max = 1;                                             /*######@jw*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_bool(&_ctx.encoder, enable);                                  /*##AyLT/1M*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_BR_SET_CONNECTABLE_RPC_CMD,      /*####%BIit*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@tFI*/

	return _result;                                                          /*##BX7TDLc*/
}

static void bt_le_scan_timeout_rpc_handler(CborValue *_value, void *_handler_data)/*####%BrQA*/
{                                                                                 /*#####@ZO0*/

	nrf_rpc_cbor_decoding_done(_value);                                       /*##AGkSPWY*/

	bt_le_scan_timeout();                                                     /*##DtLGskE*/

	ser_rsp_send_void();                                                      /*##BEYGLxw*/

}                                                                                 /*##B9ELNqo*/


struct bt_id_get_rpc_res                                                         /*####%Bmxc*/
{                                                                                /*#####@ZAo*/

	size_t * count;                                                          /*####%CcVF*/
	bt_addr_le_t * addrs;                                                    /*#####@Nco*/

};                                                                               /*##B985gv0*/

static void bt_id_get_rpc_rsp(CborValue *_value, void *_handler_data)                 /*####%BluI*/
{                                                                                     /*#####@1Q4*/

	struct entropy_get_result *_res =                                             /*####%AdCx*/
		(struct bt_id_get_rpc_res *)_handler_data;                            /*#####@7T0*/

	*(_res->count) = ser_decode_uint(_value);                                     /*####%DZiC*/
	ser_decode_buffer(_value, _res->addrs, *(_res->count) * sizeof(bt_addr_le_t));/*#####@Ip8*/

}                                                                                     /*##B9ELNqo*/

void bt_id_get(bt_addr_le_t *addrs, size_t *count)
{
	SERIALIZE(OUT(addrs));
	SERIALIZE(SIZE_PARAM_EX(addrs, *$, count));
	SERIALIZE(INOUT(count));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	struct bt_id_get_rpc_res _result;                                        /*######AE2*/
	size_t _buffer_size_max = 5;                                             /*######@f0*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, *count);                                  /*##A0IY0+8*/

	_result.count = count;                                                   /*####%C9M/*/
	_result.addrs = addrs;                                                   /*#####@zo8*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_GET_RPC_CMD,                  /*####%BLnI*/
		&_ctx, bt_id_get_rpc_rsp, &_result);                             /*#####@SsE*/
}


struct bt_id_create_rpc_res                                                      /*####%BoIG*/
{                                                                                /*#####@PlM*/

	int _result;                                                             /*####%CZCW*/
	bt_addr_le_t * addr;                                                     /*#####@VRk*/

};                                                                               /*##B985gv0*/

static void bt_id_create_rpc_rsp(CborValue *_value, void *_handler_data)         /*####%BqRo*/
{                                                                                /*#####@GSg*/

	struct entropy_get_result *_res =                                        /*####%AWX7*/
		(struct bt_id_create_rpc_res *)_handler_data;                    /*#####@L6A*/

	_res->_result = ser_decode_int(_value);                                  /*####%DfP8*/
	ser_decode_buffer(_value, _res->addr, sizeof(bt_addr_le_t));             /*#####@JwU*/

}                                                                                /*##B9ELNqo*/

int bt_id_create(bt_addr_le_t *addr, uint8_t *irk)
{
	SERIALIZE(NULLABLE(addr));
	SERIALIZE(INOUT(addr));
	SERIALIZE(NULLABLE(irk));
	SERIALIZE(INOUT(irk));
	SERIALIZE(SIZE(irk, 16));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AX*/
	size_t _irk_size;                                                        /*#######sg*/
	struct bt_id_create_rpc_res _result;                                     /*#######gW*/
	size_t _buffer_size_max = 8;                                             /*#######@g*/

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*######%CB*/
	_irk_size = !irk ? 0 : sizeof(uint8_t) * 16;                             /*######cKg*/
	_buffer_size_max += _irk_size;                                           /*######@u4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*####%Ax4U*/
	ser_encode_buffer(&_ctx.encoder, irk, _irk_size);                        /*#####@/YU*/

	_result.addr = addr;                                                     /*##C3EHO0A*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_CREATE_RPC_CMD,               /*####%BMH+*/
		&_ctx, bt_id_create_rpc_rsp, &_result);                          /*#####@log*/

	return _result._result;                                                  /*##BW0ge3U*/
}