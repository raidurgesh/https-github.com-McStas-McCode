''' UI classes. 

Some autogenerated (do not edit).

@author: jaga
'''
from PyQt4 import QtCore, QtGui


''' View class containing all windows and dialogs.
ALL explicit ui widget updates MUST be handled by this class
'''
class McView(object):
    def __init__(self):
        self.__mw = McMainWindow()
        self.mwui = self.__mw.ui
        
    def initMainWindow(self):
        self.mwui.lblInstrument.setText("")
        self.mwui.lblWorkDir.setText("")
        
    def showMainWindow(self):
        self.__mw.show()
        
    ''' Update UI data
    '''
    def updateInstrumentFile(self, instrumentFile):
        self.mwui.lblInstrument.setText(instrumentFile)
        
    def updateWorkDir(self, workDir):
        self.mwui.lblWorkDir.setText(workDir)
        
    def updateStatus(self, text=''):
        self.mwui.statusbar.showMessage(text)
        
    def appendToMessages(self, text=''):
        self.mwui.textBrowser.append(text)
        
    ''' UI push actions - open dialogs
    '''
    def showOpenInstrumentDlg(self):
        dlg = QtGui.QFileDialog()
        dlg.setNameFilter("mcstas instruments (*.instr)");
        if dlg.exec_():
            return dlg.selectedFiles()[0]
    
    def showChangeWorkDirDlg(self):
        dlg = QtGui.QFileDialog()
        dlg.setFileMode(QtGui.QFileDialog.Directory)
        if dlg.exec_():
            return dlg.selectedFiles()[0]


''' Main Window widgets wrapper class
'''
class McMainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(McMainWindow, self).__init__(parent)
        self.ui =  Ui_MainWindow()
        self.ui.setupUi(self)


''' Auto-generated view created with QtCreator.
Generate this from .ui source with pyside-uic. Do not edit.
'''
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        MainWindow.setToolTip("")
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.groupBox_2 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox_2.setObjectName("groupBox_2")
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.label_2 = QtGui.QLabel(self.groupBox_2)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 2, 0, 1, 1)
        self.lblInstrument = QtGui.QLabel(self.groupBox_2)
        self.lblInstrument.setObjectName("lblInstrument")
        self.gridLayout.addWidget(self.lblInstrument, 2, 2, 1, 1)
        self.tbtnInstrument = QtGui.QToolButton(self.groupBox_2)
        self.tbtnInstrument.setObjectName("tbtnInstrument")
        self.gridLayout.addWidget(self.tbtnInstrument, 2, 3, 1, 1)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 2, 4, 1, 1)
        self.label = QtGui.QLabel(self.groupBox_2)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 3, 0, 1, 1)
        self.lblWorkDir = QtGui.QLabel(self.groupBox_2)
        self.lblWorkDir.setObjectName("lblWorkDir")
        self.gridLayout.addWidget(self.lblWorkDir, 3, 2, 1, 1)
        self.tbtnWorkDir = QtGui.QToolButton(self.groupBox_2)
        self.tbtnWorkDir.setObjectName("tbtnWorkDir")
        self.gridLayout.addWidget(self.tbtnWorkDir, 3, 3, 1, 1)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem1, 3, 4, 1, 1)
        self.verticalLayout_2.addLayout(self.gridLayout)
        self.verticalLayout.addWidget(self.groupBox_2)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.groupBox = QtGui.QGroupBox(self.centralwidget)
        self.groupBox.setObjectName("groupBox")
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.textBrowser = QtGui.QTextBrowser(self.groupBox)
        self.textBrowser.setObjectName("textBrowser")
        self.verticalLayout_3.addWidget(self.textBrowser)
        self.horizontalLayout_2.addWidget(self.groupBox)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 25))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuSimulation = QtGui.QMenu(self.menubar)
        self.menuSimulation.setObjectName("menuSimulation")
        self.menuHelp = QtGui.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.toolBar = QtGui.QToolBar(MainWindow)
        self.toolBar.setObjectName("toolBar")
        MainWindow.addToolBar(QtCore.Qt.RightToolBarArea, self.toolBar)
        self.actionOpen_instrument = QtGui.QAction(MainWindow)
        self.actionOpen_instrument.setObjectName("actionOpen_instrument")
        self.actionQuit = QtGui.QAction(MainWindow)
        self.actionQuit.setObjectName("actionQuit")
        self.actionRun_Simulation = QtGui.QAction(MainWindow)
        self.actionRun_Simulation.setObjectName("actionRun_Simulation")
        self.actionCompile_Instrument = QtGui.QAction(MainWindow)
        self.actionCompile_Instrument.setObjectName("actionCompile_Instrument")
        self.actionMcstas_User_Manual = QtGui.QAction(MainWindow)
        self.actionMcstas_User_Manual.setObjectName("actionMcstas_User_Manual")
        self.actionMcstas_Web_Page = QtGui.QAction(MainWindow)
        self.actionMcstas_Web_Page.setObjectName("actionMcstas_Web_Page")
        self.actionAbout = QtGui.QAction(MainWindow)
        self.actionAbout.setObjectName("actionAbout")
        self.actionRS = QtGui.QAction(MainWindow)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("run-icon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        icon.addPixmap(QtGui.QPixmap("../../GIMP/run-icon.png"), QtGui.QIcon.Normal, QtGui.QIcon.On)
        self.actionRS.setIcon(icon)
        self.actionRS.setObjectName("actionRS")
        self.actionCS = QtGui.QAction(MainWindow)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap("compile-icon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        icon1.addPixmap(QtGui.QPixmap("../../GIMP/compile-icon.png"), QtGui.QIcon.Normal, QtGui.QIcon.On)
        self.actionCS.setIcon(icon1)
        self.actionCS.setObjectName("actionCS")
        self.actionPS = QtGui.QAction(MainWindow)
        self.actionPS.setObjectName("actionPS")
        self.actionMcstas_Component_Manual = QtGui.QAction(MainWindow)
        self.actionMcstas_Component_Manual.setObjectName("actionMcstas_Component_Manual")
        self.actionChange_Working_Dir = QtGui.QAction(MainWindow)
        self.actionChange_Working_Dir.setObjectName("actionChange_Working_Dir")
        self.menuFile.addAction(self.actionOpen_instrument)
        self.menuFile.addAction(self.actionChange_Working_Dir)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionQuit)
        self.menuSimulation.addAction(self.actionRun_Simulation)
        self.menuSimulation.addAction(self.actionCompile_Instrument)
        self.menuHelp.addAction(self.actionMcstas_User_Manual)
        self.menuHelp.addAction(self.actionMcstas_Component_Manual)
        self.menuHelp.addAction(self.actionMcstas_Web_Page)
        self.menuHelp.addSeparator()
        self.menuHelp.addAction(self.actionAbout)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuSimulation.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())
        self.toolBar.addAction(self.actionRS)
        self.toolBar.addAction(self.actionCS)
        self.toolBar.addAction(self.actionPS)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        MainWindow.setTabOrder(self.textBrowser, self.tbtnInstrument)
        MainWindow.setTabOrder(self.tbtnInstrument, self.tbtnWorkDir)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "mcgui-py", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_2.setTitle(QtGui.QApplication.translate("MainWindow", "mcstas Files:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("MainWindow", "Instrument file: ", None, QtGui.QApplication.UnicodeUTF8))
        self.lblInstrument.setText(QtGui.QApplication.translate("MainWindow", "<Instrument file>", None, QtGui.QApplication.UnicodeUTF8))
        self.tbtnInstrument.setToolTip(QtGui.QApplication.translate("MainWindow", "Browse instrument...", None, QtGui.QApplication.UnicodeUTF8))
        self.tbtnInstrument.setText(QtGui.QApplication.translate("MainWindow", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("MainWindow", "Work dir.:", None, QtGui.QApplication.UnicodeUTF8))
        self.lblWorkDir.setText(QtGui.QApplication.translate("MainWindow", "<not defined>", None, QtGui.QApplication.UnicodeUTF8))
        self.tbtnWorkDir.setToolTip(QtGui.QApplication.translate("MainWindow", "Browse work dir...", None, QtGui.QApplication.UnicodeUTF8))
        self.tbtnWorkDir.setText(QtGui.QApplication.translate("MainWindow", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("MainWindow", "mcstas Log:", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuSimulation.setTitle(QtGui.QApplication.translate("MainWindow", "Simulation", None, QtGui.QApplication.UnicodeUTF8))
        self.menuHelp.setTitle(QtGui.QApplication.translate("MainWindow", "Help", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("MainWindow", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen_instrument.setText(QtGui.QApplication.translate("MainWindow", "Load Instrument...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuit.setText(QtGui.QApplication.translate("MainWindow", "Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRun_Simulation.setText(QtGui.QApplication.translate("MainWindow", "Run Simulation...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCompile_Instrument.setText(QtGui.QApplication.translate("MainWindow", "Compile Instrument", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMcstas_User_Manual.setText(QtGui.QApplication.translate("MainWindow", "mcstas User Manual", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMcstas_Web_Page.setText(QtGui.QApplication.translate("MainWindow", "mcstas Web Page", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout.setText(QtGui.QApplication.translate("MainWindow", "About...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRS.setText(QtGui.QApplication.translate("MainWindow", "RS", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRS.setToolTip(QtGui.QApplication.translate("MainWindow", "Run simulation...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCS.setText(QtGui.QApplication.translate("MainWindow", "CS", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCS.setToolTip(QtGui.QApplication.translate("MainWindow", "Compile instrument", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPS.setText(QtGui.QApplication.translate("MainWindow", "PS", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPS.setToolTip(QtGui.QApplication.translate("MainWindow", "Plot results...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMcstas_Component_Manual.setText(QtGui.QApplication.translate("MainWindow", "mcstas Component Manual", None, QtGui.QApplication.UnicodeUTF8))
        self.actionChange_Working_Dir.setText(QtGui.QApplication.translate("MainWindow", "Change Work Dir...", None, QtGui.QApplication.UnicodeUTF8))

