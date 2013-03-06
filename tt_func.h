/*
 * changed by Jin (2013-03-05)
 * <change> change the TT_HOST_IP "192.168.0.18"
 */
#ifndef _TT_FUNC
#define _TT_FUNC
#define MD_PORT 11211
#define ION_PORT 11212
#define FN_PORT 11213
#define SWPN_PORT 11214
#define IO_DATA_PORT 11215
#define TT_HOST_IP "192.168.0.18"
typedef enum
{
	META_DATA = 01,
	IO_NODE,
	FILE_NODE,
	SWAP_NODE,
	IO_DATA
}value_t;
_PROTOTYPE(u32 tt_get,(u8 * path,u8 * zv,value_t vt));
_PROTOTYPE(u32 tt_out,(u8 * key,value_t vt));
_PROTOTYPE(u32 tt_put,(u8 * key,u8 * zv,u32 nv,value_t vt));
/************* md_func *****************/
_PROTOTYPE(u32 md_get,(u8 * path,u8 * md));
_PROTOTYPE(u32 md_out,(u8 * path));
_PROTOTYPE(u32 md_put,(u8 * path,u8 * md));
/************** ion_func ****************/
_PROTOTYPE(u32 ion_get,(u8 * path,u8 * ion));
_PROTOTYPE(u32 ion_out,(u8 * path));
_PROTOTYPE(u32 ion_put,(u8 * path,u8 * ion));
/************** fn_func ****************/
_PROTOTYPE(u32 fn_get,(u8 * path,u8 * fn));
_PROTOTYPE(u32 fn_out,(u8 * path));
_PROTOTYPE(u32 fn_put,(u8 * path,u8 * fn));
/************** fn_q_stat_func ****************/
_PROTOTYPE(u32 fn_q_stat_get,(u8 * path,u8 * fn_q_stat));
_PROTOTYPE(u32 fn_q_stat_out,(u8 * path));
_PROTOTYPE(u32 fn_q_stat_put,(u8 * path,u8 * fn_q_stat));
/************* io_data_func *******************/
_PROTOTYPE(u8 *iod_get,(u8 *key,u32 *nv));
_PROTOTYPE(u32 iod_out,(u8 *key));
_PROTOTYPE(u32 iod_put,(u8 *key,u8 *value,u32 count));
#endif
