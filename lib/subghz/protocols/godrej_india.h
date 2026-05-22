#pragma once

#include <lib/subghz/protocols/base.h>
#include <lib/subghz/blocks/const.h>
#include <lib/subghz/blocks/decoder.h>
#include <lib/subghz/blocks/encoder.h>
#include <lib/subghz/blocks/generic.h>
#include <lib/subghz/blocks/math.h>

// HackNlearn India - Godrej Lock Protocol
#define SUBGHZ_PROTOCOL_GODREJ_INDIA_NAME "Godrej India"

typedef struct SubGhzProtocolDecoderGodrejIndia SubGhzProtocolDecoderGodrejIndia;
typedef struct SubGhzProtocolEncoderGodrejIndia SubGhzProtocolEncoderGodrejIndia;

extern const SubGhzProtocol subghz_protocol_godrej_india;

void* subghz_protocol_decoder_godrej_india_alloc(SubGhzEnvironment* environment);
void subghz_protocol_decoder_godrej_india_free(void* context);
void subghz_protocol_decoder_godrej_india_reset(void* context);
void subghz_protocol_decoder_godrej_india_feed(void* context, bool level, uint32_t duration);
uint8_t subghz_protocol_decoder_godrej_india_get_hash_data(void* context);
SubGhzProtocolStatus subghz_protocol_decoder_godrej_india_serialize(void* context, FlipperFormat* flipper_format, SubGhzRadioPreset* preset);
SubGhzProtocolStatus subghz_protocol_decoder_godrej_india_deserialize(void* context, FlipperFormat* flipper_format);
void subghz_protocol_decoder_godrej_india_get_string(void* context, FuriString* output);

void* subghz_protocol_encoder_godrej_india_alloc(SubGhzEnvironment* environment);
void subghz_protocol_encoder_godrej_india_free(void* context);
SubGhzProtocolStatus subghz_protocol_encoder_godrej_india_deserialize(void* context, FlipperFormat* flipper_format);
void subghz_protocol_encoder_godrej_india_stop(void* context);
LevelDuration subghz_protocol_encoder_godrej_india_yield(void* context);
