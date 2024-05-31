from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from utils.smtpServer import SMTPserver
import secret as sc

SMTP_USERNAME = sc.EMAIL_ID
SMTP_PASSWORD = sc.PASSWORD
RECEIVER_EMAIL=sc.DEFAULT_USER
app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

model=SMTPserver(SMTP_USERNAME=SMTP_USERNAME,SMTP_PASSWORD=SMTP_PASSWORD)

@app.get('/')
async def root():
    return 'This is Health Mate backend fetch API'

@app.post('/sendalert/')
async def sendAlert(address:str):
    alert="Device not found in "+address+" address"
    try:
        model.sendAlert(alertText=alert,receiver_email=RECEIVER_EMAIL)
        return {"response":"operation successful"}
    except Exception as e:
        return {"response":"there is a error : {}".format(e)}