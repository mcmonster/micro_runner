/**
 * @file SensorTest.cpp
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */

#include <boost/thread.hpp>
#include <iostream>

#include "SensorTest.h"
#include "SerialConfig.h"

using namespace boost;
using namespace std;
using namespace testing;
using namespace tiny_runner;

SerialConfig* SerialConfig::instance = NULL;

SensorTest::SensorTest() : serialPort(serialService) {
}

SensorTest::~SensorTest() {

}

void SensorTest::executeTest(const std::string& command,
                             int timeOut,
                             std::map<std::string, std::string>& results) {
    boost::thread watchdog(&SensorTest::executeTestImpl, this,
                    command, boost::ref(results)); // Start the watchdog thread

    // If the test fails to complete in time
    if (!watchdog.timed_join(posix_time::seconds(timeOut))) {
            FAIL(); // Fail the test
    }
}

// Called by watchdog thread
void SensorTest::executeTestImpl(const std::string& command, std::map<std::string, std::string>& results) {
    asio::streambuf buffer;
    std::string line;

    cout << "Sending command to sensor..." << endl;
    size_t bytesWritten = asio::write(serialPort, asio::buffer(command + "\n"));
    if (bytesWritten != command.size() + 1) {
            std::cerr << "Only wrote " << bytesWritten << "/" << command.size() << " bytes!" << std::endl;
            FAIL();
    }

    cout << "Parsing the results..." << endl;
    do {
        unsigned int paramSize = 5;
        unsigned int returnCodeSize = 11;
        unsigned int returnMessageSize = 14;

        asio::read_until(serialPort, buffer, '\n'); // Read the next line
        std::istream bufferStream(&buffer);
        std::getline(bufferStream, line);

        //line = line.substr(0, line.size() - 1); // Pop off the carriage return

        std::cout << "Serial: " << line << std::endl;

        if ((line.size() > paramSize) && // If this is a param line
            (line.substr(0, paramSize).compare("VALUE") == 0)) {
            std::size_t splitPos = line.find('=');

            // Extract the parameter value
            std::pair<std::string, std::string> entry(
                            line.substr(0, splitPos), line.substr(splitPos+1)
                            );
            results.insert(entry);
        } else if ((line.size() > returnCodeSize) && // If this is a result line
                           (line.substr(0, returnCodeSize).compare("RETURN_CODE") == 0)) {
            std::size_t splitPos = line.find('=');

            // Extract the result value
            std::pair<std::string, std::string> entry(
                            line.substr(0, splitPos), line.substr(splitPos+1, line.size())
                            );
            results.insert(entry);
        } else if ((line.size() > returnMessageSize) && // If this is a message line
                           (line.substr(0, returnMessageSize).compare("RETURN_MESSAGE") == 0)) {
            std::size_t splitPos = line.find('=');

            // Extract the result value
            std::pair<std::string, std::string> entry(
                            line.substr(0, splitPos), line.substr(splitPos+1, line.size())
                            );
            results.insert(entry);
        }
    }
    // While the end of the test hasn't been found
    while ((line.size() < 11) || (line.substr(0, 11).compare("END OF TEST") != 0));
}

void SensorTest::SetUp() {
    SerialConfig*      config = SerialConfig::getInstance();
    system::error_code errorCode; // Holds error results from boost

    // Open the serial port
    serialPort.open(config->getPort(), errorCode);
    if (errorCode) { // If an error occurred
        std::cerr << "Failed to open serial port(" << config->getPort() << ") failed!" << std::endl;
        FAIL();
    }

    // Configure the transmission settings
    serialPort.set_option(asio::serial_port_base::baud_rate(config->getBaudRate()));
    serialPort.set_option(asio::serial_port_base::character_size(config->getCharacterSize()));
    serialPort.set_option(asio::serial_port_base::stop_bits(config->getStopBits()));
    serialPort.set_option(asio::serial_port_base::parity(config->getParity()));
    serialPort.set_option(asio::serial_port_base::flow_control(config->getFlowControl()));
}

void SensorTest::TearDown() {
    // Clean up the serial port
    serialPort.cancel();
    serialPort.close();

    // Clean up the serial service
    serialService.stop();
    serialService.reset();
}
