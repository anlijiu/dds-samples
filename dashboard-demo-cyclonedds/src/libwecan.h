#ifndef LIBWECAN_H
#define LIBWECAN_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FALSE     0
#define TRUE      1
#define UNSIGNED  2
#define SIGNED    3
#define INTEL     4
#define MOTOROLA  5

void compute_indexes(const uint8_t startbit, const uint8_t length, uint8_t endianness, 
                uint8_t *offset_lsb, uint8_t *byte_index_lsb, uint8_t *byte_index_msb);

uint64_t frame_to_local(const uint8_t *frame, uint8_t byte_index_lsb, 
                uint8_t byte_index_msb, uint8_t endianness);

uint64_t extract(const uint8_t *frame, const uint8_t startbit, 
                const uint8_t length, uint8_t signedness, uint8_t endianness);

void insert(uint8_t *frame, uint8_t startbit, uint8_t length,uint64_t value, 
                uint8_t endianness);

void encode_uint64_t(uint8_t *frame, uint64_t physical_value, uint8_t startbit,
                uint8_t length, uint8_t endianness, double factor, double offset);

void encode_int64_t(uint8_t *frame, int64_t physical_value, uint8_t startbit,
                uint8_t length, uint8_t endianness, double factor, double offset);

void encode_double(uint8_t *frame, double physical_value, uint8_t startbit,
                uint8_t length, uint8_t endianness, double factor, double offset);

void encode_float(uint8_t *frame, float physical_value, uint8_t startbit,
                uint8_t length, uint8_t endianness, double factor, double offset);

uint64_t decode_uint64_t(uint8_t *frame, uint8_t startbit, uint8_t length, 
                uint8_t endianness, double factor, double offset);

int64_t decode_int64_t(uint8_t *frame, uint8_t startbit, uint8_t length, 
                uint8_t endianness, double factor, double offset);

double decode_double(uint8_t *frame, uint8_t startbit, uint8_t length, 
                uint8_t endianness, double factor, double offset);

float decode_float(uint8_t *frame, uint8_t startbit, uint8_t length, 
                uint8_t endianness, double factor, double offset);

#ifdef __cplusplus
}
#endif
#endif /* LIBWECAN_H */
