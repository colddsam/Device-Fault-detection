import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart


class SMTPserver:
    def __init__(self, SMTP_USERNAME, SMTP_PASSWORD) -> None:
        self.SMTP_USERNAME = SMTP_USERNAME
        self.SMTP_PASSWORD = SMTP_PASSWORD

    def sendAlert(self, alertText, receiver_email: str) -> None:
        smtp_server = 'smtp.gmail.com'
        smtp_port = 587
        sender_email = self.SMTP_USERNAME
        subject = "Alert from Device!!!"
        msg = MIMEMultipart()
        msg['From'] = sender_email
        msg['To'] = receiver_email
        msg['Subject'] = subject
        msg.attach(MIMEText(alertText, 'plain'))
        msg_str = msg.as_string()

        try:
            with smtplib.SMTP(smtp_server, smtp_port) as server:
                server.starttls()
                server.login(self.SMTP_USERNAME, self.SMTP_PASSWORD)
                server.sendmail(sender_email, receiver_email, msg_str)
                print("Operation successful")
        except Exception as e:
            print(f"Operation not successful: {e}")
