#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : cap(capacity), length(0), buffer(), bytes_r(0), bytes_w(0) {}

size_t ByteStream::write(const string &data) {
    _ended = false;
    _eof = false;

    int size = data.size();
    if (size + length > cap) {
        size = cap - length;
    }

    for (int i = 0; i < size; i++) {
        buffer.push_back(data[i]);
        length++;
    }

    bytes_w += size;
    return size;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string output;

    for (size_t i = 0; i < min(len, length); i++) {
        output += buffer[i];
    }

    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    for (size_t i = 0; i < len; i++) {
        buffer.pop_front();
        length--;

        bytes_r++;
    }

    if (length == 0 && _ended) {
        _eof = true;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string output;

    for (size_t i = 0; i < min(len, length); i++) {
        output += buffer.front();
        buffer.pop_front();
        length--;

        bytes_r++;
    }

    if (length == 0 && _ended) {
        _eof = true;
    }
    return output;
}

void ByteStream::end_input() {
    _ended = true;
    if (length == 0) {
        _eof = true;
    }
}

bool ByteStream::input_ended() const { return _ended; }

size_t ByteStream::buffer_size() const { return length; }

bool ByteStream::buffer_empty() const { return length == 0; }

bool ByteStream::eof() const { return _eof; }

size_t ByteStream::bytes_written() const { return bytes_w; }

size_t ByteStream::bytes_read() const { return bytes_r; }

size_t ByteStream::remaining_capacity() const { return cap - length; }
