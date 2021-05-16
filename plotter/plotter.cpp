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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>

#include "../lib/bls-signatures/src/bls.hpp"
#include "../lib/include/picosha2.hpp"
#include "cxxopts.hpp"
#include "plotter_disk.hpp"
#include "prover_disk.hpp"
#include "verifier.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace bls;

vector<uint8_t> Random(std::vector<uint8_t> v, int n, int l, int r)  //生成范围在l~r的随机数
{
    srand(time(0));  //设置时间种子
    for (int i = 0; i < n; i++) {
        v.push_back(rand() % (r - l + 1) + l);  //生成区间r~l的随机数
    }
    return v;
}

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

int main(int argc, char *argv[])
try {
    cxxopts::Options options(
        "ProofOfSpace", "Utility for plotting, generating and verifying proofs of space.");
    options.positional_help("(create/prove/verify/check) param1 param2 ").show_positional_help();

    // Default values
    uint8_t k = 32;
    uint32_t num_buckets = 128;
    uint32_t num_stripes = 65536;
    uint8_t num_threads = 2;
    string filename = "plot.dat";
    string tempdir = ".";
    string tempdir2 = ".";
    string finaldir = ".";
    string operation = "help";
    string memo = "0102030405";
    string id = "022fb42c08c12de3a6af053880199806532e79515f94e83461612101f9412f9e";
    string farmer_public_key = "";
    string pool_public_key = "";
    bool nobitfield = false;
    bool show_progress = false;
    uint32_t buffmegabytes = 3389;
    options.allow_unrecognised_options().add_options()(
            "k, size", "Plot size", cxxopts::value<uint8_t>(k))(
            "r, threads", "Number of threads", cxxopts::value<uint8_t>(num_threads))(
                "u, buckets", "Number of buckets", cxxopts::value<uint32_t>(num_buckets))(
            "s, stripes", "Size of stripes", cxxopts::value<uint32_t>(num_stripes))(
            "t, tempdir", "Temporary directory", cxxopts::value<string>(tempdir))(
        "2, tempdir2", "Second Temporary directory", cxxopts::value<string>(tempdir2))(
        "d, finaldir", "Final directory", cxxopts::value<string>(finaldir))(
        "f, file", "Filename", cxxopts::value<string>(filename))(
        "F, farmer_public_key", "Farmer Public Key", cxxopts::value<string>(farmer_public_key))(
        "P, pool_public_key", "Pool Public Key", cxxopts::value<string>(pool_public_key))(
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
        int n = 32;  //数组元素的个数，即生成随机数的个数
        vector<uint8_t> seed;
        seed = Random(seed, n, 0, 256);
        std::cout << "seed size=============" << seed.size() << std::endl;
        bls::PrivateKey sk = bls::AugSchemeMPL().KeyGen(seed);
        //  The plot public key is the combination of the harvester and farmer keys
        //  The plot id is based on the harvester, farmer, and pool keys
        //  The plot meno : pool_public_key, farmer_public_key, sk
        uint32_t derivePath[4];
        derivePath[0] = 12381;
        derivePath[1] = 8844;
        derivePath[2] = 3;
        derivePath[3] = 0;
        for (int i = 0; i < 4; i++) {
            sk = bls::AugSchemeMPL().DeriveChildSk(sk, i);
        }
        bls::G1Element localSk = sk.GetG1Element();
        std::vector<uint8_t> farmerArray(48);
        HexToBytes(farmer_public_key, farmerArray.data());
        bls::G1Element farmerPublicKey;
        farmerPublicKey = bls::G1Element::FromByteVector(farmerArray);
        std::vector<uint8_t> poolArray(48);
        HexToBytes(farmer_public_key, poolArray.data());
        bls::G1Element poolPublicKey;
        poolPublicKey = bls::G1Element::FromByteVector(poolArray);
        bls::G1Element plotPublicKey = localSk + farmerPublicKey;
        vector<uint8_t> msg_id;
        msg_id.insert(
            msg_id.end(), poolPublicKey.Serialize().begin(), poolPublicKey.Serialize().end());
        msg_id.insert(
            msg_id.end(), plotPublicKey.Serialize().begin(), plotPublicKey.Serialize().end());
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
        memo_bytes.insert(
            memo_bytes.end(), poolPublicKey.Serialize().begin(), poolPublicKey.Serialize().end());
        memo_bytes.insert(
            memo_bytes.end(),
            farmerPublicKey.Serialize().begin(),
            farmerPublicKey.Serialize().end());
        memo_bytes.insert(memo_bytes.end(), localSk.Serialize().begin(), localSk.Serialize().end());
        std::array<uint8_t, 32> id_bytes;
        bls::Util::Hash256(id_bytes.data(), (const uint8_t *)msg_id.data(), msg_id.size());
        char *idp = new char[sizeof(id_bytes)];
        std::memcpy(idp, id_bytes.data(), sizeof(id_bytes));
        idp[sizeof(id_bytes)] = 0;
        string idstr(idp);
        id = idstr;
        //        HexToBytes(memo, memo_bytes.data());
        //        HexToBytes(id, id_bytes.data());
        std::stringstream ss;
        ss << static_cast<int>(k);
        std::string kStr;
        ss >> kStr;
        time_t timep;
        time(&timep);
        char dt_string[64];
        strftime(dt_string, sizeof(dt_string), "%Y-%m-%d-%H-%M", localtime(&timep));
        //        std::string idStr((char *) id_bytes.data());
        filename = "plot-k" + kStr + "-" + dt_string + "-" + id + ".plot";
        cout << "tempdir=" << tempdir << ";tempdir2=" << tempdir2 << ";finaldir=" << finaldir
             << ";k=" << static_cast<int>(k) << ";memo=" << memo_bytes.data() << ";id=" << id
             << ";buffmegabytes=" << static_cast<int>(buffmegabytes)
             << ";num_buckets=" << static_cast<int>(num_buckets) << ";num_stripes"
             << static_cast<int>(num_stripes) << ";num_threads=" << static_cast<int>(num_threads)
             << ";nobitfield=" << static_cast<bool>(nobitfield)
             << ";show_progress=" << static_cast<bool>(show_progress) << ";filename=" << filename
             << endl;
        //        DiskPlotter plotter = DiskPlotter();
        //        plotter.CreatePlotDisk(
        //            tempdir,
        //            tempdir,
        //            finaldir,
        //            filename,
        //            k,
        //            memo_bytes.data(),
        //            memo_bytes.size(),
        //            id_bytes.data(),
        //            id_bytes.size(),
        //            buffmegabytes,
        //            num_buckets,
        //            num_stripes,
        //            num_threads,
        //            nobitfield,
        //            show_progress);
    }
    return 0;
} catch (const cxxopts::OptionException &e) {
    cout << "error parsing options: " << e.what() << endl;
    return 1;
} catch (const std::exception &e) {
    std::cerr << "Caught exception: " << e.what() << endl;
    throw e;
}
