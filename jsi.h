// Pristine mirror of the JSI interface design as it exists today in
// API/jsi/jsi/jsi.h: I-prefixed interfaces have a `protected:` non-virtual
// destructor. This is intentional — interfaces are not destructible by
// their consumers; only the concrete object that produced them owns its
// lifetime.
//
// No Swift attributes, no #ifdefs, no macros. The Swift fix lives
// entirely in JSI.apinotes.

#pragma once

struct UUID {
  unsigned long long hi;
  unsigned long long lo;
};

struct ICast {
  virtual ICast* castInterface(const UUID& interfaceUUID) = 0;
 protected:
  ~ICast() = default;
};

class IRuntime : public ICast {
 public:
  virtual int evaluate(const char* source) = 0;
 protected:
  virtual ~IRuntime() = default;
};

struct ISerialization {
  virtual int doSomething() = 0;
 protected:
  ~ISerialization() = default;
};

int runScript(IRuntime* rt, const char* source);
