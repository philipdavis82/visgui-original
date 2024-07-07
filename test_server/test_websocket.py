import websockets
from threading import Thread
import sys,time,asyncio
import pathlib
import ssl
import math

# ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
# localhost_cert_pem = pathlib.Path(__file__).with_name("cert.pem")
# localhost_key_pem = pathlib.Path(__file__).with_name("key.pem")
# # ssl_context.load_cert_chain(localhost_pem)
# # ssl_context.load_cert_chain(localhost_pem)
# ssl_context.load_cert_chain(
#     localhost_cert_pem,
#     localhost_key_pem,
#     password='cake'
# )
# ssl.SSL
    

async def handler(websocket):
    counter = 0
    angle   = 0
    rotations_per_min = 2.0
    io_per_second = 60.0
    counts_per_repeat = 60*60/rotations_per_min
    # This is a forever context... From asyncio.Future()?
    async for message in websocket:
        await asyncio.sleep(1/io_per_second)
        # if(counter % 100 == 0): print(f"Sending 'Server {counter}'")
        await websocket.send(f"{angle}")
        counter += 1
        angle = (counter%counts_per_repeat)/counts_per_repeat*2*math.pi

async def main(ip):
    async with websockets.serve(handler,ip,8765):
        await asyncio.Future() # Run forever


if __name__ == "__main__":
    ip = '127.0.0.1'
    ip = '0.0.0.0'
    print(f"Websocket Serving: {ip}")
    asyncio.run(main(ip))
