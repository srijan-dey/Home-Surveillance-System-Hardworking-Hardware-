from flask import Flask, request, jsonify
import numpy as np
import requests
import tensorflow as tf
from tensorflow import keras
from PIL import Image
import io

# Telegram settings
BOT_TOKEN = ""8294174398:AAFnaZYAedZdHqhhuCKnZXvHyBVPSYKb1Ko"; 
CHAT_ID = "1891910582"

model = keras.models.load_model("intruder_detector.h5")
classes = ["intruder", "normal"]

def send_to_telegram(image_url, prediction, confidence):
    message = f"Prediction: {prediction} ({confidence*100:.2f}%)"
    requests.get(f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage",
                 params={"chat_id": CHAT_ID, "text": message})
    requests.get(f"https://api.telegram.org/bot{BOT_TOKEN}/sendPhoto",
                 params={"chat_id": CHAT_ID, "photo": image_url})

def classify_from_url(image_url, img_size=(128, 128)):
    try:
        response = requests.get(image_url, timeout=10)
        response.raise_for_status()
        img = Image.open(io.BytesIO(response.content)).convert("RGB")
        img = img.resize(img_size)
        img_array = np.expand_dims(np.array(img) / 255.0, axis=0)
        predictions = model.predict(img_array)
        predicted_class = np.argmax(predictions, axis=1)[0]
        confidence = float(np.max(predictions))
        send_to_telegram(image_url, classes[predicted_class], confidence)
        print(f"✅ Sent: {classes[predicted_class]} ({confidence*100:.2f}%)")
        return f"{classes[predicted_class]} ({confidence*100:.2f}%)"
    except Exception as e:
        print(f"❌ Error: {e}")
        return f"Error: {e}"

app = Flask(__name__)

@app.route('/classify', methods=['POST'])
def classify():
    data = request.get_json()
    image_url = data.get('image_url')
    if not image_url:
        return jsonify({"error": "No image_url provided"}), 400
    result = classify_from_url(image_url)
    return jsonify({"result": result}), 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)