#include "hamming.h"

std::vector<bool> ToCode(const std::vector<bool>& block) {
    std::vector<bool> hamming_block;

    size_t hamming_block_pos = 0;
    size_t control_bit_pos = 1;

    for (size_t i = 0; i < block.size(); ++i) {
        if (hamming_block_pos == control_bit_pos - 1) {
            hamming_block.push_back(false);
            control_bit_pos <<= 1;
            --i;
        } else {
            hamming_block.push_back(block[i]);
        }
        ++hamming_block_pos;
    }

    control_bit_pos = 1;
    while (control_bit_pos <= hamming_block.size()) {
        bool control_bit_value = false;
        size_t cur_pos = control_bit_pos - 1;

        while (cur_pos < hamming_block.size()) {
            for (size_t i = 0; i < control_bit_pos && cur_pos < hamming_block.size(); ++i, ++cur_pos) {
                control_bit_value ^= hamming_block[cur_pos];
            }
            cur_pos += control_bit_pos;
        }

        hamming_block[control_bit_pos - 1] = control_bit_value;
        control_bit_pos <<= 1;
    }

    return hamming_block;
}

bool IsDamaged(const std::vector<bool>& block) {
    size_t control_bit_pos = 1;
    while (control_bit_pos <= block.size()) {
        bool control_bit_value = false;
        size_t cur_pos = control_bit_pos;

        while (cur_pos <= block.size()) {
            for (size_t i = 0; i < control_bit_pos && cur_pos <= block.size(); ++i, ++cur_pos) {
                control_bit_value ^= block[cur_pos - 1];
            }
            cur_pos += control_bit_pos;
        }

        if (control_bit_value) {
            return true;
        }
        control_bit_pos <<= 1;
    }
    return false;
}

uint8_t FixErrors(std::vector<bool>& block) {
    size_t control_bit_pos = 1;
    std::vector<size_t> invalid_control_bits;
    while (control_bit_pos <= block.size()) {
        bool control_bit_value = false;
        size_t cur_pos = control_bit_pos;

        while (cur_pos <= block.size()) {
            for (size_t i = 0; i < control_bit_pos && cur_pos <= block.size(); ++i, ++cur_pos) {
                control_bit_value ^= block[cur_pos - 1];
            }
            cur_pos += control_bit_pos;
        }

        if (control_bit_value) {
            invalid_control_bits.push_back(control_bit_pos);
        }
        control_bit_pos <<= 1;
    }

    if (invalid_control_bits.empty()) {
        return ZERO_ERROR;
    }
    size_t invalid_bit_pos = 0;
    for (int i = 0; i < invalid_control_bits.size(); ++i) {
        invalid_bit_pos += invalid_control_bits[i];
    }
    --invalid_bit_pos;

    block[invalid_bit_pos] = !block[invalid_bit_pos];
    if (IsDamaged(block)) {
        return MULTIPLE_ERRORS;
    }
    return ONE_ERROR;
}

std::vector<bool> ToDecode(const std::vector<bool>& block) {
    std::vector<bool> decoded;
    size_t control_bit_pos = 1;

    for (size_t i = 0; i < block.size(); ++i) {
        if (i == control_bit_pos - 1) {
            control_bit_pos <<= 1;
        } else {
            decoded.push_back(block[i]);
        }
    }

    return decoded;
}