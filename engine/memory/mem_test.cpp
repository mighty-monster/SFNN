#include <iostream>
#include <assert.h>

#include "nne.hpp"
#include "utils/memon.hpp"
#include "utils/nnexpect.hpp"

#include "memory/simple_heap.hpp"
#include "memory/simple_heap.cpp"

#include "memory/block_heap.hpp"
#include "memory/block_heap.cpp"

#include "utils/general.cpp"

int object_counter;

class Vector
{
public:
  Vector()
  {
    std::cout<<"Vector Empty Constructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
    object_counter++;
  }

  Vector(const float& p_x,const float& p_y,const float& p_z) : x(p_x), y(p_y), z(p_z)
  {
    std::cout<<"Vector Full Constructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
    object_counter++;
  }

  Vector(const Vector& p1): x(p1.x), y(p1.y), z(p1.z)
  {
    std::cout<<"Vector Copy Constructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
    object_counter++;
  }

  Vector(const Vector&& p1) noexcept: x(std::move(p1.x)), y(std::move(p1.y)), z(std::move(p1.z))
  {
    std::cout<<"Vector Move Constructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
    object_counter++;
  }

  virtual ~Vector()
  {
    std::cout<<"Vector Destructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
    object_counter++;
  }


  Vector& operator = (const Vector& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    std::cout <<"Vector Assignment Operator Called"<<std::endl;
    std::cout<<"This address: \n" <<this <<std::endl;
    return *this;
  }

  void init(float p_x, float p_y, float p_z)
  {
    x = p_x;
    y = p_y;
    z = p_z;
  };
  float x;
  float y;
  float z;
};

class VectorPrime : public Vector
{
public:
  VectorPrime()
  {
    std::cout<<"VectorPrime Empty Constructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
  }

  VectorPrime(const float& p_x,const float& p_y,const float& p_z) : Vector(p_x, p_y, p_z)
  {
    std::cout<<"VectorPrime Full Constructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;

  }

   ~VectorPrime()
  {
    std::cout<<"VectorPrime Destructor Called\n";
    std::cout<<"This address: \n" <<this <<std::endl;
  }
public:
  char messafg[100];

};

Vector func(Vector vp)
{
  vp.x = 5;
  return vp;
};

int main()
{
  nne::Logger::Init(nne::Logger::LevelDebug, true, true);
  ENABLE_PRINT_ALLOC
  {
    char buffer[6];

    int res = nne::sprintf_nne(buffer, 6, "sdfsdfsdsdfdsfsf ");

    if (res <= 0)
    {
      NNE_ERORR_LL("Error in nne::sprintf_nne, reading errno!");
      const size_t reason_length = 256;
      char reason[reason_length];
      NNE_ERRNO_PRINT_REASON(reason, reason_length, errno);

    }
    else
    {
      NNE_PRINTL(buffer);
    }
  }

  DISABLE_PRINT_ALLOC
  PRINT_ALLOC_SUMMERY

  return 0;
}
