//
//  ReceiverApplication.cpp
//  Error-Detecting Serial Packet Communications for Arduino Microcontrollers
//  Originally designed for use in the Office Chairiot Mark II motorized office chair
//
//  Created by Andy Frey on 4/13/15.
//  Copyright (c) 2015 Andy Frey. All rights reserved.
//


#include "ReceiverApplication.h"
#include "SerialPacket.h"


#define LED_SEND 13
#define LED_GOOD 12
#define LED_BAD 11
#define LED_DELAY() delayMicroseconds(100)


ReceiverApplication::ReceiverApplication() {}

void ReceiverApplication::_dumpStats(HardwareSerial *s) {
    uint32_t total = _stats.recv_good+_stats.send_bad+_stats.send_good+_stats.send_bad;
    uint32_t elapsed_secs = (millis() - _stats.start_time) / 1000;
    s->print("Send> Total:" + String(total, DEC));
    s->print(" Secs:" + String(elapsed_secs, DEC));
    s->print(" PPS:" + String(total/elapsed_secs, DEC));
    s->print(" RGood:" + String(_stats.recv_good, DEC));
    s->print(" RBad:" + String(_stats.recv_bad, DEC));
    s->print(" SGood:" + String(_stats.send_good, DEC));
    s->print(" SBad:" + String(_stats.send_bad, DEC));
    s->print(" SOoS:" + String(_stats.oos, DEC));
    s->println(" Acks:" + String(_stats.acks, DEC));
}

/*
 *  Packet Delegate Method: Called when a valid packet is received
 */
void ReceiverApplication::didReceiveGoodPacket(SerialPacket *p) {

    Serial.println("Good packet!");
    // copy bytes for structure from packet buffer into structre memory
    Command _receivedCommand;
    memcpy(&_receivedCommand, p->buffer, p->getDataLength());

    digitalWrite(LED_GOOD, HIGH);
    LED_DELAY();
    digitalWrite(LED_GOOD, LOW);
    
    if (_receivedCommand.serial != _expectedSerial) {
        _stats.oos++;
        _expectedSerial = _receivedCommand.serial;
    } else {
        _stats.recv_good++;
    }

    // if ack is STATUS_NACK, sender is expecting us to change it to STATUS_ACK and return packet
    if (_receivedCommand.ack == STATUS_NACK) {
        digitalWrite(LED_SEND, HIGH);
        _stats.acks++;
        // sender wants an acknowledgment
        _receivedCommand.ack = STATUS_ACK;
        uint8_t bytesSent = p->send((uint8_t *)&_receivedCommand, sizeof(_receivedCommand));
        if (bytesSent > 0) {
            digitalWrite(LED_GOOD, HIGH);
            _stats.send_good++;
            LED_DELAY();
            digitalWrite(LED_GOOD, LOW);
        } else {
            digitalWrite(LED_BAD, HIGH);
            _stats.send_bad++;
            LED_DELAY();
            digitalWrite(LED_BAD, LOW);
        }
        digitalWrite(LED_SEND, LOW);
    }

    _expectedSerial++;

    if ((_stats.recv_good > 0) && (_stats.recv_good % 1000 == 0)) {
        _dumpStats(&Serial);
    }

}

/*
 *  Packet Delegate Method: Called when an error is encountered
 */
void ReceiverApplication::didReceiveBadPacket(SerialPacket *p, uint8_t err) {
    Serial.print("Bad packet! ");
    switch (err) {
        case SerialPacket::ERROR_CRC: Serial.println("CRC"); break;
        case SerialPacket::ERROR_LENGTH: Serial.println("LENGTH"); break;
        case SerialPacket::ERROR_OVERFLOW: Serial.println("OVERFLOW"); break;
        case SerialPacket::ERROR_TIMEOUT: Serial.println("TIMEOUT"); break;
        default: Serial.println("???"); break;
    }
    _stats.recv_bad++;
    _expectedSerial++;
    digitalWrite(LED_BAD, HIGH);
    LED_DELAY(); // keep red on a bit
    digitalWrite(LED_BAD, LOW);

    if ((_stats.recv_good > 0) && (_stats.recv_good % 1000 == 0)) {
        _dumpStats(&Serial);
    }
    
}

/*
 *  Hard to tell, I know, but this is the main app loop.
 *  Sorry for the lack of self-documenting code. :(
 */
void ReceiverApplication::main() {

    pinMode(LED_SEND, OUTPUT);
    digitalWrite(LED_SEND, LOW);
    pinMode(LED_GOOD, OUTPUT);
    digitalWrite(LED_GOOD, LOW);
    pinMode(LED_BAD, OUTPUT);
    digitalWrite(LED_BAD, LOW);
    
    Serial.begin(115200);  // debugging
    Serial1.begin(230400); // packets

    _expectedSerial = 0;

    SerialPacket p;
    p.setDelegate(this);
    p.use(&Serial1);

    Serial.println("GO!");
    _stats.start_time = millis();

    while(1) {
        p.loop();
    }

}
