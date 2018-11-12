#encoding:utf-8
import time
from datetime import datetime

def currentDate():
    date = time.localtime()
    today = str(date.tm_year)+"-"+ str(date.tm_mon) + "-" + str(date.tm_mday)
    return today

def currentTime():
    timeStr = datetime.now()
    now = timeStr.strftime('%H-%M-%S')
    return now

if __name__ =="__main__":
    currentDate()
    currentTime()