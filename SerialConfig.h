/**
 * @file SerialConfig.h
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */
#ifndef SERIAL_H_
#define SERIAL_H_

#include <string>

#include "boost/asio.hpp"

namespace tiny_runner {

/** Singleton configuration for the serial device used to talk to the test runner. */
class SerialConfig {
public:
    virtual ~SerialConfig();

    /**
     * @returns Singleton instance of the SerialConfig.
     */
    static SerialConfig* getInstance();

    int getBaudRate() const;
    int getCharacterSize() const;
    boost::asio::serial_port_base::flow_control::type getFlowControl() const;
    boost::asio::serial_port_base::parity::type getParity() const;
    const std::string& getPort() const;
    boost::asio::serial_port_base::stop_bits::type getStopBits() const;

    void setBaudRate(int baudRate);
    void setCharacterSize(int characterSize);
    void setFlowControl(boost::asio::serial_port_base::flow_control::type flowControl);
    void setParity(boost::asio::serial_port_base::parity::type parity);
    void setPort(const std::string& port);
    void setStopBits(boost::asio::serial_port_base::stop_bits::type stopBits);

private:
    SerialConfig(); /**< Singleton; Hidden constructor. */

    int baudRate; /**< Baud rate of the connection. */

    int characterSize; /**< Character size of the connection. */

    boost::asio::serial_port_base::flow_control::type flowControl; /**< Flow control setting. */

    static SerialConfig* instance; /**< Singleton instance reference. */

    boost::asio::serial_port_base::parity::type parity; /**< Parity setting. */

    std::string port; /**< Serial port to use. */

    boost::asio::serial_port_base::stop_bits::type stopBits; /**< Stop bits setting. */
};

} /* namespace tiny_runner */

#endif /* SERIAL_H_ */
