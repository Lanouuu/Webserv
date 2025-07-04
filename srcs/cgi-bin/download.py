import cgi
import os
import sys

def main():

    if os.environ.get("REQUEST_METHOD", "") == "POST":
        try:
            content_length = int(os.environ.get("CONTENT_LENGTH", 0))
            if content_length > 0:
                post_data = sys.stdin.read(content_length)
            else:
                post_data = ""
        except Exception:
            pass
    filepath = "./Makefile"
    if not os.path.isfile(filepath):
        print("Status: 404 Not Found")
        print("Content-Type: text/plain\r\n")
        print("Fichier non trouvé.")
        return
    sys.stdout.write("Content-Type: application/octet-stream\r\n")
    sys.stdout.write(f"Content-Disposition: attachment; filename=\"{os.path.basename(filepath)}\"\r\n")
    sys.stdout.write("Content-Length: {}\r\n".format(os.path.getsize(filepath)))
    sys.stdout.write("\r\n")
    sys.stdout.flush()
    with open(filepath, "rb") as f:
        sys.stdout.buffer.write(f.read())
        sys.stdout.buffer.flush()

if __name__ == "__main__":
    main()