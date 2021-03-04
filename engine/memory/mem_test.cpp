#include "utils/general.hpp"
#include "memory/block_heap.hpp"
#include "memory/block_heap.cpp"

int main()
{
  //nne::Logger::Init(Logger::LevelDebug, true, true);

  size_t mem_length = 2048;
  BlockHeapMemory<float> mem(mem_length);

  for (size_t i=0; i<mem.Length(); i++)
    mem[i] = i*2.0f;

  mem.Reshape(11);
  mem.Reshape(7);
  mem.Reshape(1);
  mem.Reshape(128);
  mem.Reshape(3);

  mem.Resize(mem_length+1);
  mem[mem_length] = mem_length*2.0f;
  mem.Resize(mem_length-1024);
  mem.Resize(mem_length);

  for (size_t i=0; i<mem.Length(); i++)
  {
    NNE_PRINTL(mem[i]);
    assert(mem[i] == i*2.0f);
  }

  return 0;
}
