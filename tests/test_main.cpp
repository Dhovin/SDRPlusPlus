#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <cassert>

#include <dsp/types.h>
#include <dsp/buffer/buffer.h>
#include <server_protocol.h>
#include <command_args.h>

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_CASE(name) static void name(); \
    struct Register_##name { \
        Register_##name() { runTest(#name, name); } \
    };

#define ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        std::cerr << "  FAIL: " << #expr << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        g_tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "  FAIL: " << #a << " == " << #b << " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        g_tests_failed++; \
        return; \
    } \
} while(0)

void runTest(const char* name, void (*fn)()) {
    g_tests_run++;
    int before = g_tests_failed;
    std::cout << "[ RUN      ] " << name << std::endl;
    fn();
    if (g_tests_failed == before) {
        std::cout << "[       OK ] " << name << std::endl;
    }
}

// 1. Test DSP Buffer Allocation and SIMD Alignment
void test_buffer_allocation() {
    constexpr int count = 4096;
    float* buf = dsp::buffer::alloc<float>(count);
    ASSERT_TRUE(buf != nullptr);

    // Verify SIMD alignment
    uintptr_t addr = reinterpret_cast<uintptr_t>(buf);
    unsigned int alignment = volk_get_alignment();
    ASSERT_TRUE(alignment > 0);
    ASSERT_EQ(addr % alignment, 0);

    // Clear and verify
    dsp::buffer::clear(buf, count);
    for (int i = 0; i < count; i++) {
        ASSERT_EQ(buf[i], 0.0f);
    }

    dsp::buffer::free(buf);
}

// 2. Test DSP Complex Types
void test_dsp_complex_types() {
    dsp::complex_t c1{ 3.0f, 4.0f };
    ASSERT_EQ(c1.re, 3.0f);
    ASSERT_EQ(c1.im, 4.0f);

    dsp::complex_t c2{ 1.0f, 2.0f };
    dsp::complex_t c_sum = c1 + c2;
    ASSERT_EQ(c_sum.re, 4.0f);
    ASSERT_EQ(c_sum.im, 6.0f);

    float magSq = c1.re * c1.re + c1.im * c1.im;
    ASSERT_EQ(magSq, 25.0f);
}

// 3. Test Server Protocol Packet Validation Bounds
void test_server_protocol_bounds() {
    // Valid packet header
    server::PacketHeader validHeader;
    validHeader.type = server::PACKET_TYPE_COMMAND;
    validHeader.size = sizeof(server::PacketHeader) + sizeof(server::CommandHeader);

    ASSERT_TRUE(validHeader.size >= sizeof(server::PacketHeader));
    ASSERT_TRUE(validHeader.size <= SERVER_MAX_PACKET_SIZE);

    // Oversized packet header simulation
    server::PacketHeader oversizedHeader;
    oversizedHeader.type = server::PACKET_TYPE_BASEBAND;
    oversizedHeader.size = SERVER_MAX_PACKET_SIZE + 1024;

    bool isOversizedValid = (oversizedHeader.size <= SERVER_MAX_PACKET_SIZE && oversizedHeader.size >= sizeof(server::PacketHeader));
    ASSERT_FALSE(isOversizedValid);

    // Undersized packet header simulation
    server::PacketHeader undersizedHeader;
    undersizedHeader.type = server::PACKET_TYPE_COMMAND;
    undersizedHeader.size = sizeof(server::PacketHeader) - 1;

    bool isUndersizedValid = (undersizedHeader.size >= sizeof(server::PacketHeader) && undersizedHeader.size <= SERVER_MAX_PACKET_SIZE);
    ASSERT_FALSE(isUndersizedValid);
}

// 4. Test Command Line Arguments Parser
void test_command_args_parser() {
    CommandArgsParser parser;
    parser.define('r', "root", "Root directory", std::string("."));
    parser.define('s', "server", "Server mode");
    parser.define('p', "port", "Port number", 5259);

    const char* argv[] = { "sdrpp", "-r", "root_test", "-s", "-p", "1234" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    int res = parser.parse(argc, (char**)argv);
    ASSERT_EQ(res, 0);

    ASSERT_EQ((std::string)parser["root"], std::string("root_test"));
    ASSERT_TRUE((bool)parser["server"]);
    ASSERT_EQ((int)parser["port"], 1234);
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "Running SDR++ Automated Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    runTest("test_buffer_allocation", test_buffer_allocation);
    runTest("test_dsp_complex_types", test_dsp_complex_types);
    runTest("test_server_protocol_bounds", test_server_protocol_bounds);
    runTest("test_command_args_parser", test_command_args_parser);

    std::cout << "========================================" << std::endl;
    std::cout << "Test Summary: " << g_tests_run << " run, "
              << g_tests_failed << " failed." << std::endl;
    std::cout << "========================================" << std::endl;

    return (g_tests_failed == 0) ? 0 : 1;
}
