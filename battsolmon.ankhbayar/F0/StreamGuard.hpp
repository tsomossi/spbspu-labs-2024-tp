#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <iostream>

class StreamGuard
{
public:
  StreamGuard(std::basic_ios<char>& stream) :
    stream(stream),
    precision(stream.precision()),
    flags(stream.flags())
  {}
  ~StreamGuard()
  {
    stream.precision(precision);
    stream.flags(flags);
  }

private:
  std::basic_ios<char>& stream;
  std::streamsize precision;
  std::basic_ios<char>::fmtflags flags;
};

#endif
