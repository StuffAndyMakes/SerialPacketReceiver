#include "Arduino.h"

#ifndef COBSPacket_h
#define COBSPacket_h


///
/// @class	<#Description#>
///
class COBSPacket {
  
public:
  ///
  /// @brief	Cosntructor
  ///
  COBSPacket();
  
  ///
  /// @brief	Initialisation
  ///
  void begin();
  
  ///
  ///	@brief Who am I?
  ///	@return Who am I? string
  ///
  String WhoAmI();
  
  ///
  ///	@brief	<#Description#>
  ///	@param	data <#data description#>
  ///
  void set(uint8_t data);
  
  ///
  ///	@brief	<#Description#>
  ///	@return	<#return value description#>
  ///
  uint8_t get();
  
private:
  uint8_t _data;
};

#endif
