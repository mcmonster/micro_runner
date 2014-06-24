/**
 * @file SensorTest.h
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */
#ifndef SENSORTEST_H_
#define SENSORTEST_H_

#include "boost/asio.hpp"
#include <map>
#include <string>

#include "gtest/gtest.h"

namespace tiny_runner {

/** Sets up the sensor for any sensor-centric test. */
class SensorTest : public testing::Test {
public:
    SensorTest();
    virtual ~SensorTest();

    virtual void SetUp();
    virtual void TearDown();

    /**
     * Executes the test specified by command and filters out the results.
     *
     * @param command Must not be the empty string.
     * @param timeout In seconds. Must be > 0. 0 means no timeout.
     * @param results. Key of PARAM(myParam) is value of return-by-param's value
     * Key of RETURN is value of return.
     */
     void executeTest(const std::string& command,
                      int timeout,
                      std::map<std::string, std::string>& results);

protected:
    boost::asio::serial_port& getSerialPort() { return serialPort; }

private:
    /**
     * Called by the watchdog thread in executeTest.
     *
     * @param command Must not be the empty string.
     * @param results Populated with the command results.
     */
    void executeTestImpl(const std::string& command,
                         std::map<std::string, std::string>& results);

    /** Interface for controlling the state of the serial interface. */
    boost::asio::io_service serialService;

    /** Serial interface for talking to the sensor's UART interface. */
    boost::asio::serial_port serialPort;
};

} // namespace tiny_runner

#endif /* SENSORTEST_H_ */
