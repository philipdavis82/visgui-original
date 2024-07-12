# Copy all the 
FROM philipdavis82/visgui.example.export AS example
FROM philipdavis82/visgui.example_im.export AS example_im
# FROM philipdavis82/example.export as example
# FROM philipdavis82/example.export as example

FROM --platform=$BUILDPLATFORM python:3.11-alpine AS builder

# Server Enviroment
WORKDIR /server
COPY requirements.txt /server
RUN pip3 install -r requirements.txt --no-cache-dir
COPY . /server

# COPY
COPY --from=example    /export /example/.
COPY --from=example_im /export /example_im/.

CMD ["python3", "main.py"]