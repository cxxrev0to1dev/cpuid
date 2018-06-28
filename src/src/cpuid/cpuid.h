#ifndef CPUID_H_
#define CPUID_H_

void cpuid(unsigned long func, unsigned long arg,
  unsigned long &a, unsigned long &b,
  unsigned long &c, unsigned long &d);


#endif