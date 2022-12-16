#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
    _buffer.resize(capacity, '\0');
    _end_index = _capacity;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof) {
        _eof_index = index + data.length();
    }

    if (_end_index - _start_index < _capacity && _output.buffer_empty()) {
        _buffer.resize(_capacity, '\0');
        _end_index = _start_index + _capacity;
    } 

    while(_output.remaining_capacity() > 0 && !_assembled_buffer.empty()) {
        size_t write_into_output = _output.write(_assembled_buffer.front());
        if (write_into_output < _assembled_buffer.front().size()) {
            _assembled_buffer.begin()->erase(0, write_into_output);
        } else {
            _assembled_buffer.pop_front();
        }
    }
    
    if (data.length() + index < _start_index || index >= min(_eof_index, _end_index)) {
        if (_start_index == _eof_index) {
            _output.end_input();
        }
        return;
    }

    size_t str_start_index = index < _start_index ? _start_index - index : 0;

    for (size_t i = str_start_index; i < min(min(_end_index, _eof_index) - index, data.length()); i++) {
        if (_buffer_received[i + index] == false) {
            _buffer[i + index - _start_index] = data[i];
            _buffer_received[i + index] = true;
            _unassembled_bytes++;
        }
    }

    string assembled_str = "";
    while (_buffer_received[_start_index] == true && _start_index < min(_end_index, _eof_index)) {
        assembled_str.push_back(_buffer[0]);

        _buffer.pop_front();
        _start_index++;
        _unassembled_bytes--;
    }

    size_t write_into_output = _output.write(assembled_str);
    if (write_into_output < assembled_str.length()) {
        assembled_str.erase(0, write_into_output);
        _assembled_buffer.push_back(assembled_str);
    }

    if (_start_index == _eof_index) {
        _output.end_input();
    } else if (_end_index - _start_index < _capacity && _output.buffer_empty()) {
        _buffer.resize(_capacity, '\0');
        _end_index = _start_index + _capacity;
    } 
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unassembled_bytes == 0; }
