# Get wasm files
FROM philipdavis82/visgui.example.export AS example
FROM philipdavis82/visgui.example_im.export AS example_im

FROM --platform=$BUILDPLATFORM python:3.11-alpine AS builder

EXPOSE 8000
WORKDIR /server
COPY requirements.txt /server
RUN pip3 install -r requirements.txt --no-cache-dir
COPY . /server

# COPY
COPY --from=example     /export/static       /example
COPY --from=example     /export/templates    /server/templates/example/.
COPY --from=example     /export/websockets   /websockets/example

COPY --from=example_im  /export/static       /example_im
COPY --from=example_im  /export/templates    /server/templates/example_im/.
COPY --from=example_im  /export/websockets   /server/websockets/example_im/.

ENTRYPOINT ["python3"] 
CMD ["manage.py", "runserver", "0.0.0.0:8000"]
