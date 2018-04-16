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

            render(request, 'core/loader.html')

            createConfigFile(formData)

            #return redirect('core/loader.html')
            return render(request, 'core/loader.html')
            #return render(request, 'core/success.html')

def success(request):
    return render(request, 'core/success.html')
