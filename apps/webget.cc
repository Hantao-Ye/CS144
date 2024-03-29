#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

void get_URL(const string &host, const string &path) {
    const Address web_server(host, "http");

    TCPSocket sock;
    sock.connect(web_server);

    const string get_str = "GET " + path + " HTTP/1.1\r\n";
    const string host_str = "Host: " + host + "\r\n";
    const string close_str = "Connection: close\r\n\r\n";

    sock.write(get_str);
    sock.write(host_str);
    sock.write(close_str);

    while(!sock.eof()) {
        cout << sock.read();
    }

    sock.shutdown(SHUT_RDWR);
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
