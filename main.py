import serial
import time
import serial.tools.list_ports

#### Create .txt file ####
current_time = time.strftime("%m/%d/%Y_%H.%M.%S")
data = open('data_{}.txt'.format(time.strftime("%m.%d.%Y-%H.%M.%S")), 'w+')
data.write('Format: rpm, time(seconds)')

#### Arduino Data ####
# Select COM port
def selectPort():
    # Print ports and their descriptions
    ports = list(serial.tools.list_ports.comports())
    print("Ports:")
    for i in ports:
        print(i)

    if(len(ports) == 0):
        print("\nNo COM ports detected. Please connect an Arduino and try again.")
        againIn = input("Press enter to scan again. Enter any other key to exit.")
        if(againIn == ""):
            comPort = selectPort()
            return comPort
        else:
            print("\n\nExiting...")
            exit()
    if(len(ports) == 1):
        comPort = ports[0].device
        print("\nSelected:", comPort)
        return comPort
    else:
        print("\nMore than one COM port detected. Please enter the COM port you would like to use.")
        comPort = input("Enter COM port (Ex. com3 or COM3): ")
        return comPort
comPort = selectPort()

arduinoData = serial.Serial(comPort, 115200)
print('\nFormat: rpm, time(seconds)')
while True:
    ### Receive data from Arduino ###
    while arduinoData.inWaiting() == 0: # do nothing while there's no data
        pass # do nothing

    dataPacket = arduinoData.readline().decode('ascii') # read the data from the arduino
    print(dataPacket) # print data read from arduino to terminal
    data.write(dataPacket)
    data.flush()