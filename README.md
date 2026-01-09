# IoT Environment Sensor Full Stack Project

A system to collect, transmit, and visualize air quality data in
real-time.

This project collects air quality data from a physical sensor,
transmits it through a microcontroller and MQTT-based messaging
infrastructure, processes data through backend services, and presents
it to users through a web-based frontend application.

## Features
- Users interactively connect sensor to local network
- Collects air quality readings at set interval
- Supports multiple sensor types
- Provides a backend API for retrieving live and historical sensor readings

## System Components

The system includes:
- Air quality sensor
- Microcontroller for data acquisition and transmission (ESP32C6)
- MQTT bridge for JSON message forwarding
- Cloud-native backend microservices for data ingestion and processing
- Web frontend for data visualization

## System Architecture

The system is designed to operate in real-time and be accessible
remotely over a network. However, all parts can also be run locally,
on the same network.

The project is divided into two main parts:

1. On-Site Network
   - Sensor:
	 - Measures eCO2 and TVOC
   - Microcontroller:
	 - Reads sensor data through I2C
	 - Publishes sensor data to MQTT broker
   - Bridge Service:
	 - Subscribes to sensor MQTT topics
	 - Performs JSON formatting on data
	 - Forwards data to cloud backend
	 
 2. Cloud
	- Backend Services:
		- Receive data from the bridge
		- Process and stores sensor readings
		- Expose REST API for frontend
	- Web Frontend:
		- Accesses backend API data
		- Displays air-quality information to users

![Dataflow graph](dataflow.png)

## Technology Stack
- Microcontroller:
  - C/C++
  - Arduino Libraries
- Bridge Service:
  - Python
  - Mosquitto runs MQTT broker
- Backend Services:
  - Node.js
  - REST API
  - MongoDB (future)
- Web Frontend:
  - HTML/CSS UI
  - React (future)

## Future Expansions

- Live graph view on frontend
- AI summary and interpretation of raw data
- AI powered actionable feedback based on data ("Open a window", "Turn
  on AC", etc.)
- Add MongoDB for backend data collection
		
