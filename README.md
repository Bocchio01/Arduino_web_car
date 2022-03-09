# Arduino_web_car
Small car controlled via WebSocket and driven by Arduino

---

## Steps to do
1. Attach motor, servo and Arduino shield to the chassis
2. Setup ESP8266 shield to transfer information via internet
3. Setup PS2 controller to plugin into computer
4. Setup a cam and send video stream to computer
5. Setup a GPS location stream or at least a log file to print al gps coord
6. Clean up al together (battery, hooks, cable and so on)
7. Buy Coral ML chip and re-setup wifi-connectivities, camera and coord stream
8. Implement ML system

## Wanted features
- Switch between super reactive mode and some sort of softer mode
- Switch between ML controlled to User-controll based
- By external sensor, get rid of distances and read form external events
- L/S parking
- Lighting for turn or reverse drive

## Pro features
- Learn the spaces around. After a tour, being able to have a conscious state of what's around and being able to drive 'from -> to' autonomously
- Have a connectivities also out of Wifi with mobile data services
- Have a client for PS2 joystick as also for a normal mouse and so on 
- Longest possible battery support