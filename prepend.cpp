/*
 * Copyright (C) Ilya Taranov 2017
 * Distributed under the MIT License (http://opensource.org/licenses/MIT).
 */

#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

/*
 * Reusable add prefix context operation with preserving buffers
 */
struct AddPrefix {
    size_t bufSz;

    unique_ptr<char[]> buf1;
    unique_ptr<char[]> buf2;

    const char * prefix;
    size_t prefixSize;

    AddPrefix(const char * a_prefix, size_t a_prefixSize)
        : prefix(a_prefix), prefixSize(a_prefixSize)
    {
        constexpr static int bufferAlign = 4*1024;

        /* Initial block must be greater than prefix */
        bufSz = ((prefixSize / bufferAlign) + 1) * bufferAlign;

        buf1.reset(new char[bufSz]);
        buf2.reset(new char[bufSz]);
    }

    int operator ()(const char * fileName) {
        fstream bigFile(fileName);

        /* Read the initial block */
        size_t sz = bigFile.readsome(buf1.get(), bufSz);

        /* Write the prefix */
        bigFile.seekp(0);
        bigFile.write(prefix, prefixSize);
        size_t c = 0;

        /* TODO : We don't really need the whole prefix-sized 
         * block after the prefix is written, this can be optimized */

        do {
            bigFile.seekg((c + 1) * bufSz);
            size_t sz2 = bigFile.readsome(buf2.get(), bufSz);

            bigFile.seekp(c * bufSz + prefixSize);
            bigFile.write(buf1.get(), sz);

            buf1.swap(buf2);
            sz = sz2;

            ++c;
        } while (sz == bufSz);

        /* Write the leftover */
        bigFile.seekp(c * bufSz + prefixSize);
        bigFile.write(buf1.get(), sz);
        bigFile.close();

        return 0;
    }
};

int main(int argc, char **argv) 
{
    if (argc < 3) {
        /* Print help and exit */

        cout << "Prepend v0.01 - "
        "Add data to the beginning of the file inplace (without copying the file)\n\n"
        " USAGE: prepend PREFIXFILE FILE [FILE]*\n\n"
        " WARNING: The process is non-transactional. If interrupted in the middle,\n"
        "the file is left corrupted.\n";

        return -1;
    }

    ifstream prefixFile(argv[1], ios::binary | ios::ate);
    size_t prefixSize = prefixFile.tellg();
    unique_ptr<char[]> prefix(new char[prefixSize]);
    prefixFile.seekg(0);
    prefixFile.read(prefix.get(), prefixSize);

    AddPrefix addPrefixOperator(prefix.get(), prefixSize);

    for (int i = 2; i < argc; ++i) {
        addPrefixOperator(argv[i]);
    }

    return 0;
}
