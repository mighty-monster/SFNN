///////////////////////////////////////////////////////////////////////////////
// @File Name:     general.hpp                                               //
// @Author:        Arash Fatehi                                              //
// @L.M.D:         19th Feb 2021                                             //
// @Description:   Contains general useful fucntions                         //
//                                                                           //
// Detail Description:                                                       //
//                                                                           //
// List of functions: BufferToHex, HexToBuffer                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cinttypes>
#include <string>

namespace nnc {
  // Dumps a block of memory as Hex string
  std::string BufferToHex(void* buffer, size_t size);

  // Loads an hex string into a block of memory
  // The buffer should have enough allocated memory
  // Length of hex string determines size of buffer
  void HexToBuffer(void* buffer, const std::string& hex);
}

