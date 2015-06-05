//
// COBSPacket.cpp 
// Class library C++ code
// ----------------------------------
// Developed with embedXcode+ 
// http://embedXcode.weebly.com
//
// Project 		SerialPacketReceiver
//
// Created by 	Andy, 6/4/15 7:07 AM
// 				StuffAndyMakes.com
//
// Copyright 	(c) Andy, 2015
// Licence		<#license#>
//
// See 			COBSPacket.h and ReadMe.txt for references
//


// Library header
#include "COBSPacket.h"

// Code
COBSPacket::COBSPacket() {
    
}

void COBSPacket::begin() {
    _data = 0;
}

String COBSPacket::WhoAmI() {
    return "COBSPacket";
}

void COBSPacket::set(uint8_t data) {
    _data = data;
}

uint8_t COBSPacket::get() {
    return _data; 
}

