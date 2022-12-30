#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    if (seg.header().syn && !_isn.has_value()) {
        _isn = seg.header().seqno;
    } else if ((seg.header().syn ^ _isn.has_value()) == false) {
        return;
    }

    if (seg.header().seqno == _isn.value() && !seg.header().syn) {
        return;
    }
    
    uint64_t index = seg.header().seqno == _isn.value() ? 0 : unwrap(seg.header().seqno, _isn.value() + 1, 0);
    bool eof = seg.header().fin;
    _reassembler.push_substring(seg.payload().copy(), index, eof);

    _ackno = wrap(_reassembler.stream_out().bytes_written() + 1, _isn.value());
    if (_reassembler.stream_out().input_ended()) {
        _ackno = _ackno.value() + 1;
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const { return _ackno; }

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
