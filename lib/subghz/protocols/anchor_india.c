#include "anchor_india.h"

#include "../blocks/const.h"
#include "../blocks/decoder.h"
#include "../blocks/encoder.h"
#include "../blocks/generic.h"
#include "../blocks/math.h"

/*
 * HackNlearn India - Anchor Lock Protocol
 * Based on OOK PWM modulation
 * Common frequencies: 433.92 MHz
 * Bit count: 24 bit
 * TE: 500us (slightly different from Godrej)
 */

#define TAG "SubGhzProtocolAnchorIndia"
#define ANCHOR_GUARD_TIME_DEFAULT 30

static const SubGhzBlockConst subghz_protocol_anchor_india_const = {
    .te_short = 500,
    .te_long  = 1500,
    .te_delta = 350,
    .min_count_bit_for_found = 24,
};

struct SubGhzProtocolDecoderAnchorIndia {
    SubGhzProtocolDecoderBase base;
    SubGhzBlockDecoder decoder;
    SubGhzBlockGeneric generic;
    uint32_t te;
    uint32_t last_data;
    uint32_t guard_time;
};

struct SubGhzProtocolEncoderAnchorIndia {
    SubGhzProtocolEncoderBase base;
    SubGhzProtocolBlockEncoder encoder;
    SubGhzBlockGeneric generic;
    uint32_t te;
    uint32_t guard_time;
};

typedef enum {
    AnchorDecoderStepReset = 0,
    AnchorDecoderStepSaveDuration,
    AnchorDecoderStepCheckDuration,
} AnchorDecoderStep;

const SubGhzProtocolDecoder subghz_protocol_anchor_india_decoder = {
    .alloc = subghz_protocol_decoder_anchor_india_alloc,
    .free = subghz_protocol_decoder_anchor_india_free,
    .feed = subghz_protocol_decoder_anchor_india_feed,
    .reset = subghz_protocol_decoder_anchor_india_reset,
    .get_hash_data = subghz_protocol_decoder_anchor_india_get_hash_data,
    .serialize = subghz_protocol_decoder_anchor_india_serialize,
    .deserialize = subghz_protocol_decoder_anchor_india_deserialize,
    .get_string = subghz_protocol_decoder_anchor_india_get_string,
};

const SubGhzProtocolEncoder subghz_protocol_anchor_india_encoder = {
    .alloc = subghz_protocol_encoder_anchor_india_alloc,
    .free = subghz_protocol_encoder_anchor_india_free,
    .deserialize = subghz_protocol_encoder_anchor_india_deserialize,
    .stop = subghz_protocol_encoder_anchor_india_stop,
    .yield = subghz_protocol_encoder_anchor_india_yield,
};

const SubGhzProtocol subghz_protocol_anchor_india = {
    .name = SUBGHZ_PROTOCOL_ANCHOR_INDIA_NAME,
    .type = SubGhzProtocolTypeStatic,
    .flag = SubGhzProtocolFlag_433 | SubGhzProtocolFlag_315 |
            SubGhzProtocolFlag_AM | SubGhzProtocolFlag_Decodable |
            SubGhzProtocolFlag_Load | SubGhzProtocolFlag_Save |
            SubGhzProtocolFlag_Send,
    .decoder = &subghz_protocol_anchor_india_decoder,
    .encoder = &subghz_protocol_anchor_india_encoder,
};

// ENCODER

void* subghz_protocol_encoder_anchor_india_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    SubGhzProtocolEncoderAnchorIndia* instance =
        malloc(sizeof(SubGhzProtocolEncoderAnchorIndia));
    instance->base.protocol = &subghz_protocol_anchor_india;
    instance->generic.protocol_name = instance->base.protocol->name;
    instance->encoder.repeat = 10;
    instance->encoder.size_upload = 52;
    instance->encoder.upload = malloc(instance->encoder.size_upload * sizeof(LevelDuration));
    instance->encoder.is_running = false;
    return instance;
}

void subghz_protocol_encoder_anchor_india_free(void* context) {
    furi_assert(context);
    SubGhzProtocolEncoderAnchorIndia* instance = context;
    free(instance->encoder.upload);
    free(instance);
}

static bool
    subghz_protocol_encoder_anchor_india_get_upload(
        SubGhzProtocolEncoderAnchorIndia* instance) {
    furi_assert(instance);
    size_t index = 0;
    size_t size_upload = (instance->generic.data_count_bit * 2) + 2;
    if(size_upload > instance->encoder.size_upload) {
        FURI_LOG_E(TAG, "Size upload exceeds allocated encoder buffer.");
        return false;
    } else {
        instance->encoder.size_upload = size_upload;
    }
    for(uint8_t i = instance->generic.data_count_bit; i > 0; i--) {
        if(bit_read(instance->generic.data, i - 1)) {
            instance->encoder.upload[index++] =
                level_duration_make(true, (uint32_t)instance->te * 3);
            instance->encoder.upload[index++] =
                level_duration_make(false, (uint32_t)instance->te);
        } else {
            instance->encoder.upload[index++] =
                level_duration_make(true, (uint32_t)instance->te);
            instance->encoder.upload[index++] =
                level_duration_make(false, (uint32_t)instance->te * 3);
        }
    }
    instance->encoder.upload[index++] =
        level_duration_make(true, (uint32_t)instance->te);
    instance->encoder.upload[index++] =
        level_duration_make(false, (uint32_t)instance->te * instance->guard_time);
    return true;
}

SubGhzProtocolStatus subghz_protocol_encoder_anchor_india_deserialize(
    void* context,
    FlipperFormat* flipper_format) {
    furi_assert(context);
    SubGhzProtocolEncoderAnchorIndia* instance = context;
    SubGhzProtocolStatus ret = SubGhzProtocolStatusError;
    do {
        ret = subghz_block_generic_deserialize_check_count_bit(
            &instance->generic,
            flipper_format,
            subghz_protocol_anchor_india_const.min_count_bit_for_found);
        if(ret != SubGhzProtocolStatusOk) break;
        if(!flipper_format_rewind(flipper_format)) {
            FURI_LOG_E(TAG, "Rewind error");
            ret = SubGhzProtocolStatusErrorParserOthers;
            break;
        }
        if(!flipper_format_read_uint32(
               flipper_format, "TE", (uint32_t*)&instance->te, 1)) {
            FURI_LOG_E(TAG, "Missing TE");
            ret = SubGhzProtocolStatusErrorParserTe;
            break;
        }
        if(!flipper_format_read_uint32(
               flipper_format, "Guard_time", (uint32_t*)&instance->guard_time, 1)) {
            instance->guard_time = ANCHOR_GUARD_TIME_DEFAULT;
        } else {
            if((instance->guard_time < 15) || (instance->guard_time > 72)) {
                instance->guard_time = ANCHOR_GUARD_TIME_DEFAULT;
            }
        }
        flipper_format_read_uint32(
            flipper_format, "Repeat", (uint32_t*)&instance->encoder.repeat, 1);
        if(!subghz_protocol_encoder_anchor_india_get_upload(instance)) {
            ret = SubGhzProtocolStatusErrorEncoderGetUpload;
            break;
        }
        instance->encoder.is_running = true;
    } while(false);
    return ret;
}

void subghz_protocol_encoder_anchor_india_stop(void* context) {
    SubGhzProtocolEncoderAnchorIndia* instance = context;
    instance->encoder.is_running = false;
}

LevelDuration subghz_protocol_encoder_anchor_india_yield(void* context) {
    SubGhzProtocolEncoderAnchorIndia* instance = context;
    if(instance->encoder.repeat == 0 || !instance->encoder.is_running) {
        instance->encoder.is_running = false;
        return level_duration_reset();
    }
    LevelDuration ret = instance->encoder.upload[instance->encoder.front];
    if(++instance->encoder.front == instance->encoder.size_upload) {
        instance->encoder.repeat--;
        instance->encoder.front = 0;
    }
    return ret;
}

// DECODER

void* subghz_protocol_decoder_anchor_india_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    SubGhzProtocolDecoderAnchorIndia* instance =
        malloc(sizeof(SubGhzProtocolDecoderAnchorIndia));
    instance->base.protocol = &subghz_protocol_anchor_india;
    instance->generic.protocol_name = instance->base.protocol->name;
    return instance;
}

void subghz_protocol_decoder_anchor_india_free(void* context) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;
    free(instance);
}

void subghz_protocol_decoder_anchor_india_reset(void* context) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;
    instance->decoder.parser_step = AnchorDecoderStepReset;
    instance->last_data = 0;
}

void subghz_protocol_decoder_anchor_india_feed(
    void* context,
    bool level,
    uint32_t duration) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;

    switch(instance->decoder.parser_step) {
    case AnchorDecoderStepReset:
        if((!level) &&
           (DURATION_DIFF(
                duration,
                subghz_protocol_anchor_india_const.te_short * 36) <
            subghz_protocol_anchor_india_const.te_delta * 36)) {
            instance->decoder.parser_step = AnchorDecoderStepSaveDuration;
            instance->decoder.decode_data = 0;
            instance->decoder.decode_count_bit = 0;
            instance->te = 0;
            instance->guard_time = ANCHOR_GUARD_TIME_DEFAULT;
        }
        break;
    case AnchorDecoderStepSaveDuration:
        if(level) {
            instance->decoder.te_last = duration;
            instance->te += duration;
            instance->decoder.parser_step = AnchorDecoderStepCheckDuration;
        }
        break;
    case AnchorDecoderStepCheckDuration:
        if(!level) {
            if(duration >= ((uint32_t)subghz_protocol_anchor_india_const.te_long * 2)) {
                instance->decoder.parser_step = AnchorDecoderStepSaveDuration;
                if(instance->decoder.decode_count_bit ==
                   subghz_protocol_anchor_india_const.min_count_bit_for_found) {
                    if((instance->last_data == instance->decoder.decode_data) &&
                       instance->last_data) {
                        instance->te /=
                            (instance->decoder.decode_count_bit * 4 + 1);
                        instance->generic.data = instance->decoder.decode_data;
                        instance->generic.data_count_bit =
                            instance->decoder.decode_count_bit;
                        instance->guard_time =
                            roundf((float)duration / instance->te);
                        if((instance->guard_time < 15) ||
                           (instance->guard_time > 72)) {
                            instance->guard_time = ANCHOR_GUARD_TIME_DEFAULT;
                        }
                        if(instance->base.callback)
                            instance->base.callback(
                                &instance->base, instance->base.context);
                    }
                    instance->last_data = instance->decoder.decode_data;
                }
                instance->decoder.decode_data = 0;
                instance->decoder.decode_count_bit = 0;
                instance->te = 0;
                break;
            }
            instance->te += duration;
            if((DURATION_DIFF(
                    instance->decoder.te_last,
                    subghz_protocol_anchor_india_const.te_short) <
                subghz_protocol_anchor_india_const.te_delta) &&
               (DURATION_DIFF(
                    duration, subghz_protocol_anchor_india_const.te_long) <
                subghz_protocol_anchor_india_const.te_delta * 3)) {
                subghz_protocol_blocks_add_bit(&instance->decoder, 0);
                instance->decoder.parser_step = AnchorDecoderStepSaveDuration;
            } else if(
                (DURATION_DIFF(
                     instance->decoder.te_last,
                     subghz_protocol_anchor_india_const.te_long) <
                 subghz_protocol_anchor_india_const.te_delta * 3) &&
                (DURATION_DIFF(
                     duration, subghz_protocol_anchor_india_const.te_short) <
                 subghz_protocol_anchor_india_const.te_delta)) {
                subghz_protocol_blocks_add_bit(&instance->decoder, 1);
                instance->decoder.parser_step = AnchorDecoderStepSaveDuration;
            } else {
                instance->decoder.parser_step = AnchorDecoderStepReset;
            }
        } else {
            instance->decoder.parser_step = AnchorDecoderStepReset;
        }
        break;
    }
}

static void subghz_protocol_anchor_india_check_remote_controller(
    SubGhzBlockGeneric* instance) {
    instance->serial = instance->data >> 4;
    instance->btn = instance->data & 0xF;
}

uint8_t subghz_protocol_decoder_anchor_india_get_hash_data(void* context) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;
    return subghz_protocol_blocks_get_hash_data(
        &instance->decoder,
        (instance->decoder.decode_count_bit / 8) + 1);
}

SubGhzProtocolStatus subghz_protocol_decoder_anchor_india_serialize(
    void* context,
    FlipperFormat* flipper_format,
    SubGhzRadioPreset* preset) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;
    SubGhzProtocolStatus ret =
        subghz_block_generic_serialize(&instance->generic, flipper_format, preset);
    if((ret == SubGhzProtocolStatusOk) &&
       !flipper_format_write_uint32(flipper_format, "TE", &instance->te, 1)) {
        FURI_LOG_E(TAG, "Unable to add TE");
        ret = SubGhzProtocolStatusErrorParserTe;
    }
    if((ret == SubGhzProtocolStatusOk) &&
       !flipper_format_write_uint32(
           flipper_format, "Guard_time", &instance->guard_time, 1)) {
        FURI_LOG_E(TAG, "Unable to add Guard_time");
        ret = SubGhzProtocolStatusErrorParserOthers;
    }
    return ret;
}

SubGhzProtocolStatus subghz_protocol_decoder_anchor_india_deserialize(
    void* context,
    FlipperFormat* flipper_format) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;
    SubGhzProtocolStatus ret = SubGhzProtocolStatusError;
    do {
        ret = subghz_block_generic_deserialize_check_count_bit(
            &instance->generic,
            flipper_format,
            subghz_protocol_anchor_india_const.min_count_bit_for_found);
        if(ret != SubGhzProtocolStatusOk) break;
        if(!flipper_format_rewind(flipper_format)) {
            FURI_LOG_E(TAG, "Rewind error");
            ret = SubGhzProtocolStatusErrorParserOthers;
            break;
        }
        if(!flipper_format_read_uint32(
               flipper_format, "TE", (uint32_t*)&instance->te, 1)) {
            FURI_LOG_E(TAG, "Missing TE");
            ret = SubGhzProtocolStatusErrorParserTe;
            break;
        }
        if(!flipper_format_read_uint32(
               flipper_format, "Guard_time", (uint32_t*)&instance->guard_time, 1)) {
            instance->guard_time = ANCHOR_GUARD_TIME_DEFAULT;
        } else {
            if((instance->guard_time < 15) || (instance->guard_time > 72)) {
                instance->guard_time = ANCHOR_GUARD_TIME_DEFAULT;
            }
        }
    } while(false);
    return ret;
}

void subghz_protocol_decoder_anchor_india_get_string(
    void* context,
    FuriString* output) {
    furi_assert(context);
    SubGhzProtocolDecoderAnchorIndia* instance = context;
    subghz_protocol_anchor_india_check_remote_controller(&instance->generic);
    uint32_t data_rev = subghz_protocol_blocks_reverse_key(
        instance->generic.data, instance->generic.data_count_bit);
    furi_string_cat_printf(
        output,
        "%s %dbit\r\n"
        "Key:0x%08lX\r\n"
        "Rev:0x%08lX\r\n"
        "Sn:0x%05lX Btn:%01X\r\n"
        "Te:%luus GT:Te*%lu\r\n",
        instance->generic.protocol_name,
        instance->generic.data_count_bit,
        (uint32_t)(instance->generic.data & 0xFFFFFF),
        data_rev,
        instance->generic.serial,
        instance->generic.btn,
        instance->te,
        instance->guard_time);
}
