#pragma once

#include "block.hpp"
#include "block.cpp"

namespace nne {
  template <typename T>
  class BlockHeapMemory : public BlockMemory<nullptr, nullptr>
  {
  public:
    BlockHeapMemory();
    BlockHeapMemory(const char* p_file_path);
    BlockHeapMemory(const char* p_file_path, const uint16_t& p_no_of_blocks);
    BlockHeapMemory(const size_t& p_length);
    BlockHeapMemory(const size_t& p_length, const uint16_t& p_no_of_blocks);
    ~BlockHeapMemory();

    void Allocate(const size_t& p_length, const uint16_t& p_no_of_blocks);
    void Deallocate();

    virtual void LoadFromFile(const char* p_file_path) override;
    virtual void LoadFromFile(const char* p_file_path, const uint16_t& p_no_of_blocks) override;

    virtual void LoadFromHexFile(const char* p_file_path) override;
    virtual void LoadFromHexFile(const char* p_file_path, const uint16_t& p_no_of_blocks) override;

    void Resize(const size_t& p_length) override;
    void Reshape(const uint16_t& p_no_of_blocks) override;

    T& operator [] (const size_t& p_index);
    const T& operator [] (const size_t& p_index) const;

    inline size_t Length();

  private:
    size_t m_length = 0;
  };

}
