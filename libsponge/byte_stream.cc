#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    _capacity = capacity;
    _buffer.resize(capacity);
}

size_t ByteStream::write(const string &data) {
    if (_ended || _size == _capacity) {
        return {};
    }

    size_t new_bytes_written = data.length() + _size > _capacity ? _capacity - _size : data.length();
    _bytes_written += new_bytes_written;
    _size += new_bytes_written;

    for (size_t i = 0; i < new_bytes_written; i++) {
        _buffer[_tail] = data[i];
        _tail = (_tail + 1) % _capacity;
    }

    return new_bytes_written;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    if (_size == 0 || len == 0) {
        return {};
    }

    size_t bytes_peek = len > _size ? _size : len;

    string output (bytes_peek, '\0');
    for (size_t i = 0;  i < bytes_peek; i++) {
        output[i] = _buffer[(_head + i) % _capacity];
    }

    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    if (_size == 0 || len == 0) {
        return;
    }

    size_t bytes_pop = len > _size ? _size : len;
    _bytes_read += bytes_pop;
    _size -= bytes_pop;

    for (size_t i = 0; i < bytes_pop; i++) {
        _buffer[_head] = '\0';
        _head = (_head + 1) % _capacity;
    }
 }

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    if (_size == 0 || len == 0) {
        return {};
    }

    size_t max_bytes_read = len > _size ? _size : len;
    _bytes_read += max_bytes_read;
    _size -= max_bytes_read;

    string output (max_bytes_read, '\0');
    for (size_t i = 0;  i < max_bytes_read; i++) {
        output[i] = _buffer[_head];
        _buffer[_head] = '\0';
        _head = (_head + 1) % _capacity;
    }

    return output;
}

void ByteStream::end_input() {
    _ended = true;
}

bool ByteStream::input_ended() const { return _ended; }

size_t ByteStream::buffer_size() const { return _size; }

bool ByteStream::buffer_empty() const { return _size == 0; }

bool ByteStream::eof() const { return _size == 0 && _head == _tail && _ended; }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _size; }
