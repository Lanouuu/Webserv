import os
import sys

def main():

    filepath = "./Makefile"

    if not os.path.isfile(filepath):
        print("Status: 404 Not Found")
        print("Content-Type: text/plain\r\n")
        print("Fichier non trouvé.")
        return
    
    headers = (
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        f"Content-Disposition: attachment; filename=\"{os.path.basename(filepath)}\"\r\n"
        f"Content-Length: {os.path.getsize(filepath)}\r\n"
        "\r\n"
    )

    sys.stdout.buffer.write(headers.encode())
    with open(filepath, "rb") as f:
        sys.stdout.buffer.write(f.read())
    sys.stdout.buffer.flush()

if __name__ == "__main__":
    main()