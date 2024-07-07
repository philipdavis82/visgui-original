
from http.server import HTTPServer, BaseHTTPRequestHandler,SimpleHTTPRequestHandler, test
import sys,time,os,signal,ssl

class CORSRequestHandler (SimpleHTTPRequestHandler):
    def end_headers (self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header('Access-Control-Allow-Origin', '*')
        SimpleHTTPRequestHandler.end_headers(self)

# ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
# ssl_context.load_cert_chain(
#     keyfile="key.pem", 
#     certfile="cert.pem",
#     password='cake'
# )

if __name__ == '__main__':
    # httpd = HTTPServer(('0.0.0.0', 8000), BaseHTTPRequestHandler)

    # httpd.socket = ssl_context.wrap_socket (
    #         httpd.socket, 
    #         server_side=True)

    # httpd.serve_forever()

    test(CORSRequestHandler, HTTPServer, port=int(sys.argv[1]) if len(sys.argv) > 1 else 8000)

