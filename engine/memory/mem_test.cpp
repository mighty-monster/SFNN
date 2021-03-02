#include <iostream>
#include <assert.h>

#include "nne.hpp"
#include "utils/memonitor.hpp"

#include "memory/simple_heap.hpp"
#include "memory/simple_heap.cpp"

#include "memory/block_heap.hpp"
#include "memory/block_heap.cpp"

int object_counter;

class Vector
{
public:
  Vector()
  {
    std::cout<<"Vector Empty Constructor Called\n";
    std::cout<<"This address: " <<this <<std::endl;
    object_counter++;
  }

  Vector(const float& p_x,const float& p_y,const float& p_z) : x(p_x), y(p_y), z(p_z)
  {
    std::cout<<"Vector Full Constructor Called\n";
    std::cout<<"This address: " <<this <<std::endl;
    object_counter++;
  }

  Vector(const Vector& p1): x(p1.x), y(p1.y), z(p1.z)
  {
    std::cout<<"Vector Copy Constructor Called\n";
    std::cout<<"This address: " <<this <<std::endl;
    object_counter++;
  }

  Vector(const Vector&& p1) noexcept: x(std::move(p1.x)), y(std::move(p1.y)), z(std::move(p1.z))
  {
    std::cout<<"Vector Move Constructor Called\n";
    std::cout<<"This address: " <<this <<std::endl;
    object_counter++;
  }

  ~Vector()
  {
    std::cout<<"Vector Destructor Called\n";
    std::cout<<"This address: " <<this <<std::endl;
    object_counter++;
  }


  Vector& operator = (const Vector& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    std::cout <<"Vector Assignment Operator Called"<<std::endl;
    std::cout<<"This address: " <<this <<std::endl;
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
  VectorPrime(const float& p_x,const float& p_y,const float& p_z) : Vector(p_x, p_y, p_z)
  {
    std::cout<<"VectorPrime Full Constructor Called\n";
  }
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


  }

  DISABLE_PRINT_ALLOC
  //PRINT_ALLOC_SUMMERY

  return 0;
}
