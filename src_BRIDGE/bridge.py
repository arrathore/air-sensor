import time
import queue

import json
import threading
import requests

import paho.mqtt.client as mqtt

# broker settings
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPICS = [("sensor/test_data", 0)] # (topic, qos)

BACKEND_URL = "http://localhost:3000/ingest"

# fifo for data
msg_queue = queue.Queue(maxsize=1000)

# mqtt callback
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected to mqtt broker")
        for topic, qos in MQTT_TOPICS:
            client.subscribe(topic, qos=qos)
            print(f"subscribed to {topic} with qos {qos}")
    else:
        print(f"mqtt connection failed with code {rc}")

# get mqtt message, add metadata and send to fifo
def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode("utf-8") # messages are json
        envelope = {
            "timestamp": time.time(), # unix timestamp
            "topic": msg.topic,
            "qos": msg.qos,
            "payload": json.loads(payload)
        }
        msg_queue.put(envelope)
    except Exception as e:
        print(f"failed to process message from {msg.topic}: {e}")

# configure mqtt        
def mqtt_thread():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_BROKER, MQTT_PORT)
    client.loop_forever()

# send data to backend
def backend_sender():
    while True:
        msg = msg_queue.get()
        if msg is None:
            continue
        try:
            print("sending data...")
            response = requests.post(BACKEND_URL, json=msg, timeout=5)
            if response.status_code != 200:
                print(f"backend returned {response.status_code}, requeueing")
                msg_queue.put(msg)
                time.sleep(1)
            else:
                print(f"success!")
        except requests.exceptions.RequestException as e:
            print(f"failed to send to backend: {e}, requeueing")
            msg_queue.put(msg)
            time.sleep(1)

# main            
if __name__ == "__main__":
    # start mqtt thread
    threading.Thread(target=mqtt_thread, daemon=True).start()
    # start backend sender thread
    threading.Thread(target=backend_sender, daemon=True).start()

    print("bridge running. ctrl+c to exit.")
    try:
        while True:
            time.sleep(10)
    except KeyboardInterrupt:
        print("shutting down bridge")
        
