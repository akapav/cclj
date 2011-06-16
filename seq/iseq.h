#ifndef _ISEQ_
#define _ISEQ_

#include <memory>

template <typename T>
struct ISeq {
  typedef T element_type;
  typedef std::shared_ptr<ISeq<T> > ref;

  virtual bool nilp() const { return false; }

  virtual T first() const = 0; 
  virtual ref rest() const = 0;
  virtual ref conj(T) const = 0;
  virtual ~ISeq() {}
};

#endif //_ISEQ_
