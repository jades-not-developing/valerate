#pragma once

#include <cstddef>
#include <cstdlib>

class ArenaAllocator {
public:

  ArenaAllocator(size_t bytes) 
    : m_Size(bytes)
  {
    m_Buffer = static_cast<std::byte*>(malloc(m_Size));
    m_Offset = m_Buffer;
  }


  ~ArenaAllocator()
  {
    free(m_Buffer);
  }


  template<typename T>
  T* alloc()
  {
    void* offset = m_Offset;
    m_Offset += sizeof(T);

    memset(offset, 0, sizeof(T));

    return static_cast<T*>(offset);
  }

  inline ArenaAllocator(const ArenaAllocator& other) = delete;
  inline ArenaAllocator operator=(const ArenaAllocator& other) = delete;

private:
  size_t m_Size; 
  std::byte* m_Buffer;
  std::byte* m_Offset;
};
