/*
  Provides a non-STL alternative to the STL vector<...>
  used inside FTvectorizer and FTGlyphContainer.
  
  Implementation:
    - Dynamically resizable container.
    - Try to mimic the calls made to the STL vector API.

  Caveats:
    - No templates, use poor macro substition where :
      FT_VECTOR_CLASS_NAME: is the name of the class
      FT_VECTOR_ITEM_TYPE: is the type of the object to store
*/

#define FT_VECTOR_CLASS_DEBUG 0

#include "FTGL.h"

class FTGL_EXPORT FT_VECTOR_CLASS_NAME
{
public:
  
  typedef FT_VECTOR_ITEM_TYPE value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef size_t size_type;

  FT_VECTOR_CLASS_NAME();

  virtual ~FT_VECTOR_CLASS_NAME();

  FT_VECTOR_CLASS_NAME& operator =(const FT_VECTOR_CLASS_NAME& v);
  
  size_type size() const;
  size_type capacity() const;
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  bool empty() const;

  reference operator [](size_type pos);
  const_reference operator [](size_type pos) const;

  void clear();
  void reserve(size_type);
  void push_back(const value_type&);
  void resize(size_type, value_type);

protected:
  
  void expand(size_type = 0);

private:

  size_type Size;
  size_type NumberOfItems;
  value_type* Items;
};


inline 
FT_VECTOR_CLASS_NAME::FT_VECTOR_CLASS_NAME()
{
  this->Size = this->NumberOfItems = 0;
  this->Items = 0;
}


inline 
FT_VECTOR_CLASS_NAME::~FT_VECTOR_CLASS_NAME()
{
  this->clear();
}


inline 
FT_VECTOR_CLASS_NAME& FT_VECTOR_CLASS_NAME::operator =(const FT_VECTOR_CLASS_NAME& v)
{
  // Warning: the vector is not cleared and resized to v capacity for
  // efficiency reasons.
  // this->clear();
  this->reserve(v.capacity());
  
  iterator ptr = this->begin();
  const_iterator vbegin = v.begin();
  const_iterator vend = v.end();

  while (vbegin != vend)
    {
    *ptr++ = *vbegin++;
    }
  this->NumberOfItems = v.size();
  return *this;
}


inline 
FT_VECTOR_CLASS_NAME::size_type FT_VECTOR_CLASS_NAME::size() const 
{ 
  return this->NumberOfItems; 
}


inline 
FT_VECTOR_CLASS_NAME::size_type FT_VECTOR_CLASS_NAME::capacity() const 
{ 
  return this->Size; 
}


inline 
FT_VECTOR_CLASS_NAME::iterator FT_VECTOR_CLASS_NAME::begin() 
{ 
  return this->Items; 
}


inline 
FT_VECTOR_CLASS_NAME::iterator FT_VECTOR_CLASS_NAME::end() 
{ 
  return this->begin() + this->size(); 
}


inline 
FT_VECTOR_CLASS_NAME::const_iterator FT_VECTOR_CLASS_NAME::begin() const 
{ 
  return this->Items; 
}


inline 
FT_VECTOR_CLASS_NAME::const_iterator FT_VECTOR_CLASS_NAME::end() const 
{ 
  return this->begin() + this->size(); 
}


inline 
bool FT_VECTOR_CLASS_NAME::empty() const 
{ 
  return this->size() == 0; 
}


inline 
FT_VECTOR_CLASS_NAME::reference FT_VECTOR_CLASS_NAME::operator [](FT_VECTOR_CLASS_NAME::size_type pos) 
{ 
  return (*(begin() + pos)); 
}


inline 
FT_VECTOR_CLASS_NAME::const_reference FT_VECTOR_CLASS_NAME::operator [](FT_VECTOR_CLASS_NAME::size_type pos) const 
{ 
  return (*(begin() + pos)); 
}


inline 
void FT_VECTOR_CLASS_NAME::clear()
{
  if (this->Size)
    {
#if FT_VECTOR_CLASS_DEBUG    
    printf("FT_VECTOR_CLASS_NAME: clear() (%d / %d)\n", 
           this->size(), this->capacity());
#endif
    delete [] this->Items;
    this->Size = this->NumberOfItems = 0;
    this->Items = 0;
    }
}


inline 
void FT_VECTOR_CLASS_NAME::expand(size_type size_hint)
{
#if FT_VECTOR_CLASS_DEBUG    
  printf("FT_VECTOR_CLASS_NAME: expand() (%d / %d) hint: %d\n", 
         this->size(), this->capacity(), size_hint);
#endif

  // Allocate new vector (size doubles)

  size_type new_size = (this->size() == 0) ? 256 : this->size() * 2;
  if (size_hint)
    {
    while (new_size < size_hint)
      {
      new_size *= 2;
      }
    }
  
  value_type *ptr = new value_type[new_size];

  // Copy values to new vector

  iterator begin = this->begin();
  iterator end = this->end();
  while (begin != end)
    {
    *ptr++ = *begin++;
    }

  // Deallocate old vector and use new vector

  delete [] this->Items;
  this->Items = ptr;
  this->Size = new_size;
}


inline 
void FT_VECTOR_CLASS_NAME::reserve(size_type n)
{
#if FT_VECTOR_CLASS_DEBUG    
  printf("FT_VECTOR_CLASS_NAME: reserve() (%d / %d) n: %d\n", 
         this->size(), this->capacity(), n);
#endif
  if (this->capacity() < n)
    {
    this->expand(n);
    }
}


inline 
void FT_VECTOR_CLASS_NAME::push_back(const value_type& x)
{
#if FT_VECTOR_CLASS_DEBUG    
  printf("FT_VECTOR_CLASS_NAME: push_back() (%d / %d)\n", 
         this->size(), this->capacity());
#endif
  if (this->size() == this->capacity())
    {
    this->expand();
    }
  (*this)[this->size()] = x;
  this->NumberOfItems++;
}


inline 
void FT_VECTOR_CLASS_NAME::resize(size_type n, value_type x)
{
#if FT_VECTOR_CLASS_DEBUG    
  printf("FT_VECTOR_CLASS_NAME: resize() (%d / %d) n: %d\n", 
         this->size(), this->capacity(), n);
#endif
  this->reserve(n);
  iterator end = this->end();
  iterator end_capacity = this->begin() + this->capacity();
  while (end != end_capacity)
    {
    *end++ = x;
    }
}
