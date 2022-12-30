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
    const constexpr uint64_t INT32_RANGE = 1L << 32;
    if (n_abs == checkpoint)
        return n_abs;

    if(checkpoint > n_abs) {
        uint64_t real_checkpoint = (checkpoint - n_abs) + (INT32_RANGE >> 1);
        uint64_t wrap_num = real_checkpoint / INT32_RANGE;
        return wrap_num * INT32_RANGE + n_abs;
    }
    else
       return n_abs;
}
