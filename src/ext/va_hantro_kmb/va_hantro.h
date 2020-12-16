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
/** Padding size in 4-bytes */
#define HANTRO_PADDING_LOW          4
#define HANTRO_PADDING_MEDIUM       8
#define HANTRO_PADDING_HIGH         16
#define HANTRO_PADDING_LARGE        32

#define HANTRO_PROFILE_START 100
#define HANTRO_CONFIG_ATTRIB_TYPE_START 100
#define HANTRO_BUFFER_TYPE_START 100
#define HANTRO_ENC_MISC_PARAMETER_TYPE_START 100


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



/** Currently defined profiles */
typedef enum {
    HANTROProfileHEVCMainStill = HANTRO_PROFILE_START,
    HANTROProfileH264High10,

} HANTROProfile;

/** Currently defined configuration attribute types */
typedef enum {
    /**
    * \brief Encoder embedded preprocessing attribute. Read-only.
    *
    * This attribute conveys whether the driver supports encoder embedded preprocessing features
    * The attribute value is partitioned into fields as defined in the
    * HANTROConfigAttribValEncEmbeddedPreprocess union.
    */
    HANTROConfigAttribEncEmbeddedPreprocess = HANTRO_CONFIG_ATTRIB_TYPE_START ,
    /**
     * \brief Encoder recon frame compression attribute. Read-only.
     *
     * This attribute conveys whether the driver supports encoder recon frame compression features
     * The attribute value is partitioned into fields as defined in the
     * HANTROConfigAttribValEncReconFrameCompression union.
     */
    HANTROConfigAttribEncReconFrameCompression ,
    /**
    * \brief packed header output control attribute. Read-only.
    *
    * This attribute conveys whether the driver supports packed header output such as sps, pps sei
    * and aud can be contolled
    * The attribute value is partitioned into fields as defined in the
    * HANTROConfigAttribValEncPackedHeaderOutputControl union.
    */
    HANTROConfigAttribEncPackedHeaderOutputControl ,
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
    HANTROConfigAttribEncIPCM ,
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
    /**@}*/
    HANTROConfigAttribTypeMax
} HANTROConfigAttribType;

/** \brief Attribute value for HANTROConfigAttribValDecEmbeddedPostprocess */
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


/** \brief Attribute value for HANTROConfigAttribValEncROIMap */
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
        /** \brief if support mb or cu encoding information output to application,
                1 : supported; 0 : not supported. */
        uint32_t mb_cu_info_output              : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncMBCUInfo;

/** \brief Attribute value for HANTROConfigAttribValEncCIR */

typedef union _HANTROConfigAttribValEncCIR {
    struct {
        /** \brief 1 if  support CIR encoding,
                0 if not support CIR encoding. */
        uint32_t cir_encoding_supported         : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncCIR;

/** \brief Attribute value for HANTROConfigAttribValEncGDR */

typedef union _HANTROConfigAttribValEncGDR {
    struct {
        /** \brief 1 if  support GDR encoding,
                0 if not support GDR encoding. */
        uint32_t gdr_encoding_supported         : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncGDR;

/** \brief Attribute value for HANTROConfigAttribValEncVideoFullRange */

typedef union _HANTROConfigAttribValEncVideoFullRange {
    struct {
        /** \brief 1 if  support video full range config,
                0 if not support video full range config. */
        uint32_t video_full_range_supported         : 1;
        uint32_t reserved                   : 31;
    } bits;
    uint32_t value;
} HANTROConfigAttribValEncVideoFullRange;




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

        uint32_t reserved                                       : 28;
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

        uint32_t reserved                                       : 31;
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

/** \brief Extension of HANTROBufferType */
typedef enum {
    HANTRODecEmbeddedPostprocessParameterBufferType = HANTRO_BUFFER_TYPE_START,
    HANTRODecMiscParameterBufferType,
    HANTROEncROIMapBufferType,
    HANTROEncMBCUOutputInfoBufferType,
    HANTROEncPSNROutputInfoBufferType
} HANTROBufferType;


typedef enum {
    HANTROEncMiscParameterTypeEmbeddedPreprocess = HANTRO_ENC_MISC_PARAMETER_TYPE_START,
    /** \brief Buffer type used for encoder recon frame compression parameters. */
    HANTROEncMiscParameterTypeReconFrameCompression  ,
    /** \brief Buffer type used for encoder packed header output control parameters. */
    HANTROEncMiscParameterTypePackedHeaderOutputControl  ,
    /** \brief Buffer type used for IPCM parameters. */
    HANTROEncMiscParameterTypeIPCM ,
    /** \brief Buffer type used for region-of-interest (ROI) Map parameters. */
    HANTROEncMiscParameterTypeROIMap ,
    /** \brief Buffer type used for intra area region parameters. */
    HANTROEncMiscParameterTypeIntraArea  ,
    /** \brief Buffer type used for mb or cu encoding information output parameters. */
    HANTROEncMiscParameterTypeMBCUInfoOutput,
    /** \brief Buffer type used for cyclic intra refresh(CIR) parameters. */
    HANTROEncMiscParameterTypePSNRInfoOutput,
    /** \brief Buffer type used for cyclic intra refresh(CIR) parameters. */
    HANTROEncMiscParameterTypeCIR ,
    /** \brief Buffer type used for gradual decoder refresh(GDR) parameters. */
    HANTROEncMiscParameterTypeGDR ,
    /** \brief Buffer type used for video full range parameters. */
    HANTROEncMiscParameterTypeVideoFullRange ,
    /** \brief Buffer type used for RateControl extend  parameters. */
    HANTROEncMiscParameterTypeRateControl,
    /** \brief Buffer type used for ROI  extend  parameters. */
    HANTROEncMiscParameterTypeROI

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
    HANTROBufferID          roimap_buf_id;


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
            /** \brief Same as the HEVC bitstream syntax element. */
            uint32_t    pcm_loop_filter_disabled_flag  : 1;

            uint32_t  reserved                         : 27;
        } bits;
        uint32_t value;
    } roi_flags;

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t                hantro_reserved[HANTRO_PADDING_LOW];
} HANTROEncMiscParameterBufferROIMap;

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



/** \brief Attribute value for HANTROEncMiscParameterBufferPSNROutput */
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
             *  equal to 0  indicates \c recon frame is not compressed.
             */
            uint32_t  recon_frame_compression_mode                  : 2;

            uint32_t  reserved                                      : 30;
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
             * equal to 0  indicates \c sps will not be reoutput.
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
 * \brief decoder embedded post processing config.
 *
 * The decoding embedded postprocess can be set through HANTRODecEmbeddedPostprocessParameterBuffer, if the implementation
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
    /** \brief force output in 8 bits per pixel for HEVC Main 10 profile */
    uint32_t        use_8bit_output;
    /** \brief output in P010 format for HEVC Main 10 profile. */
    uint32_t        use_p010_output;
    /** \brief output in big endian format(128bit) for HEVC stream. */
    uint32_t        use_pbe_output;

} HANTRODecEmbeddedPostprocessParameterBuffer;

/**
 * The misc parameter can be set to decoder.
 */
typedef struct _HANTRODecMiscParameterBuffer {
    union {
        struct {
            /** \brief reference frame need to be compressed or not */
            uint32_t  ref_frame_compress_bypass                     : 1;
            /** \brief enable H264 mvc decoding  */
            uint32_t  enable_h264_mvc_decoding                      : 1;
            /** \brief store top/bottom fields in seperate */
            uint32_t  separate_fields_in_dpb                        : 1;
            /** \brief enable milticore decoding or not */
            uint32_t  enable_multicore                              : 1;

            uint32_t  reserved                                      : 28;
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
