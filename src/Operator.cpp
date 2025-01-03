#include "Operator.h"
#include "Aircraft.h"
#include "ComSys.h"
#include "Display.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <unistd.h>
#include "globals.h"
#include "Logger.h"
#include "cTimer.h"
#include <chrono>
/////Constructors/////

// OperatorConsole constructor
Operator::Operator() {

}

// Destructor
Operator::~Operator() {

}

void Operator::checkForMessages(name_attach_t *attach) {
	char msg[256];
	int rcvid;
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Operator waiting for messages" << std::endl;
	}
	// Non-blocking receive
	rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
	if (rcvid != -1) {
		handleMessage(rcvid, msg);
	}
}

void Operator::handleMessage(int rcvid, const char *msg) {
    std::string receivedMessage(msg);

    if (receivedMessage == "Parameter") {
        int newN;
        std::cout << "WRITE NEW PARAMETER: " << std::endl;
        std::cin >> newN;

        MsgReply(rcvid, 0, &newN, sizeof(newN));

    } else if (receivedMessage.find("Collision:") == 0) {
        size_t colonPos = receivedMessage.find(":");
        size_t commaPos = receivedMessage.find(",", colonPos);
        int aircraftID1 = std::stoi(receivedMessage.substr(colonPos + 1, commaPos - colonPos - 1));
        int aircraftID2 = std::stoi(receivedMessage.substr(commaPos + 1));

        std::cout << "Collision detected between Aircraft " << aircraftID1
                  << " and Aircraft " << aircraftID2 << std::endl;

        // Prompt for Z-axis speed adjustment commands
        giveCommand(aircraftID1);
        giveCommand(aircraftID2);

        MsgReply(rcvid, 0, "Collision Handled", strlen("Collision Handled") + 1);

    } else {
        MsgReply(rcvid, 0, "Unknown Message", strlen("Unknown Message") + 1);
    }
}



/////Aircraft related commands/////
// Method to send command to aircraft
void Operator::sendCommandToAircraft(int aircraftID,
		const std::string &command) {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "sending command" << std::endl;
	}
	// Format the command to include aircraft ID and the actual command

}

void Operator::changeParameterN(int newN) {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Changing Parameter N to:" << newN << std::endl;
	}

}

// Private method to log the commands sent
void Operator::logCommand(const std::string &command) {
	if (logFile.is_open()) {
		logFile << "Command: " << command << std::endl;
		std::cout << "Logging command: " << command << std::endl;
	} else {
		std::cerr << "Error: Log file is not open!" << std::endl;
	}

	//OperatorLog.log_OperatorCommand(commands, commands);
}


void Operator::giveCommand(int aircraftID) {
    std::string message;



    // Prompt for z-axis speed adjustment
    std::cout << "Enter Z-axis speed adjustment for Aircraft " << aircraftID << ": ";
    std::cin >> message;

    // Send the command to Comms
    sendCommandToComms(aircraftID, "ChangeSpeedZ:" + message);
}




void Operator::sendCommandToComms(int aircraftID, const std::string &message) {
    std::string commsName = "commsServer";
    int coid = name_open(commsName.c_str(), 0);
    if (coid == -1) {
        std::cerr << "Error: Failed to connect to Comms server!" << std::endl;
        return;
    }

    std::string command = std::to_string(aircraftID) + ":" + message;

    if (MsgSend(coid, command.c_str(), command.size() + 1, NULL, 0) == -1) {
        std::cerr << "Error: Failed to send command to Comms!" << std::endl;
    } else {
        std::cout << "Command sent to Comms: " << command << std::endl;
    }

    name_close(coid);
}



void Operator::manualInputLoop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for 5 seconds before next prompt

        std::cout << "\nEnter 'M' to manually send a command or 'Q' to quit:\n";
        char choice;
        std::cin >> choice;

        if (choice == 'M' || choice == 'm') {
            int aircraftID;
            std::cout << "Enter Aircraft ID for manual command: ";
            std::cin >> aircraftID;
            giveCommand(aircraftID);
        } else if (choice == 'Q' || choice == 'q') {
            std::cout << "Operator shutting down manual input..." << std::endl;
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}





void* Operator::startOperator(void *arg) {

	((Operator*) arg)->runOperator();
	return NULL;

}



void Operator::runOperator() {
    cTimer time = cTimer(1, 0); // 1-second timer
    time.startTimer();

    std::string OperatorName = "Operator_1";

    // Register Operator with the name service
    name_attach_t *attach = name_attach(NULL, "Operator_1", 0);
    if (attach == NULL) {
        std::cerr << "Error: Failed to register Operator with name service!" << std::endl;
        return;
    }

    int parameterTimer = 0;
    int inputPromptTimer = 0; // Timer for input prompts

    while (true) {
        time.tick();

        // Check for messages periodically (every 30 ticks)
        if (parameterTimer % 30 == 0) {
            checkForMessages(attach);
        }

        // Prompt for manual input every 10 seconds (adjustable)
        if (inputPromptTimer % 10 == 0) {
            std::cout << "\nEnter 'M' to manually send a command or 'Q' to quit:\n";
            char choice;

            std::cin >> choice;

            if (choice == 'M' || choice == 'm') {
                int aircraftID;
                std::cout << "Enter Aircraft ID for manual command: ";
                std::cin >> aircraftID;

                giveCommand(aircraftID);
            } else if (choice == 'Q' || choice == 'q') {
                std::cout << "Operator shutting down manual input..." << std::endl;
                break;
            } else {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }

        time.waitTimer();
        time.tock();

        parameterTimer++;
        inputPromptTimer++;


        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    name_detach(attach, 0);
}




//void Operator::runOperator() {
//	cTimer time = cTimer(1, 0);
//	time.startTimer();
//
//	std::string OperatorName = "Operator_1";
//
//	name_attach_t *attach = name_attach(NULL, "Operator_1", 0);
//	if (attach == NULL) {
//		std::cerr << "Error: Failed to register Operator " <<" with name service!" << std::endl;
//		return;
//	}
//	int parameterTimer=0;
//
//	while (true) {
//		time.tick();
//if(parameterTimer%30==0){
//		checkForMessages(attach);
//}
//		time.waitTimer();
//		time.tock();
//parameterTimer++;
//	}
//}
