# Get wasm files
FROM philipdavis82/visgui.example.export AS example
FROM philipdavis82/visgui.example_im.export AS example_im

FROM --platform=$BUILDPLATFORM python:3.11-alpine AS builder


ENV PATH="/server/scripts:${PATH}"

RUN apk add --update --no-cache --virtual .tmp gcc libc-dev linux-headers

# RUN apk update && \
#     apk add lighttpd
# COPY lighttpd/lighttpd.conf /etc/lighttpd/lighttpd.conf



WORKDIR /server
COPY requirements.txt /server
RUN pip3 install -r requirements.txt --no-cache-dir

RUN apk del .tmp

COPY templates /server/templates
COPY vvis /server/vvis
COPY scripts /server/scripts
COPY manage.py /server

# COPY . /server

COPY static /static

# COPY
COPY --from=example     /export/static       /static/example
COPY --from=example     /export/templates    /server/templates/example/.
# COPY --from=example     /export/websockets   /websockets/example

COPY --from=example_im  /export/static       /static/example_im
COPY --from=example_im  /export/templates    /server/templates/example_im/.
# COPY --from=example_im  /export/websockets   /server/websockets/example_im/.

RUN chmod +x /server/scripts

RUN adduser -D user 
RUN mkdir -p /vol/static/media
RUN chown user:user -R /vol
RUN chmod 755 -R /vol

USER user

CMD ["sh","/server/scripts/entrypoint.sh"]

# Used this when not using compose
# ENTRYPOINT ["python3"] 
# CMD ["manage.py", "runserver", "0.0.0.0:8000"]
