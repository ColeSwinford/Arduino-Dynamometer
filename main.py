import serial
import time
import serial.tools.list_ports
import threading

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
done = False

### proof of concept for Arduino ###
# torque and hp calculations based on data from engine curves
def getTorque(rpm):
    # torque = y = 16.6 + 4.25E-03x + -1.43E-06x^2
    torque = 16.6 + (0.00425)*rpm + (-0.00000143)*(rpm**2)
    return torque
def getHP(rpm):
    # hp = y = -5.99 + 9.7E-03x + -1.52E-06x^2
    hp = -5.99 + (0.0097)*rpm + (-0.00000152)*(rpm**2)
    return hp
### proof of concept for Arduino ###

def record():
    #### Create .txt file ####
    data = open('data_{}.txt'.format(time.strftime("%m.%d.%Y-%H.%M.%S")), 'w+')
    data.write('Format: rpm, torque(FT-LB based on engine datasheet), curveHP(HP based on engine datasheet), hp(HP calculated from torque), momentInertia(torque / angularAccel), time(seconds)\n')

    # global vars
    global arduinoData
    global done
    # print('\nFormat: rpm, time(seconds)')
    while not done:
        ### Receive data from Arduino ###
        while arduinoData.inWaiting() == 0: # do nothing while there's no data
            pass # do nothing
        dataPacket = arduinoData.readline().decode('ascii') # read the data from the arduino
        print(dataPacket) # print data read from arduino to terminal
        data.write(dataPacket)
        data.flush()
    done = False
    return

while True:
    selection = input("\nPress enter to start recording data.")
    if(selection == ""):
        # Create a thread
        threading.Thread(target=record).start()
        while done == False:
            selection = input("Enter 'x' to stop recording and save data.\n")
            if(selection == "x" or selection == "X"):
                done = True
