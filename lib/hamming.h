#include <vector>
#include <cstdint>

std::vector<bool> ToCode(const std::vector<bool>& block);

enum ErrorsInBlock {
    ZERO_ERROR,
    ONE_ERROR,
    MULTIPLE_ERRORS,
};

bool IsDamaged(const std::vector<bool>& block);

uint8_t FixErrors(std::vector<bool>& block);

std::vector<bool> ToDecode(const std::vector<bool>& block);