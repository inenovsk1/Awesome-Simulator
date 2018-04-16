import subprocess

def createConfigFile(data):
    """Creates a configuration file with the information passed to the
    HTML form, because the simulator requires a ini file passed to it as
    a command line argument
    """
    with open("webConfigurations.ini", 'w') as f:
        print('[Universe]', file=f)
        print('Data_Directory=/home/inenovski/Storage/home/programming/Final-Project/Awesome-Simulator/stock-data/S&P500', file=f)
        print('Universe_File=/home/inenovski/Storage/home/programming/Final-Project/Awesome-Simulator/stock-data/S&P500-tickers', file=f)

        print('[Fields]\nField1=Open\nField2=High\nField3=Low\nFiled4=Close\nField5=Adj_Close\nField6=Volume\nField7=Dividends\nField8=Splits', file=f)
        print('[Entry]\nEntry_Signal_Threshold=' + data['entryCondition'], file=f)

        print('[Exit]\nExit_Signal_Threshold=' + data['exitCondition'], file=f)
        print('Days_In_Position=' + data['daysInPosition'], file=f)

        print('[PortfolioConstraints]\nAvailable_Capital=' + data['availableCapital'], file=f)
        print('Max_Cap_Per_Stock=' + data['maxCapPerStock'], file=f)
        print('Max_Positions_Per_Instrument=' + data['maxPositionsPerInstrument'], file=f)

        print('[Trading]\nPositions_Per_Trade_Buy=' + data['positionsPerTradeBuy'], file=f)
        print('Positions_Per_Trade_Sell=' + data['positionsPerTradeSell'], file=f)

        print('[Reports]\nDAILY=' + data['dailyReport'], file=f)
        print('TRANSACTION=' + data['transactionReport'], file=f)

        # Unimplemented parameters in the simulator
        print('MONTHLY=' + '0', file=f)


def runProcess():
    """Runs the simulator with the given configurations in the HTML form
    written out in webConfigurations.ini !!The path given to the simulator is relative!!!
    If one wants to run the simulator, and is not logged in as myself, then one should
    change to path to use an absolute path!!
    """
    # use this when debugging and still changing source code on simulator
    #subprocess.call('~/Storage/home/programming/Final-Project/Awesome-Simulator/simulator/cmake-build-debug/simulator webConfigurations.ini', shell=True)
    
    # Once simulator is complete, move a copy of the executable in the root directory of the server
    # and use it instead for simpler and shorter command
    subprocess.call('./simulator webConfigurations.ini', shell=True)
