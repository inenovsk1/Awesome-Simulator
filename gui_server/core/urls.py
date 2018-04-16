from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='home'),
    path('simulating', views.loader, name='simulating'),
    path('success', views.success, name='success')
]