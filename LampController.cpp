
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;

atomic<bool> leftBlinkerOn(false);
atomic<bool> rightBlinkerOn(false);
atomic<bool> brakeOn(false);

mutex ioMutex;

void blinkerLoop(const string& side, atomic<bool>& blinkerState) {
    bool lampState = false;
    while (true) {
        if (blinkerState) {
            lampState = !lampState;
        }
        else {
            lampState = false;
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void printLampState() {
    lock_guard<mutex> lock(ioMutex);
    cout << "\n=== Lamp Status ===" << endl;
    cout << "[Left Blinker] " << (leftBlinkerOn ? "ON" : "OFF") << endl;
    cout << "[Right Blinker] " << (rightBlinkerOn ? "ON" : "OFF") << endl;
    cout << "[Brake Lamp] " << (brakeOn ? "ON" : "OFF") << endl;
}

int main()
{
    cout << "===Car Ramp Controller Simulator===\n";
    thread leftThread(blinkerLoop, "Left", ref(leftBlinkerOn));
    thread rightThread(blinkerLoop, "Right", ref(rightBlinkerOn));
    
    //inputLoop();
    char input;
    while (true) {
        printLampState();
        cout << "\nInput command (L/R/B/Q): ";
        cin >> input;

        switch (toupper(input)) {
        case 'L':
            leftBlinkerOn = !leftBlinkerOn;
            cout << "[INFO] Left Blinker " << (leftBlinkerOn ? "Enabled" : "Disabled") << endl;
            break;
        case 'R':
            rightBlinkerOn = !rightBlinkerOn;
            cout << "[INFO] Right Blinker " << (rightBlinkerOn ? "Enabled" : "Disabled") << endl;
            break;
        case 'B':
            brakeOn = !brakeOn;
            break;
        case 'Q':
            cout << "Exiting program..." << endl;
            exit(0);
        default:
            cout << "Unknown command." << endl;
        }
    }

    leftThread.join();
    rightThread.join();

    return 0;
}
