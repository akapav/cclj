#ifndef _SEQLIB_
#define _SEQLIB_

#include "cons.h"
#include "lazy.h"

template <typename Fn, typename Seq>
auto xmap(Fn fn, Seq seq) -> typename ISeq<decltype(fn(seq->first()))>::ref {
  typedef decltype(fn(seq->first())) T;
  if(!seq) return nil<T>();
  return lazy_seq(cons(fn(seq->first()), xmap(fn, seq->rest())));
}

template <typename Fn, typename T>
typename ISeq<T>::ref iterate(Fn fn, T val)
{ return lazy_seq(cons(val, iterate(fn, fn(val)))); }

template <typename Seq>
Seq take(int n, Seq seq) {
  typedef decltype(seq->first()) T;
  if(!n) return nil<T>();
  return lazy_seq(cons(seq->first(), take(n - 1, seq->rest())));
}

template <typename Seq, typename Fn>
auto doseq(Seq seq, Fn fn) -> typename ISeq<decltype(seq->first())>::ref {
  typedef decltype(seq->first()) T;
  if(!seq) return nil<T>();
  fn(seq->first());
  return doseq(seq->rest(), fn);
}

template <typename Seq>
Seq concat(Seq ls, Seq rs) {
  if(!ls) return rs;
  return lazy_seq(cons(ls->first(), concat(ls->rest(), rs)));
}

template <typename Fn, typename Seq1, typename Seq2>
auto zip(Fn fn, Seq1 ls, Seq2 rs)
  -> typename ISeq<decltype(fn(ls->first(), rs->first()))>::ref {
  typedef decltype(fn(ls->first(), rs->first())) T;
  if(!ls || !rs) return nil<T>();
  return lazy_seq(cons(fn(ls->first(), rs->first()), zip(fn, ls->rest(), rs->rest())));
}

template <typename Fn>
auto repeatedly(Fn fn) -> typename ISeq<decltype(fn())>::ref
{ return lazy_seq(cons(fn(), repeatedly(fn))); }

template <typename T>
typename ISeq<T>::ref repeat(T val)
{ return repeatedly([=]() { return val; }); }

template <typename Fn, typename Seq>
Seq filter(Fn pred, Seq seq) {
  typedef decltype(seq->first()) T;
  if(!seq) return nil<T>();
  if(pred(seq->first()))
    return lazy_seq(cons(seq->first(), filter(pred, seq->rest())));
  return filter(pred, seq->rest());
}

template <typename Fn, typename Seq>
Seq remove(Fn pred, Seq seq) {
  typedef decltype(seq->first()) T;
  return filter([=](T val) { return !pred(val); }, seq);
}

template <typename Seq>
static Seq cycle_(Seq seq, Seq orig) {
  auto s = seq ? seq : orig;
  return lazy_seq(cons(s->first(), cycle_(s->rest(), orig)));
}

template <typename Seq>
Seq cycle(Seq seq)
{ return cycle_(seq, seq); }

template <typename Fn, typename T, typename Seq>
T reduce(Fn fn, T acc, Seq seq) {
  if(!seq) return acc;
  return reduce(fn, fn(acc, seq->first()), seq->rest());
}

//todo: needs complete redesign
//template <typename T, typename It>
//typename ISeq<T>::ref iter_seq(It curr, It end) {
//  if(curr != end)
//    return lazy_seq([&]() { return cons(*curr, iter_seq<T, It>(++curr, end)); });
//  return nil<T>();
//}

#endif //_SEQLIB_
