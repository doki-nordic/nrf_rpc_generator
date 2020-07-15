

#include "bluetooth/bluetooth.h"

SERIALIZE(HOST_FILE("bt_gap_host.c"));
SERIALIZE(CMD_ID(BT_RPC_GAP_CMD_, _ID));
SERIALIZE(EVT_ID(BT_RPC_GAP_EVT_, _ID));
SERIALIZE(GROUP(bt_rpc_grp));


static char *bt_name_cache = NULL;


int bt_enable(bt_ready_cb_t cb)
{
	SERIALIZE();

	struct nrf_rpc_cbor_ctx _ctx;                                            /*######%AS*/
	size_t _buffer_size_max = 5;                                             /*######rYb*/
	int _result;                                                             /*######@uM*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	ser_encode_callback(&_ctx.encoder, cb, bt_ready_cb_t_rpc_cbk_handler);   /*##Axd82fM*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_CMD_BT_ENABLE_ID,        /*####%BL8G*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@wxc*/

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

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_CMD_BT_SET_NAME_ID,      /*####%BOos*/
		&_ctx, ser_rsp_simple_i32, &_result);                            /*#####@nPk*/

	return _result;                                                          /*##BX7TDLc*/
}


const char *bt_get_name(void)
{
	SERIALIZE(IGNORE_RETURN);

	struct nrf_rpc_cbor_ctx _ctx;                                            /*####%ATMv*/
	size_t _buffer_size_max = 0;                                             /*#####@1d4*/

	NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);                              /*##AvrU03s*/

	nrf_rpc_cbor_cmd_no_err(&bt_rpc_grp, BT_RPC_GAP_CMD_BT_GET_NAME_ID,      /*####%BGgO*/
		&_ctx, ser_rsp_simple_void, NULL);                               /*#####@9LI*/

	return bt_name_cache;
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
