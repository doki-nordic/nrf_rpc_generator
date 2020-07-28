
#include <stdint.h>

#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_cli.c"));

SERIALIZE(RAW_STRUCT(bt_addr_le_t));


static void bt_enable_rpc_handler(CborValue *_value, void *_handler_data)        /*####%Bims*/
{                                                                                /*#####@U54*/

	bt_ready_cb_t cb;                                                        /*####%AX3q*/
	int _result;                                                             /*#####@ImM*/

	cb = (bt_ready_cb_t)ser_decode_callback(_value, bt_ready_cb_t_encoder);  /*##Ctv5nEY*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_enable(cb);                                                 /*##DqdsuHg*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

	return;                                                                  /*######%B+*/
decoding_error:                                                                  /*#######XK*/
	report_decoding_error(BT_ENABLE_RPC_CMD, _handler_data);                 /*#######Ex*/
}                                                                                /*#######@I*/


static void bt_set_name_rpc_handler(CborValue *_value, void *_handler_data)      /*####%BgLQ*/
{                                                                                /*#####@EdU*/

	const char * name;                                                       /*######%AW*/
	int _result;                                                             /*######V1+*/
	struct ser_scratchpad _scratchpad;                                       /*######@Wo*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                              /*##EZKHjKY*/

	name = ser_decode_str_sp(&_scratchpad);                                  /*##Ct1cGC4*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_set_name(name);                                             /*##Du4mNCE*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*##Eq1r7Tg*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

	return;                                                                  /*#######%B*/
decoding_error:                                                                  /*#######zA*/
	report_decoding_error(BT_SET_NAME_RPC_CMD, _handler_data);               /*#######Ua*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*#######eM*/
}                                                                                /*########@*/


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
	bool _result;                                                            /*#######dn*/
	size_t size;                                                             /*########x*/
	size_t _name_strlen;                                                     /*########+*/
	char * name;                                                             /*########1*/
	size_t _buffer_size_max = 6;                                             /*########4*/
	struct ser_scratchpad _scratchpad;                                       /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                              /*##EZKHjKY*/

	size = ser_decode_uint(_value);                                          /*##Ckcz6jM*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	name = ser_scratchpad_get(size);                                         /*##D2U0FqY*/

	_result = bt_get_name_out(name, size);                                   /*##DoiOW8M*/

	_name_strlen = strlen(name);                                             /*####%CFOk*/
	_buffer_size_max += _name_strlen;                                        /*#####@f8c*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_bool(&_ctx.encoder, &_result);                                /*####%DI6/*/
	ser_encode_str(&_ctx.encoder, name, _name_strlen);                       /*#####@Xnk*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*##Eq1r7Tg*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*#######%B*/
decoding_error:                                                                  /*#######7b*/
	report_decoding_error(BT_GET_NAME_OUT_RPC_CMD, _handler_data);           /*#######DV*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*#######Lc*/
}                                                                                /*########@*/

static void bt_id_delete_rpc_handler(CborValue *_value, void *_handler_data)     /*####%Bn4o*/
{                                                                                /*#####@h9M*/

	uint8_t id;                                                              /*####%Abyj*/
	int _result;                                                             /*#####@HaA*/

	id = ser_decode_uint(_value);                                            /*##Cgs+Vr8*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_id_delete(id);                                              /*##DilWuoM*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

	return;                                                                  /*######%B5*/
decoding_error:                                                                  /*#######iP*/
	report_decoding_error(BT_ID_DELETE_RPC_CMD, _handler_data);              /*#######YY*/
}                                                                                /*#######@Y*/

static void bt_le_adv_stop_rpc_handler(CborValue *_value, void *_handler_data)   /*####%BlBQ*/
{                                                                                /*#####@4G8*/

	int _result;                                                             /*##AWc+iOc*/

	nrf_rpc_cbor_decoding_done(_value);                                      /*##AGkSPWY*/

	_result = bt_le_adv_stop();                                              /*##Du9+9xY*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

}                                                                                /*##B9ELNqo*/

static void bt_le_scan_stop_rpc_handler(CborValue *_value, void *_handler_data)  /*####%BjGj*/
{                                                                                /*#####@TTc*/

	int _result;                                                             /*##AWc+iOc*/

	nrf_rpc_cbor_decoding_done(_value);                                      /*##AGkSPWY*/

	_result = bt_le_scan_stop();                                             /*##DjTVckk*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

}                                                                                /*##B9ELNqo*/


static void bt_le_whitelist_clear_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bs7f*/
{                                                                                    /*#####@2mU*/

	int _result;                                                                 /*##AWc+iOc*/

	nrf_rpc_cbor_decoding_done(_value);                                          /*##AGkSPWY*/

	_result = bt_le_whitelist_clear();                                           /*##DjMtZbs*/

	ser_rsp_send_i32(_result);                                                   /*##BNedR2Y*/

}                                                                                    /*##B9ELNqo*/

static void bt_le_scan_recv(const struct bt_le_scan_recv_info *info,
		     struct net_buf_simple *buf)
{
	//SERIALIZE(); TODO
}



static void bt_le_scan_timeout(void)
{
	SERIALIZE(EVENT);

	struct nrf_rpc_cbor_ctx _ctx;                                            /*####%ATMv*/
	size_t _buffer_size_max = 0;                                             /*#####@1d4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_evt_no_err(&bt_rpc_grp,                                     /*####%BGHA*/
		BT_LE_SCAN_TIMEOUT_RPC_EVT, &_ctx);                              /*#####@KsQ*/
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

	nrf_rpc_cbor_decoding_done(_value);                                             /*##AGkSPWY*/

	bt_le_scan_cb_reg_enable();                                                     /*##DmupDOk*/

	ser_rsp_send_void();                                                            /*##BEYGLxw*/

}                                                                                       /*##B9ELNqo*/

static void bt_le_set_chan_map_rpc_handler(CborValue *_value, void *_handler_data)/*####%Blh4*/
{                                                                                 /*#####@46c*/

	uint8_t * chan_map;                                                       /*######%AT*/
	int _result;                                                              /*######ICz*/
	struct ser_scratchpad _scratchpad;                                        /*######@8E*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                               /*##EZKHjKY*/

	chan_map = ser_decode_buffer_sp(&_scratchpad);                            /*##CqcDErc*/

	if (!ser_decoding_done_and_check(_value)) {                               /*######%AE*/
		goto decoding_error;                                              /*######QTM*/
	}                                                                         /*######@1Y*/

	_result = bt_le_set_chan_map(chan_map);                                   /*##DmtQNns*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                        /*##Eq1r7Tg*/

	ser_rsp_send_i32(_result);                                                /*##BNedR2Y*/

	return;                                                                   /*#######%B*/
decoding_error:                                                                   /*#######8X*/
	report_decoding_error(BT_LE_SET_CHAN_MAP_RPC_CMD, _handler_data);         /*#######7v*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                        /*#######/Y*/
}                                                                                 /*########@*/

static void bt_br_discovery_stop_rpc_handler(CborValue *_value, void *_handler_data)/*####%BpH4*/
{                                                                                   /*#####@DyM*/

	int _result;                                                                /*##AWc+iOc*/

	nrf_rpc_cbor_decoding_done(_value);                                         /*##AGkSPWY*/

	_result = bt_br_discovery_stop();                                           /*##DufBJOI*/

	ser_rsp_send_i32(_result);                                                  /*##BNedR2Y*/

}                                                                                   /*##B9ELNqo*/

static void bt_br_set_discoverable_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bg3/*/
{                                                                                     /*#####@LpE*/

	bool enable;                                                                  /*####%AYG1*/
	int _result;                                                                  /*#####@DQA*/

	enable = ser_decode_bool(_value);                                             /*##CszoDnE*/

	if (!ser_decoding_done_and_check(_value)) {                                   /*######%AE*/
		goto decoding_error;                                                  /*######QTM*/
	}                                                                             /*######@1Y*/

	_result = bt_br_set_discoverable(enable);                                     /*##Dq1uczc*/

	ser_rsp_send_i32(_result);                                                    /*##BNedR2Y*/

	return;                                                                       /*######%B/*/
decoding_error:                                                                       /*#######SU*/
	report_decoding_error(BT_BR_SET_DISCOVERABLE_RPC_CMD, _handler_data);         /*#######qf*/
}                                                                                     /*#######@I*/

static void bt_br_set_connectable_rpc_handler(CborValue *_value, void *_handler_data)/*####%BkM2*/
{                                                                                    /*#####@M5k*/

	bool enable;                                                                 /*####%AYG1*/
	int _result;                                                                 /*#####@DQA*/

	enable = ser_decode_bool(_value);                                            /*##CszoDnE*/

	if (!ser_decoding_done_and_check(_value)) {                                  /*######%AE*/
		goto decoding_error;                                                 /*######QTM*/
	}                                                                            /*######@1Y*/

	_result = bt_br_set_connectable(enable);                                     /*##DuRkEo8*/

	ser_rsp_send_i32(_result);                                                   /*##BNedR2Y*/

	return;                                                                      /*######%B/*/
decoding_error:                                                                      /*#######Vh*/
	report_decoding_error(BT_BR_SET_CONNECTABLE_RPC_CMD, _handler_data);         /*#######/q*/
}                                                                                    /*#######@w*/

static void bt_set_id_addr_rpc_handler(CborValue *_value, void *_handler_data)   /*####%BvyT*/
{                                                                                /*#####@wb0*/

	bt_addr_le_t _addr_data;                                                 /*######%AX*/
	const bt_addr_le_t * addr;                                               /*######+Ee*/
	int _result;                                                             /*######@rM*/

	addr = ser_decode_buffer(_value, &_addr_data, sizeof(bt_addr_le_t));     /*##CmLJMDg*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_set_id_addr(addr);                                          /*##DuZOZN8*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

	return;                                                                  /*######%B8*/
decoding_error:                                                                  /*#######PY*/
	report_decoding_error(BT_SET_ID_ADDR_RPC_CMD, _handler_data);            /*#######vX*/
}                                                                                /*#######@s*/

static void bt_id_get_rpc_handler(CborValue *_value, void *_handler_data)        /*####%Bgag*/
{                                                                                /*#####@m9Q*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	size_t _count_data;                                                      /*#######Uj*/
	size_t * count = &_count_data;                                           /*#######2X*/
	bt_addr_le_t * addrs;                                                    /*########C*/
	size_t _buffer_size_max = 10;                                            /*########k*/
	struct ser_scratchpad _scratchpad;                                       /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                              /*##EZKHjKY*/

	*count = ser_decode_uint(_value);                                        /*##CgKRZw4*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	addrs = ser_scratchpad_get(*count * sizeof(bt_addr_le_t));               /*##Dzyaolg*/

	bt_id_get(addrs, count);                                                 /*##DsYIgTQ*/

	_buffer_size_max += *count * sizeof(bt_addr_le_t);                       /*##CO2r4Ws*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_uint(&_ctx.encoder, *count);                                  /*####%DKBB*/
	ser_encode_buffer(&_ctx.encoder, addrs, *count * sizeof(bt_addr_le_t));  /*#####@ACo*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*##Eq1r7Tg*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*#######%B*/
decoding_error:                                                                  /*#######wM*/
	report_decoding_error(BT_ID_GET_RPC_CMD, _handler_data);                 /*#######UE*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*#######YA*/
}                                                                                /*########@*/


uint8_t apply_addr_irk_flags(uint8_t flags, bt_addr_le_t **addr, uint8_t **irk)
{
	if (flags & 1) {
		*irk = NULL;
	}
	if (flags & 2) {
		*addr = NULL;
	}
}

static void bt_id_create_rpc_handler(CborValue *_value, void *_handler_data)     /*####%BnmX*/
{                                                                                /*#####@/TY*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	int _result;                                                             /*#######RE*/
	bt_addr_le_t _addr_data;                                                 /*########X*/
	bt_addr_le_t * addr;                                                     /*########x*/
	uint8_t * irk;                                                           /*########i*/
	size_t _buffer_size_max = 13;                                            /*########Y*/
	struct ser_scratchpad _scratchpad;                                       /*########@*/

	uint8_t flags;

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                              /*##EZKHjKY*/

	addr = ser_decode_buffer(_value, &_addr_data, sizeof(bt_addr_le_t));     /*####%CgHr*/
	irk = ser_decode_buffer_sp(&_scratchpad);                                /*#####@AWw*/

	flags = ser_decode_uint(_value);

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_id_create(addr, irk);                                       /*##Di/SBrY*/

	apply_addr_irk_flags(flags, &addr, &irk);

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*####%CN0D*/
	_buffer_size_max += !irk ? 0 : sizeof(uint8_t) * 16;                     /*#####@JH4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*######%DM*/
	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*######8cw*/
	ser_encode_buffer(&_ctx.encoder, irk, sizeof(uint8_t) * 16);             /*######@ug*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*##Eq1r7Tg*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*#######%B*/
decoding_error:                                                                  /*#######1z*/
	report_decoding_error(BT_ID_CREATE_RPC_CMD, _handler_data);              /*#######Qy*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*#######Ho*/
}                                                                                /*########@*/

static void bt_id_reset_rpc_handler(CborValue *_value, void *_handler_data)      /*####%BoL+*/
{                                                                                /*#####@SEk*/

	struct nrf_rpc_cbor_ctx _ctx;                                            /*#######%A*/
	int _result;                                                             /*########Z*/
	int id;                                                                  /*########T*/
	bt_addr_le_t _addr_data;                                                 /*########A*/
	bt_addr_le_t * addr;                                                     /*########n*/
	uint8_t * irk;                                                           /*########X*/
	size_t _buffer_size_max = 13;                                            /*########I*/
	struct ser_scratchpad _scratchpad;                                       /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                              /*##EZKHjKY*/

	id = ser_decode_int(_value);                                             /*######%Co*/
	addr = ser_decode_buffer(_value, &_addr_data, sizeof(bt_addr_le_t));     /*######Hnm*/
	irk = ser_decode_buffer_sp(&_scratchpad);                                /*######@3k*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_id_reset(id, addr, irk);                                    /*##DuJfNtE*/

	apply_addr_irk_flags(flags, &addr, &irk);

	_buffer_size_max += addr ? sizeof(bt_addr_le_t) : 0;                     /*####%CN0D*/
	_buffer_size_max += !irk ? 0 : sizeof(uint8_t) * 16;                     /*#####@JH4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                 /*######%DM*/
	ser_encode_buffer(&_ctx.encoder, addr, sizeof(bt_addr_le_t));            /*######8cw*/
	ser_encode_buffer(&_ctx.encoder, irk, sizeof(uint8_t) * 16);             /*######@ug*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*##Eq1r7Tg*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                          /*##BFLm1vw*/

	return;                                                                  /*#######%B*/
decoding_error:                                                                  /*#######6p*/
	report_decoding_error(BT_ID_RESET_RPC_CMD, _handler_data);               /*#######d/*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*#######oY*/
}                                                                                /*########@*/


void bt_le_adv_param_dec(struct ser_scratchpad *_scratchpad, struct bt_le_adv_param *_data)/*####%Bklr*/
{                                                                                          /*#####@MEU*/

	CborValue *_value = _scratchpad->value;                                            /*##AU3cSLw*/

	_data->id = ser_decode_uint(_value);                                               /*#######%C*/
	_data->sid = ser_decode_uint(_value);                                              /*#######uD*/
	_data->secondary_max_skip = ser_decode_uint(_value);                               /*########8*/
	_data->options = ser_decode_uint(_value);                                          /*########H*/
	_data->interval_min = ser_decode_uint(_value);                                     /*########O*/
	_data->interval_max = ser_decode_uint(_value);                                     /*########8*/
	_data->peer = ser_decode_buffer_sp(_scratchpad);                                   /*########@*/

}                                                                                          /*##B9ELNqo*/

void bt_data_dec(struct ser_scratchpad *_scratchpad, struct bt_data *_data)      /*####%Bne5*/
{                                                                                /*#####@+3g*/

	CborValue *_value = _scratchpad->value;                                  /*##AU3cSLw*/

	_data->type = ser_decode_uint(_value);                                   /*######%Ck*/
	_data->data_len = ser_decode_uint(_value);                               /*######cDJ*/
	_data->data = ser_decode_buffer_sp(_scratchpad);                         /*######@Vg*/

}                                                                                /*##B9ELNqo*/

static void bt_le_adv_start_rpc_handler(CborValue *_value, void *_handler_data)  /*####%BkMc*/
{                                                                                /*#####@+8A*/

	struct bt_le_adv_param param;                                            /*#######%A*/
	size_t ad_len;                                                           /*########W*/
	const struct bt_data * ad;                                               /*########w*/
	size_t sd_len;                                                           /*########H*/
	const struct bt_data * sd;                                               /*########F*/
	int _result;                                                             /*########3*/
	struct ser_scratchpad _scratchpad;                                       /*########o*/
	size_t _i;                                                               /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                              /*##EZKHjKY*/

	bt_le_adv_param_dec(&_scratchpad, &param);                               /*########%*/
	ad_len = ser_decode_uint(_value);                                        /*########C*/
	ad = ser_scratchpad_get(&_scratchpad, ad_len * sizeof(struct bt_data));  /*########j*/
	if (ad == NULL) {                                                        /*########c*/
		goto decoding_error;                                             /*########r*/
	}                                                                        /*########S*/
	for (_i = 0; _i < ad_len; _i++) {                                        /*########q*/
		bt_data_dec(&_scratchpad, &ad[_i]);                              /*########E*/
	}                                                                        /*#########*/
	sd_len = ser_decode_uint(_value);                                        /*#########*/
	sd = ser_scratchpad_get(&_scratchpad, sd_len * sizeof(struct bt_data));  /*#########*/
	if (sd == NULL) {                                                        /*#########*/
		goto decoding_error;                                             /*#########*/
	}                                                                        /*#########*/
	for (_i = 0; _i < sd_len; _i++) {                                        /*#########*/
		bt_data_dec(&_scratchpad, &sd[_i]);                              /*#########*/
	}                                                                        /*########@*/

	if (!ser_decoding_done_and_check(_value)) {                              /*######%AE*/
		goto decoding_error;                                             /*######QTM*/
	}                                                                        /*######@1Y*/

	_result = bt_le_adv_start(&param, ad, ad_len, sd, sd_len);               /*##DovAJXw*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*##Eq1r7Tg*/

	ser_rsp_send_i32(_result);                                               /*##BNedR2Y*/

	return;                                                                  /*#######%B*/
decoding_error:                                                                  /*#######3c*/
	report_decoding_error(BT_LE_ADV_START_RPC_CMD, _handler_data);           /*#######56*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                       /*#######jI*/
}                                                                                /*########@*/

static void bt_le_adv_update_data_rpc_handler(CborValue *_value, void *_handler_data)/*####%Buyg*/
{                                                                                    /*#####@rwo*/

	size_t ad_len;                                                               /*#######%A*/
	const struct bt_data * ad;                                                   /*#######fl*/
	size_t sd_len;                                                               /*########l*/
	const struct bt_data * sd;                                                   /*########1*/
	int _result;                                                                 /*########F*/
	struct ser_scratchpad _scratchpad;                                           /*########E*/
	size_t _i;                                                                   /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                                  /*##EZKHjKY*/

	ad_len = ser_decode_uint(_value);                                            /*########%*/
	ad = ser_scratchpad_get(&_scratchpad, ad_len * sizeof(struct bt_data));      /*########C*/
	if (ad == NULL) {                                                            /*########j*/
		goto decoding_error;                                                 /*########J*/
	}                                                                            /*########0*/
	for (_i = 0; _i < ad_len; _i++) {                                            /*########C*/
		bt_data_dec(&_scratchpad, &ad[_i]);                                  /*########i*/
	}                                                                            /*########M*/
	sd_len = ser_decode_uint(_value);                                            /*#########*/
	sd = ser_scratchpad_get(&_scratchpad, sd_len * sizeof(struct bt_data));      /*#########*/
	if (sd == NULL) {                                                            /*#########*/
		goto decoding_error;                                                 /*#########*/
	}                                                                            /*#########*/
	for (_i = 0; _i < sd_len; _i++) {                                            /*#########*/
		bt_data_dec(&_scratchpad, &sd[_i]);                                  /*#########*/
	}                                                                            /*########@*/

	if (!ser_decoding_done_and_check(_value)) {                                  /*######%AE*/
		goto decoding_error;                                                 /*######QTM*/
	}                                                                            /*######@1Y*/

	_result = bt_le_adv_update_data(ad, ad_len, sd, sd_len);                     /*##DkrIIuE*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                           /*##Eq1r7Tg*/

	ser_rsp_send_i32(_result);                                                   /*##BNedR2Y*/

	return;                                                                      /*#######%B*/
decoding_error:                                                                      /*#######+2*/
	report_decoding_error(BT_LE_ADV_UPDATE_DATA_RPC_CMD, _handler_data);         /*#######rK*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                           /*#######Ow*/
}                                                                                    /*########@*/

static void bt_le_ext_adv_create_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bhf+*/
{                                                                                   /*#####@/Lg*/

	struct nrf_rpc_cbor_ctx _ctx;                                               /*#######%A*/
	int _result;                                                                /*########e*/
	struct bt_le_adv_param param;                                               /*########m*/
	struct bt_le_ext_adv_cb cb;                                                 /*########O*/
	struct bt_le_ext_adv *_adv_data;                                            /*########i*/
	struct bt_le_ext_adv ** adv = &_adv_data;                                   /*########i*/
	size_t _buffer_size_max = 10;                                               /*########k*/
	struct ser_scratchpad _scratchpad;                                          /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                                 /*##EZKHjKY*/

	bt_le_adv_param_dec(&_scratchpad, &param);                                  /*####%ClxW*/
	bt_le_ext_adv_cb_dec(_value, &cb);                                          /*#####@lnk*/

	if (!ser_decoding_done_and_check(_value)) {                                 /*######%AE*/
		goto decoding_error;                                                /*######QTM*/
	}                                                                           /*######@1Y*/

	_result = bt_le_ext_adv_create(&param, &cb, adv);                           /*##DmWVOT0*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                                 /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                    /*####%DLFj*/
	ser_encode_uint(&_ctx.encoder, (uintptr_t)(*adv));                          /*#####@vHs*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                          /*##Eq1r7Tg*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                             /*##BFLm1vw*/

	return;                                                                     /*#######%B*/
decoding_error:                                                                     /*#######7i*/
	report_decoding_error(BT_LE_EXT_ADV_CREATE_RPC_CMD, _handler_data);         /*#######o6*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                          /*#######pE*/
}                                                                                   /*########@*/

void bt_le_ext_adv_cb_dec(CborValue *_value, struct bt_le_ext_adv_cb *_data)                                               /*####%BguJ*/
{                                                                                                                          /*#####@Ks0*/

	_data->sent = (bt_le_ext_adv_cb_sent_t)ser_decode_callback(_value, bt_le_ext_adv_cb_sent_t_encoder);               /*######%Ci*/
	_data->connected = (bt_le_ext_adv_cb_connected_t)ser_decode_callback(_value, bt_le_ext_adv_cb_connected_t_encoder);/*######3RV*/
	_data->scanned = (bt_le_ext_adv_cb_scanned_t)ser_decode_callback(_value, bt_le_ext_adv_cb_scanned_t_encoder);      /*######@4A*/

}                                                                                                                          /*##B9ELNqo*/

void bt_le_ext_adv_start_param_dec(CborValue *_value, struct bt_le_ext_adv_start_param *_data)/*####%Bmcv*/
{                                                                                             /*#####@efo*/

	_data->timeout = ser_decode_uint(_value);                                             /*####%CqFq*/
	_data->num_events = ser_decode_uint(_value);                                          /*#####@gQs*/

}                                                                                             /*##B9ELNqo*/


static void bt_le_ext_adv_start_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bp9Z*/
{                                                                                  /*#####@3s0*/

	struct bt_le_ext_adv * adv;                                                /*######%AW*/
	struct bt_le_ext_adv_start_param param;                                    /*######OdA*/
	int _result;                                                               /*######@zU*/

	adv = (struct bt_le_ext_adv *)ser_decode_uint(_value);                     /*####%Ch45*/
	bt_le_ext_adv_start_param_dec(_value, &param);                             /*#####@ke4*/

	if (!ser_decoding_done_and_check(_value)) {                                /*######%AE*/
		goto decoding_error;                                               /*######QTM*/
	}                                                                          /*######@1Y*/

	_result = bt_le_ext_adv_start(adv, &param);                                /*##DunCU3s*/

	ser_rsp_send_i32(_result);                                                 /*##BNedR2Y*/

	return;                                                                    /*######%Bx*/
decoding_error:                                                                    /*#######fp*/
	report_decoding_error(BT_LE_EXT_ADV_START_RPC_CMD, _handler_data);         /*#######nM*/
}                                                                                  /*#######@4*/
static void bt_le_ext_adv_stop_rpc_handler(CborValue *_value, void *_handler_data)/*####%BtKz*/
{                                                                                 /*#####@fXM*/

	struct bt_le_ext_adv * adv;                                               /*####%AX6g*/
	int _result;                                                              /*#####@WOc*/

	adv = (struct bt_le_ext_adv *)ser_decode_uint(_value);                    /*##CovnGwc*/

	if (!ser_decoding_done_and_check(_value)) {                               /*######%AE*/
		goto decoding_error;                                              /*######QTM*/
	}                                                                         /*######@1Y*/

	_result = bt_le_ext_adv_stop(adv);                                        /*##DrNBryg*/

	ser_rsp_send_i32(_result);                                                /*##BNedR2Y*/

	return;                                                                   /*######%Bz*/
decoding_error:                                                                   /*#######VV*/
	report_decoding_error(BT_LE_EXT_ADV_STOP_RPC_CMD, _handler_data);         /*#######FU*/
}                                                                                 /*#######@E*/

static void bt_le_ext_adv_set_data_rpc_handler(CborValue *_value, void *_handler_data)/*####%BsJY*/
{                                                                                     /*#####@uhw*/

	struct bt_le_ext_adv * adv;                                                   /*#######%A*/
	size_t ad_len;                                                                /*########V*/
	const struct bt_data * ad;                                                    /*########y*/
	size_t sd_len;                                                                /*########M*/
	const struct bt_data * sd;                                                    /*########b*/
	int _result;                                                                  /*########3*/
	struct ser_scratchpad _scratchpad;                                            /*########U*/
	size_t _i;                                                                    /*########@*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                                   /*##EZKHjKY*/

	adv = (struct bt_le_ext_adv *)ser_decode_uint(_value);                        /*########%*/
	ad_len = ser_decode_uint(_value);                                             /*########C*/
	ad = ser_scratchpad_get(&_scratchpad, ad_len * sizeof(struct bt_data));       /*########l*/
	if (ad == NULL) {                                                             /*########x*/
		goto decoding_error;                                                  /*########I*/
	}                                                                             /*########r*/
	for (_i = 0; _i < ad_len; _i++) {                                             /*########6*/
		bt_data_dec(&_scratchpad, &ad[_i]);                                   /*########U*/
	}                                                                             /*#########*/
	sd_len = ser_decode_uint(_value);                                             /*#########*/
	sd = ser_scratchpad_get(&_scratchpad, sd_len * sizeof(struct bt_data));       /*#########*/
	if (sd == NULL) {                                                             /*#########*/
		goto decoding_error;                                                  /*#########*/
	}                                                                             /*#########*/
	for (_i = 0; _i < sd_len; _i++) {                                             /*#########*/
		bt_data_dec(&_scratchpad, &sd[_i]);                                   /*#########*/
	}                                                                             /*########@*/

	if (!ser_decoding_done_and_check(_value)) {                                   /*######%AE*/
		goto decoding_error;                                                  /*######QTM*/
	}                                                                             /*######@1Y*/

	_result = bt_le_ext_adv_set_data(adv, ad, ad_len, sd, sd_len);                /*##Dm9i5+c*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                            /*##Eq1r7Tg*/

	ser_rsp_send_i32(_result);                                                    /*##BNedR2Y*/

	return;                                                                       /*#######%B*/
decoding_error:                                                                       /*#######+o*/
	report_decoding_error(BT_LE_EXT_ADV_SET_DATA_RPC_CMD, _handler_data);         /*#######1w*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                            /*#######ho*/
}                                                                                     /*########@*/

static void bt_le_ext_adv_update_param_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bo1A*/
{                                                                                         /*#####@5sY*/

	struct bt_le_ext_adv * adv;                                                       /*######%AY*/
	struct bt_le_adv_param param;                                                     /*#######mK*/
	int _result;                                                                      /*#######M9*/
	struct ser_scratchpad _scratchpad;                                                /*#######@c*/

	SER_SCRATCHPAD_ALLOC(&_scratchpad, _value);                                       /*##EZKHjKY*/

	adv = (struct bt_le_ext_adv *)ser_decode_uint(_value);                            /*####%Cpjp*/
	bt_le_adv_param_dec(&_scratchpad, &param);                                        /*#####@fv4*/

	if (!ser_decoding_done_and_check(_value)) {                                       /*######%AE*/
		goto decoding_error;                                                      /*######QTM*/
	}                                                                                 /*######@1Y*/

	_result = bt_le_ext_adv_update_param(adv, &param);                                /*##DjIgW08*/

	SER_SCRATCHPAD_FREE(&_scratchpad);                                                /*##Eq1r7Tg*/

	ser_rsp_send_i32(_result);                                                        /*##BNedR2Y*/

	return;                                                                           /*#######%B*/
decoding_error:                                                                           /*#######0q*/
	report_decoding_error(BT_LE_EXT_ADV_UPDATE_PARAM_RPC_CMD, _handler_data);         /*#######3h*/
	SER_SCRATCHPAD_FREE(&_scratchpad);                                                /*#######nQ*/
}                                                                                         /*########@*/

static void bt_le_ext_adv_delete_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bng3*/
{                                                                                   /*#####@ydU*/

	struct bt_le_ext_adv * adv;                                                 /*####%AX6g*/
	int _result;                                                                /*#####@WOc*/

	adv = (struct bt_le_ext_adv *)ser_decode_uint(_value);                      /*##CovnGwc*/

	if (!ser_decoding_done_and_check(_value)) {                                 /*######%AE*/
		goto decoding_error;                                                /*######QTM*/
	}                                                                           /*######@1Y*/

	_result = bt_le_ext_adv_delete(adv);                                        /*##Dk4ka24*/

	ser_rsp_send_i32(_result);                                                  /*##BNedR2Y*/

	return;                                                                     /*######%B9*/
decoding_error:                                                                     /*#######R9*/
	report_decoding_error(BT_LE_EXT_ADV_DELETE_RPC_CMD, _handler_data);         /*#######g2*/
}                                                                                   /*#######@I*/

static void bt_le_ext_adv_get_index_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bju3*/
{                                                                                      /*#####@GBw*/

	struct bt_le_ext_adv * adv;                                                    /*####%AUD1*/
	uint8_t _result;                                                               /*#####@iW8*/

	adv = (struct bt_le_ext_adv *)ser_decode_uint(_value);                         /*##CovnGwc*/

	if (!ser_decoding_done_and_check(_value)) {                                    /*######%AE*/
		goto decoding_error;                                                   /*######QTM*/
	}                                                                              /*######@1Y*/

	_result = bt_le_ext_adv_get_index(adv);                                        /*##DtNeWKM*/

	ser_rsp_send_u8(_result);                                                      /*##BNaHThE*/

	return;                                                                        /*######%B+*/
decoding_error:                                                                        /*#######Zw*/
	report_decoding_error(BT_LE_EXT_ADV_GET_INDEX_RPC_CMD, _handler_data);         /*#######kZ*/
}                                                                                      /*#######@E*/


void bt_le_ext_adv_info_enc(CborEncoder *_encoder, struct bt_le_ext_adv_info *_data)/*####%BvUt*/
{                                                                                   /*#####@MDo*/

	SERIALIZE(STRUCT(struct bt_le_ext_adv_info));

	SERIALIZE(STRUCT_BUFFER_CONST(7));                                          /*##E51nTj8*/

	ser_encode_uint(_encoder, _data->id);                                       /*####%AzCC*/
	ser_encode_int(_encoder, _data->tx_power);                                  /*#####@as4*/

}                                                                                   /*##B9ELNqo*/


static void bt_le_ext_adv_get_info_rpc_handler(CborValue *_value, void *_handler_data)/*####%BjWD*/
{                                                                                     /*#####@+G0*/

	struct nrf_rpc_cbor_ctx _ctx;                                                 /*#######%A*/
	int _result;                                                                  /*#######f5*/
	const struct bt_le_ext_adv * adv;                                             /*#######ZG*/
	struct bt_le_ext_adv_info _info_data;                                         /*########Z*/
	struct bt_le_ext_adv_info * info = &_info_data;                               /*########4*/
	size_t _buffer_size_max = 12;                                                 /*########@*/

	adv = (const struct bt_le_ext_adv *)ser_decode_uint(_value);                  /*##Cq9wDKI*/

	if (!ser_decoding_done_and_check(_value)) {                                   /*######%AE*/
		goto decoding_error;                                                  /*######QTM*/
	}                                                                             /*######@1Y*/

	_result = bt_le_ext_adv_get_info(adv, info);                                  /*##DvTyTw8*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                                   /*##AvrU03s*/

	ser_encode_int(&_ctx.encoder, &_result);                                      /*####%DLQX*/
	bt_le_ext_adv_info_enc(&_ctx.encoder, info);                                  /*#####@2+0*/

	nrf_rpc_cbor_rsp_no_err(&_ctx);                                               /*##BFLm1vw*/

	return;                                                                       /*######%B2*/
decoding_error:                                                                       /*#######Tp*/
	report_decoding_error(BT_LE_EXT_ADV_GET_INFO_RPC_CMD, _handler_data);         /*#######z1*/
}                                                                                     /*#######@Y*/

void bt_le_scan_param_dec(CborValue *_value, struct bt_le_scan_param *_data)     /*####%Bjc4*/
{                                                                                /*#####@EWA*/

	_data->type = ser_decode_uint(_value);                                   /*#######%C*/
	_data->options = ser_decode_uint(_value);                                /*#######sD*/
	_data->interval = ser_decode_uint(_value);                               /*########m*/
	_data->window = ser_decode_uint(_value);                                 /*########T*/
	_data->timeout = ser_decode_uint(_value);                                /*########W*/
	_data->interval_coded = ser_decode_uint(_value);                         /*########c*/
	_data->window_coded = ser_decode_uint(_value);                           /*########@*/

}                                                                                /*##B9ELNqo*/

static void bt_le_scan_start_rpc_handler(CborValue *_value, void *_handler_data)   /*####%BtgB*/
{                                                                                  /*#####@oaA*/

	struct bt_le_scan_param param;                                             /*######%AU*/
	bt_le_scan_cb_t cb;                                                        /*######20I*/
	int _result;                                                               /*######@EE*/

	bt_le_scan_param_dec(_value, &param);                                      /*####%CvTk*/
	cb = (bt_le_scan_cb_t)ser_decode_callback(_value, bt_le_scan_cb_t_encoder);/*#####@aGc*/

	if (!ser_decoding_done_and_check(_value)) {                                /*######%AE*/
		goto decoding_error;                                               /*######QTM*/
	}                                                                          /*######@1Y*/

	_result = bt_le_scan_start(&param, cb);                                    /*##DiKuFZA*/

	ser_rsp_send_i32(_result);                                                 /*##BNedR2Y*/

	return;                                                                    /*######%By*/
decoding_error:                                                                    /*#######Xo*/
	report_decoding_error(BT_LE_SCAN_START_RPC_CMD, _handler_data);            /*#######QA*/
}                                                                                  /*#######@8*/

static void bt_le_whitelist_add_rpc_handler(CborValue *_value, void *_handler_data)/*####%BvAc*/
{                                                                                  /*#####@JS8*/

	bt_addr_le_t _addr_data;                                                   /*######%AX*/
	const bt_addr_le_t * addr;                                                 /*######+Ee*/
	int _result;                                                               /*######@rM*/

	addr = ser_decode_buffer(_value, &_addr_data, sizeof(bt_addr_le_t));       /*##CmLJMDg*/

	if (!ser_decoding_done_and_check(_value)) {                                /*######%AE*/
		goto decoding_error;                                               /*######QTM*/
	}                                                                          /*######@1Y*/

	_result = bt_le_whitelist_add(addr);                                       /*##DhFK5fs*/

	ser_rsp_send_i32(_result);                                                 /*##BNedR2Y*/

	return;                                                                    /*######%B6*/
decoding_error:                                                                    /*#######OQ*/
	report_decoding_error(BT_LE_WHITELIST_ADD_RPC_CMD, _handler_data);         /*#######Gg*/
}                                                                                  /*#######@Q*/

static void bt_le_whitelist_rem_rpc_handler(CborValue *_value, void *_handler_data)/*####%Bq5A*/
{                                                                                  /*#####@p48*/

	bt_addr_le_t _addr_data;                                                   /*######%AX*/
	const bt_addr_le_t * addr;                                                 /*######+Ee*/
	int _result;                                                               /*######@rM*/

	addr = ser_decode_buffer(_value, &_addr_data, sizeof(bt_addr_le_t));       /*##CmLJMDg*/

	if (!ser_decoding_done_and_check(_value)) {                                /*######%AE*/
		goto decoding_error;                                               /*######QTM*/
	}                                                                          /*######@1Y*/

	_result = bt_le_whitelist_rem(addr);                                       /*##Dpt0chE*/

	ser_rsp_send_i32(_result);                                                 /*##BNedR2Y*/

	return;                                                                    /*######%B+*/
decoding_error:                                                                    /*#######vp*/
	report_decoding_error(BT_LE_WHITELIST_REM_RPC_CMD, _handler_data);         /*#######88*/
}                                                                                  /*#######@4*/

