/**
 * @file SerialConfig.cpp
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */

#include "SerialConfig.h"

using namespace std;
using namespace tiny_runner;

SerialConfig::SerialConfig() :
    baudRate(9600),
    characterSize(8),
    flowControl(boost::asio::serial_port_base::flow_control::none),
    parity(boost::asio::serial_port_base::parity::none),
    port("PORT NOT SET!"),
    stopBits(boost::asio::serial_port_base::stop_bits::one) { }

SerialConfig::~SerialConfig() { }

SerialConfig* SerialConfig::getInstance() {
    if (instance == NULL) {
        instance = new SerialConfig();
    }

    return instance;
}

void SerialConfig::setBaudRate(int baudRate) {
    this->baudRate = baudRate;
}

void SerialConfig::setCharacterSize(int characterSize) {
    this->characterSize = characterSize;
}

void SerialConfig::setFlowControl(boost::asio::serial_port_base::flow_control::type flowControl) {
    this->flowControl = flowControl;
}

void SerialConfig::setParity(boost::asio::serial_port_base::parity::type parity) {
    this->parity = parity;
}

void SerialConfig::setPort(const std::string& port) {
    this->port = port;
}

void SerialConfig::setStopBits(boost::asio::serial_port_base::stop_bits::type stopBits) {
    this->stopBits = stopBits;
}

int SerialConfig::getBaudRate() const {
    return baudRate;
}

int SerialConfig::getCharacterSize() const {
    return characterSize;
}

boost::asio::serial_port_base::flow_control::type SerialConfig::getFlowControl() const {
    return flowControl;
}

boost::asio::serial_port_base::parity::type SerialConfig::getParity() const {
    return parity;
}

const std::string& SerialConfig::getPort() const {
    return port;
}

boost::asio::serial_port_base::stop_bits::type SerialConfig::getStopBits() const {
    return stopBits;
}
