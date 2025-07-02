#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace std;

enum class LampState { OFF, ON };

atomic<bool> leftBlinkerOn(false);
atomic<bool> rightBlinkerOn(false);
atomic<bool> hazardOn(false);
atomic<bool> brakeOn(false);

atomic<bool> leftLamp(false);
atomic<bool> rightLamp(false);

mutex ioMutex;

void blinkerLoop() {
    while (true) {
        if (hazardOn) {
            leftLamp = !leftLamp;
            rightLamp = leftLamp;
        } else {
            if (leftBlinkerOn) leftLamp = !leftLamp;
            else leftLamp = false;

            if (rightBlinkerOn) rightLamp = !rightLamp;
            else rightLamp = false;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void printStatus() {
    lock_guard<mutex> lock(ioMutex);
    cout << "\n=== Lamp Status ===" << endl;
    cout << "[Left Blinker Lamp]  " << (leftLamp ? "ON" : "OFF") << endl;
    cout << "[Right Blinker Lamp] " << (rightLamp ? "ON" : "OFF") << endl;
    cout << "[Brake Lamp]         " << (brakeOn ? "ON" : "OFF") << endl;
    cout << "[Hazard Mode]        " << (hazardOn ? "ON" : "OFF") << endl;
}

void handleInput(char input) {
    switch (toupper(input)) {
        case 'L':
            leftBlinkerOn = !leftBlinkerOn;
            hazardOn = false;
            break;
        case 'R':
            rightBlinkerOn = !rightBlinkerOn;
            hazardOn = false;
            break;
        case 'H':
            hazardOn = !hazardOn;
            if (hazardOn) {
                leftBlinkerOn = false;
                rightBlinkerOn = false;
            }
            break;
        case 'B':
            brakeOn = !brakeOn;
            break;
        case 'Q':
            cout << "프로그램을 종료합니다." << endl;
            exit(0);
        default:
            cout << "잘못된 입력입니다. [L/R/H/B/Q] 중 하나를 입력하세요." << endl;
    }
}

int main() {
    cout << "=== TCU Simulator ===" << endl;
    thread blinkThread(blinkerLoop);

    while (true) {
        printStatus();
        cout << "\n[L] [R] [H] hazard [B] Brake [Q] > ";
        char input;
        cin >> input;
        handleInput(input);
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    blinkThread.join();  // 이 줄은 도달하지 않음 (loop 무한)
    return 0;
}
