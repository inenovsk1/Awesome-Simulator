from django.shortcuts import render, redirect

from .forms import ConfigurationsForm
from .utilities import *

# Create your views here.

def index(request):
    form = ConfigurationsForm()
    context = {'form' : form}
    return render(request, 'core/index.html', context)

def loader(request):
    if request.method == 'POST':
        form = ConfigurationsForm(request.POST)

        if form.is_valid():
            # create a dictionary with all request.POST data
            formData = {
                'entryCondition': request.POST['entryCondition'],
                'exitCondition': request.POST['exitCondition'],
                'daysInPosition': request.POST['daysInPosition'],
                'availableCapital': request.POST['availableCapital'],
                'maxCapPerStock': request.POST['maxCapPerStock'],
                'maxPositionsPerInstrument': request.POST['maxPositionsPerInstrument'],
                'positionsPerTradeBuy': request.POST['positionsPerTradeBuy'],
                'positionsPerTradeSell': request.POST['positionsPerTradeSell'],
                'dailyReport': request.POST['dailyReport'],
                'transactionReport': request.POST['transactionReport']
            }

            createConfigFile(formData)

            return render(request, 'core/simulation.html')
    
    # This else is simply for debugging purposes
    elif request.method == 'GET':
        return render(request, 'core/simulation.html')

def simulationProgress(request):
    if request.method == 'POST':

        passedData = request.POST['data']

        if passedData == 'Simulation started':
            runProcess()

            # Actally not rendering, however, each view should return some HTTP object back!!
            return render(request, 'core/simulation.html')
