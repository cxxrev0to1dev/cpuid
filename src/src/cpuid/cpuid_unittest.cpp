// cpuid.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include <array>
#include <vector>
#include <bitset>
#include "cpuid.h"
#include <gtest/gtest.h>
#if defined(OS_WIN_X86)
#pragma comment(lib,"gtest.lib")
#else
#pragma comment(lib,"gtest_x64.lib")
#endif
//reference:https://en.wikipedia.org/wiki/CPUID

std::bitset<32> f_1_ECX;
std::bitset<32> f_1_EDX;
std::bitset<32> f_7_EBX;
std::bitset<32> f_7_ECX;
std::bitset<32> f_81_ECX;
std::bitset<32> f_81_EDX;
bool isIntel;
bool isAMD;

static bool SSE3(void) { return f_1_ECX[0]; }
static bool PCLMULQDQ(void) { return f_1_ECX[1]; }
static bool MONITOR(void) { return f_1_ECX[3]; }
static bool SSSE3(void) { return f_1_ECX[9]; }
static bool FMA(void) { return f_1_ECX[12]; }
static bool CMPXCHG16B(void) { return f_1_ECX[13]; }
static bool SSE41(void) { return f_1_ECX[19]; }
static bool SSE42(void) { return f_1_ECX[20]; }
static bool MOVBE(void) { return f_1_ECX[22]; }
static bool POPCNT(void) { return f_1_ECX[23]; }
static bool AES(void) { return f_1_ECX[25]; }
static bool XSAVE(void) { return f_1_ECX[26]; }
static bool OSXSAVE(void) { return f_1_ECX[27]; }
static bool AVX(void) { return f_1_ECX[28]; }
static bool F16C(void) { return f_1_ECX[29]; }
static bool RDRAND(void) { return f_1_ECX[30]; }

static bool MSR(void) { return f_1_EDX[5]; }
static bool CX8(void) { return f_1_EDX[8]; }
static bool SEP(void) { return f_1_EDX[11]; }
static bool CMOV(void) { return f_1_EDX[15]; }
static bool CLFSH(void) { return f_1_EDX[19]; }
static bool MMX(void) { return f_1_EDX[23]; }
static bool FXSR(void) { return f_1_EDX[24]; }
static bool SSE(void) { return f_1_EDX[25]; }
static bool SSE2(void) { return f_1_EDX[26]; }

static bool FSGSBASE(void) { return f_7_EBX[0]; }
static bool BMI1(void) { return f_7_EBX[3]; }
static bool HLE(void) { return isIntel && f_7_EBX[4]; }
static bool AVX2(void) { return f_7_EBX[5]; }
static bool BMI2(void) { return f_7_EBX[8]; }
static bool ERMS(void) { return f_7_EBX[9]; }
static bool INVPCID(void) { return f_7_EBX[10]; }
static bool RTM(void) { return isIntel && f_7_EBX[11]; }
static bool AVX512F(void) { return f_7_EBX[16]; }
static bool RDSEED(void) { return f_7_EBX[18]; }
static bool ADX(void) { return f_7_EBX[19]; }
static bool AVX512PF(void) { return f_7_EBX[26]; }
static bool AVX512ER(void) { return f_7_EBX[27]; }
static bool AVX512CD(void) { return f_7_EBX[28]; }
static bool SHA(void) { return f_7_EBX[29]; }

static bool PREFETCHWT1(void) { return f_7_ECX[0]; }

static bool LAHF(void) { return f_81_ECX[0]; }
static bool LZCNT(void) { return isIntel && f_81_ECX[5]; }
static bool ABM(void) { return isAMD && f_81_ECX[5]; }
static bool SSE4a(void) { return isAMD && f_81_ECX[6]; }
static bool XOP(void) { return isAMD && f_81_ECX[11]; }
static bool TBM(void) { return isAMD && f_81_ECX[21]; }

static bool SYSCALL(void) { return isIntel && f_81_EDX[11]; }
static bool MMXEXT(void) { return isAMD && f_81_EDX[22]; }
static bool RDTSCP(void) { return isIntel && f_81_EDX[27]; }
static bool _3DNOWEXT(void) { return isAMD && f_81_EDX[30]; }
static bool _3DNOW(void) { return isAMD && f_81_EDX[31]; }


TEST(CPIUD, RandomArgCall) {
  for (int a = 0; a < 5; a++){
    unsigned long t1, t2, t3, t4;
    cpuid(a, 0, t1, t2, t3, t4);
    std::cout << "The code " << t1 << " , " << t2 << ", " << t3 << ", " << t4 << std::endl;
  }
}
TEST(CPUID, Vendor){
  char xxx[sizeof(unsigned long) * 5] = { 0 };
  unsigned long t1[4] = {0};
  cpuid(0, 0, t1[0], t1[1], t1[2], t1[3]);
  std::array<int, 4> cpui;
  std::vector<std::array<int, 4>> data;
  int nIds_ = t1[0];
  for (int i = 0; i <= nIds_; ++i)
  {
    memset(t1, 0, sizeof(unsigned long) * 4);
    cpuid(i, 0, t1[0], t1[1], t1[2], t1[3]);
    cpui[0] = t1[0];
    cpui[1] = t1[1];
    cpui[2] = t1[2];
    cpui[3] = t1[3];
    data.push_back(cpui);
  }
  *reinterpret_cast<int*>(xxx) = data[0][1];
  *reinterpret_cast<int*>(xxx + 4) = data[0][3];
  *reinterpret_cast<int*>(xxx + 8) = data[0][2];
  if (!strcmp(xxx, "GenuineIntel")){
    isIntel = true;
  }
  else if (!strcmp(xxx,"AuthenticAMD")){
    isAMD = true;
  }
  if (nIds_ >= 1){
    f_1_ECX = data[1][2];
    f_1_EDX = data[1][3];
  }
  if (nIds_ >= 7){
    f_7_EBX = data[7][1];
    f_7_ECX = data[7][2];
  }
  std::cout << xxx << std::endl;
}
TEST(CPUID, Brand) {
  char xxx[0x40] = { 0 };
  std::vector<std::array<int, 4>> extdata;
  unsigned long t1[4] = { 0 };
  std::array<int, 4> cpui;
  cpuid(0x80000000, 0, t1[0], t1[1], t1[2], t1[3]);
  int nExIds_ = t1[0];
  for (int i = 0x80000000; i <= nExIds_; ++i)
  {
    memset(t1, 0, sizeof(unsigned long) * 4);
    cpuid(i, 0, t1[0], t1[1], t1[2], t1[3]);
    cpui[0] = t1[0];
    cpui[1] = t1[1];
    cpui[2] = t1[2];
    cpui[3] = t1[3];
    extdata.push_back(cpui);
  }
  if (nExIds_ >= 0x80000001)
  {
    f_81_ECX = extdata[1][2];
    f_81_EDX = extdata[1][3];
  }
  if (nExIds_ >= 0x80000004){
    memcpy(xxx, extdata[2].data(), sizeof(cpui));
    memcpy(xxx + 16, extdata[3].data(), sizeof(cpui));
    memcpy(xxx + 32, extdata[4].data(), sizeof(cpui));
  }
  std::cout << xxx << std::endl;
}
TEST(CPUID, InstructionSet) {
  auto& outstream = std::cout;

  auto support_message = [&outstream](std::string isa_feature, bool is_supported) {
    outstream << isa_feature << (is_supported ? " supported" : " not supported") << std::endl;
  };
  support_message("3DNOW", _3DNOW());
  support_message("3DNOWEXT", _3DNOWEXT());
  support_message("ABM", ABM());
  support_message("ADX", ADX());
  support_message("AES", AES());
  support_message("AVX", AVX());
  support_message("AVX2", AVX2());
  support_message("AVX512CD", AVX512CD());
  support_message("AVX512ER", AVX512ER());
  support_message("AVX512F", AVX512F());
  support_message("AVX512PF", AVX512PF());
  support_message("BMI1", BMI1());
  support_message("BMI2", BMI2());
  support_message("CLFSH", CLFSH());
  support_message("CMPXCHG16B", CMPXCHG16B());
  support_message("CX8", CX8());
  support_message("ERMS", ERMS());
  support_message("F16C", F16C());
  support_message("FMA", FMA());
  support_message("FSGSBASE", FSGSBASE());
  support_message("FXSR", FXSR());
  support_message("HLE", HLE());
  support_message("INVPCID", INVPCID());
  support_message("LAHF", LAHF());
  support_message("LZCNT", LZCNT());
  support_message("MMX", MMX());
  support_message("MMXEXT", MMXEXT());
  support_message("MONITOR", MONITOR());
  support_message("MOVBE", MOVBE());
  support_message("MSR", MSR());
  support_message("OSXSAVE", OSXSAVE());
  support_message("PCLMULQDQ", PCLMULQDQ());
  support_message("POPCNT", POPCNT());
  support_message("PREFETCHWT1", PREFETCHWT1());
  support_message("RDRAND", RDRAND());
  support_message("RDSEED", RDSEED());
  support_message("RDTSCP", RDTSCP());
  support_message("RTM", RTM());
  support_message("SEP", SEP());
  support_message("SHA", SHA());
  support_message("SSE", SSE());
  support_message("SSE2", SSE2());
  support_message("SSE3", SSE3());
  support_message("SSE4.1", SSE41());
  support_message("SSE4.2", SSE42());
  support_message("SSE4a", SSE4a());
  support_message("SSSE3", SSSE3());
  support_message("SYSCALL", SYSCALL());
  support_message("TBM", TBM());
  support_message("XOP", XOP());
  support_message("XSAVE", XSAVE());
}

int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  getchar();
  return 0;
}

