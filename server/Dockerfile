# Copy all the 
FROM philipdavis82/visgui.example.export as example
# FROM philipdavis82/example.export as example
# FROM philipdavis82/example.export as example

FROM --platform=$BUILDPLATFORM python:3.11-alpine AS builder

# Server Enviroment
WORKDIR /server
COPY requirements.txt /server
RUN pip3 install -r requirements.txt --no-cache-dir
COPY . /server

# COPY
COPY --from=example /export /example/.


CMD ["python3", "main.py"]