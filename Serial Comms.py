import serial
port = "/dev/rfcomm0"
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
    input_d.strip('\n')

    if input_d is not '0' or input_d is not '1':
        print('junk : {} {}', format(input_d, type(input_d)))
        continue
    # # print(len(mesg))
    # if(input_d != prev):
    #     print(input_d)

    # if len(mesg) == 8:
    #     print("\n\n The conversion for now is : {}".format(int(mesg, 2)))
    #     mesg = ''
    # else:
    #     mesg = mesg + input_d
    #     print(mesg)

    #   # ctrl c to stop

    # prev = input_d
    print(input_d)