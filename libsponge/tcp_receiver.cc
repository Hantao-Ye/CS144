#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader &header = seg.header();

    if (header.syn && !_isn.has_value()) {
        _isn = header.seqno;
    } else if ((header.syn ^ _isn.has_value()) == false) {
        return;
    }

    uint64_t abs_ackno = _reassembler.stream_out().bytes_written() + 1;
    uint64_t curr_abs_seqno = unwrap(header.seqno, _isn.value(), abs_ackno);
    
    uint64_t index = curr_abs_seqno - 1 + (header.syn);
    _reassembler.push_substring(seg.payload().copy(), index, header.fin);

    _ackno = wrap(_reassembler.stream_out().bytes_written() + 1, _isn.value());
    if (_reassembler.stream_out().input_ended()) {
        _ackno = _ackno.value() + 1;
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const { return _ackno; }

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
