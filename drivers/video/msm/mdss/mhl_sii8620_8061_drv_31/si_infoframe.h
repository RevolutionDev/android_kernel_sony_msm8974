/*

SiI8620 Linux Driver
 
Copyright (C) 2013 Silicon Image, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation version 2.
This program is distributed AS-IS WITHOUT ANY WARRANTY of any
kind, whether express or implied; INCLUDING without the implied warranty
of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.  See
the GNU General Public License for more details at http://www.gnu.org/licenses/gpl-2.0.html.

*/ 

#if !defined(SI_INFOFRAME_H)
#define SI_INFOFRAME_H
#include "mhl_common.h"
#include "mhl_defs.h"

typedef struct __attribute__((__packed__)) _info_frame_header_t {
	uint8_t  type_code;
	uint8_t  version_number;
	uint8_t  length;
} info_frame_header_t;

typedef enum
{
     acsRGB         = 0
    ,acsYCbCr422    = 1
    ,acsYCbCr444    = 2
    ,acsFuture      = 3
}AviColorSpace_e;

/*
 * AVI Info Frame Structure
 */
typedef struct __attribute__((__packed__)) _avi_info_frame_data_byte_1_t {
	uint8_t			ScanInfo:2;
	uint8_t			BarInfo:2;
	uint8_t			ActiveFormatInfoPresent:1;
	AviColorSpace_e	colorSpace:2;
	uint8_t			futureMustBeZero:1;
} avi_info_frame_data_byte_1_t;

typedef struct __attribute__((__packed__)) _avi_info_frame_data_byte_2_t {
	uint8_t			ActiveFormatAspectRatio:4;
	uint8_t			PictureAspectRatio:2;
	uint8_t			Colorimetry:2;
} avi_info_frame_data_byte_2_t;

typedef struct __attribute__((__packed__)) _avi_info_frame_data_byte_3_t {
	uint8_t			NonUniformPictureScaling:2;
 	uint8_t 		RGBQuantizationRange:2;
 	uint8_t 		ExtendedColorimetry:3;
 	uint8_t 		ITContent:1;
} avi_info_frame_data_byte_3_t;

typedef struct __attribute__((__packed__)) _avi_info_frame_data_byte_4_t {
	uint8_t			VIC:7;
	uint8_t			futureMustBeZero:1;
} avi_info_frame_data_byte_4_t;

typedef enum
{
    cnGraphics  = 0
    ,cnPhoto    = 1
    ,cnCinema   = 2
    ,cnGame     = 3
}BitsContent_e;

typedef enum
{
    aqLimitedRange = 0
    ,aqFullRange    = 1
    ,aqReserved0    = 2
    ,aqReserved1    = 3
}AviQuantization_e;

typedef struct __attribute__((__packed__)) _avi_info_frame_data_byte_5_t {
	uint8_t				pixelRepetionFactor:4;
	BitsContent_e		content				:2;
	AviQuantization_e	quantization		:2;
} avi_info_frame_data_byte_5_t;

typedef struct __attribute__((__packed__)) _hw_avi_named_payload_t {
	uint8_t  checksum;
	union {
		struct __attribute__((__packed__)) {
			avi_info_frame_data_byte_1_t	pb1;
			avi_info_frame_data_byte_2_t	colorimetryAspectRatio;
			avi_info_frame_data_byte_3_t	pb3;
			avi_info_frame_data_byte_4_t	VIC;
			avi_info_frame_data_byte_5_t	pb5;
			uint8_t							LineNumEndTopBarLow;
			uint8_t							LineNumEndTopBarHigh;
			uint8_t							LineNumStartBottomBarLow;
			uint8_t							LineNumStartBottomBarHigh;
			uint8_t							LineNumEndLeftBarLow;
			uint8_t							LineNumEndLeftBarHigh;
			uint8_t							LineNumStartRightBarLow;
			uint8_t							LineNumStartRightBarHigh;
		} bitFields;
		uint8_t	infoFrameData[13];
	} ifData_u;
} hw_avi_named_payload_t;

// this union overlays the TPI HW for AVI InfoFrames, starting at REG_TPI_AVI_CHSUM.
typedef union _hw_avi_payload_t {
	hw_avi_named_payload_t	namedIfData;
	uint8_t					ifData[14];
} hw_avi_payload_t;

typedef struct __attribute__((__packed__)) _avi_payload_t {
	hw_avi_payload_t	hwPayLoad;
	uint8_t				byte_14;
	uint8_t				byte_15;
} avi_payload_t;

typedef struct __attribute__((__packed__)) _avi_info_frame_t {
	info_frame_header_t		header;
	avi_payload_t			payLoad;
} avi_info_frame_t;

// these values determine the interpretation of PB5
typedef enum
{
     hvfNoAdditionalHDMIVideoFormatPresent =0
    ,hvfExtendedResolutionFormatPresent    =1
    ,hvf3DFormatIndicationPresent          =2
}HDMI_Video_Format_e;

typedef enum {
     tdsFramePacking = 0x00
    ,tdsTopAndBottom = 0x06
    ,tdsSideBySide   = 0x08
} _3D_structure_e;

typedef enum
{
     tdedHorizontalSubSampling    = 0x0
    ,tdedQuincunxOddLeftOddRight  = 0x4
    ,tdedQuincunxOddLeftEvenRight = 0x5
    ,tdedQuincunxEvenLeftOddRight = 0x6
    ,tdedQuincunxEvenLeftEvenRight= 0x7

}ThreeDExtData_e;

typedef enum
{
    tdmdParallaxIso23022_3Section6_x_2_2 = 0
}ThreeDMetaDataType_e;

typedef struct __attribute__((__packed__)) _vendor_specific_payload_t {
	uint8_t             checksum;
	uint8_t             IEEERegistrationIdentifier[3];  // must be 0x000C03 Little Endian
	struct __attribute__((__packed__)){
		unsigned		reserved:5;

		HDMI_Video_Format_e HDMI_Video_Format:3; //HDMI_Video_Format_e
	} pb4;
	union {
		uint8_t HDMI_VIC;
		struct __attribute__((__packed__)) _ThreeDStructure {
			unsigned	reserved:3;
			unsigned	ThreeDMetaPresent:1;
            _3D_structure_e	threeDStructure:4;  //_3D_structure_e
        } ThreeDStructure;
	} pb5;
	struct __attribute__((__packed__)) {
		uint8_t		reserved:4;
		uint8_t		threeDExtData:4; //ThreeDExtData_e
	} pb6;
	struct __attribute__((__packed__)) _PB7 {
		uint8_t		threeDMetaDataLength:5;
		uint8_t		threeDMetaDataType:3; //ThreeDMetaDataType_e
	} pb7;
} vendor_specific_payload_t;

typedef struct __attribute__((__packed__)) _vendor_specific_info_frame_t {
	info_frame_header_t			header;
	vendor_specific_payload_t	payLoad;
} vendor_specific_info_frame_t;

/*
 * MPEG Info Frame Structure
 * Table 8-11 on page 141 of HDMI Spec v1.4
 */
typedef struct __attribute__((__packed__)) {
	info_frame_header_t	header;
	uint8_t		checksum;
	uint8_t		byte_1;
	uint8_t		byte_2;
	uint8_t		byte_3;
	uint8_t		byte_4;
	uint8_t		byte_5;
	uint8_t		byte_6;
} unr_info_frame_t;

typedef struct SI_PACK_THIS_STRUCT _mhl3_vsif_t{

	info_frame_header_t header;
	uint8_t	checksum;
	uint8_t	iee_oui[3];
	uint8_t	pb4;
	uint8_t pb5_reserved;
	uint8_t pb6;
	MHL_high_low_t mhl_hev_fmt_type;
	uint8_t pb9;

	MHL_high_low_t av_delay_sync;
}mhl3_vsif_t;

/* the enum's in the following section are 
	defined "in position" to avoid 
	shifting on the fly 
*/
#define MHL3_VSIF_TYPE		0x81
#define MHL3_VSIF_VERSION	0x03
#define IEEE_OUI_MHL3 0x76A61D
#define	PB4_MASK_MHL_VID_FMT		0x03
enum mhl_vid_fmt_e{
	 mhl_vid_fmt_no_additional	= 0x00
	,mhl_vid_fmt_3d_fmt_present = 0x01
	,mhl_vid_fmt_multi_view		= 0x02
	,mhl_vid_fmt_dual_3d		= 0x03
};
#define PB4_MASK_MHL_3D_FMT_TYPE	0x1C
enum mhl_3d_fmt_type_e{
	 mhl_3d_fmt_type_frame_sequential			=0x00
	,mhl_3d_fmt_type_top_bottom					=0x04
	,mhl_3d_fmt_type_left_right					=0x08
	,mhl_3d_fmt_type_frame_sequential_top_bottom=0x0C
	,mhl_3d_fmt_type_frame_sequential_left_right=0x10
	,mhl_3d_fmt_type_top_bottom_left_right		=0x14
};
#define PB4_MASK_SEP_AUD			0x20
enum mhl_sep_audio_e{
	 mhl_sep_audio_not_available	=0x00
	,mhl_sep_audio_available		=0x20
};
#define PB4_MASK_RESERVED			0xC0
#define MHL3_VSIF_PB4(vid_fmt,_3d_fmt_type,sep_aud) \
(uint8_t)(((vid_fmt      )& PB4_MASK_MHL_VID_FMT) \
		 |((_3d_fmt_type )& PB4_MASK_MHL_3D_FMT_TYPE) \
		 |((sep_aud		)& PB4_MASK_SEP_AUD) )

#define PB6_MASK_MHL_HEV_FMT		0x03
enum mhl_hev_fmt_e{
	 mhl_hev_fmt_no_additional	= 0
	,mhl_hev_fmt_hev_present	=1
	,mhl_hev_fmt_reserved_2	=2
	,mhl_hev_fmt_reserved_3	=3
};
#define PB6_MASK_RESERVED			0xFC
#define MHL3_VSIF_PB6(hev_fmt) (uint8_t)((hev_fmt) & PB6_MASK_MHL_HEV_FMT)

#define PB9_MASK_AV_DELAY_SYNC_19_16	0x0F
#define PB9_MASK_AV_DELAY_DIR			0x10
enum mhl_av_delay_dir_e{
	 mhl_av_delay_dir_audio_earlier	=0x00
	,mhl_av_delay_dir_video_earlier	=0x10
};
#define PB9_MASK_RESERVED				0xE0
#define MHL3_VSIF_PB9(delay_sync,delay_dir) \
(uint8_t)((((delay_sync) >> 16)& PB9_MASK_AV_DELAY_SYNC_19_16) \
		| ( (delay_dir)        & PB9_MASK_AV_DELAY_DIR) )

typedef struct __attribute__((__packed__)) _info_frame_t {
	union {
		info_frame_header_t				header;
		avi_info_frame_t				avi;
		vendor_specific_info_frame_t	vendorSpecific;
		mhl3_vsif_t						mhl3_vsif;
		unr_info_frame_t				unr;
	} body;
} info_frame_t;

typedef enum
{
        // just define these three for now
     InfoFrameType_AVI
    ,InfoFrameType_VendorSpecific
    ,InfoFrameType_VendorSpecific_MHL3
    ,InfoFrameType_Audio

}InfoFrameType_e;


#ifdef ENABLE_DUMP_INFOFRAME //(

void DumpIncomingInfoFrameImpl(char *pszId,char *pszFile,int iLine,info_frame_t *pInfoFrame,uint8_t length);
#define DumpIncomingInfoFrame(pData,length) DumpIncomingInfoFrameImpl(#pData,__FILE__,__LINE__,(info_frame_t *)pData,length)

#else //)(

#define DumpIncomingInfoFrame(pData,length) /* do nothing */

#endif //)

#ifdef ENABLE_COLOR_SPACE_DEBUG_PRINT //(
#define COLOR_SPACE_DEBUG_PRINT_WRAPPER(...) SiiOsDebugPrint(__FILE__,__LINE__,SII_OSAL_DEBUG_TX,__VA_ARGS__)
    #define COLOR_SPACE_DEBUG_PRINT(x)  COLOR_SPACE_DEBUG_PRINT_WRAPPER x
#else //)(
    #define COLOR_SPACE_DEBUG_PRINT(...)	/* nothing */
#endif //)

#endif /* if !defined(SI_INFOFRAME_H) */
