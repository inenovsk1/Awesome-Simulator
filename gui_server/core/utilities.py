import subprocess
import os

def createConfigFile(data):
    with open("webConfigurations.ini", 'w') as f:
        print('[Universe]', file=f)
        print('Data_Directory=/home/inenovski/Storage/home/programming/Final-Project/Awesome-Simulator/stock-data/S&P500', file=f)
        print('Universe_File=/home/inenovski/Storage/home/programming/Final-Project/Awesome-Simulator/stock-data/S&P500-tickers', file=f)

        print('[Fields]\nField1=Open\nField2=High\nField3=Low\nFiled4=Close\nField5=Adj_Close\nField6=Volume\nField7=Dividends\nField8=Splits', file=f)
        print('[Entry]\nEntry_Signal_Threshold=' + data['entryCondition'], file=f)
        print('Exit_Signal_Threshold=' + data['exitCondition'], file=f)
        print('Days_In_Position=' + data['daysInPosition'], file=f)

        print('[PortfolioConstraints]\nAvailable_Capital=' + data['availableCapital'], file=f)
        print('Max_Cap_Per_Stock=' + data['maxCapPerStock'], file=f)
        print('Max_Positions_Per_Instrument=' + data['maxPositionsPerInstrument'], file=f)

        print('[Trading]\nPositions_Per_Trade_Buy=' + data['positionsPerTradeBuy'], file=f)
        print('Positions_Per_Trade_Sell=' + data['positionsPerTradeSell'], file=f)

        print('[Reports]\nDAILY=' + data['dailyReport'], file=f)
        print('TRANSACTION=' + data['transactionReport'], file=f)


def runProcess():
    pass
