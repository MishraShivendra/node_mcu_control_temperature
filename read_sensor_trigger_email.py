import smtplib
from email.mime.multipart import MIMEMultipart
from email.MIMEText import MIMEText
import urllib2
import time

# Following function get temprature readings from local server(Node Mcu)
def get_data_from_sensor():
    response = urllib2.urlopen('http://192.168.11.184')
    html = response.read()
    value_list = html.replace('<',' ')
    values = value_list.split(' ')
    readings = values[4:8]
    return readings

# Following function sends an email to 'toaddr'
def send_email():
    fromaddr = "xyz@gmail.com"
    toaddr = "pqr@live.com"
    msg = MIMEMultipart()
    msg['From'] = fromaddr
    msg['To'] = toaddr
    msg['Subject'] = "Temp Email"
    body = "Temp Sensor is blowing"
    msg.attach(MIMEText(body, 'plain'))
    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.ehlo()
    server.starttls()
    server.ehlo()
    server.login("xyz@gmail.com", "password")
    text = msg.as_string()
    server.sendmail(fromaddr, toaddr, text)

if __name__ == "__main__":
	#Get the tempareture, test if this is state change - if so trigger email.
    while True:
        sensor_readings = get_data_from_sensor()
		#Check out her temper every 30 sec.
        print sensor_readings
        for each_value in sensor_readings:
            if each_value == '1024':
                print each_value
                send_email()
        time.sleep(30)