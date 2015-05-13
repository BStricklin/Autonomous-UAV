#the pwm distance code as well as the publisher 
import rospy
from std_msgs.msg import String

import RPi.GPIO as GPIO
import time
import numpy as np


global r1
global r2
global r3
global st
global ed
r1 = 0
r2 = 0
r3 = 0
st = 0
ed = 0

GPIO.setmode(GPIO.BOARD)
# Waits for a rising edge
GPIO.setup(7, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
# Waits for a falling edge
GPIO.setup(11, GPIO.IN, pull_up_down=GPIO.PUD_UP)

def start_signal(channel):
    global st
    st = time.time()*1000000
    #print st

def end_signal(channel):
    ed = time.time()*1000000
    global st, r1, r2
    # update previous variables
    r3 = r2
    r2 = r1
    #determine the distance and set it as the first reading
    # give it in centimeters
    r1 = (ed - st)/58


    # Can display values as they're read in
    # print in feet
    #print r1/2.54/12
    

#Set Pin 7 as the input for the rising edge of the PWM signal coming in
GPIO.add_event_detect(7, GPIO.RISING, callback=start_signal)
#Set Pin 11 as the input for the Falling edge of the incoming PWM signal
GPIO.add_event_detect(11, GPIO.FALLING, callback=end_signal)

while(1):

    # can adjust sleep time to update values, or can 'call' values at any time
    time.sleep(1)
    median_val = np.median([r1, r2, r3])
    # median_val is in centimeters, I print it in feet
    print median_val/2.54/12

# the bottom added to create the publisher
def talker():
   7     pub = rospy.Publisher('chatter', String, queue_size=10)
   8     rospy.init_node('talker', anonymous=True)
   9     rate = rospy.Rate(10) # 10hz
  10     while not rospy.is_shutdown():
  12         rospy.loginfo(medium_val)
  13         pub.publish(medium_val)
  14         rate.sleep()
  15 
  16 if __name__ == '__main__':
  17     try:
  18         talker()
  19     except rospy.ROSInterruptException:
  20         pass

