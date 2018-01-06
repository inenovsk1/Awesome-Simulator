"""
Use this tool to gather historical stock data using pandas datareader library!
Run by typing "python data.py" on the command line! Before running make sure
that 'BeautifulSoup' and 'pandas_datareader' modules are installed on the machine!
Enjoy!!
"""


# External dependencies
from bs4 import BeautifulSoup
import pandas as pd
import pandas_datareader.data as web

# Python built-in modules
import datetime
import urllib
import os
import shutil
import time


def initiate_folder(folder):
    if not os.path.exists(folder):
        os.mkdir(folder)
    else:
        shutil.rmtree(folder)
        os.mkdir(folder)


def generate_tickers_from_universe(source, universe, directory):
    page = urllib.request.urlopen(source).read()
    soup = BeautifulSoup(page, 'lxml')

    os.chdir(directory)

    with open(universe + '-tickers', 'w') as f:
        anchor_list = soup.find_all('a', class_='external text')

        for link in anchor_list:
            href = link['href']
            if href.startswith('https://www.nyse.com/') or href.startswith('http://www.nasdaq.com/'):
                print(link.text, file=f)


def main():
    sources_universes = {
        'https://en.wikipedia.org/wiki/List_of_S%26P_500_companies': 'S&P500'
    }

    data_sources = ['yahoo', 'yahoo-dividends', 'yahoo-actions']

    working_directory = '../stock-data'

    initiate_folder(working_directory)

    for source, universe in sources_universes.items():
        generate_tickers_from_universe(source, universe, working_directory)

    # read tickers from each universe file
    for index, universe in enumerate(sources_universes.values()):
        os.chdir(working_directory)

        with open(universe + '-tickers', 'r') as f:
            tickers = f.read()

        # use this as a log file. Report if a piece of data could not be read
        log = open(universe + '_log.txt', 'w')

        initiate_folder(os.path.join('./', universe))
        os.chdir(os.path.join('./', universe))

        tickers = tickers.splitlines()

        # start and end date to get historical data from
        start_date = datetime.datetime(2000, 1, 1)
        end_date = datetime.datetime.now()

        iteration = 1
        start_time = time.perf_counter()

        print('Universe: {}'.format(universe))

        # Iterate over each ticker, and pull historical data for it from yahoo
        # Also, save the pulled data in a csv file in the 'data' folder
        for ticker in tickers:
            print('Ticker #: {0}. Ticker: {1}'.format(iteration, ticker))
            iteration += 1

            data = list()  # store the dataframes here
            recv_sources = set()  # sources already receieved

            # Keep nagging the data provider until you get the stock data or until you nagged them 20 times
            nagging_counter = 0
            while True:
                if nagging_counter == 10:
                    print('Could not get ticker "{0}". Data provider blocked!'.format(ticker), file=log)
                    break

                try:
                    for data_source in data_sources:
                        if data_source not in recv_sources:
                            print("Getting data from {}".format(data_source))
                            df = web.DataReader(ticker, data_source, start_date, end_date)
                            data.append(df)
                            recv_sources.add(data_source)

                    # if all available data gathered, merge and output to file
                    df_intermediary = data[0].merge(data[1], how='outer', left_index=True, right_index=True)
                    df_intermediary.fillna(value=0, inplace=True)
                    df_final = df_intermediary.merge(data[2], how='outer', left_index=True, right_index=True)
                    df_final = df_final[df_final.action != 'DIVIDEND']
                    df_final.drop(labels='action', axis=1, inplace=True)
                    df_final = df_final.rename(columns={'value':'Split'})
                    df_final.fillna(value=1, inplace=True)
                    df_final.to_csv(ticker + '.csv')
                    break

                except ValueError:  # In case no data was gathered from yahoo-actions
                    df_actions = pd.DataFrame()
                    df_actions['Date'] = pd.Series(['2000-1-1'])
                    df_actions['action'] = pd.Series(['DIVIDEND'])
                    df_actions['value'] = pd.Series([2])
                    df_actions.set_index('Date', inplace=True)
                    data.append(df_actions)
                    recv_sources.add(data_source)
                    continue

                except Exception as err:
                    time.sleep(.5)
                    nagging_counter += 1
                    print('Nag #: {}'.format(nagging_counter))
                    continue

        end_time = time.perf_counter()
        log.close()

        elapsed = end_time - start_time
        print('It took {0} minutes to grab data for Universe {1}.'.format(round(elapsed / 60, 2), universe))


if __name__ == '__main__':
    main()
