#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    uint64_t isn_raw = static_cast<uint64_t>(isn.raw_value());

    return WrappingInt32{static_cast<uint32_t>((n + isn_raw) % (1UL << 32))};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t n_abs = static_cast<uint64_t>(n.raw_value() - isn.raw_value());
    if (n_abs == checkpoint)
        return n_abs;

    uint64_t left = 0;
    uint64_t right = UINT32_MAX;
    while (left <= right) {
        uint64_t mid = left + (right - left) / 2;
        uint64_t temp_n_abs = n_abs + mid * (1UL << 32);

        if (temp_n_abs == checkpoint) {
            return temp_n_abs;
        } else if (temp_n_abs < checkpoint) {
            if (mid == UINT32_MAX) {
                return n_abs + UINT32_MAX * (1UL << 32);
            }
            left = mid + 1;
        } else {
            if (mid == 0) {
                return n_abs;
            }
            right = mid - 1;
        }
    }

    return max(n_abs + left * (1UL << 32), checkpoint) - min(n_abs + left * (1UL << 32), checkpoint) <
                   max(n_abs + right * (1UL << 32), checkpoint) - min(n_abs + right * (1UL << 32), checkpoint)
               ? n_abs + left * (1UL << 32)
               : n_abs + right * (1UL << 32);
}
