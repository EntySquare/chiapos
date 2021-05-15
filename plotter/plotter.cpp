// Copyright 2018 Chia Network Inc

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//    http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <ctime>
#include <set>

#include "cxxopts.hpp"
#include "../lib/include/picosha2.hpp"
#include "plotter_disk.hpp"
#include "prover_disk.hpp"
#include "verifier.hpp"

using std::string;
using std::vector;
using std::endl;
using std::cout;

void HexToBytes(const string &hex, uint8_t *result)
{
    for (uint32_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t)strtol(byteString.c_str(), NULL, 16);
        result[i / 2] = byte;
    }
}

vector<unsigned char> intToBytes(uint32_t paramInt, uint32_t numBytes)
{
    vector<unsigned char> arrayOfByte(numBytes, 0);
    for (uint32_t i = 0; paramInt > 0; i++) {
        arrayOfByte[numBytes - i - 1] = paramInt & 0xff;
        paramInt >>= 8;
    }
    return arrayOfByte;
}

string Strip0x(const string &hex)
{
    if (hex.size() > 1 && (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X")) {
        return hex.substr(2);
    }
    return hex;
}

void HelpAndQuit(cxxopts::Options options)
{
    cout << options.help({""}) << endl;
    cout << "./ProofOfSpace create" << endl;
    cout << "./ProofOfSpace prove <challenge>" << endl;
    cout << "./ProofOfSpace verify <proof> <challenge>" << endl;
    cout << "./ProofOfSpace check" << endl;
    exit(0);
}

int main(int argc, char *argv[]) try {
    cxxopts::Options options(
        "ProofOfSpace", "Utility for plotting, generating and verifying proofs of space.");
    options.positional_help("(create/prove/verify/check) param1 param2 ")
        .show_positional_help();

    // Default values
    uint8_t k = 32;
    uint32_t num_buckets = 0;
    uint32_t num_stripes = 0;
    uint8_t num_threads = 0;
    string filename = "plot.dat";
    string tempdir = ".";
    string tempdir2 = ".";
    string finaldir = ".";
    string operation = "help";
    string memo = "0102030405";
    string id = "022fb42c08c12de3a6af053880199806532e79515f94e83461612101f9412f9e";
    bool nobitfield = false;
    bool show_progress = false;
    uint32_t buffmegabytes = 0;

    options.allow_unrecognised_options().add_options()(
            "k, size", "Plot size", cxxopts::value<uint8_t>(k))(
            "r, threads", "Number of threads", cxxopts::value<uint8_t>(num_threads))(
                "u, buckets", "Number of buckets", cxxopts::value<uint32_t>(num_buckets))(
            "s, stripes", "Size of stripes", cxxopts::value<uint32_t>(num_stripes))(
            "t, tempdir", "Temporary directory", cxxopts::value<string>(tempdir))(
        "2, tempdir2", "Second Temporary directory", cxxopts::value<string>(tempdir2))(
        "d, finaldir", "Final directory", cxxopts::value<string>(finaldir))(
        "f, file", "Filename", cxxopts::value<string>(filename))(
        "m, memo", "Memo to insert into the plot", cxxopts::value<string>(memo))(
        "i, id", "Unique 32-byte seed for the plot", cxxopts::value<string>(id))(
        "e, nobitfield", "Disable bitfield", cxxopts::value<bool>(nobitfield))(
        "b, buffer",
        "Megabytes to be used as buffer for sorting and plotting",
        cxxopts::value<uint32_t>(buffmegabytes))(
        "p, progress", "Display progress percentage during plotting",
        cxxopts::value<bool>(show_progress))(
        "help", "Print help");

    auto result = options.parse(argc, argv);

    if (result.count("help") || argc < 2) {
        HelpAndQuit(options);
    }
    operation = argv[1];
    std::cout << "operation: " << operation << std::endl;

    if (operation == "help") {
        HelpAndQuit(options);
    } else if (operation == "create") {
        cout << "Generating plot for k=" << static_cast<int>(k) << " filename=" << filename
             << " id=" << id << endl
             << endl;
        id = Strip0x(id);
        if (id.size() != 64) {
            cout << "Invalid ID, should be 32 bytes (hex)" << endl;
            exit(1);
        }
        memo = Strip0x(memo);
        if (memo.size() % 2 != 0) {
            cout << "Invalid memo, should be only whole bytes (hex)" << endl;
            exit(1);
        }
        std::vector<uint8_t> memo_bytes(memo.size() / 2);
        std::array<uint8_t, 32> id_bytes;

        HexToBytes(memo, memo_bytes.data());
        HexToBytes(id, id_bytes.data());
        std::string kStr( (char *) k);
        std::string idStr((char *) id_bytes.data());
        filename = "plot-k"+kStr+"-"+ idStr +".plot";
        cout << "tempdir=" << tempdir << ";tempdir2=" << tempdir2 << ";finaldir=" << finaldir << ";k=" \
             << static_cast<char>(k) << ";memo=" << memo << ";id=" << id << ";buffmegabytes=" \
             << static_cast<char>(buffmegabytes) << ";num_buckets=" << static_cast<char>(num_buckets) \
             << ";num_stripes" << static_cast<char>(num_stripes) << ";num_threads=" \
             << static_cast<char>(num_threads) << ";nobitfield=" << static_cast<char>(nobitfield) \
             << ";show_progress=" << static_cast<char>(show_progress) << endl;
//        DiskPlotter plotter = DiskPlotter();
//        plotter.CreatePlotDisk(
//                tempdir,
//                tempdir2,
//                finaldir,
//                filename,
//                k,
//                memo_bytes.data(),
//                memo_bytes.size(),
//                id_bytes.data(),
//                id_bytes.size(),
//                buffmegabytes,
//                num_buckets,
//                num_stripes,
//                num_threads,
//                nobitfield,
//                show_progress);
//    }
    return 0;
} catch (const cxxopts::OptionException &e) {
    cout << "error parsing options: " << e.what() << endl;
    return 1;
} catch (const std::exception &e) {
    std::cerr << "Caught exception: " << e.what() << endl;
    throw e;
}
