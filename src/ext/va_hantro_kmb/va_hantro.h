/*
 * Copyright(C) 2018 Verisilicon
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL Verisilicon AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _VA_HANTRO_H_
#define _VA_HANTRO_H_

#include <va/va.h>

#ifdef __cplusplus
extern "C" {
#endif

/** %uint32_t */

/**
 * \mainpage Introduction
 *
 * The libva open-source software library is a Video Acceleration API (VA-API) which provides
 * access to hardware accelerated video encoding/decoding and video processing. libVA code is
 * Open Source distributed under the MIT License.
 *
 * This document describes the VA-API extensions of the Hantro VCCORE Video Encoder and Decoder
 * hardware. Details of the extensions are described in this document.
 *
 * The code is written in C and parameter types follow standard C conventions. This document
 * assumes that the readers understand the fundamentals of C-language and the HEVC (H.265) and
 * AVC(H.264) standard.
 *
 * \section s1 Supported Standards
 * The API discussed in this version of the document is compatible with the following video
 * encoder standards and profiles. Additional levels may be supported via software.
 *
 * - HEVC (H.265) - ITU-T Rec. H.265 (04/2013), ISO/IEC 23008-2 9
 *  - Main Profile, Level 5.1, High Tier
 *  - Main10 profile, Level 5.1, High Tier
 *  - Main Still Profile
 * -  AVC (H.264) - ITU-T Rec. H.264 (03/2010) / ISO/IEC 14496-10
 *  - Main Profile, levels 1 - 5.2
 *  - High Profile, levels 1 - 5.2
 *  - High 10 Profile, levels 1 - 5.2
 *  - Baseline Profile, levels 1 - 5.2
 *
 * \section s2 Compatible Hardware
 * - Hantro VC8000E Video Encoder v6.2.x
 * - Hantro VC8000D Video Decoder v7.1.x
 *
 * \section s3 Compatible Software
 * -  VA-API version 2.1.0. you can check the versiono of the vaapi using command:
 *  <pre> pkg-config --modversion libva. </pre>
 *
 * \section s4 References
 *
 * \subsection s4s1 Vivante VC Documentation
 * - Hantro.VC.Video Encoder API describes the API of the Hantro HEVC/AVC video encoder hardware.
 * - Hantro.VC.VC8000D Video Decoder API - HEVC/VP9 describes the API of the Hantro HEVC/VP9 video decoder hardware.
 *
 * \subsection s4s2 Standards Documentation
 * [1] ITU-T H.265. High Efficiency Video Coding
 *
 */

/**
 * \page api Vivante Extension Support for VAAPI
 *
 * This section describes the Vivante extensions in the VAAPI declared in "va_hantro.h".
 * Unless specified otherwise, support for the referenced extension is based on va.h version 2.1.0.
 *
 * "va_hantro.h" should be included in the release package in the vsi_vaapi_driver/src subdirectory,
 * after the driver is compiled, it can be installed to specified path. If not specified, the
 * va_hantro.h is installed into directory "/usr/local/va_hantro/".
 *
 * To use the extensions in your program, include file "va_hantro.h" in your project.
 *
 * This API includes some deprecated functions which may have been supported for earlier
 * Hantro encoders but which are no longer supported for use with VCCORE hardware. These
 * are listed in a separate section near the end of the document.
 *
 * \section api_s1 Extension Introduction
 *
 * The extension includes 3 parts:
 * - the picture format extension
 * - the profile extension
 * - the configure attribute extension
 * - the buffer type extnsion
 *
 * The detail descriptions are in module chapter.
 */

/** Padding size in 4-bytes */
#define HANTRO_PADDING_LOW          4
#define HANTRO_PADDING_MEDIUM       8
#define HANTRO_PADDING_HIGH         16
#define HANTRO_PADDING_LARGE        32

#define HANTRO_PROFILE_START 100
#define HANTRO_CONFIG_ATTRIB_TYPE_START 100
#define HANTRO_BUFFER_TYPE_START 100
#define HANTRO_ENC_MISC_PARAMETER_TYPE_START 100
#define HANTRO_SURFACE_STATUS_START 100
#define HANTRO_SURFACE_ATTRIB_START 100

/** \Extend Surface status types. */
typedef enum {
    HANTROSurfaceCompressedByDec400 = HANTRO_SURFACE_STATUS_START    /* Compressed surface */
} HANTROSurfaceStatus;

/** \Extend Surface attribute types. */
typedef enum {
    HANTROSurfaceAttribDec400Mode = HANTRO_SURFACE_ATTRIB_START
} HANTROSurfaceAttribType;

#define HANTRO_SURFACE_ATTRIB_DEC400_DISABLE 0
#define HANTRO_SURFACE_ATTRIB_DEC400_STREAM_BY_PASS_MODE 1
#define HANTRO_SURFACE_ATTRIB_DEC400_WORK_MODE 2

/**
 * \defgroup va_fmt Hantro Image Format Extension
 *
 * VC8000E have embedded pre-processing HW. This make it can support more raw picutre as input.
 *  The extension defines the raw picture format with the FOURCC code and RT_FORMAT.
 *
 * @{
 */
/** \brief RGB444 format takes 16 bit for one sample */
#define HANTRO_FOURCC_RGB444 VA_FOURCC('H','4','4','4')
/** \brief RGB565 format takes 16 bit for one sample */
#define HANTRO_FOURCC_RGB565 VA_FOURCC('H','5','6','5')
/** \brief RGB555 format takes 16 bit for one sample */
#define HANTRO_FOURCC_RGB555 VA_FOURCC('H','5','5','5')
/** \brief RGB888 format takes 32 bit for one sample */
#define HANTRO_FOURCC_RGB888 VA_FOURCC('H','8','8','8')
/** \brief RGB101010 format takes 32 bit for one sample */
#define HANTRO_FOURCC_RGB101010 VA_FOURCC('H','a','a','a')

/** \brief YA00 is 10-bit greyscale.*/
#define HANTRO_FOURCC_YA00 VA_FOURCC('Y','A','0','0') //0x30304159

//vsi using libva version didn't have VA_RT_FORMAT_YUV420_10, but VA_RT_FORMAT_YUV420_10BPP
#ifndef VA_RT_FORMAT_YUV420_10
#define VA_RT_FORMAT_YUV420_10 0x00000100
#endif
/** \brief 10 bit YUV420 format, each component is saved as 16 bits in big-endian format.*/
#define HANTRO_RT_FORMAT_YUV420_10BPP_BE (VA_RT_FORMAT_YUV420_10 + 1)
/** \brief 10 bit YUV420 format, each component is saved as 16 bits in little-endian format.*/
#define HANTRO_RT_FORMAT_YUV420_10BPP_LE (VA_RT_FORMAT_YUV420_10 + 2)
/** \brief 10 bit YUV400 format, each component is saved as 16 bits in big-endian format.*/
#define HANTRO_RT_FORMAT_YUV400_10BPP_BE (VA_RT_FORMAT_YUV420_10 + 3)
/** \brief 10 bit YUV400 format, each component is saved as 16 bits in little-endian format.*/
#define HANTRO_RT_FORMAT_YUV400_10BPP_LE (VA_RT_FORMAT_YUV420_10 + 4)

#define HANTRO_MAGIC(ch0, ch1, ch2, ch3) \
    ((unsigned long)(unsigned char) (ch0) | ((unsigned long)(unsigned char) (ch1) << 8) | \
    ((unsigned long)(unsigned char) (ch2) << 16) | ((unsigned long)(unsigned char) (ch3) << 24 ))

#define HANTRO_IMAGE_GPUNODE_MAGIC HANTRO_MAGIC('N','O','D','E')
#define HANTRO_IMAGE_DEC400_MAGIC HANTRO_MAGIC('D','4','0','0')

/** values of _HANTROImageFormat tile_mode */
typedef enum {
/** \brief un-compressed data.*/
    HantroTileModeNone = 0,
/** \brief compress tile 256 input.*/
    HantroTileMode256,
    HantroTileModeEnd
} HANTROTileMode;

/** values of _HANTROImage flag */
typedef enum {
/** \brief didn't assign gpunodes .*/
    HantroImageNotAssignGpuNodes = 0,
/** \brief flag of assign gpunodes*/
    HantroImageAssignGpuNodes,
    HANTROImageGpuNodeVaReservedFlagEnd
} HANTROImageGpuNodeVaReservedFlag;

/** \Extension of VAImage va_reserved to tell which slice belong to*/
typedef struct _HANTROImageGpuNodeVaReserved
{
    uint32_t	magic;
    uint32_t	flag;
    uint32_t	vagpunodes;
    uint32_t	reserved;
} HANTROImageGpuNodeVaReserved;

/** \Extension of VAImageForamt va_reserved to tell dec400 table info*/
typedef struct _HANTROImageFormat
{
    uint32_t	magic;
    uint32_t	tile_mode;
    uint32_t	table_size;
    uint32_t	table_offset;
} HANTROImageFormat;

/**@}*/

typedef struct _HANTRORectangle {
    int16_t x;
    int16_t y;
    uint16_t width;
    uint16_t height;
} HANTRORectangle;

/** Generic ID type, can be re-typed for specific implementation */
typedef unsigned int HANTROGenericID;

typedef HANTROGenericID HANTROConfigID;

typedef HANTROGenericID HANTROBufferID;

/**
 * \defgroup va_profile Hantro Profile Extension
 *
 * Profile defines the toolset used in the stream.
 *
 * @{
 */
/** Extension of VAProfile */
typedef enum {
    /**
    * \brief HEVC/H265 main still proifle.
    *
    * This profile is defined in HEVC specification. In such profile, only intra frame is encoded.
    */
    HANTROProfileHEVCMainStill = HANTRO_PROFILE_START,
    /**
    * \brief AVC/H264 high 10 profile.
    *
    * This profile is defined in H264 specification. In such profile, pixel samples can be encoded as
    * 10 bits depth.
    */
    HANTROProfileH264High10,

    HANTROProfileMax
} HANTROProfile;

/**@}*/

/**
 * \defgroup va_attr Hantro Configure Attribute Extensions
 *
 * Configure defines the feature set when initialize on context. It includes a list of attributes.
 *  Each attribute defines a feature. To create a configure, application should query the configure
 *  list firstly by calling vaGetConfigAttributes(). The query result show what feature can be
 *  supported and what's the range of valid parameters. The vaCreateConfig() is used to create a
 *  valid configure with a list of attribute with their parameter. Such configure will be use to
 *  create context.
 *
 * @{
 */

/** Extension of VAConfigAttribType */
typedef enum {
    /**
    * \brief Encoder embedded preprocessing attribute. Read-only.
    *
    * This attribute conveys whether the driver supports encoder embedded preprocessing features
    * The attribute value is partitioned into fields as defined in the
    * HANTROConfigAttribValEncEmbeddedPreprocess union.
    */
    HANTROConfigAttribEncEmbeddedPreprocess = HANTRO_CONFIG_ATTRIB_TYPE_START,
    /**
     * \brief Encoder recon frame compression attribute. Read-only.
     *
     * This attribute conveys whether the driver supports encoder recon frame compression features
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncReconFrameCompression union.
     */
    HANTROConfigAttribEncReconFrameCompression,
    /**
    * \brief packed header output control attribute. Read-only.
    *
    * This attribute conveys whether the driver supports packed header output such as sps, pps sei
    * and aud can be contolled
    * The attribute value is partitioned into fields as defined in the
    * HANTROConfigAttribValEncPackedHeaderOutputControl union.
    */
    HANTROConfigAttribEncPackedHeaderOutputControl,
    /**
     * \brief Encoding IPCM region attribute. Read-only.
     *
     * This attribute conveys whether the driver supports IPCM region encoding,
     * based on user provided IPCM region rectangles.  The attribute value is partitioned into fields
     * as defined in the HANTROConfigAttribValEncIPCM union.
     *
     * If IPCM region encoding is supported, the IPCM region information is passed to the driver using
     * HANTROEncMiscParameterTypeIPCM.
     */
    HANTROConfigAttribEncIPCM,
    /**
     * \brief Encoding region-of-interest (ROI) Map attribute. Read-only.
     *
     * This attribute conveys whether the driver supports region-of-interest (ROI) Map encoding,
     * based on user provided ROI Map.  The attribute value is partitioned into fields
     * as defined in the HANTROConfigAttribValEncROIMap union.
     *
     * If ROI Map encoding is supported, the ROI Map information is passed to the driver using
     * HANTROEncMiscParameterTypeROIMap.
     */
    HANTROConfigAttribEncROIMap,
    /**
     * \brief Encoding intra area region attribute. Read-only.
     *
     * This attribute conveys whether the driver supports intra area region encoding,
     * based on user provided intra area region rectangles.  The attribute value is partitioned into fields
     * as defined in the HANTROConfigAttribValEncIntraArea union.
     *
     * If intrea area region encoding is supported, the intra area region information is passed to the driver using
     * HANTROEncMiscParameterTypeIntraArea.
     */
    HANTROConfigAttribEncIntraArea,

    /**
     * \brief Encoding MB or CU infomation output attribute. Read-only.
     *
     * This attribute conveys whether the driver supports encoding MB or CU infomation output,
     * if user provided MB or CU information buffer which type is HANTROEncMBCUOutPutInfoBufferType.
     * The attribute value is partitioned into fields
     * as defined in the HANTROConfigAttribValEncMBCUInfo union.
     *
     * If MB CU Out Info is supported, the MB CU encoding information is passed to application using
     * MB or CU buffer defined in HANTROEncMiscParameterTypeMBCUInfoBuffer.
     */
    HANTROConfigAttribEncMBCUInfo,

    /**
     * \brief cyclic intra refresh(CIR) Encoding attribute. Read-only.
     *
     * This attribute conveys whether the driver supports CIR encoding,
     *  based on user provided the first start MB or CTU and interval.
     *  If CIR encoding is supported, the CIR information is passed to the driver using
     *  HANTROEncMiscParameterCIR
     */
    HANTROConfigAttribEncCIR,
    /**
     * \brief Gradual decoder refresh(GDR) Encoding attribute. Read-only.
     *
     * This attribute conveys whether the driver supports GDR encoding,
     *  based on user provided the gdr interval.
     *  If GDR encoding is supported, the GDR information is passed to the driver using
     *  HANTROEncMiscParameterGDR
     */
    HANTROConfigAttribEncGDR,

    /**
     * \brief video full range attribute. Read-only.
     *
     * This attribute conveys whether the driver supports video full range config,
     *  If video full range is supported, the video full range information is passed to the driver using
     *  HANTROEncMiscParameterVideoFullRange
     */
    HANTROConfigAttribEncVideoFullRange,

    /**
     * \brief Extended Encoding region-of-interest (ROI) attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support ROI attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncROIMoreBit union.
     *
     * The HANTROConfigAttribValEncROIMoreBit attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncROIMoreBit,

    /**
     * \brief Extended Encoding coding control attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support coding control attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncCodingCtrl union.
     *
     * The HANTROConfigAttribValEncCodingCtrl attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncCodingCtrl,

    /**
     * \brief Extended Encoding global mv attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support global mv attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncGlobalMV union.
     *
     * The HANTROConfigAttribValEncGlobalMV attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncGlobalMV,

    /**
     * \brief Extended Encoding special frame attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support special frame attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncSpecialFrame union.
     *
     * The HANTROConfigAttribValEncSpecialFrame attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncSpecialFrame,

    /**
     * \brief Extended Encoding skip frame attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support  skip frame attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncSkipFrame union.
     *
     * The HANTROConfigAttribValEncSkipFrame attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncSkipFrame,

    /**
     * \brief Extended reporting parameters attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support reporting parameters attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncReportingParams union.
     *
     * The HANTROConfigAttribValEncReportingParams attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncReportingParams,

    /**
     * \brief Extended log level parameters attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support log level attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncLogLevel union.
     *
     * The HANTROConfigAttribValEncLogLevel attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncLogLevel,

    /**
     * \brief Extended HDR10 parameters attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support HDR10 attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValHDR10 union.
     *
     * The HANTROConfigAttribValHDR10 attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncHDR10,

    /**
     * \brief Extended JPEGLossless parameters attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support JPEGLossless attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValJPEGLossless union.
     *
     * The HANTROConfigAttribValJPEGLossless attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncJPEGLossless,

    /**
     * \brief Extended SEI User Data parameters attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support SEI User Data attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValSEIUserData union.
     *
     * The HANTROConfigAttribValSEIUserData attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncSEIUserData,

    /**
     * \brief Extended low latency parameters attribute. Read-only.
     *
     * This attribute conveys some extension attribute to support low latency attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValLowLatency union.
     *
     * The HANTROConfigAttribValLowLatency attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncLowLatency,

    /**
    * \brief Extended Encoding Lookahead attribute. Read-only.
    *
    * This attribute conveys some extension attribute to support ROI attribute.
    * The attribute value is partitioned into fields as defined in the
    * HANTROConfigAttribValEncROIMoreBit union.
    *
    * The HANTROConfigAttribValEncROIMoreBit attribute should be checked firstly before check
    * this extension.
    */
    HANTROConfigAttribEncLookahead,

    /**
     * \brief Extended Encoding multi-core. Read-only.
     *
     * This attribute conveys some extension attribute to support ROI attribute.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncROIMoreBit union.
     *
     * The HANTROConfigAttribValEncROIMoreBit attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncMultiCore,

    /**
     * \brief Extended Decoder dec400. Read-only.
     *
     * This attribute conveys some extension attribute to get dec400 attribute info for decoder.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValDecDec400 union.
     *
     * The HANTROConfigAttribValDecDec400 attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribDecDec400,


    /**
     * \brief Extended Encoder dec400. Read-only.
     *
     * This attribute conveys some extension attribute to get dec400 attribute info for encoder.
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncDec400 union.
     *
     * The HANTROConfigAttribValEncDec400 attribute should be checked firstly before check
     * this extension.
     */
    HANTROConfigAttribEncDec400,

    /** @name Attributes for decoding */
    /**
    * \brief Decoder features. Read-only.
    *
    * This attribute conveys whether the driver supports some decoder HW features
    * The attribute value is partitioned into fields as defined in the
    * HANTROConfigAttribValDecMiscParameter union.
    */
    HANTROConfigAttribDecMiscParameter,

    /**
     * \brief Decoder embedded postprocessing attribute. Read-only.
     *
     * This attribute conveys whether the driver supports decoder embedded preprocessing features
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValDecEmbeddedPostprocess union.
     */
    HANTROConfigAttribDecEmbeddedPostprocess,
    HANTROConfigAttribTypeMax
} HANTROConfigAttribType;


/**
 * \brief Attribute value for HANTROConfigAttribValDecEmbeddedPostprocess
 *
 * Embedded Post Processing is on-the-fly with decoder.
 * \deprecated use VAConfigAttribDecProcessing attribute.
 */
typedef union _HANTROConfigAttribValDecEmbeddedPostprocess {
    struct {
        /**
         * \brief whether decoder embedded postprocessing support cropping
         *
         * \ref dec_embedded_postprocessing_support_cropping  specifies
         * if decoder embedded postprocessing support cropping.
         * dec_embedded_postprocessing_support_cropping equal to 1 implies the underlying driver
         * support cropping in decoder embedded postprocessing.
         * equal to 0 implies the underlying driver doesn't support cropping
         * in decoder embedded postprocessing.
         */
        unsigned int dec_embedded_postprocessing_support_cropping              : 1;

        /**
         * \brief whether decoder embedded postprocessing support downscaling
         *
         * \ref dec_embedded_postprocessing_support_downscale  specifies
         * if decoder embedded postprocessing support downscaling.
         * dec_embedded_postprocessing_support_downscale equal to 1 implies
         * the underlying driver  support downscaling in decoder embedded postprocessing.
         * equal to 0 implies the underlying driver doesn't support downscaling
         * in decoder embedded postprocessing.
         */
        unsigned int dec_embedded_postprocessing_support_downscale             : 1;

        /**
         * \brief whether decoder embedded postprocessing support NV21 output format
         *
         * \ref dec_embedded_postprocessing_support_nv21  specifies
         * if decoder embedded postprocessing support NV21.
         * dec_embedded_postprocessing_support_nv21 equal to 1
         * implies the underlying driver  support NV21 in decoder embedded postprocessing.
         * equal to 0 implies the underlying driver doesn't support NV21 in decoder embedded postprocessing.
         */
        unsigned int dec_embedded_postprocessing_support_nv21                  : 1;

        /**
         * \brief whether decoder embedded postprocessing support P010 format
         *
         * \ref dec_embedded_postprocessing_support_p010  specifies
         * if decoder embedded postprocessing support p010, which only available for HEVC main10 streams.
         * dec_embedded_postprocessing_support_p010 equal to 1 implies the underlying driver
         * support p010 output in decoder embedded postprocessing.
         * equal to 0 implies the underlying driver doesn't support p010 output
         * in decoder embedded postprocessing.
         */
        unsigned int dec_embedded_postprocessing_support_p010                  : 1;

        /**
         * \brief whether decoder embedded postprocessing support P010 format
         *
         * \ref dec_embedded_postprocessing_support_8bits  specifies
         * if decoder embedded postprocessing support 8bits output,
         * which only available for HEVC main10 streams.
         * dec_embedded_postprocessing_support_8bits equal to 1
         * implies the underlying driver  support 8bits output in decoder embedded postprocessing.
         * equal to 0 implies the underlying driver doesn't support 8bits output in decoder embedded postprocessing.
         */
        unsigned int dec_embedded_postprocessing_support_8bits                 : 1;

        /**
        * \brief whether decoder embedded postprocessing support big edian format
        *
        * \ref dec_embedded_postprocessing_support_pbe  specifies
        * if decoder embedded postprocessing support big endian output(in 128bit),
        * which only available for HEVC streams.
        * dec_embedded_postprocessing_support_pbe equal to 1
        * implies the underlying driver support big endian output(in 128bit) in decoder embedded postprocessing.
        * equal to 0 implies the underlying driver doesn't support bigE output in decoder embedded postprocessing.
        */
        unsigned int dec_embedded_postprocessing_support_pbe                   : 1;

        unsigned int reserved                                                  : 26;
    } bits;
    unsigned int value;
} HANTROConfigAttribValDecEmbeddedPostprocess;


/** \brief Attribute value for HANTROConfigAttribValDecMiscParameter */
typedef union _HANTROConfigAttribValDecMiscParameter {
    struct {
        /**
         * \brief whether decoder support reference frame compression
         *
         * \ref dec_support_ref_frame_compression  specifies if decoder  support reference frame compression.
         * dec_support_ref_frame_compression equal to 1 implies the underlying driver  support ref frame compression.
         * equal to 0 implies the underlying driver doesn't support ref frame compression.
         */
        unsigned int dec_support_ref_frame_compression              : 1;

        /**
         * \brief whether decoder support reference frame compression
         *
         * \ref dec_support_stride  specifies if decoder support frame buffer stride.
         * dec_support_stride equal to 1 implies the underlying driver  support frame buffer stride.
         * equal to 0 implies the underlying driver doesn't support frame buffer stride.
         */
        unsigned int dec_support_stride                             : 1;

        /**
         * \brief whether decoder support separate field storage in dpb.
         *
         * \ref dec_support_field_dpb  specifies if decoder  support  separate field storage in dpb.
         * dec_support_field_dpb equal to 1 implies the underlying driver  support separate field storage.
         * equal to 0 implies the underlying driver doesn't support separate field storage.
         */
        unsigned int dec_support_field_dpb                           : 1;

        unsigned int reserved                                        : 29;
    } bits;
    unsigned int value;
} HANTROConfigAttribValDecMiscParameter;

/** \brief Attribute value HANTROConfigAttribValEncROIMoreBit */
typedef union _HANTROConfigAttribValEncROIMoreBit {

    struct {
        uint32_t roi_rc_qp_absolute_support    : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncROIMoreBit;


/** \brief Attribute value for HANTROConfigAttribValEncIPCM */
typedef union _HANTROConfigAttribValEncIPCM {
    struct {
        /** \brief The number of IPCM regions supported, 0 if IPCM region is not supported. */
        uint32_t num_roi_regions        : 8;
        uint32_t reserved                   : 24;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncIPCM;

/** \brief Attribute value for HANTROConfigAttribValEncIntraArea */
typedef union _HANTROConfigAttribValEncIntraArea {
    struct {
        /** \brief The number of intra area regions supported, 0 if intra area region is not supported. */
        uint32_t num_intra_area_regions         : 8;
        uint32_t reserved                   : 24;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncIntraArea;


/**
 * \brief Attribute value for HANTROConfigAttribValEncROIMap
 *
 * ROI Map can control the qp of each block by set render buffer with type of HANTROEncMiscParameterTypeROIMap.
 *
 */
typedef union _HANTROConfigAttribValEncROIMap {
    struct {
        /**
         * \brief A flag indicates whether ROI priority is supported
         *
         * \ref roimap_rc_priority_support equal to 1 specifies the underlying driver supports
         * ROI map priority when VAConfigAttribRateControl != VA_RC_CQP
         * \ref roimap_rc_priority_support equal to 0 specifies
         * the underlying driver doesn't support ROI map priority.
         *
         * User should ignore \ref roimap_rc_priority_support when VAConfigAttribRateControl == VA_RC_CQP
         * because ROI delta QP is always required when VAConfigAttribRateControl == VA_RC_CQP.
         */
        uint32_t roimap_rc_priority_support : 1;
        /**
         * \brief A flag indicates whether ROIMap delta QP is supported
         *
         * \ref roimap_qp_delat_support equal to 1 specifies the underlying driver supports
         * ROIMap delta QP , user can use \c these values
         * in roimap buffer to set ROIMap delta QP. \ref roimap_qp_delat_support equal to 0 specifies
         * the underlying driver doesn't support ROIMap delta QP.
         *
         */
        uint32_t roimap_qp_delta_support    : 1;
        /**
         * \brief A flag indicates whether ROIMap absolute QP is supported
         *
         * \ref roimap_qp_absolute_support equal to 1 specifies the underlying driver supports
         * ROIMap absolute QP , user can use \c these values
         * in roimap buffer to set ROIMap absolute QP. \ref roimap_qp_absolute_support equal to 0 specifies
         * the underlying driver doesn't support ROIMap absolute QP.
         *
         */
        uint32_t roimap_qp_absolute_support    : 1;
        /**
         * \brief A flag indicates whether IPCMMap  is supported
         *
         * \ref ipcm_map_support equal to 1 specifies the underlying driver supports IPCMMap
         *
         */
        uint32_t ipcm_map_support       : 1;
        /**
         * \brief A value indicates which type of  data structure in roi map buffer is supported.
         *
         * \ref
         *
         */
        uint32_t roimap_buffer_structure       : 8;
        uint32_t reserved                   : 21;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncROIMap;

/** \brief Attribute value for HANTROConfigAttribValEncMBCUInfo */
typedef union _HANTROConfigAttribValEncMBCUInfo {
    struct {
        /** \brief if support mb or cu encoding information output.
                1 : supported; 0 : not supported. */
        uint32_t mb_cu_info_output              : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncMBCUInfo;

/** \brief Attribute value for HANTROConfigAttribValEncCIR */
typedef union _HANTROConfigAttribValEncCIR {
    struct {
        /** \brief support CIR encoding or not
         *  1 support CIR encoding,
         *  0 not support CIR encoding.
         */
        uint32_t cir_encoding_supported         : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncCIR;

/** \brief Attribute value for HANTROConfigAttribValEncGDR */

typedef union _HANTROConfigAttribValEncGDR {
    struct {
        /** \brief support GDR or not.
         * 1 if  support GDR encoding,
         * 0 if not support GDR encoding.
         */
        uint32_t gdr_encoding_supported         : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncGDR;

/** \brief Attribute value for HANTROConfigAttribValEncVideoFullRange
 *
 * The information will be written into SPS VUI information, has not impact on encoding.
 */
typedef union _HANTROConfigAttribValEncVideoFullRange {
    struct {
        /** \brief 1 if  support video full range config,
                0 if not support video full range config. */
        uint32_t video_full_range_supported         : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncVideoFullRange;

/** \brief Attribute value for HANTROConfigAttribValEncLookahead */

typedef union _HANTROConfigAttribValEncLookahead {
    struct {
        /** \brief 1 if  support video full range config,
                0 if not support video full range config. */
        uint32_t lookaheadSupported         : 1;
        uint32_t lookaheadDepthMax          : 6;
        uint32_t lookaheadDepth             : 6;
        uint32_t extDSRatio                 : 1;
        uint32_t reserved                   : 18;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncLookahead;

/** \brief Attribute value for HANTROConfigAttribValEncLookahead */

typedef union _HANTROConfigAttribValEncMultiCore {
    uint32_t coreNum;
} HANTROConfigAttribValEncMultiCore;

/** \brief Attribute value for HANTROConfigAttribValDecDEC400 */

typedef union _HANTROConfigAttribValDecDec400 {
    struct {
        /** \brief 0 if dec400 didn't connect with decoder,
                   1 if dec400 connects with decoder*/
        uint32_t dec400Supported             : 1;
        uint32_t dec400StreamByPassSupported : 1;
        uint32_t Tile256Supported            : 1;
        uint32_t Tile128Supported            : 1;
        uint32_t Tile64Supported             : 1;
        uint32_t Tile32Supported             : 1;
        uint32_t BitDepth16Supported         : 1;
        uint32_t BitDepth14Supported         : 1;
        uint32_t BitDepth12Supported         : 1;
        uint32_t BitDepth10Supported         : 1;
        uint32_t BitDepth8Supported          : 1;
        uint32_t I010Supported               : 1;
        uint32_t I420Supported               : 1;
        uint32_t UYUVSupported               : 1;
        uint32_t YUY2Supported               : 1;
        uint32_t YV12Supported               : 1;
        uint32_t P016Supported               : 1;
        uint32_t P010Supported               : 1;
        uint32_t NV12Supported               : 1;
        uint32_t reserved                   : 13;
    } bits;
    uint32_t value;
} HANTROConfigAttribValDecDec400;

/** \brief Attribute value for HANTROConfigAttribValEncDEC400 */

typedef union _HANTROConfigAttribValEncDec400 {
    struct {
        /** \brief 0 if dec400 didn't connect with encoder,
                   1 if dec400 connects with encoder*/
        uint32_t dec400Supported             : 1;
        uint32_t dec400StreamByPassSupported : 1;
        uint32_t Tile256Supported            : 1;
        uint32_t Tile128Supported            : 1;
        uint32_t Tile64Supported             : 1;
        uint32_t Tile32Supported             : 1;
        uint32_t BitDepth16Supported         : 1;
        uint32_t BitDepth14Supported         : 1;
        uint32_t BitDepth12Supported         : 1;
        uint32_t BitDepth10Supported         : 1;
        uint32_t BitDepth8Supported          : 1;
        uint32_t I010Supported               : 1;
        uint32_t I420Supported               : 1;
        uint32_t UYUVSupported               : 1;
        uint32_t YUY2Supported               : 1;
        uint32_t YV12Supported               : 1;
        uint32_t P016Supported               : 1;
        uint32_t P010Supported               : 1;
        uint32_t NV12Supported               : 1;
        uint32_t reserved                   : 13;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncDec400;

#define ROI_MAP_DATA_BUFFER_STRUCTUE_FORMAT_A    0

/** \brief Attribute value for HANTROConfigAttribEncEmbeddedPreprocess */
typedef union _HANTROConfigAttribValEncEmbeddedPreprocess {
    struct {
        /**
         * \brief whether encoder embedded preprocessing support cropping
         *
         * \ref enc_embedded_preprocessing_support_cropping  specifies
         * if encoder embedded preprocessing support cropping.
         * enc_embedded_preprocessing_support_cropping equal to 1
         * implies the underlying driver  support cropping in encoder embedded preprocessing.
         * equal to 0 implies the underlying driver doesn't support
         * cropping in encoder embedded preprocessing.
         */
        uint32_t enc_embedded_preprocessing_support_cropping             : 1;

        /**
         * \brief whether encoder embedded preprocessing support ratation
         *
         * \ref enc_embedded_preprocessing_support_rotation  specifies
         * if encoder embedded preprocessing support ratation.
         * enc_embedded_preprocessing_support_rotation equal to 1 implies
         * the underlying driver  support rotation in encoder embedded preprocessing.
         * equal to 0 implies the underlying driver doesn't support rotation
         * in encoder embedded preprocessing.
         */
        uint32_t enc_embedded_preprocessing_support_rotation             : 1;

        /**
         * \brief whether encoder embedded preprocessing support mirror
         *
         * \ref enc_embedded_preprocessing_support_mirror  specifies
         * if encoder embedded preprocessing support mirror.
         * enc_embedded_preprocessing_support_mirror equal to 1 implies
         * the underlying driver  support mirror in encoder embedded preprocessing.
         * equal to 0 implies the underlying driver doesn't support mirror
         * in encoder embedded preprocessing.
         */
        uint32_t enc_embedded_preprocessing_support_mirror               : 1;

        /**
         * \brief whether encoder embedded preprocessing support constant chroma
         *
         * \ref enc_embedded_preprocessing_support_constant_chroma  specifies
         * if encoder embedded preprocessing support constant chroma.
         * enc_embedded_preprocessing_support_constant_chroma equal to 1
         * implies the underlying driver  support constant chroma in encoder embedded preprocessing.
         * equal to 0 implies the underlying driver doesn't support constant chroma
         * in encoder embedded preprocessing.
         */
        uint32_t enc_embedded_preprocessing_support_constant_chroma      : 1;

        uint32_t enc_embedded_preprocessing_support_color_conversion      : 1;

        uint32_t enc_embedded_preprocessing_support_down_scale            : 1;
        uint32_t reserved                                       : 26;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncEmbeddedPreprocess;

/** \brief Attribute value for HANTROConfigAttribValEncReconFrameCompression */
typedef union _HANTROConfigAttribValEncReconFrameCompression {
    struct {
        /**
         * \brief whether encoder support recon frame compression
         *
         * \ref enc_support_recon_frame_compression  specifies if encoder  support recon frame compression.
         * enc_support_recon_frame_compression equal to 1
         * implies the underlying driver  support recon frame compression.
         * equal to 0 implies the underlying driver doesn't support recon frame compression.
         */
        uint32_t enc_support_recon_frame_compression             : 1;
        uint32_t enable_P010_ref_attri                           : 1;

        uint32_t reserved                                       : 30;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncReconFrameCompression;

/** \brief Attribute value for HANTROConfigAttribValEncPackedHeaderOutputControl */
typedef union _HANTROConfigAttribValEncPackedHeaderOutputControl {
    struct {
        /**
         * \brief whether packed header output can be controlled
         *
         * \ref enc_support_sps_output_control  specifies if encoder  support sps can be resent.
         * enc_support_sps_output_control equal to 1 implies the underlying driver  support sps can be resent.
         * equal to 0 implies the underlying driver doesn't support sps is resent.
         * \ref enc_support_pps_output_control  specifies if encoder  support pps can be resent.
         * enc_support_pps_output_control equal to 1 implies the underlying driver  support pps can be resent.
         * equal to 0 implies the underlying driver doesn't support pps is resent.
         * \ref enc_support_sei_output_control  specifies if encoder  support sei can be sent.
         * enc_support_sei_output_control equal to 1 implies the underlying driver  support sei can be sent.
         * equal to 0 implies the underlying driver doesn't support sei is sent.
         * \ref enc_support_aud_output_control  specifies if encoder  support aud can be sent.
         * enc_support_aud_output_control equal to 1 implies the underlying driver  support aud can be sent.
         * equal to 0 implies the underlying driver doesn't support aud is sent.
         */
        uint32_t enc_support_sps_output_control                  : 1;
        uint32_t enc_support_pps_output_control                  : 1;
        uint32_t enc_support_sei_output_control                  : 1;
        uint32_t enc_support_aud_output_control                  : 1;

        uint32_t reserved                                       : 28;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncPackedHeaderOutputControl;

/** \brief Attribute value for HANTROConfigAttribValEncCodingCtrl */
typedef union _HANTROConfigAttribValEncCodingCtrl {
    struct {
        /**
         * \brief whether encoder support coding ctrl parameters.
         *
         * \ref enc_support_coding_ctrl  specifies if encoder support setting coding control parameters.
         * enc_support_coding_ctrl equal to 1
         * implies the underlying driver  support setting coding control parameters.
         * equal to 0 implies the underlying driver doesn't support setting coding control parameters.
         */
        uint32_t enc_support_setting_coding_ctrl                : 1;
        uint32_t reserved                                       : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncCodingCtrl;

/** \brief Attribute value for HANTROConfigAttribValEncGlobalMV */
typedef union _HANTROConfigAttribValEncGlobalMV {
    struct {
        /**
         * \brief whether encoder support global mv parameters.
         *
         * \ref enc_support_global_mv  specifies if encoder  support setting global mv parameters.
         * enc_support_global_mv equal to 1
         * implies the underlying driver  support setting global mv parameters.
         * equal to 0 implies the underlying driver doesn't support setting global mv parameters.
         */
        uint32_t enc_support_global_mv                          : 1;
        uint32_t reserved                                       : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncGlobalMV;

/** \brief Attribute value for HANTROConfigAttribValEncSpecialFrame */
typedef union _HANTROConfigAttribValEncSpecialFrame {
    struct {
        /**
         * \brief whether encoder support special frame parameters.
         *
         * \ref enc_support_global_mv specifies if encoder  support setting special frame parameters.
         * enc_support_special_frame equal to 1
         * implies the underlying driver  support setting special frame parameters.
         * equal to 0 implies the underlying driver doesn't support setting special frame parameters.
         */
        uint32_t enc_support_special_frame                      : 1;
        uint32_t reserved                                       : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncSpecialFrame;

/** \brief Attribute value for HANTROConfigAttribValEncSkipFrame */
typedef union _HANTROConfigAttribValEncSkipFrame {
    struct {
        /**
         * \brief whether encoder support skip frame parameters.
         *
         * \ref enc_support_skip_frame specifies if encoder  support setting skip frame parameters.
         * enc_support_skip_frame equal to 1
         * implies the underlying driver  support setting skip frame parameters.
         * equal to 0 implies the underlying driver doesn't support setting skip frame parameters.
         */
        uint32_t enc_support_skip_frame                      : 1;
        uint32_t reserved                                    : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncSkipFrame;

/** \brief Attribute value for HANTROConfigAttribValEncReportingParams */
typedef union _HANTROConfigAttribValEncReportingParams {
    struct {
        /**
         * \brief whether encoder support skip frame parameters.
         *
         * \ref enc_support_skip_frame specifies if encoder  support setting skip frame parameters.
         * enc_support_skip_frame equal to 1
         * implies the underlying driver  support setting skip frame parameters.
         * equal to 0 implies the underlying driver doesn't support setting skip frame parameters.
         */
        uint32_t enc_output_cu_info                              : 1;
        uint32_t enc_output_hashtype                             : 1;
        uint32_t enc_output_ssim                                 : 1;
        uint32_t enc_output_vui_timing_info                      : 1;
        uint32_t reserved                                        : 28;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncReportingParams;


/** \brief Attribute value for HANTROConfigAttribValEncLogLevel */
typedef union _HANTROConfigAttribValEncLogLevel {
    struct {
        /**
         * \brief whether encoder support skip frame parameters.
         *
         * \ref enc_verbose_support specifies if encoder  support setting skip frame parameters.
         * enc_verbose_support equal to 1
         * implies the underlying driver  support setting skip frame parameters.
         * equal to 0 implies the underlying driver doesn't support setting verbose parameters.
         */
        uint32_t enc_verbose_support                              : 1;
        uint32_t reserved                                         : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncLogLevel;

/** \brief Attribute value for HANTROConfigAttribValEncHDR10 */
typedef union _HANTROConfigAttribValEncHDR10 {
    struct {
        /**
         * \brief whether encoder support hdr10 parameters.
         *
         * \ref enc_hdr10_support specifies if encoder support setting hdr10 parameters.
         * enc_hdr10_support equal to 1
         * implies the underlying driver  support setting hdr10 parameters.
         * equal to 0 implies the underlying driver doesn't support setting hdr10 parameters.
         */
        uint32_t enc_hdr10_support                                : 1;
        uint32_t reserved                                         : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncHDR10;

/** \brief Attribute value for HANTROConfigAttribValEncJPEGLossless */
typedef union _HANTROConfigAttribValEncJPEGLossless {
    struct {
        /**
         * \brief whether encoder support hdr10 parameters.
         *
         * \ref enc_jpeg_lossless_support specifies if encoder support setting jpeg lossless parameters.
         * enc_jpeg_lossless_support equal to 1
         * implies the underlying driver  support setting jpeg lossless parameters.
         * equal to 0 implies the underlying driver doesn't support setting jpeg lossless parameters.
         */
        uint32_t enc_jpeg_lossless_support                                : 1;
        uint32_t reserved                                                 : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncJPEGLossless;

/** \brief Attribute value for HANTROConfigAttribValSEIUserData */
typedef union _HANTROConfigAttribValEncSEIUserData {
    struct {
        /**
         * \brief whether encoder support sei user data parameters.
         *
         * \ref enc_sei_user_data_support specifies if encoder support setting sei userdata parameters.
         * enc_sei_user_data_support equal to 1
         * implies the underlying driver  support setting sei user data parameters.
         * equal to 0 implies the underlying driver doesn't support setting sei user data parameters.
         */
        uint32_t enc_sei_user_data_support                        : 1;
        uint32_t reserved                                         : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncSEIUserData;

/** \brief Attribute value for HANTROConfigAttribValLowLatency */
typedef union _HANTROConfigAttribValEncLowLatency {
    struct {
        /**
         * \brief whether encoder support sei user data parameters.
         *
         * \ref enc_low_latency_support specifies if encoder support setting low latency parameters.
         * enc_low_latency_support equal to 1
         * implies the underlying driver  support setting low latency parameters.
         * equal to 0 implies the underlying driver doesn't support setting low latency parameters.
         */
        uint32_t enc_low_latency_support                        : 1;
        uint32_t reserved                                       : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncLowLatency;

/** @} */

/**
 * \defgroup va_buf_type Hantro Buffer Extension
 *
 * Buffers are used to render a picture. The parameter of some dynamic change feature can be setup
 *  by provide a list of parameter buffers when call vaRenderPicture(). The extensions defines new
 *  types of buffer and new sub-type of VAEncMiscParameterType buffer.
 *
 * @{
 */
/** Extension of VABufferType */
typedef enum {

    HANTRODecEmbeddedPostprocessParameterBufferType = HANTRO_BUFFER_TYPE_START,
    HANTRODecMiscParameterBufferType,
    HANTROEncROIMapBufferType,
    HANTROEncCuCtrlBufferType,
    HANTROEncCuCtrlIndexBufferType,
    HANTROEncMBCUOutputInfoBufferType,
    HANTROEncPSNROutputInfoBufferType,
    HANTROEncUserDataBufferType,
    HANTROEncAdaptiveGOPDecisionBufferType

} HANTROBufferType;


/** Extension of VAEncMiscParameterType */
typedef enum {
    /** \brief Buffer type used for encoder to configure the pre-processing parameters. */
    HANTROEncMiscParameterTypeEmbeddedPreprocess = HANTRO_ENC_MISC_PARAMETER_TYPE_START,
    /** \brief Buffer type used for encoder recon frame compression parameters. */
    HANTROEncMiscParameterTypeReconFrameCompression,
    /** \brief Buffer type used for encoder packed header output control parameters. */
    HANTROEncMiscParameterTypePackedHeaderOutputControl,
    /** \brief Buffer type used for IPCM parameters. */
    HANTROEncMiscParameterTypeIPCM,
    /** \brief Buffer type used for region-of-interest (ROI) Map parameters. */
    HANTROEncMiscParameterTypeROIMap,
    /** \brief Buffer type used for region-of-interest (ROI) cu ctrol parameters. */
    HANTROEncMiscParameterTypeCuCtrl,
    /** \brief Buffer type used for region-of-interest (ROI) cu ctrol parameters. */
    HANTROEncMiscParameterTypeCuCtrlIndex,
    /** \brief Buffer type used for intra area region parameters. */
    HANTROEncMiscParameterTypeIntraArea,
    /** \brief Buffer type used for mb or cu encoding information output parameters. */
    HANTROEncMiscParameterTypeMBCUInfoOutput,
    /** \brief Buffer type used for PSNR output information. */
    HANTROEncMiscParameterTypePSNRInfoOutput,
    /** \brief Buffer type used for cyclic intra refresh(CIR) parameters. */
    HANTROEncMiscParameterTypeCIR,
    /** \brief Buffer type used for gradual decoder refresh(GDR) parameters. */
    HANTROEncMiscParameterTypeGDR,
    /** \brief Buffer type used for video full range parameters. */
    HANTROEncMiscParameterTypeVideoFullRange,
    /** \brief Buffer type used for RateControl extend  parameters. */
    HANTROEncMiscParameterTypeRateControl,
    /** \brief Buffer type used for ROI  extend  parameters. */
    HANTROEncMiscParameterTypeROI,
    /** \brief Buffer type used for CodingCtrl extend  parameters. */
    HANTROEncMiscParameterTypeCodingCtrl,
    /** \brief Buffer type used for global mv  parameters.
     * \see HANTROEncMiscParameterBufferCodingCtrl
     */
    HANTROEncMiscParameterTypeGMV,
    /** \brief Buffer type used for special frame parameters. */
    HANTROEncMiscParameterTypeSpecialFrame,
    /** \brief Buffer type used for skip frame parameters. */
    HANTROEncMiscParameterTypeSkipFrame,
    /** \brief Buffer type used for reporting params parameters. */
    HANTROEncMiscParameterTypeReportingParams,
    /** \brief Buffer type used for Runtime Log Level parameters. */
    HANTROEncMiscParameterTypeRuntimeLogLevel,
    /** \brief Buffer type used for hdr10 parameters. */
    HANTROEncMiscParameterTypeHDR10,
    /* \brief Buffer type used for tier parameters. */
    //HANTROEncMiscParameterTypeTier,
    /** \brief Buffer type used for JPEG Lossless. */
    HANTROEncMiscParameterJPEGLossless,
    /** \brief Buffer type used for SEI user data. */
    HANTROEncMiscParameterSEIUserData,
    /** \brief Buffer type used for Low Latency. */
    HANTROEncMiscParameterLowLatency,
    /* \brief Buffer type used for flush remaining frames. */
    //HANTROEncMiscParameterFlushRemainFrames,
    /* \brief Buffer type used for Low Latency. */
    //HANTROEncMiscParameterMultiCore,
    /** \brief Buffer type used for 2pass. */
    HANTROEncMiscParameterNumberFramesForLastPic,
    /** \brief Buffer type used for SEI user data. */
    HANTROEncMiscParameterAdaptiveGOPDecision,
    HANTROEncMiscParameterTypeMax
} HANTROEncMiscParameterType;

/** \brief Attribute value for HANTROEncMiscParameterRateControl */
typedef struct _HANTROEncMiscParameterRateControl {
    /**  \brief  smooth psnr*/
    uint32_t smooth_psnr_in_GOP;
    /**  \brief  Intra QP delta. intraQP = QP + intraQpDelta
      * This can be used to change the relative quality
      * of the Intra pictures or to lower the size
      * of Intra pictures. [-12..12]
      */
    int32_t intra_qp_delta;
    /** \brief Fixed QP value for all Intra pictures, [0..51]
     * 0 = Rate control calculates intra QP.
     */
    uint32_t fixedIntraQp;

    /** \brief rate_control_mode VA_RC_NONE VA_RC_CBR VA_RC_VBR VA_RC_CQP, etc.
     */
    uint32_t rate_control_mode;

    /**  \brief  tolerance of max Moving bit rate
      * 0...2000% percent tolerance over target bitrate of moving bit rate [2000]
      */
    int32_t tolMovingBitRate;
    /**  \brief  monitor frame length for moving bit rate
      * 10...120, how many frames will be monitored for moving bit rate [frame rate]
      */
    int32_t monitorFrames;
    /**  \brief  variations over average bits per frame for I frame
      * 10...10000% percent variations over average bits per frame for I frame [10000]
      */
    int32_t bitVarRangeI;
    /**  \brief  variations over average bits per frame for P frame
      * 10...10000% percent variations over average bits per frame for P frame [10000]
      */
    int32_t bitVarRangeP;
    /**  \brief  variations over average bits per frame for B frame
      * 10...10000% percent variations over average bits per frame for B frame [10000]
      */
    int32_t bitVarRangeB;
    /**  \brief  I frame bits percent in static scene*/
    uint32_t u32StaticSceneIbitPercent;
    /**  \brief  ctb rc qp delta range
      *  0..15 Max absolute value of CU/MB Qp Delta to Frame QP by CTB RC [10] */
    uint32_t rcQpDeltaRange;
    /**  \brief  ctb rc mb complexity base
      *  0..31 MB complexity base in CTB QP adjustment for Subjective Quality [15]
      *  MB with larger complexity offset to rcBaseMBComplexity, qpOffset larger */
    uint32_t rcBaseMBComplexity;
    /**  \brief  Tolerance of Ctb Rate Control for INTER frames
      *  Tolerance of Ctb Rate Control for INTER frames. (float point number). [0.0]
      *  Ctb Rc will try to limit INTER frame bits within the range of:
      *  [targetPicSize/(1+tolCtbRcInter), targetPicSize*(1+tolCtbRcInter)].
      *  A negative number means no bit rate limit in Ctb Rc. */
    float tolCtbRcInter;
    /**  \brief  Tolerance of Ctb Rate Control for INTRA frames
      *  Tolerance of Ctb Rate Control for INTRA frames. (float point number). [-1.0] */
    float tolCtbRcIntra;
    /**  \brief  minimum pic qp delta*/
    int32_t picQpDeltaMin;
    /**  \brief  maximum pic qp delta*/
    int32_t picQpDeltaMax;
    /**  \brief  Hypothetical Reference Decoder model, [0,1]
      * restricts the instantaneous bitrate and
      * total bit amount of every coded picture.
      * Enabling HRD will cause tight constrains
      * on the operation of the rate control
      * Tolerance of Ctb Rate Control for INTRA frames. (float point number).
      * [-1.0] 0=OFF, 1=ON HRD conformance. [0]
      * Uses standard defined model to limit bitrate variance.*/
    uint32_t hrd;
    /**  \brief  Length for Group of Pictures, indicates
      * the distance of two intra pictures,
      * including first intra [1..300]
      * 1..300, Bitrate window length in frames[--intraPicRate]
      * Rate control allocates bits for one window and tries to
      * match the target bitrate at the end of each window.
      * Typically window begins with an intra frame, but this
      * is not mandatory.*/
    uint32_t bitrateWindow;
    /**  \brief  Allows rate control to skip frames if needed.
      *Allow rate control to skip pictures, [0,1]
      *0=OFF, 1=ON Picture skip rate control. [0]*/
    uint32_t pictureSkip;
    /**  \brief  QP for next encoded picture, [-1..51]
      * -1 = Let rate control calculate initial QP
      * This QP is used for all pictures if
      * HRD and pictureRc and mbRc are disabled
      * If HRD is enabled it may override this QP.
      *-1..51, Initial target QP. [26]
      *-1 = Encoder calculates initial QP. NOTE: use -q-1
      *The initial QP used in the beginning of stream.*/
    int32_t qpHdr;
    /**  \brief  Maximum QP for any P/B picture, [0..51] .*/
    uint32_t qpMax;
} HANTROEncMiscParameterRateControl;

/** \brief Attribute value for HANTROEncMiscParameterCIR */
typedef struct _HANTROEncMiscParameterCIR {
    /**
     * \brief Indicates the First MB/CTU for Cyclic Intra Refresh.
     *
     */
    uint32_t cir_start;
    /**
     * \brief Indicates MB/CTU interval for Cyclic Intra Refresh, 0=disabled
     */
    uint32_t cir_interval;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterCIR;

/** \brief Attribute value for HANTROEncMiscParameterGDR */
typedef struct _HANTROEncMiscParameterGDR {
    /**
     * \brief Indicates how many pictures(frames not fields) it will take to do GDR
     *
     * 0 : disable GDR(Gradual decoder refresh), >0: enable GDR
     */
    uint32_t gdr_duration;
    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterGDR;

/** \brief Attribute value for HANTROEncMiscParameterVideoFullRange */
typedef struct _HANTROEncMiscParameterVideoFullRange {
    /**
     * \brief Indicates Video signal sample range value in stream.
     *
     * 0 - Y range in [16..235] Cb,Cr in [16..240]
     * 1 - Y,Cb,Cr range in [0..255]
     */
    uint32_t video_full_range;
    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterVideoFullRange;

/** \brief Attribute value for HANTROEncROI */
typedef struct _HANTROEncROI {
    /** \brief Defines the ROI boundary in pixels, the driver will map it to appropriate
     *  codec coding units.  It is relative to frame coordinates for the frame case and
     *  to field coordinates for the field case. */
    HANTRORectangle     roi_rectangle;

    /**\brief ROI value
     *
     * \ref Specify the values of the parameters set to roi area. The values of roi_value
     * will be used as absolute QP value if you set parameter type to 2, will be used as
     * delta QP value if you set parameter type to 1, or will be be used as ROI priority
     * if you set parameter type to 0. */
    int8_t            roi_value;
    /**\brief type
    *
    * \ref Specify the Type of QP Delta is usedroi_value. The value of type equals to 2
    * indicates roi_value in HANTROEncROI  should be used as ROI absolute QP. The value
    * of type equals to 1 indicates roi_value in HANTROEncROI should be used as ROI delta QP.
    * The value of type equals to 0 indicates roi_value in HANTROEncROI should be used
    * as ROI priority */
    int8_t            type;
} HANTROEncROI;

/** \brief Attribute value for HANTROEncMiscParameterBufferROI */
typedef struct _HANTROEncMiscParameterBufferROI { ////should add emun for misc type
    /** \brief Number of ROIs being sent.*/
    uint32_t        num_roi;

    /** \brief Valid when VAConfigAttribRateControl != VA_RC_CQP, then the encoder's
     *  rate control will determine actual delta QPs.  Specifies the max/min allowed delta
     *  QPs. */
    int8_t                max_delta_qp;
    int8_t                min_delta_qp;

    /** \brief Pointer to a HANTROEncROI array with num_roi elements.  It is relative to frame
      *  coordinates for the frame case and to field coordinates for the field case.*/
    HANTROEncROI            *roi;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW - 1];
} HANTROEncMiscParameterBufferROI;

/** \brief Attribute value for HANTROEncMiscParameterBufferIPCM */
typedef struct _HANTROEncMiscParameterBufferIPCM {
    /** \brief Number of ipcms being sent.*/
    uint32_t        num_ipcm;

    /** \brief Pointer to a HANTRORectangle array with num_ipcm elements.  It is relative to frame
     *  coordinates for the frame case and to field coordinates for the field case.*/
    HANTRORectangle         *ipcm;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                HANTRO_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferIPCM;

/** \brief Attribute value for HANTROEncMiscParameterBufferIntraArea */
typedef struct _HANTROEncMiscParameterBufferIntraArea {
    /** \brief Number of intra areas being sent.*/
    uint32_t        num_intra_area;

    /** \brief Pointer to a HANTRORectangle array with num_intra_area elements.  It is relative to frame
     *  coordinates for the frame case and to field coordinates for the field case.*/
    HANTRORectangle         *intra_area;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferIntraArea;

/** \brief Attribute value for HANTROEncMiscParameterBufferROIMap */
typedef struct _HANTROEncMiscParameterBufferROIMap {

    /** \brief buffer to contain qp delta or/and qp absolute values or/and ipcm region flag.
     *   if roimap_qp_delat_support=1 &&
     *  roimap_qp_absolute_support=1 qp delta and qp absolute value can be used in the same buffer.
     * roi map buffer type should be HANTROEncROIMapBufferType and
     * app should call vaCreateBuffer2() to allocate this buffer.
     */
    uint32_t                roi_map_qp_delta_version;
    HANTROBufferID          roimap_buf_id;
    //HANTROBufferID          roimap_cuctrl_buf_id;


    union {
        struct {
            /**
             * \brief define if roimap is used.
             *
             * \ref roimap_is_enabled equal to 1 indicates \c encoder will read roimap ipcmmap  buffer
             * which id is roimap_ipcmmap_buf_id
             * to get roi map config. roimap_is_enabled equal to 0 indicates \c encoder will not read roimap ipcmmap  buffer
             * which id is roimap_ipcmmap_buf_id
             * \ref to get roi map config.
             */
            uint32_t  roimap_is_enabled               : 1;
            /**
             * \brief define roimap block unit.
             *
             * \ref  roimap_block_unit equal to 0-64x64
             *  roimap_block_unit equal to 1-32x32
             *  roimap_block_unit equal to 2-16x16
             *  roimap_block_unit equal to 3-8x8
             */
            uint32_t  roimap_block_unit               : 2;

            /**
             * \brief define if ipcm map is enabled.
             *
             * \ref ipcmmap_is_enabled equal to 1 indicates \c encoder will read roimap ipcmmap  buffer
             * which id is roimap_ipcmmap_buf_id
             * to get ipcm map config. ipcmmap_is_enabled equal to 0 indicates \c encoder will not read roimap ipcmmap  buffer
             * which id is roimap_ipcmmap_buf_id
             * \ref  to get ipcm map config.
             */
            uint32_t  ipcmmap_is_enabled               : 1;
            /**
             * \brief define if skip map is enabled.
             *
             * \ref skipmap_is_enabled equal to 1 indicates \c encoder will read roimap ipcmmap  buffer
             * which id is skipmap_is_enabled
             * to get skip map config. skipmap_is_enabled equal to 0 indicates \c encoder will not read skipmap  buffer
             * which id is roimap_ipcmmap_buf_id
             * \ref  to get skipmap map config.
             */
            uint32_t  skipmap_is_enabled               : 1;
            /** \brief Same as the HEVC bitstream syntax element. */
            uint32_t    pcm_loop_filter_disabled_flag  : 1;

            uint32_t  reserved                         : 26;
        } bits;
        uint32_t value;
    } roi_flags;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferROIMap;

/** \brief Attribute value for HANTROEncMiscParameterBufferCuCtrl */
typedef struct _HANTROEncMiscParameterBufferCuCtrl {

    /** \brief buffer to render buffer for cuctrlversion 3~7 for cu ctrl.
     * roi map buffer type should be HANTROEncROIMapBufferType and
     * app should call vaCreateBuffer2() to allocate this buffer.
     */
    uint32_t                roi_cu_ctrl_version;
    HANTROBufferID          cu_ctrl_buf_id;
    //HANTROBufferID          roimap_cuctrl_buf_id;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferCuCtrl;

/** \brief Attribute value for HANTROEncMiscParameterBufferCuCtrl */
typedef struct _HANTROEncMiscParameterBufferCuCtrlIndex {

    /** \brief buffer to render buffer for cuctrlversion 3~7 for cu ctrl.
     * roi map buffer type should be HANTROEncROIMapBufferType and
     * app should call vaCreateBuffer2() to allocate this buffer.
     */
    HANTROBufferID          cu_ctrl_index_buf_id;
    //HANTROBufferID          roimap_cuctrl_buf_id;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferCuCtrlIndex;



/** HANTROEncMiscParameterBufferEmbeddedPreprocess rotation value   */
#define HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_NONE         0x00000000
#define HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_270          0x00000001
#define HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_90           0x00000002
#define HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_180          0x00000003

/** \brief Attribute value for HANTROEncMiscParameterBufferMBCUInfoOutput */
typedef struct _HANTROEncMiscParameterBufferMBCUInfoOutput {

    /** \brief buffer to contain MB or CU encoding infomation.
     *  MB or CU encoding infomation  buffer type should be HANTROEncMBCUOutputInfoBufferType
     * and app should call vaCreateBuffer2() to allocate this buffer.
     */
    HANTROBufferID          mb_cu_encoding_info_buf_id;


    union {
        struct {
            /**
             * \brief define if mb or cu encoding information convey to application.
             *
             * \ref mb_cu_encoding_info_output_is_enabled equal to 1 indicates \c encoder will put mb or cu information
             * into  buffer which id is mb_cu_encoding_info_buf_id
             *  mb_cu_encoding_info_output_is_enabled equal to 0 indicates \c encoder will not put mb or cu information
             * into  buffer which id is mb_cu_encoding_info_buf_id
             * \ref
             */
            uint32_t  mb_cu_encoding_info_output_is_enabled               : 1;

            uint32_t  reserved                         : 31;
        } bits;
        uint32_t value;
    } roi_flags;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferMBCUInfoOutput;

/** \brief Attribute value for buffer type HANTROEncMiscParameterTypePSNRInfoOutput */
typedef struct _HANTROEncMiscParameterBufferPSNROutput {

    /** \brief buffer to contain MB or CU encoding infomation.
     *  psnr infomation  buffer type should be HANTROEncMiscParameterBufferPSNROutput
     * and app should call vaCreateBuffer2() to allocate this buffer.
     */
    HANTROBufferID          psnr_info_buf_id;


    union {
        struct {
            /**
             * \brief define if mb or cu encoding information convey to application.
             *
             * \ref mb_cu_encoding_info_output_is_enabled equal to 1 indicates \c encoder will put mb or cu information
             * into  buffer which id is mb_cu_encoding_info_buf_id
             *  mb_cu_encoding_info_output_is_enabled equal to 0 indicates \c encoder will not put mb or cu information
             * into  buffer which id is mb_cu_encoding_info_buf_id
             * \ref
             */
            uint32_t  psnr_info_output_is_enabled               : 1;

            uint32_t  reserved                         : 31;
        } bits;
        uint32_t value;
    } psnr_flags;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferPSNROutput;

/**
 * \brief encoder embedded preprocessing config.
 *
 * The encoding embedded preprocess can be set through HANTROEncMiscParameterBufferEmbeddedPreprocess, if the implementation
 * supports encoder embedded preprocess input. The embedded preprocess set through this structure is applicable only to the
 * current frame or field, so must be sent every frame or field to be applied.
 */


typedef struct _HANTROEncMiscParameterBufferEmbeddedPreprocess {
    /** \brief cropping offset x, in pixel unit, in original picture*/
    uint32_t        cropping_offset_x;
    /** \brief cropping offset y,in pixel unit, in original picture*/
    uint32_t        cropping_offset_y;

    /** \brief cropped width in pixel unit, in original picture. if 0, equal to input surface orig_width*/
    uint32_t        cropped_width;
    /** \brief cropped height in pixel unit, in original picture. if 0, equal to input surface orig_height*/
    uint32_t        cropped_height;

    /** \brief rotation. HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_NONE - none,
        * HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_270 - 270,
        * HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_90 - 90,
        * HANTRO_ENC_EMBEDDED_PREPROCESS_ROTATION_180 - 180 */
    uint32_t        rotation;
    /** \brief mirror. 0 - none, 1 - mirror */
    uint32_t        mirror;
    /** \brief constant Cb value. */
    uint32_t        constCb;
    /** \brief constant Cr value. */
    uint32_t        constCr;
    /** \brief colorConversion value. */
    uint32_t        colorConversion;
    /** \brief down-scaled width value.
        *Optional down-scaled output picture width,multiple of 4. 0=disabled. [16..width] */
    uint32_t        scaledWidth;
    /** \brief down-scaled height value.
        * Optional down-scaled output picture height,multiple of 2. [96..height] */
    uint32_t        scaledHeight;


    union {
        struct {
            /**
             * \brief An indication for embedded preprocess value.
             *
             * \ref embedded_preprocess_constant_chroma_is_enabled equal to 1 indicates \c constCb and  constCr will be used in
             * encoder embedded preprocess.  equal to 0  indicates \c constCb and  constCr will not be used.s
             */
            uint32_t  embedded_preprocess_constant_chroma_is_enabled         : 1;

            uint32_t  reserved                      : 31;
        } bits;
        uint32_t value;
    } preprocess_flags;
} HANTROEncMiscParameterBufferEmbeddedPreprocess;

/**
 * whether recon frame compression is enabled.
 */
typedef struct _HANTROEncMiscParameterReconFrameCompression {
    union {
        struct {
            /**
             * \brief An indication for recon frame compression .
             *
             * \ref recon_frame_compression_mode equal to 3 indicates \c Enable Both Luma and Chroma Compression.
             * \ref recon_frame_compression_mode equal to 2 indicates \c Only Enable Chroma Compression
             * \ref recon_frame_compression_mode equal to 1 indicates \c Only Enable Luma Compression.
             *  equal to 0  indicates \c recon frame is not compressed.
             */
            uint32_t  recon_frame_compression_mode                  : 2;
            uint32_t  enableP010Ref                                 : 1;
            uint32_t  reserved                                      : 29;
        } bits;
        uint32_t value;
    } rfc_flags;
} HANTROEncMiscParameterReconFrameCompression;


/**
 * whether packed header output  is enabled.
 */
typedef struct _HANTROEncMiscParameterPackedHeaderOutputControl {
    union {
        struct {
            /**
            * \brief An indication for vps re-output .
            *
            * \ref vps_reoutput_enable equal to 1 indicates \c vps will be reoutput.
            * equal to 0  indicates \c vps will not be reoutput.
            */
            uint32_t  vps_reoutput_enable                             : 1;
            /**
             * \brief An indication for sps re-output .
             *
             * \ref sps_reoutput_enable equal to 1 indicates \c sps will be reoutput.
             * equal to 0  indicates \c sps will not be reoutput.
             */
            uint32_t  sps_reoutput_enable                             : 1;
            /**
             * \brief An indication for pp re-output .
             *
             * \ref pps_reoutput_enable equal to 1 indicates \c pps will be reoutput.
             * equal to 0  indicates \c pps will not be reoutput.
             */
            uint32_t  pps_reoutput_enable                             : 1;
            /**
             * \brief An indication for sei output .
             *
             * \ref sei_output_enable equal to 1 indicates \c sei will be ouput.
             * equal to 0  indicates \c sei will not be ouput.
             */
            uint32_t  sei_output_enable                               : 1;
            /**
             * \brief An indication for aud output .
             *
             * \ref aud_output_enable equal to 1 indicates \c aud will be ouput.
             * equal to 0  indicates \c aud will not be ouput.
             */
            uint32_t  aud_output_enable                               : 1;

            uint32_t  reserved                                        : 27;
        } bits;
        uint32_t value;
    } phoc_flags;
} HANTROEncMiscParameterPackedHeaderOutputControl;

/**
 * CodingCtrl buffer.
 */
typedef struct _HANTROEncMiscParameterBufferCodingCtrl {
    /**
     *\brief cabac initial flag.
     *
     *0..1 Initialization value for CABAC. [0]
     */
    uint32_t        cabacInitFlag;
    /**
     *\brief cabac enable flag.
     *
     *0=OFF (CAVLC), 1=ON (CABAC). [1]
     */
    //uint32_t        enableCabac;
    /**
     *\brief disable deblocking flag.
     *
     *0 = Filter enabled,
     * 1 = Filter disabled,
     * 2 = Filter disabled on slice edges
     */
    //uint32_t        disableDeblockingFilter;
    /**
     *\brief enable sao flag.
     *
     *0..1 Disable or Enable SAO Filter [1]
     *0 = SAO disabled
     *1 = SAO enabled
     */
    //uint32_t        enableSao;
    /**
     *\brief deblock parameter tc_Offset.
     *
     *-6..6 Deblocking filter tc offset. [-2]
     */
    //int32_t        tc_Offset;
    /**
     *\brief deblock parameter, beta_offset.
     *
     *-6..6 Deblocking filter beta offset. [5]
     */
    //int32_t        beta_Offset;
    /**
     *\brief enable deblock override between slice.
     *
     * 0..1 deblocking override enable flag[0]
     *0 = Disable override
     * 1 = enable override
     */
    uint32_t        enableDeblockOverride;
    /**
     *\brief flag to indicate whether deblock override between slice.
     *
     *0..1 deblocking override enable flag[0]
     *0 = Disable override
     *1 = enable override
     */
    uint32_t        deblockOverride;

    /* \brief tile parameter, tiles_enabled_flag.*/
    int32_t        tiles_enabled_flag;
    /* \brief tile parameter, num_tile_columns.*/
    int32_t        num_tile_columns;
    /* \brief tile parameter, num_tile_rows.*/
    int32_t        num_tile_rows;
    /* \brief tile parameter, loop_filter_across_tiles_enabled_flag.*/
    int32_t        loop_filter_across_tiles_enabled_flag;

    /**
     *\brief enable scaling list params.
     *
     *0..1 using default scaling list or not[0]
     *0 = using average scaling list.
     *1 = using default scaling list.
     */
    uint32_t        enableScalingList;

    /**
     *\brief rps params.
     *
     *0..1 encoding rps in the slice header or not[0]
     *0 = not encoding rps in the slice header.
     *1 = encoding rps in the slice header.
     */
    uint32_t        RpsInSliceHeader;

    /**  \brief  chroma qp offset. -12..12 Chroma QP offset. [0]*/
    //int32_t         chroma_qp_offset;

    /** \brief for stream multi-segment
      * 0:single segment 1:multi-segment with no sync 2:multi-segment with sw handshake mode*/
    uint32_t streamMultiSegmentMode;
    /** \brief forstream multi-segment
      * must be more than 1*/
    uint32_t streamMultiSegmentAmount;

    /** \brief for noiseReduction, deprecated from VC8000E v6.1
     * 0 = disable noise reduction; 1 = enable noise reduction
     * deprecated from VC8000E v6.1
     */
    uint32_t noiseReductionEnable;
    /** \brief for noiseReduction, deprecated from VC8000E v6.1
     *  valid value range :[1,30] , default: 10
     */
    uint32_t noiseLow;
    /** \brief for noiseReduction, deprecated from VC8000E v6.1
     * valid value range :[1,30] , default :11
     */
    uint32_t firstFrameSigma;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t        hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferCodingCtrl;

/**
 * global MV buffer.
 */
typedef struct _HANTROEncMiscParameterBufferGMV {
    /** mv0[0]: horizontal motion vector for forward reference */
    /** mv0[1]: vertical motion vector for forward reference */
    int16_t  mv0[2];
    /** mv1[0]: horizontal motion vector for backward reference */
    /** mv1[1]: vertical motion vector for backward reference */
    int16_t  mv1[2];
} HANTROEncMiscParameterBufferGMV;

/**
 * special frame.
 */
#define VA_VCENC_MAX_REF_FRAMES 8
typedef enum {
    VA_VCENC_INTRA_FRAME = 0,
    VA_VCENC_PREDICTED_FRAME = 1,
    VA_VCENC_BIDIR_PREDICTED_FRAME = 2,
    VA_VCENC_NOTCODED_FRAME    /* Used just as a return value */
} VA_VCEncPictureCodingType;

typedef struct {
    int32_t ref_pic;          /*  delta_poc of this short reference picture relative to the poc of current picture or index of LTR */
    uint32_t used_by_cur;      /*  whether this reference picture used by current picture  */
} VA_VCEncGopPicRps;

typedef struct {
    uint32_t poc;                            /*  picture order count within a GOP  */
    int32_t QpOffset;                       /*  QP offset  */
    double QpFactor;                    /*  QP Factor  */
    int32_t temporalId;                     /*  temporal layer ID */
    VA_VCEncPictureCodingType codingType;  /*  picture coding type  */
    uint32_t numRefPics;                     /*  the number of reference pictures kept for this picture, the value should be within [0, VCENC_MAX_REF_FRAMES] */
    VA_VCEncGopPicRps refPics[VA_VCENC_MAX_REF_FRAMES];  /*  short-term reference picture sets for this picture*/
} VA_VCEncGopPicConfig;
typedef struct {
    uint32_t poc;                           /*  picture order count within a GOP  */
    int32_t QpOffset;                       /*  QP offset  */
    double QpFactor;                    /*  QP Factor  */
    int32_t temporalId;                     /*  temporal layer ID */
    VA_VCEncPictureCodingType codingType;  /*  picture coding type   */
    uint32_t numRefPics;                    /*  the number of reference pictures kept for this picture, the value should be within [0, VCENC_MAX_REF_FRAMES] */
    VA_VCEncGopPicRps refPics[VA_VCENC_MAX_REF_FRAMES];  /*  short-term reference picture sets for this picture*/
    int32_t i32Ltr;                         /* index of the long-term referencr frame,the value should be within [0, VCENC_MAX_LT_REF_FRAMES]. 0 -- not LTR; 1...VCENC_MAX_LT_REF_FRAMES--- LTR's index */
    int32_t i32Offset;                      /* offset of the special pics, relative to start of ltrInterval*/
    int32_t i32Interval;                    /* interval between two pictures using LTR as referencr picture or  interval between two pictures coded as special frame */
    int32_t i32short_change;                /* only change short term coding parameter. 0 - not change, 1- change */
} VA_VCEncGopPicSpecialConfig;

typedef struct _HANTROEncMiscParameterBufferSpecialFrame {
    unsigned char special_size;
    unsigned char ltrcnt;
    VA_VCEncGopPicSpecialConfig gopSpecialConfigArr[8];
} HANTROEncMiscParameterBufferSpecialFrame;

/**
 * skip frame.
 */
typedef struct _HANTROEncMiscParameterBufferSkipFrame {
    unsigned int skipFramePOC;
} HANTROEncMiscParameterBufferSkipFrame;

/**
 * ReportingParams.
 */
typedef struct _HANTROEncMiscParameterBufferReportingParams {
    unsigned int enableOutputCuInfo;
    unsigned int hashtype;
    unsigned int ssim;
    unsigned int enableVuiTimingInfo;
} HANTROEncMiscParameterBufferReportingParams;

/**
 * log level.
 */
typedef struct _HANTROEncMiscParameterBufferRuntimeLogLevel {
    unsigned int verbose;
} HANTROEncMiscParameterBufferRuntimeLogLevel;

/**
 * hdr10.
 */
typedef struct _HANTROEncMiscParameterBufferHDR10 {
    struct {
        unsigned char hdr10_display_enable;
        unsigned short hdr10_dx0;
        unsigned short hdr10_dy0;
        unsigned short hdr10_dx1;
        unsigned short hdr10_dy1;
        unsigned short hdr10_dx2;
        unsigned short hdr10_dy2;
        unsigned short hdr10_wx;
        unsigned short hdr10_wy;
        unsigned int hdr10_maxluma;
        unsigned int hdr10_minluma;
    } Hdr10DisplaySei;

    struct {
        unsigned char  hdr10_lightlevel_enable;
        unsigned short hdr10_maxlight;
        unsigned short hdr10_avglight;
    } Hdr10LightLevelSei;

    struct {
        unsigned char hdr10_color_enable;
        unsigned char hdr10_primary;
        unsigned char hdr10_transfer;
        unsigned char hdr10_matrix;
    } Hdr10ColorVui;
} HANTROEncMiscParameterBufferHDR10;

/** \brief Buffer type used for JPEG Lossless. */
typedef struct _HANTROEncMiscParameterBufferJPEGLossless {
    unsigned int losslessEn;
    unsigned int predictMode;
    unsigned int ptransValue;
} HANTROEncMiscParameterBufferJPEGLossless;

/** \brief Buffer type used for user data. */
typedef struct _HANTROEncMiscParameterBufferSEIUserData {
    HANTROBufferID sei_user_data_buf_id;
    unsigned int sei_user_data_size;
} HANTROEncMiscParameterBufferSEIUserData;

/**
 * LowLatency buffer.
 */
typedef struct _HANTROEncMiscParameterBufferLowLatency {
    /** \brief for low latency mode */
    uint32_t inputLineBufMode;
    /** \brief for low latency, input buffer depth in mb lines */
    uint32_t inputLineBufDepth;
    /** \brief for low latency,Handshake sync amount for every loopback */
    uint32_t amountPerLoopBack;
} HANTROEncMiscParameterBufferLowLatency;


/**
 * multi-core.
 */
//typedef struct _HANTROEncMiscParameterBufferMultiCore
//{
/** \brief for 2pass or multi-core, flush remaining frames.
Set flushRemainingFrames to 1 and send this buffer before lase frame. */
//    uint32_t numberMultiCore;
//}HANTROEncMiscParameterBufferMultiCore;


/**
 * lookahead parameters.
 */
typedef struct _HANTROEncMiscParameterBufferNumberFramesForLastPic {
    /** \brief for 2pass parameters. */
    //uint32_t extDSRatio;
    /** \brief for 2pass parameters. */
    //uint32_t lookaheadDepth;
    /** \brief for 2pass parameters. */
    uint32_t frames2Encode;
} HANTROEncMiscParameterBufferNumberFramesForLastPic;

/** \brief Buffer type used for user data. */
typedef struct _HANTROEncMiscParameterBufferAdaptiveGOPDecision {
    HANTROBufferID adaptive_gop_decision_buf_id;
    uint32_t adaptive_gop_decision_enable;
} HANTROEncMiscParameterBufferAdaptiveGOPDecision;


//typedef struct _HANTROEncMiscParameterBufferTier {
//    int tier;
//}HANTROEncMiscParameterBufferTier;
/**@}*/


/**
 * \brief decoder embedded post processing config.
 *
 * The decoding embedded postprocess can be set through VADecEmbeddedPostprocessParameterBuffer, if the implementation
 * supports decoder embedded postprocess output. The embedded postprocess set through this structure is applicable only to the
 * current frame or field, so must be sent every frame or field to be applied.
 */
typedef struct _HANTRODecEmbeddedPostprocessParameterBuffer {
    /** \brief cropping offset x, in pixel unit.*/
    uint32_t        cropping_offset_x;
    /** \brief cropping offset y,in pixel unit.*/
    uint32_t        cropping_offset_y;

    /** \brief cropped width in pixel unit. if 0, no cropping. */
    uint32_t        cropped_width;
    /** \brief cropped height in pixel unit. if 0, no cropping. */
    uint32_t        cropped_height;

    /** \brief down scaled width in pixel unit. if 0, no downscaling. */
    uint32_t        scaled_width;
    /** \brief down scaled height in pixel unit. if 0, no downscaling. */
    uint32_t        scaled_height;

    /** \brief chroma pixels in CrCb order*/
    uint32_t        use_nv21_output;
    /** \brief force output in 8 bits per pixel for HEVC Main 10 profile, H264 H10, VP9 10bit*/
    uint32_t        use_8bit_output;
    /** \brief output in P010 format for HEVC Main 10 profile, H264 H10, VP9 10bit. */
    uint32_t        use_p010_output;
    /** \brief output in big endian format(128bit) for HEVC stream. */
    uint32_t        use_pbe_output;
    /** \brief external pp buffer surface id. if 0, no external pp buffer but have internal pp buffer*/
    VASurfaceID     external_pp_surface;
} HANTRODecEmbeddedPostprocessParameterBuffer;

/**
 * The misc parameter can be set to decoder.
 */
typedef struct _HANTRODecMiscParameterBuffer {
    union {
        struct {
            /** \brief reference frame need to be compressed or not only for G2 decoder 1 disable 0 enable rfc*/
            uint32_t  ref_frame_compress_bypass                     : 1;
            /** \brief enable H264 mvc decoding  */
            uint32_t  enable_h264_mvc_decoding                      : 1;
            /** \brief store top/bottom fields in seperate */
            uint32_t  separate_fields_in_dpb                        : 1;
            /** \brief enable milticore decoding or not */
            uint32_t  enable_multicore                              : 1;
            uint32_t  reserved                                      : 27;
        } bits;
        uint32_t value;
    } flags;

    /** \brief the value frame buffer stride aligned to  */
    uint32_t  frame_buffer_alignment;

} HANTRODecMiscParameterBuffer;


#ifdef __cplusplus
}
#endif

#endif
