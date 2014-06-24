/**
 * @file tiny_runner.h
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */

#ifndef TINY_RUNNER_H_
#define TINY_RUNNER_H_

#include "stdbool.h"
#include "stdint.h"
#include "tiny_io.h"

/** Ends the test, allowing the PC's test manager to evaluate the results. */
void endTest();

/**
 * An example of how to right a test case handler. This handler is automatically
 * registered and can be used as simple "is my test runner working?" test. Your
 * test case handlers should have a match signature.
 *
 * @param command C-string, The command to be inspected
 * @param commandSize Size of the command (without null terminator)
 * @returns true if the command was handled
 *          false if the command was not handled
 */
bool handleExampleTests(const uint8_t* command, const size_t commandSize);

/**
 * Adds the provided test case handling function to the list of handlers who
 * have a chance to handle test commands.
 *
 * @param testCaseHandler Handler function that inspects a command and handles it if needed
 */
extern void registerTestCase(bool (*testCaseHandler)(const uint8_t*, const size_t));

/**
 * Reports the return value of the function under test and a status message to
 * the PC's test manager.
 *
 * @param returnValue Value to be returned
 * @param returnMessage C-string, status message regarding the test
 */
extern void reportReturnValueInt(const uint32_t returnValue, const char* returnMessage);

/**
 * Reports the provided key-value pair to the PC's test manager.
 *
 * @param key C-string, id of the value
 * @param value C-string, value to be reported
 */
extern void reportValueString(const uint8_t* key, const uint8_t* value);

/** Starts the test runner listening for test commands. */
extern void runTests();

#endif /* TINY_RUNNER_H_ */
