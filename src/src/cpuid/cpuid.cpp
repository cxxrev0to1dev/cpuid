// cpuid.cpp : Defines the entry point for the console application.
//

#include <cstdio>

#if defined(OS_WIN) && defined(OS_WIN_X64)
extern "C" void cpuid64(unsigned long func, unsigned long arg, unsigned long *words);
#endif

void cpuid(unsigned long func, unsigned long arg,
  unsigned long &a, unsigned long &b,
  unsigned long &c, unsigned long &d)
{
#ifdef OS_WIN
#ifdef OS_WIN_X64
  unsigned long words[4];
  cpuid64(func, arg, words);

  a = words[0];
  b = words[1];
  c = words[2];
  d = words[3];
#else
  // I don't know how to copy back to arguments passed by reference, that's
  // why I use temporary variables first.
  unsigned long t1, t2, t3, t4;

  __asm
  {
    mov eax, func
    mov ecx, arg
    cpuid
    mov dword ptr t1, eax
    mov dword ptr t2, ebx
    mov dword ptr t3, ecx
    mov dword ptr t4, edx
  };

  a = t1;
  b = t2;
  c = t3;
  d = t4;
#endif
#else
  // 32-bit PIC compatible version.
#if defined(__i386__) && defined(__PIC__)
  asm("pushl %%ebx\n"
    "cpuid\n"
    "movl %%ebx,%1\n"
    "popl %%ebx\n"
    :"=a"(a),
    "=r"(b),
    "=c"(c),
    "=d"(d)
    : "a"(func),
    "c"(arg));
#else
  asm("cpuid"
    :"=a"(a),
    "=b"(b),
    "=c"(c),
    "=d"(d)
    : "a"(func),
    "c"(arg));
#endif
#endif
}
