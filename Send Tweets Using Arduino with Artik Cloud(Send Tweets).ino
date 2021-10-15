#libraries
import tweepy 
import time 
import serial

ser = serial.Serial("COM5", 9600)  #change 'COM5' to your serial port used in your arduino IDE
while(1):
    sequence = ser.read() #read serial from Arduino
    if(sequence > 0):
        auth = tweepy.OAuthHandler("Consumer Key (API Key)", "Consumer Secret (API Secret)") #change this to your twitter keys
        auth.set_access_token("Access Token", "Access Token Secret")    #change this to your twitter access tokens
        api = tweepy.API(auth)
        
        if(sequence=='1'):
            api.update_status(status="Hi friends I'm home! (Sent by Python script) #home "
                          +time.strftime("%d/%m/%Y") + " " + time.strftime("%H:%M:%S"))
        else:
            api.update_status(status="I'm busy right now. (Sent by Python script) #busy "
                          +time.strftime("%d/%m/%Y") + " " + time.strftime("%H:%M:%S"))
        print "Status tweeted!"
        print "Sequence: " + sequence;
        print (time.strftime("%d/%m/%Y"))
        ## 12 hour format ##
        print (time.strftime("%I:%M:%S"))
        ## 24 hour format ##
        print (time.strftime("%H:%M:%S"))
ser.close #close serial
