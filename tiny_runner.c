/**
 * @file tiny_runner.c
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */

#include "string.h"
#include "tiny_runner.h"

/** Number of currently registered test handlers. */
static size_t numTestCaseHandlers = 0;

/** Maximum number of supported test handlers. */
#define MAX_TEST_CASE_HANDLERS 500

/** Registered test case handlers. */
static bool (*testCaseHandlers[MAX_TEST_CASE_HANDLERS]) (const uint8_t*, const size_t);

static void readCommand(uint8_t* command, const size_t maxCommandSize, size_t* commandSize);

void endTest() {
    tiny_printf("END OF TEST\n");
}

bool handleExampleTests(const uint8_t* command, const size_t commandSize) {
    // If this is a status "test"
    if (strcmp(command, "tiny_runner status") == 0) {
        reportValueString("version", "1.0");
        reportReturnValueInt(0, "tiny_runner is good!");
        endTest();

        return true;
    }

    // Command could not be handled by this test case
    return false;
}

/**
 * Reads in the next command.
 *
 * @param command (out) Buffer to place command in
 * @param maxCommandSize (in) Maximum allowable command size
 * @param commandSize (out) Size of the read command
 */
void readCommand(uint8_t* command, const size_t maxCommandSize, size_t* commandSize) {
    bool isCommandRead = false;

    // Prepare to read the command
    command[maxCommandSize-1] = '\0';
    *commandSize = 0;

    // While we have not used up the command buffer space
    while (*commandSize < maxCommandSize - 1) {
        uint8_t nextChar = tiny_getchar();

        // If the command has been terminated
        if (nextChar == '\r' || nextChar == '\n') {
            command[*commandSize] = '\0';
            isCommandRead = true;

            break;
        }
        // If a character was successfully read
        else if (nextChar != '\0') {
            command[(*commandSize)++] = nextChar;
        }
    }

    // If we ran out of space before finishing the command
    if (false == isCommandRead) {
        tiny_printf("Failed to read entire command '%s'!", command);
    }
}

void registerTestCase(bool (*testCaseHandler)(const uint8_t*, const size_t)) {
    // If there is still room for more test case handlers
    if (numTestCaseHandlers + 1 <= MAX_TEST_CASE_HANDLERS) {
        testCaseHandlers[numTestCaseHandlers++] = testCaseHandler;
    }
    // If there is no more room for this test case handler
    else {
        tiny_printf("Failed to add test case handler, out of room! Increase size of 'maxTestCaseHandlers'\n");
    }
}

void reportReturnValueInt(const uint32_t returnValue, const char* returnMessage) {
    tiny_printf("RETURN_CODE=%u\n", returnValue);
    tiny_printf("RETURN_MESSAGE=%s\n", returnMessage);
}

void reportValueString(const uint8_t* key, const uint8_t* value) {
    tiny_printf("VALUE(%s)=%s\n", key, value);
}

void runTests() {
    size_t       commandSize;
    const size_t maxCommandSize = 1024;
    uint8_t      command[maxCommandSize];

    // Register the example test
    registerTestCase(&handleExampleTests);

    // Initialize any required IO devices
    setupIO();

    for (;;) { // Run forever
        uint32_t handlerIter;

        // Read in the next command
        readCommand(command, maxCommandSize, &commandSize);

        // Let the registered test handlers try to handle it
        for (handlerIter = 0; handlerIter < numTestCaseHandlers; handlerIter++) {
            // If the test case handler successfully handled the command
            if (true == (*testCaseHandlers[handlerIter])(command, commandSize)) {
                break;
            }
        }

        // If the test was not handled at all
        if (handlerIter == numTestCaseHandlers) {
            tiny_printf("Failed to handle command '%s'!\n", command);
        }
    }
}
