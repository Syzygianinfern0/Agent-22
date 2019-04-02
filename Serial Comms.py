import serial
import re
port = "COM4"
bt = serial.Serial(port, 9600)
print("connected")
bt.flushInput()
prev = '-1'
mesg = ''
while(1):
    # print("sending!!")
    # bt.write(b"SPS" + str.encode(str(i)))

    input_d = bt.readline()
    input_d = input_d.decode()
    input_d = re.sub(r'\s+', '', input_d)

    if (input_d != '0') and (input_d != '1') and(input_d != 'IDLE'):            #junk case
        print('junk : {} {}'.format(input_d, str(type(input_d))))
        continue

    if(input_d == prev):                                                        #debounce
        continue

    if len(mesg) == 8:                                                          # now 8 bits
        print("\n\n The conversion for now is : {}".format(int(mesg, 2)))
        mesg = ''

    elif(input_d != 'IDLE'):                                                    #if 0 or 1
        mesg = mesg + input_d
        print(mesg)

      # ctrl c to stop

    prev = input_d
