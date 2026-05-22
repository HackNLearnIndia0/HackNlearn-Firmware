#pragma once

#include <lib/subghz/protocols/base.h>
#include <lib/subghz/blocks/const.h>
#include <lib/subghz/blocks/decoder.h>
#include <lib/subghz/blocks/encoder.h>
#include <lib/subghz/blocks/generic.h>
#include <lib/subghz/blocks/math.h>

// HackNlearn India - Anchor Lock Protocol
#define SUBGHZ_PROTOCOL_ANCHOR_INDIA_NAME "Anchor India"

typedef struct SubGhzProtocolDecoderAnchorIndia SubGhzProtocolDecoderAnchorIndia;
typedef struct SubGhzProtocolEncoderAnchorIndia SubGhzProtocolEncoderAnchorIndia;

extern const SubGhzProtocol subghz_protocol_anchor_india;

void* subghz_protocol_decoder_anchor_india_alloc(SubGhzEnvironment* environment);
void subghz_protocol_decoder_anchor_india_free(void* context);
void subghz_protocol_decoder_anchor_india_reset(void* context);
void subghz_protocol_decoder_anchor_india_feed(void* context, bool level, uint32_t duration);
uint8_t subghz_protocol_decoder_anchor_india_get_hash_data(void* context);
SubGhzProtocolStatus subghz_protocol_decoder_anchor_india_serialize(void* context, FlipperFormat* flipper_format, SubGhzRadioPreset* preset);
SubGhzProtocolStatus subghz_protocol_decoder_anchor_india_deserialize(void* context, FlipperFormat* flipper_format);
void subghz_protocol_decoder_anchor_india_get_string(void* context, FuriString* output);

void* subghz_protocol_encoder_anchor_india_alloc(SubGhzEnvironment* environment);
void subghz_protocol_encoder_anchor_india_free(void* context);
SubGhzProtocolStatus subghz_protocol_encoder_anchor_india_deserialize(void* context, FlipperFormat* flipper_format);
void subghz_protocol_encoder_anchor_india_stop(void* context);
LevelDuration subghz_protocol_encoder_anchor_india_yield(void* context);
