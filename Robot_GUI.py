from PyQt5.QtWidgets import * #QMainWindow, QApplication, QPushButton, QLineEdit, QLabel, QStackedWidget, QComboBox, QDateEdit, QTableView, QTableWidgetItem, QMessageBox, QErrorMessage
from PyQt5 import uic, QtWidgets
import sys, time, serial, serial.tools.list_ports, time
import pyqtgraph as pg

#Check to see if comport is available
comPort=None
while comPort==None:
    #Checks for the com something is plugged into.
    portData=serial.tools.list_ports.comports()
    try:
        comPort=str(portData[0])[0:5]
    except:
        comPort=None

time.sleep(1)

global arduinoData, mode, direction, run_program
arduinoData=serial.Serial(comPort.lower(), 9600)

mode="Manual"
direction="None"
run_program=False

class UI(QMainWindow):
    def __init__(self):
        super(UI, self).__init__()

        #Load the ui file
        uic.loadUi("RobotQT_GUI.ui", self)

        #BUTTON DEFINITIONS
        self.btn_forward.clicked.connect(self.MoveForward)
        self.btn_backward.clicked.connect(self.MoveBackward)
        self.btn_left.clicked.connect(self.MoveLeft)
        self.btn_right.clicked.connect(self.MoveRight)
        self.btn_auto.clicked.connect(self.AutoRun)
        self.btn_reset.clicked.connect(self.Reset)
        self.AutoOff

        self.show()

    #Send data through Serial Port to Arduino for Transmition
    def SendData(self, cmd):
        cmd=cmd+'\r'
        arduinoData.write(cmd.encode())

    #Command to move the device forward
    def MoveForward(self):
        self.SendData("Forward")
        direction="Forward"

    #Command to move the device backward
    def MoveBackward(self):
        self.SendData("Backward")
        direction="Backward"

    #Command to move device left
    def MoveLeft(self):
        self.SendData("Left")
        direction="Left"

    #Command to move device right
    def MoveRight(self):
        self.SendData("Right")
        direction="Right"

    #Command to auto pilot device
    def AutoRun(self):
        self.SendData("AutoRun")
        self.btn_auto.clicked.disconnect(self.AutoRun)
        self.btn_auto.clicked.connect(self.AutoOff)
        self.btn_auto.setStyleSheet("background-color : green")

    #Command to turn auto pilot off
    def AutoOff(self):
        self.SendData("AutoOff")
        self.btn_auto.clicked.disconnect(self.AutoOff)
        self.btn_auto.clicked.connect(self.AutoRun)
        self.btn_auto.setStyleSheet("background-color : red")

    #Command to set autopilot off
    def Reset(self):
        self.AutoOff
        self.SendData("Reset")


app = QApplication(sys.argv)
UIWindow = UI()
app.exec_()