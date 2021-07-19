# Importing the relevant libraries
import websockets
#import asyncio
import tkinter as tk
from threading import Thread

# The main function that will handle connection and communication 
# with the server
async def listen():
    url = "ws://127.0.0.1:5000"
    # Connect to the server
    async with websockets.connect(url) as ws:
        # Send a greeting message
        await ws.send("Hello Server!")
        # Stay alive forever, listening to incoming msgs
        while True:
            msg = await ws.recv()
            print(msg)

def send_msg ():
    global ws
    ws.send(tool.ge())

def connection():
   #enableTrace(True)
   url = "ws://127.0.0.1:5000"
   global ws
   ws = websockets.connect(url)
   
   # ws = WebSocketApp("ws://echo.websocket.org/", on_message = on_message, on_error = on_error, on_close = on_close)
   #ws.on_open = on_open

   ws.run_forever()
   return

#Thread(target=connection).start()
# Start the connection
#asyncio.get_event_loop().run_until_complete(listen())

ws = []

windows = tk.Tk()
windows.geometry('500x300')
main_frame = tk.Frame(windows, borderwidth=2, relief='solid')
main_frame.pack(fill=tk.BOTH, expand=True)

tool = tk.Scale(main_frame, orient=tk.HORIZONTAL, length=300, from_=1.0, to=255.0, command=send_msg)
tool.pack()

tk.mainloop()