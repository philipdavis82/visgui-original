#!/bin/bash

set -e
cd /server/
python3 manage.py collectstatic --noinput
uwsgi --socket :8000 --master --enable-threads --module vvis.wsgi  