/*
 *  Copyright 2008-2009 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <thrust/random/xor_combine_engine.h>

namespace thrust
{

namespace experimental
{

namespace random
{

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  xor_combine_engine<Engine1,s1,Engine2,s2>
    ::xor_combine_engine(void)
      :m_b1(),m_b2()
{
} // end xor_combine_engine::xor_combine_engine()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  xor_combine_engine<Engine1,s1,Engine2,s2>
    ::xor_combine_engine(const base1_type &urng1, const base2_type &urng2)
      :m_b1(urng1),m_b2(urng2)
{
} // end xor_combine_engine::xor_combine_engine()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  xor_combine_engine<Engine1,s1,Engine2,s2>
    ::xor_combine_engine(result_type s)
      :m_b1(s),m_b2(s)
{
} // end xor_combine_engine::xor_combine_engine()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  void xor_combine_engine<Engine1,s1,Engine2,s2>
    ::seed(void)
{
  m_b1.seed();
  m_b2.seed();
} // end xor_combine_engine::seed()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  void xor_combine_engine<Engine1,s1,Engine2,s2>
    ::seed(result_type s)
{
  m_b1.seed(s);
  m_b2.seed(s);
} // end xor_combine_engine::seed()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  const typename xor_combine_engine<Engine1,s1,Engine2,s2>::base1_type &
    xor_combine_engine<Engine1,s1,Engine2,s2>
      ::base1(void) const
{
  return m_b1;
} // end xor_combine_engine::base1()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  const typename xor_combine_engine<Engine1,s1,Engine2,s2>::base2_type &
    xor_combine_engine<Engine1,s1,Engine2,s2>
      ::base2(void) const
{
  return m_b2;
} // end xor_combine_engine::base2()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  typename xor_combine_engine<Engine1,s1,Engine2,s2>::result_type
    xor_combine_engine<Engine1,s1,Engine2,s2>
      ::operator()(void)
{
  return (result_type(m_b1() - base1_type::min) << shift1) ^
         (result_type(m_b2() - base2_type::min) << shift2);
} // end xor_combine_engine::operator()()

template<typename Engine1, size_t s1,
         typename Engine2, size_t s2>
  void xor_combine_engine<Engine1, s1, Engine2, s2>
    ::discard(unsigned long long z)
{
  for(; z > 0; --z)
  {
    this->operator()();
  } // end for
} // end xor_combine_engine::discard()


template<typename Engine1_, size_t s1_, typename Engine2_, size_t s2_,
         typename CharT, typename Traits>
std::basic_ostream<CharT,Traits>&
operator<<(std::basic_ostream<CharT,Traits> &os,
           const xor_combine_engine<Engine1_,s1_,Engine2_,s2_> &e)
{
  typedef std::basic_ostream<CharT,Traits> ostream_type;
  typedef typename ostream_type::ios_base  ios_base;

  // save old flags and fill character
  const typename ios_base::fmtflags flags = os.flags();
  const CharT fill = os.fill();

  const CharT space = os.widen(' ');
  os.flags(ios_base::dec | ios_base::fixed | ios_base::left);
  os.fill(space);

  // output each base engine in turn
  os << e.base1() << space << e.base2();

  // restore old flags and fill character
  os.flags(flags);
  os.fill(fill);
  return os;
}


template<typename Engine1_, size_t s1_, typename Engine2_, size_t s2_,
         typename CharT, typename Traits>
std::basic_istream<CharT,Traits>&
operator>>(std::basic_istream<CharT,Traits> &is,
           xor_combine_engine<Engine1_,s1_,Engine2_,s2_> &e)
{
  typedef std::basic_istream<CharT,Traits> istream_type;
  typedef typename istream_type::ios_base  ios_base;

  // save old flags
  const typename ios_base::fmtflags flags = is.flags();

  is.flags(ios_base::skipws);

  // input each base engine in turn
  is >> e.m_b1 >> e.m_b2;

  // restore old flags
  is.flags(flags);
  return is;
}


template<typename Engine1, size_t s1, typename Engine2, size_t s2>
bool operator==(const xor_combine_engine<Engine1,s1,Engine2,s2> &lhs,
                const xor_combine_engine<Engine1,s1,Engine2,s2> &rhs)
{
  return (lhs.m_b1 == rhs.m_b1) && (lhs.m_b2 == rhs.m_b2);
}


template<typename Engine1, size_t s1, typename Engine2, size_t s2>
bool operator!=(const xor_combine_engine<Engine1,s1,Engine2,s2> &lhs,
                const xor_combine_engine<Engine1,s1,Engine2,s2> &rhs)
{
  return !(lhs == rhs);
}


} // end random

} // end experimental

} // end thrust

