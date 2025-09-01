# Home-Surveillance-System-Hardworking-Hardware-
##  Overview
This project is a low-cost IoT-based home surveillance system built using ESP32-CAM, ESP32 DevKit, and GSM modules.  
It detects intruders with sensors, captures images/videos, and sends instant alerts to the user via Telegram Bot and GSM call.  

Our system ensures real-time intrusion detection with multi-sensor accuracy and provides affordable security compared to traditional CCTV solutions.  

---

##  Features
-  Capture intruder images/videos with ESP32-CAM  
-  Detect motion using PIR sensor and unusual sounds with a mic  
-  On-site buzzer alarm for immediate warning  
-  Instant Telegram Bot notifications with intruder photo/video    
-  Works without Wi-Fi (thanks to GSM fallback)  
-  Multi-sensor detection (motion + sound + image) reduces false alarms  

---

##  Hardware Used
- ESP32-CAM (AI Thinker)  
- ESP32 DevKit V1  
- PIR Sensor (HC-SR501)    
- KY-038 Sound Sensor Module (Mic)    
- GSM Module (SIM800L / SIM900A)  
- Buzzer + Power Supply  

---

##  Software & Tools
- Arduino IDE / PlatformIO  
- Python (for Telegram Bot logic, hosted on Replit)  
- Telegram Bot API  
- Programming Languages: C++ (ESP32) & Python (Bot)  

---

##  Working Principle
1. PIR sensor/mic detects unusual activity.  
2. ESP32-CAM captures photo/video of intruder.  
3. ESP32 triggers buzzer alarm at the site.  
4. Alerts sent instantly via:  
   -  Telegram Bot (with images, videos, and options).  
   -  GSM module works without internet sending via sms 
5. Admin can control the system via Telegram Bot options:  
   - Stop buzzer    

---
