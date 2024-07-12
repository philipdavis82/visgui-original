import websockets
from threading import Thread
import sys,time,asyncio,pathlib,ssl,math,os

async def rotation(websocket:websockets.WebSocketServer):
    counter = 0
    angle   = 0
    rotations_per_min = 6.0
    io_per_second = 60.0
    counts_per_repeat = 60*60/rotations_per_min
    print(f"New Connection!",file=sys.stderr)
    # This is a forever context... From asyncio.Future()?
    try:
        async for message in websocket:
            await asyncio.sleep(1/io_per_second)
            await websocket.send(f"{angle}")
            counter += 1
            angle = (counter%counts_per_repeat)/counts_per_repeat*2*math.pi
    except:
        print(f"Send Failed!",file=sys.stderr)
        await websocket.close()

async def handler(websocket:websockets.WebSocketServer):
    await rotation(websocket)
    print(f"Socket Closed!",file=sys.stderr)


async def main(ip,port):
    async with websockets.serve(handler,ip,port):
        await asyncio.Future() # Run forever

if __name__ == "__main__":
    ip = '0.0.0.0'
    port = 8000
    ip   = os.environ["WEBIP"]
    port = int(os.environ["WEBPORT"])
    print(f"Websocket IM Serving: {ip},{port}")
    asyncio.run(main(ip,port))
