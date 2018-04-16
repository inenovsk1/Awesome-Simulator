from django import forms

class ConfigurationsForm(forms.Form):
    entryCondition = forms.FloatField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '2.3'}))
    exitCondition = forms.FloatField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '9.6'}))
    daysInPosition = forms.IntegerField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '8'}))
    availableCapital = forms.FloatField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '10000000'}))
    maxCapPerStock = forms.FloatField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '80000'}))
    maxPositionsPerInstrument = forms.IntegerField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '5000'}))
    positionsPerTradeBuy = forms.IntegerField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '50'}))
    positionsPerTradeSell = forms.IntegerField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '75'}))
    dailyReport = forms.IntegerField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '1'}), min_value=0, max_value=1)
    transactionReport = forms.IntegerField(widget=forms.NumberInput(attrs={'class' : 'form-control form-control-lg', 'value' : '1'}), min_value=0, max_value=1)
