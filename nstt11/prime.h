#ifndef PRIME_H
#define PRIME_H

#include <type_traits>

template <unsigned N>
struct nth_prime;

template <unsigned N>
static const unsigned nth_prime_v = nth_prime<N>::value;

template <>
struct nth_prime<0>;

template <>
struct nth_prime<1> : std::integral_constant<unsigned, 2> {};

namespace internal {

template <unsigned N, unsigned M>
struct coprime : std::integral_constant<bool, (N % M) && (M % N)> {};

template <unsigned N, unsigned M>
static const bool coprime_v = coprime<N, M>::value;

template <
    unsigned N,
    unsigned M = nth_prime<N - 1>::value + 1,
    unsigned I = N - 1>
struct find_nth_prime : std::conditional_t<
                            coprime_v<M, nth_prime_v<N - I>>,
                            find_nth_prime<N, M, I - 1>,
                            find_nth_prime<N, M + 1>>::type {};

template <unsigned N, unsigned M>
struct find_nth_prime<N, M, 0> : std::integral_constant<unsigned, M> {};

};  // namespace internal

template <unsigned N>
struct nth_prime : internal::find_nth_prime<N>::type {};

#endif
