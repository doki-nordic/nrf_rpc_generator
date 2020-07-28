
#include <stdint.h>

#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_host.c"));
SERIALIZE(CMD_ID(, _RPC_CMD));
SERIALIZE(EVT_ID(, _RPC_EVT));
SERIALIZE(GROUP(bt_rpc_grp));

SERIALIZE(STRUCT_RAW(bt_addr_le_t));
SERIALIZE(OPAQUE_STRUCT(struct bt_le_ext_adv));

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

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AY*/
	struct bt_get_name_out_rpc_res _result;                                  /*#######X2*/
	size_t _scratchpad_size = 0;                                             /*#######Jd*/
	size_t _buffer_size_max = 10;                                            /*#######@I*/

	_scratchpad_size += SCRATCHPAD_ALIGN(size);                              /*##EB+3ycA*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

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

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	size_t _chan_map_size;                                                   /*#######Sz*/
	int _result;                                                             /*#######xQ*/
	size_t _scratchpad_size = 0;                                             /*#######pA*/
	size_t _buffer_size_max = 10;                                            /*########@*/

	_chan_map_size = sizeof(uint8_t) * 5;                                    /*####%CGlM*/
	_buffer_size_max += _chan_map_size;                                      /*#####@6lU*/

	_scratchpad_size += SCRATCHPAD_ALIGN(_chan_map_size);                    /*##EJHhpZ4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

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

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AU*/
	struct bt_id_get_rpc_res _result;                                        /*#######FC*/
	size_t _scratchpad_size = 0;                                             /*#######Cq*/
	size_t _buffer_size_max = 10;                                            /*#######@o*/

	_scratchpad_size += SCRATCHPAD_ALIGN(*count * sizeof(bt_addr_le_t));     /*##EM0RMfw*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	ser_encode_uint(&_ctx.encoder, *count);                                  /*##A0IY0+8*/

	_result.count = count;                                                   /*####%C9M/*/
	_result.addrs = addrs;                                                   /*#####@zo8*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_GET_RPC_CMD,                  /*####%BLnI*/
		&_ctx, bt_id_get_rpc_rsp, &_result);                             /*#####@SsE*/
}


uint8_t generate_addr_irk_flags(bt_addr_le_t *addr, uint8_t *irk)
{
	uint8_t flags = 0;

	if (irk) {
		for (int i = 0; i < 16; i++) {
			if (irk[i] != 0) {
				flags |= 1;
				break;
			}
		}
	}

	if (addr) {
		static const bt_addr_le_t any = BT_ADDR_LE_ANY;
		if (memcmp(&any, addr) != 0) {
			flags |= 2;
		}
	}

	return flags;
}

struct bt_id_create_rpc_res                                                      /*####%BoIG*/
{                                                                                /*#####@PlM*/

	int _result;                                                             /*######%CT*/
	bt_addr_le_t * addr;                                                     /*######Jyh*/
	uint8_t * irk;                                                           /*######@+s*/

};                                                                               /*##B985gv0*/

static void bt_id_create_rpc_rsp(CborValue *_value, void *_handler_data)         /*####%BqRo*/
{                                                                                /*#####@GSg*/

	struct entropy_get_result *_res =                                        /*####%AWX7*/
		(struct bt_id_create_rpc_res *)_handler_data;                    /*#####@L6A*/

	_res->_result = ser_decode_int(_value);                                  /*######%DQ*/
	ser_decode_buffer(_value, _res->addr, sizeof(bt_addr_le_t));             /*######lk5*/
	ser_decode_buffer(_value, _res->irk, sizeof(uint8_t) * 16);              /*######@5o*/

}                                                                                /*##B9ELNqo*/

int bt_id_create(bt_addr_le_t *addr, uint8_t *irk)
{
	SERIALIZE(NULLABLE(addr));
	SERIALIZE(INOUT(addr));
	SERIALIZE(NULLABLE(irk));
	SERIALIZE(INOUT(irk));
	SERIALIZE(SIZE(irk, 16));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	size_t _irk_size;                                                        /*#######Xr*/
	struct bt_id_create_rpc_res _result;                                     /*#######Lm*/
	size_t _scratchpad_size = 0;                                             /*#######Ak*/
	size_t _buffer_size_max = 13;                                            /*########@*/

	size_t i;

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*######%CB*/
	_irk_size = !irk ? 0 : sizeof(uint8_t) * 16;                             /*######cKg*/
	_buffer_size_max += _irk_size;                                           /*######@u4*/

	_buffer_size_max += 1;

	_scratchpad_size += SCRATCHPAD_ALIGN(_irk_size);                         /*##ELqQtKI*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*####%Ax4U*/
	ser_encode_buffer(&_ctx.encoder, irk, _irk_size);                        /*#####@/YU*/

	ser_encode_uint(generate_addr_irk_flags(addr, irk));

	_result.addr = addr;                                                     /*####%CxTc*/
	_result.irk = irk;                                                       /*#####@QsM*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_CREATE_RPC_CMD,               /*####%BMH+*/
		&_ctx, bt_id_create_rpc_rsp, &_result);                          /*#####@log*/

	return _result._result;                                                  /*##BW0ge3U*/
}


struct bt_id_reset_rpc_res                                                       /*####%Bs1i*/
{                                                                                /*#####@i/M*/

	int _result;                                                             /*######%CT*/
	bt_addr_le_t * addr;                                                     /*######Jyh*/
	uint8_t * irk;                                                           /*######@+s*/

};                                                                               /*##B985gv0*/

static void bt_id_reset_rpc_rsp(CborValue *_value, void *_handler_data)          /*####%BoSY*/
{                                                                                /*#####@/xk*/

	struct entropy_get_result *_res =                                        /*####%AYXk*/
		(struct bt_id_reset_rpc_res *)_handler_data;                     /*#####@RoU*/

	_res->_result = ser_decode_int(_value);                                  /*######%DQ*/
	ser_decode_buffer(_value, _res->addr, sizeof(bt_addr_le_t));             /*######lk5*/
	ser_decode_buffer(_value, _res->irk, sizeof(uint8_t) * 16);              /*######@5o*/

}                                                                                /*##B9ELNqo*/

int bt_id_reset(u8_t id, bt_addr_le_t *addr, uint8_t *irk)
{
	SERIALIZE(INOUT(addr));
	SERIALIZE(NULLABLE(addr));
	SERIALIZE(INOUT(irk));
	SERIALIZE(NULLABLE(irk));
	SERIALIZE(SIZE(irk, 16));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	size_t _irk_size;                                                        /*#######VA*/
	struct bt_id_reset_rpc_res _result;                                      /*#######80*/
	size_t _scratchpad_size = 0;                                             /*#######ao*/
	size_t _buffer_size_max = 18;                                            /*########@*/

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*######%CB*/
	_irk_size = !irk ? 0 : sizeof(uint8_t) * 16;                             /*######cKg*/
	_buffer_size_max += _irk_size;                                           /*######@u4*/

	_scratchpad_size += SCRATCHPAD_ALIGN(_irk_size);                         /*##ELqQtKI*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	ser_encode_int(&_ctx.encoder, id);                                       /*######%Aw*/
	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*######bYz*/
	ser_encode_buffer(&_ctx.encoder, irk, _irk_size);                        /*######@+4*/

	ser_encode_uint(generate_addr_irk_flags(addr, irk));

	_result.addr = addr;                                                     /*####%CxTc*/
	_result.irk = irk;                                                       /*#####@QsM*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_ID_RESET_RPC_CMD,                /*####%BDbH*/
		&_ctx, bt_id_reset_rpc_rsp, &_result);                           /*#####@DeY*/

	return _result._result;                                                  /*##BW0ge3U*/
}


size_t bt_le_adv_param_sp_size(struct bt_le_adv_param *_data)                         /*####%Bhf3*/
{                                                                                     /*#####@LEI*/

	size_t _scratchpad_size = 0;                                                  /*##ATz5YrA*/

	_scratchpad_size += !_data->peer ? 0 : SCRATCHPAD_ALIGN(sizeof(bt_addr_le_t));/*##EL1Agek*/

	return _scratchpad_size;                                                      /*##BRWAmyU*/

}                                                                                     /*##B9ELNqo*/

size_t bt_le_adv_param_buf_size(struct bt_le_adv_param *_data)                   /*####%BvWP*/
{                                                                                /*#####@qjc*/

	size_t _buffer_size_max = 0;                                             /*##AW2oACE*/

	_buffer_size_max += !_data->peer ? 0 : 2 + sizeof(bt_addr_le_t);         /*##CASBfgs*/

	return _buffer_size_max;                                                 /*##BWmN6G8*/

}                                                                                /*##B9ELNqo*/

void bt_le_adv_param_enc(CborEncoder *_encoder, struct bt_le_adv_param *_data)   /*####%BmQP*/
{                                                                                /*#####@RZs*/

	SERIALIZE(STRUCT(struct bt_le_adv_param));
	SERIALIZE(NULLABLE(peer));

	SERIALIZE(STRUCT_BUFFER_CONST(22));                                      /*##EzbvbRY*/

	ser_encode_uint(_encoder, _data->id);                                    /*#######%A*/
	ser_encode_uint(_encoder, _data->sid);                                   /*#######zA*/
	ser_encode_uint(_encoder, _data->secondary_max_skip);                    /*########f*/
	ser_encode_uint(_encoder, _data->options);                               /*########O*/
	ser_encode_uint(_encoder, _data->interval_min);                          /*########T*/
	ser_encode_uint(_encoder, _data->interval_max);                          /*########M*/
	ser_encode_buffer(_encoder, &_data->peer, sizeof(bt_addr_le_t));         /*########@*/

}                                                                                /*##B9ELNqo*/


size_t bt_data_buf_size(struct bt_data *_data)                                   /*####%Bh+W*/
{                                                                                /*#####@gQY*/

	size_t _buffer_size_max = 0;                                             /*##AW2oACE*/

	_buffer_size_max += sizeof(uint8_t) * _data->data_len;                   /*##CBQ2Maw*/

	return _buffer_size_max;                                                 /*##BWmN6G8*/

}                                                                                /*##B9ELNqo*/

size_t bt_data_sp_size(struct bt_data *_data)                                    /*####%Bpd6*/
{                                                                                /*#####@6Xk*/

	size_t _scratchpad_size = 0;                                             /*##ATz5YrA*/

	_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(uint8_t) * _data->data_len); /*##EGeYfNs*/

	return _scratchpad_size;                                                 /*##BRWAmyU*/

}                                                                                /*##B9ELNqo*/

void bt_data_enc(CborEncoder *_encoder, struct bt_data *_data)                      /*####%Brep*/
{                                                                                   /*#####@Iq0*/

	SERIALIZE(STRUCT(struct bt_data));
	SERIALIZE(SIZE_PARAM(data, data_len));

	SERIALIZE(STRUCT_BUFFER_CONST(9));                                          /*##E6pA5Eo*/

	ser_encode_uint(_encoder, _data->type);                                     /*######%A8*/
	ser_encode_uint(_encoder, _data->data_len);                                 /*######fsW*/
	ser_encode_buffer(_encoder, _data->data, sizeof(uint8_t) * _data->data_len);/*######@EY*/

}                                                                                   /*##B9ELNqo*/


int bt_le_adv_start(const struct bt_le_adv_param *param,
		    const struct bt_data *ad, size_t ad_len,
		    const struct bt_data *sd, size_t sd_len)
{
	SERIALIZE(SIZE_PARAM(ad, ad_len));
	SERIALIZE(SIZE_PARAM(sd, sd_len));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	int _result;                                                             /*#######Vr*/
	size_t _scratchpad_size = 0;                                             /*#######45*/
	size_t _buffer_size_max = 37;                                            /*#######io*/
	size_t _i;                                                               /*########@*/

	_buffer_size_max += bt_le_adv_param_buf_size(param);                     /*########%*/
	for (_i = 0; _i < ad_len; _i++) {                                        /*########C*/
		_buffer_size_max += bt_data_buf_size(&ad[_i]);                   /*########N*/
		_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(struct bt_data));    /*########1*/
		_scratchpad_size += bt_data_sp_size(&ad[_i]);                    /*########8*/
	}                                                                        /*########F*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*########7*/
		_buffer_size_max += bt_data_buf_size(&sd[_i]);                   /*########o*/
		_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(struct bt_data));    /*#########*/
		_scratchpad_size += bt_data_sp_size(&sd[_i]);                    /*#########*/
	}                                                                        /*########@*/

	_scratchpad_size += bt_le_adv_param_sp_size(param);                      /*##EEZ/Gv4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	bt_le_adv_param_enc(&_ctx.encoder, param);                               /*########%*/
	ser_encode_uint(&_ctx.encoder, ad_len);                                  /*########A*/
	for (_i = 0; _i < ad_len; _i++) {                                        /*########+*/
		bt_data_enc(&_ctx.encoder, &ad[_i]);                             /*########o*/
	}                                                                        /*########a*/
	ser_encode_uint(&_ctx.encoder, sd_len);                                  /*########E*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*########B*/
		bt_data_enc(&_ctx.encoder, &sd[_i]);                             /*########g*/
	}                                                                        /*########@*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_ADV_START_RPC_CMD,            /*####%BNew*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@Uc8*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_adv_update_data(const struct bt_data *ad, size_t ad_len,
			  const struct bt_data *sd, size_t sd_len)
{
	SERIALIZE(SIZE_PARAM(ad, ad_len));
	SERIALIZE(SIZE_PARAM(sd, sd_len));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	int _result;                                                             /*#######dr*/
	size_t _scratchpad_size = 0;                                             /*#######k2*/
	size_t _buffer_size_max = 15;                                            /*#######pA*/
	size_t _i;                                                               /*########@*/

	for (_i = 0; _i < ad_len; _i++) {                                        /*########%*/
		_buffer_size_max += bt_data_buf_size(&ad[_i]);                   /*########C*/
		_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(struct bt_data));    /*########J*/
		_scratchpad_size += bt_data_sp_size(&ad[_i]);                    /*########/*/
	}                                                                        /*########m*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*########7*/
		_buffer_size_max += bt_data_buf_size(&sd[_i]);                   /*########f*/
		_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(struct bt_data));    /*########M*/
		_scratchpad_size += bt_data_sp_size(&sd[_i]);                    /*#########*/
	}                                                                        /*########@*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	ser_encode_uint(&_ctx.encoder, ad_len);                                  /*#######%A*/
	for (_i = 0; _i < ad_len; _i++) {                                        /*########/*/
		bt_data_enc(&_ctx.encoder, &ad[_i]);                             /*########P*/
	}                                                                        /*########G*/
	ser_encode_uint(&_ctx.encoder, sd_len);                                  /*########S*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*########r*/
		bt_data_enc(&_ctx.encoder, &sd[_i]);                             /*########c*/
	}                                                                        /*########@*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_ADV_UPDATE_DATA_RPC_CMD,      /*####%BHqc*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@6aY*/

	return _result;                                                          /*##BX7TDLc*/
}


void bt_le_ext_adv_cb_enc(CborEncoder *_encoder, struct bt_le_ext_adv_cb *_data)                   /*####%Bv4Q*/
{                                                                                                  /*#####@y84*/

	SERIALIZE(STRUCT(struct bt_le_ext_adv_cb));

	SERIALIZE(STRUCT_BUFFER_CONST(15));                                                        /*##EwLC0KQ*/

	ser_encode_callback(&_ctx.encoder, _data->sent, bt_le_ext_adv_cb_sent_t_decoder);          /*######%Ax*/
	ser_encode_callback(&_ctx.encoder, _data->connected, bt_le_ext_adv_cb_connected_t_decoder);/*######5wD*/
	ser_encode_callback(&_ctx.encoder, _data->scanned, bt_le_ext_adv_cb_scanned_t_decoder);    /*######@lE*/

}                                                                                                  /*##B9ELNqo*/

struct bt_le_ext_adv_create_rpc_res                                              /*####%Bm6e*/
{                                                                                /*#####@dOE*/

	int _result;                                                             /*####%CRV7*/
	struct bt_le_ext_adv ** adv;                                             /*#####@36w*/

};                                                                               /*##B985gv0*/

static void bt_le_ext_adv_create_rpc_rsp(CborValue *_value, void *_handler_data)  /*####%Bno+*/
{                                                                                 /*#####@e2c*/

	struct entropy_get_result *_res =                                         /*####%AWti*/
		(struct bt_le_ext_adv_create_rpc_res *)_handler_data;             /*#####@L2Q*/

	_res->_result = ser_decode_int(_value);                                   /*####%DfFr*/
	*(_res->adv) = (struct bt_le_ext_adv *)(uintptr_t)ser_decode_uint(_value);/*#####@lr4*/

}                                                                                 /*##B9ELNqo*/


int bt_le_ext_adv_create(const struct bt_le_adv_param *param,
			 const struct bt_le_ext_adv_cb *cb,
			 struct bt_le_ext_adv **adv)
{
	SERIALIZE(OUT(adv));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ac*/
	struct bt_le_ext_adv_create_rpc_res _result;                             /*#######/y*/
	size_t _scratchpad_size = 0;                                             /*#######a4*/
	size_t _buffer_size_max = 42;                                            /*#######@I*/

	_buffer_size_max += bt_le_adv_param_buf_size(param);                     /*##CKvr4W4*/

	_scratchpad_size += bt_le_adv_param_sp_size(param);                      /*##EEZ/Gv4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	bt_le_adv_param_enc(&_ctx.encoder, param);                               /*####%A7sT*/
	bt_le_ext_adv_cb_enc(&_ctx.encoder, cb);                                 /*#####@fDU*/

	_result.adv = adv;                                                       /*##Cx5Tf1c*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_CREATE_RPC_CMD,       /*####%BOJo*/
		&_ctx, bt_le_ext_adv_create_rpc_rsp, &_result);                  /*#####@vEw*/

	return _result._result;                                                  /*##BW0ge3U*/
}

void bt_le_ext_adv_start_param_enc(CborEncoder *_encoder, struct bt_le_ext_adv_start_param *_data)/*####%BiBT*/
{                                                                                                 /*#####@q7M*/

	SERIALIZE(STRUCT(struct bt_le_ext_adv_start_param));

	SERIALIZE(STRUCT_BUFFER_CONST(5));                                                        /*##E/p/Vwo*/

	ser_encode_uint(_encoder, _data->timeout);                                                /*####%A4oh*/
	ser_encode_uint(_encoder, _data->num_events);                                             /*#####@m6E*/

}                                                                                                 /*##B9ELNqo*/

int bt_le_ext_adv_start(struct bt_le_ext_adv *adv,
			struct bt_le_ext_adv_start_param *param)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ac*/
	int _result;                                                             /*######PRx*/
	size_t _buffer_size_max = 10;                                            /*######@Yo*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*####%A3ca*/
	bt_le_ext_adv_start_param_enc(&_ctx.encoder, param);                     /*#####@wyE*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_START_RPC_CMD,        /*####%BFEC*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@ryw*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_ext_adv_stop(struct bt_le_ext_adv *adv)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*##A7bUUXs*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_STOP_RPC_CMD,         /*####%BAJN*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@JxE*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_ext_adv_set_data(struct bt_le_ext_adv *adv,
			   const struct bt_data *ad, size_t ad_len,
			   const struct bt_data *sd, size_t sd_len)
{
	SERIALIZE(SIZE_PARAM(ad, ad_len));
	SERIALIZE(SIZE_PARAM(sd, sd_len));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	int _result;                                                             /*#######ci*/
	size_t _scratchpad_size = 0;                                             /*#######Rw*/
	size_t _buffer_size_max = 20;                                            /*#######Eo*/
	size_t _i;                                                               /*########@*/

	for (_i = 0; _i < ad_len; _i++) {                                        /*########%*/
		_buffer_size_max += bt_data_buf_size(&ad[_i]);                   /*########C*/
		_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(struct bt_data));    /*########J*/
		_scratchpad_size += bt_data_sp_size(&ad[_i]);                    /*########/*/
	}                                                                        /*########m*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*########7*/
		_buffer_size_max += bt_data_buf_size(&sd[_i]);                   /*########f*/
		_scratchpad_size += SCRATCHPAD_ALIGN(sizeof(struct bt_data));    /*########M*/
		_scratchpad_size += bt_data_sp_size(&sd[_i]);                    /*#########*/
	}                                                                        /*########@*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*########%*/
	ser_encode_uint(&_ctx.encoder, ad_len);                                  /*########A*/
	for (_i = 0; _i < ad_len; _i++) {                                        /*########3*/
		bt_data_enc(&_ctx.encoder, &ad[_i]);                             /*########m*/
	}                                                                        /*########T*/
	ser_encode_uint(&_ctx.encoder, sd_len);                                  /*########w*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*########G*/
		bt_data_enc(&_ctx.encoder, &sd[_i]);                             /*########Q*/
	}                                                                        /*########@*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_SET_DATA_RPC_CMD,     /*####%BGCj*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@0sg*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_ext_adv_update_param(struct bt_le_ext_adv *adv,
			       const struct bt_le_adv_param *param)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AQ*/
	int _result;                                                             /*#######Bt*/
	size_t _scratchpad_size = 0;                                             /*#######o4*/
	size_t _buffer_size_max = 32;                                            /*#######@s*/

	_buffer_size_max += bt_le_adv_param_buf_size(param);                     /*##CKvr4W4*/

	_scratchpad_size += bt_le_adv_param_sp_size(param);                      /*##EEZ/Gv4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*####%AoDN*/
	ser_encode_uint(&_ctx.encoder, _scratchpad_size);                        /*#####@BNc*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*####%A82t*/
	bt_le_adv_param_enc(&_ctx.encoder, param);                               /*#####@9JA*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_UPDATE_PARAM_RPC_CMD, /*####%BBz+*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@6Kg*/

	return _result;                                                          /*##BX7TDLc*/
}

int bt_le_ext_adv_delete(struct bt_le_ext_adv *adv)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Aa*/
	int _result;                                                             /*######Qso*/
	size_t _buffer_size_max = 5;                                             /*######@uA*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*##A7bUUXs*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_DELETE_RPC_CMD,       /*####%BKnQ*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@zKE*/

	return _result;                                                          /*##BX7TDLc*/
}


uint8_t bt_le_ext_adv_get_index(struct bt_le_ext_adv *adv)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%Ac*/
	uint8_t _result;                                                         /*######lxx*/
	size_t _buffer_size_max = 5;                                             /*######@q0*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*##A7bUUXs*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_GET_INDEX_RPC_CMD,    /*####%BK4M*/
		&_ctx, ser_rsp_simple_u8, &_result);                             /*#####@Hi8*/

	return _result;                                                          /*##BX7TDLc*/
}

void bt_le_ext_adv_info_dec(CborValue *_value, struct bt_le_ext_adv_info *_data) /*####%BvJh*/
{                                                                                /*#####@D8M*/

	_data->id = ser_decode_uint(_value);                                     /*####%CvH0*/
	_data->tx_power = ser_decode_int(_value);                                /*#####@OLs*/

}                                                                                /*##B9ELNqo*/

struct bt_le_ext_adv_get_info_rpc_res                                            /*####%BtHv*/
{                                                                                /*#####@AD0*/

	int _result;                                                             /*####%CU6/*/
	struct bt_le_ext_adv_info * info;                                        /*#####@vlc*/

};                                                                               /*##B985gv0*/

static void bt_le_ext_adv_get_info_rpc_rsp(CborValue *_value, void *_handler_data)/*####%BhTB*/
{                                                                                 /*#####@DEk*/

	struct entropy_get_result *_res =                                         /*####%AR5V*/
		(struct bt_le_ext_adv_get_info_rpc_res *)_handler_data;           /*#####@d9A*/

	_res->_result = ser_decode_int(_value);                                   /*####%DVh4*/
	bt_le_ext_adv_info_dec(_value, _res->info);                               /*#####@cvQ*/

}                                                                                 /*##B9ELNqo*/

int bt_le_ext_adv_get_info(const struct bt_le_ext_adv *adv,
			   struct bt_le_ext_adv_info *info)
{
	SERIALIZE(OUT(info));

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AQ*/
	struct bt_le_ext_adv_get_info_rpc_res _result;                           /*######uj1*/
	size_t _buffer_size_max = 5;                                             /*######@40*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, (uintptr_t)adv);                          /*##A7bUUXs*/

	_result.info = info;                                                     /*##C65VXp0*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_EXT_ADV_GET_INFO_RPC_CMD,     /*####%BNVz*/
		&_ctx, bt_le_ext_adv_get_info_rpc_rsp, &_result);                /*#####@93Y*/

	return _result._result;                                                  /*##BW0ge3U*/
}

void bt_le_scan_param_enc(CborEncoder *_encoder, struct bt_le_scan_param *_data) /*####%BtCb*/
{                                                                                /*#####@haw*/

	SERIALIZE(STRUCT(struct bt_le_scan_param));

	SERIALIZE(STRUCT_BUFFER_CONST(22));                                      /*##EzbvbRY*/

	ser_encode_uint(_encoder, _data->type);                                  /*#######%A*/
	ser_encode_uint(_encoder, _data->options);                               /*#######4W*/
	ser_encode_uint(_encoder, _data->interval);                              /*########D*/
	ser_encode_uint(_encoder, _data->window);                                /*########I*/
	ser_encode_uint(_encoder, _data->timeout);                               /*########n*/
	ser_encode_uint(_encoder, _data->interval_coded);                        /*########Q*/
	ser_encode_uint(_encoder, _data->window_coded);                          /*########@*/

}                                                                                /*##B9ELNqo*/


int bt_le_scan_start(const struct bt_le_scan_param *param, bt_le_scan_cb_t cb)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AR*/
	int _result;                                                             /*######PI0*/
	size_t _buffer_size_max = 27;                                            /*######@Mk*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	bt_le_scan_param_enc(&_ctx.encoder, param);                              /*####%AyRM*/
	ser_encode_callback(&_ctx.encoder, cb, bt_le_scan_cb_t_decoder);         /*#####@JJU*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_SCAN_START_RPC_CMD,           /*####%BEk6*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@M28*/

	return _result;                                                          /*##BX7TDLc*/
}


int bt_le_whitelist_add(const bt_addr_le_t *addr)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AR*/
	int _result;                                                             /*######RDP*/
	size_t _buffer_size_max = 3;                                             /*######@sI*/

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*##CHCgvU0*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*##A9rQrRg*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_WHITELIST_ADD_RPC_CMD,        /*####%BOf6*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@eVE*/

	return _result;                                                          /*##BX7TDLc*/
}

int bt_le_whitelist_rem(const bt_addr_le_t *addr)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AR*/
	int _result;                                                             /*######RDP*/
	size_t _buffer_size_max = 3;                                             /*######@sI*/

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*##CHCgvU0*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*##A9rQrRg*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_LE_WHITELIST_REM_RPC_CMD,        /*####%BI9r*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@j9w*/

	return _result;                                                          /*##BX7TDLc*/
}


void bt_data_parse(struct net_buf_simple *ad,
		   bool (*func)(struct bt_data *data, void *user_data),
		   void *user_data)
{
	while (ad->len > 1) {
		struct bt_data data;
		u8_t len;

		len = net_buf_simple_pull_u8(ad);
		if (len == 0U) {
			/* Early termination */
			return;
		}

		if (len > ad->len) {
			BT_WARN("Malformed data");
			return;
		}

		data.type = net_buf_simple_pull_u8(ad);
		data.data_len = len - 1;
		data.data = ad->data;

		if (!func(&data, user_data)) {
			return;
		}

		net_buf_simple_pull(ad, len - 1);
	}
}

void bt_le_oob_enc() 
{
	SERIALIZE(STRUCT(struct bt_le_oob));
}

int bt_le_oob_get_local(uint8_t id, struct bt_le_oob *oob)
{
	SERIALIZE();
}

