#ifndef reverb_api_h__
#define reverb_api_h__

#ifndef u8
#define u8  unsigned char
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef s16
#define s16 short
#endif


#ifndef u32
#define u32 unsigned int
#endif


#ifndef s32
#define s32 int
#endif

#ifndef s16
#define s16 signed short
#endif


#define delay_MAX 100


#define ECHO_PARM   0   ///0：回声，1:混响(暂不支持)

#if ECHO_PARM
typedef struct _REVERB_PARM_STRUCT_
{
	u16 feedback;           //0-128
	u16 hf_damping;         //0-128
	u16 room_scale;         //0-128

	u16 pre_delay_ms;
	u16 wet_gain;            //NOMAL ONE <=> 128
	u16 dry_gain;            //NOMAL ONE <=> 128

	u16 comb_num;            //max 8
	u16 allpass_num;         //max 4

	u16 nch;
	u16 sr;

}REVERB_PARM_STRUCT;

#else

typedef struct _REVERB_H_PARM_STRUCT_
{
	s16 vol_val;
	s16 shift_val;
	s16 deep_val;               //0-2048
	u16 nch;
	u16 sr;
	u16 ad_sr;
}REVERB_PARM_STRUCT;

#endif

typedef struct _REVERB_IO_
{
	void *priv;
	void (*output)(void *priv,short *outdata,short len);               //len是多少个byte
}REVERB_IO;



typedef struct _REVERB_FUNC_API_
{
	u32 (*need_buf)(void);
	u32 (*init)(unsigned char *ptr,REVERB_PARM_STRUCT *reverb_parm_obj,REVERB_IO *rv_io);  //中途改变参数，可以调init
	u32 (*run)(unsigned char *ptr,short *indata,short len);                                //len是多少个byte
	u32 (*change_parm)(unsigned char *ptr,u16 sample_rate,u16 ch);                         //如果只是改变采样率 跟 声道，可以调这个

}REVERB_FUNC_API;


//另外，输入输出关系是：单声道输入，单声道输出； 双声道输入，双声道输出的

#endif // reverb_api_h__
