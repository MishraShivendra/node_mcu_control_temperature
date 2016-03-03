'''
Author: Shivendra Mishra
Environment: Python 2.7.3 in Linux
Created: 1-Mar-2016
Description: Following methods read temperature sensor data triggers an E-mail if temperature reaches threshold.
'''

import smtplib
from email.mime.multipart import MIMEMultipart
from email.MIMEText import MIMEText
import urllib2
import email.utils
import poplib
from email import parser
import datetime
import time

date_list = []
subject_list = []
stamp = 0
alarm_ringing = True
sensors_email_id = 'ccc@gmail.com'
sensors_email_pwd = 'pwd'
human_email_id = 'asd@live.com'
threshold = 512
time_alarm_shut_down = 60*15
sensors_address = 'http://address'
sampling_T = 30

# Following function get temperature readings from local server(Node Mcu)
def get_data_from_sensor():
    response = urllib2.urlopen(sensors_address)
    html = response.read()
    value_list = html.replace('<',' ')
    values = value_list.split(' ')
    readings = values[4:8]
    return readings

# Following method gets E-mail and parses date/subject from it.
def get_email_subject_and_date():
   pop_ins = poplib.POP3_SSL('pop.gmail.com')
   pop_ins.user(sensors_email_id)
   pop_ins.pass_(sensors_email_pwd)
   messages = [pop_ins.retr(i) for i in range(1, len(pop_ins.list()[1]) + 1)]
   messages = ["\n".join(mssg[1]) for mssg in messages]
   messages = [parser.Parser().parsestr(mssg) for mssg in messages]
   global_date = []
   for message in messages:
       global_date.append(message['Date'])
       subject_list.append(message['Subject'])

   for this_date in global_date:
       quality_time = email.utils.parsedate(this_date)
       time_stamp = time.mktime( quality_time )
       this_time = datetime.datetime.fromtimestamp( time_stamp )
       date_list.append(this_time)
   print date_list, subject_list

# Following function tests if a human took care of alarm
def test_if_human_replied():
    for subject in subject_list:
        if str(stamp) in subject:
                alarm_ringing = False

# Following function sends an email to 'toaddr'
def send_email():
    fromaddr = sensors_email_id
    toaddr = human_email_id
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
    server.login(sensors_email_id, sensors_email_pwd)
    text = msg.as_string()
    server.sendmail(fromaddr, toaddr, text)


if __name__ == "__main__":
    #Get the temperature, test if this is state change - if so trigger email.
    while True:
        sensor_readings = get_data_from_sensor()
        #Check out her temper every 30 sec.
        get_email_subject_and_date()
        test_if_human_replied()
        for each_value in sensor_readings:
            if int(each_value) > threshold and alarm_ringing == True:
                send_email()
                stamp = stamp+1
            elif int(each_value) > threshold and alarm_ringing == False:
                time.sleep(time_alarm_shut_down)
        time.sleep(sampling_T)
