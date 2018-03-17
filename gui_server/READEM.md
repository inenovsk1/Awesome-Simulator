# Django Web Server for the Simulator

Usage
------

1. To run the server one needs to generate a secret key first.
To do that use the django-generate-secret-key module 
```
pip3 install django-generate-secret-key
```
After that add the app to INSTALLED APPS.
```py
INSTALLED_APPS = [
    ...,
    'django_generate_secret_key',
]
```
Once the moduel is there one can run this command to generate a key
```
python3 manage.py generate_secret_key
```
or this use to --replace flag to replace the key in the same file
```
python3 manage.py generate_secret_key --replace
```

2. After there is a secret key one can simply run the server by
```
python3 manage.py runserver
```

3. If there are any migrations that need to be applied, make sure you do so by
```
python3 manage.py makemigrations
python3 manage.py migrate
```
