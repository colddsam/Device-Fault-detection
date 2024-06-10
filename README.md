# Lamp Post Fault Detection System

This project outlines a robust system for detecting and reporting faults in lamp posts using a combination of hardware sensors, a FastAPI backend, and email alerts.

## System Architecture

The system comprises three main components:

1. **Hardware Sensors:** Deployed on each lamp post, these sensors continuously monitor various parameters like voltage, current, and temperature. They also check for physical tampering or damage.

2. **FastAPI Backend:** This serves as the central communication hub. It receives data from the sensors, processes it to identify potential faults, and triggers alerts when necessary.

3. **Email Alert System:** Upon detecting a fault, the backend sends real-time notifications to designated personnel via email, enabling swift response and maintenance.

## Code Breakdown

### File Structure

| Filename              | Description                                                                        |
|-----------------------|------------------------------------------------------------------------------------|
| `.gitignore`         | Lists files and directories to be ignored by Git version control.                  |
| `README.md`          | This file, providing a comprehensive overview of the project.                     |
| `main.py`            | Core FastAPI application file handling API endpoints and logic.                 |
| `requirements.txt`    | Lists Python libraries required for running the project.                          |
| `secret.py`          | Stores sensitive credentials like email login details.                                |
| `smtpServer.py`       | Contains the class responsible for sending email alerts.                         |

### Backend (`main.py`)

Built using the FastAPI framework, the backend exposes two key endpoints:

| Endpoint       | Method | Description                                                            |
|----------------|--------|--------------------------------------------------------------------|
| `/`             | `GET`  | Returns a simple message confirming the API is running.            |
| `/sendalert/` | `POST` | Accepts a lamp post address and triggers an email alert.             |

**Key Functionality:**

- **Dependency Injection:** Leverages FastAPI's dependency injection system for managing the `SMTPserver` instance.
- **CORS Support:** Enables cross-origin requests for interaction with various front-end applications.

### Email Alerts (`smtpServer.py`)

This module handles the email alert functionality using the `smtplib` library. 

**Key Features:**

- **Secure Connection:** Employs TLS encryption for secure communication with the SMTP server.
- **Customizable Alerts:**  Allows modification of email content, subject, and recipient addresses for flexibility.

## Advantages

- **Real-time Monitoring:** Constant sensor data analysis enables immediate fault detection.
- **Proactive Maintenance:** Timely alerts minimize downtime and reduce maintenance costs.
- **Improved Safety:**  Early identification of potential hazards enhances public safety.
- **Scalability:** The system can be easily scaled to accommodate a large number of lamp posts.

## Future Enhancements

- **Data Visualization:**  Integrate a dashboard for visualizing sensor data and fault trends.
- **Machine Learning:** Implement predictive maintenance models by analyzing historical data.
- **Mobile App Integration:** Develop a mobile application for real-time alerts and system management.

This project provides a robust and scalable solution for lamp post fault detection, leading to significant cost savings, improved safety, and increased efficiency in urban lighting infrastructure.