#include <iostream>

#include "cons.h"
#include "nil.h"
#include "lazy.h"
#include "seqlib.h"
#include "ndxseq.h"

using namespace std;

template <typename Seq>
void dump(Seq seq) {
  typedef decltype(seq->first()) T;
  doseq(seq, [](T el){ cout << el << endl;});
}

template <typename T>
T xplus(T ls, T rs) { return ls + rs; }

template <typename T>
T xdup(T val) { return val + val; }

bool oddp(int n) { return n % 2; }

int main() {
  auto N0 = iterate([](int n){return n + 1;}, 0);
  auto foo = cons(3, cons(2, cons_nil(1)));
  foo = foo->conj(4);
  auto lz = lazy_seq_fn([&foo](){ cout << "lazy\n"; return cons(4, foo); });
  lz = lazy_seq(cons(5, lz));

  dump(xmap(xdup<int>, concat(take(5, N0), lz))->conj(100));
  dump(zip(xplus<int>, foo, N0));

  dump(take(4, repeatedly([](){ return "hžv"; })));
  dump(take(4, repeat("hžv")));

  dump(take(5, filter(oddp, N0)));
  dump(take(5, remove(oddp, N0)));

  dump(take(7, cycle(foo)));

  cout << reduce(xplus<int>, 0, take(1000, N0)) << endl;

  auto strc = cons_nil("foo");

  auto ndxs = DummyArr<int>::make();
  for(auto i = 0; i < 3; ndxs = ndxs->push(i++));
  auto ndxs2 = ndxs->set_nth(0, 100);
  for(auto i = 0; i < ndxs->size(); ++i)
    cout << ndxs->nth(i) << ' ' << ndxs2->nth(i) << endl;

  auto sq3 = ndxs->seq();
  dump(cons(5, sq3->conj(4)));

  //warning: memory leaks!
  //ISeq<int>::ref z = lazy_seq([&z]() { return cons(0, z); });
  //dump(take(5, z));

  //ISeq<int>::ref fibo =
  //cons(0, cons(1, lazy_seq([&]() { return zip(plus<int>, fibo, fibo->rest()); })));
  //dump(take(10, fibo));

  cout << "some\n";
  auto foo1 = cons(3, cons(2, cons_nil(1)));
  auto foo2 = cons(6, cons(2, cons_nil(0)));
  auto foo3 = nil<int>();
  cout << somep(oddp, foo1) << endl;
  cout << somep(oddp, foo2) << endl;
  cout << somep(oddp, foo3) << endl;
  cout << somep(oddp, N0) << endl;

  cout << "range\n";
  cout << everyp(oddp, (range(1, 10, 2))) << endl;

  cout << "into\n";
  dump(into(foo1, foo2));
  cout << "into2\n";
  dump(into(sq3, foo2));
  dump(into(nil<int>(), foo2));

  return 0;
}
